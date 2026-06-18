import base64
import hashlib
import hmac
import json
import time


class ZappyBroadcastCoordinator:
    def __init__(self, team_name, leader_id):
        self.team_name = team_name
        self.leader_id = str(leader_id)
        
        self.secret_key = hashlib.sha256(f"{team_name}:broadcast:v1".encode()).digest()
        
        phase_hash = hashlib.sha256(f"phase:{self.leader_id}".encode()).digest()
        self.announce_phase = (int.from_bytes(phase_hash[:4], "big") / 0xFFFFFFFF) * 0.9
        
        self.last_announce = 0.0
        self.active_session = None
        self.last_seen_session = 0.0

    def _get_keystream(self, length, nonce):
        stream = bytearray()
        counter = 0
        while len(stream) < length:
            block = hashlib.sha256(self.secret_key + nonce + counter.to_bytes(4, "big")).digest()
            stream.extend(block)
            counter += 1
        return bytes(stream[:length])

    def _crypt(self, data, nonce):
        keystream = self._get_keystream(len(data), nonce)
        return bytes(b1 ^ b2 for b1, b2 in zip(data, keystream))

    def _sign(self, text):
        return hmac.new(self.secret_key, text.encode(), hashlib.sha256).hexdigest()

    def can_announce(self, interval=1.4):
        now = time.time()
        if now - self.last_announce < (interval + self.announce_phase):
            return False
        self.last_announce = now
        return True

    def build_elevation_packet(self, level, payload=None):
        nonce = hashlib.sha256(f"{self.leader_id}:{level}:{time.time_ns()}".encode()).digest()[:16]
        bucket = int(time.time() // 4)
        
        frame = {
            "v": 1,
            "team": self.team_name,
            "kind": "elevation-request",
            "level": level,
            "leader": self.leader_id,
            "session": f"{self.team_name}:{level}:{self.leader_id}:{bucket}",
            "payload": payload or {}
        }
        
        raw_json = json.dumps(frame, separators=(",", ":"), sort_keys=True).encode()
        cipher = self._crypt(raw_json, nonce)
        encoded = base64.urlsafe_b64encode(nonce + cipher).decode()
        
        return f"BC1|{encoded}|{self._sign(encoded)}"

    def parse_incoming(self, raw_message):
        direction = 0
        text = raw_message.strip()

        if text.startswith("message "):
            try:
                parts = text.split(", ", 1)
                direction = int(parts[0].split()[1])
                text = parts[1]
            except (IndexError, ValueError):
                return None

        if text.startswith("BC1|"):
            parts = text.split("|")
            if len(parts) != 3:
                return None
            
            _, encoded, signature = parts
            if not hmac.compare_digest(self._sign(encoded), signature):
                return None

            try:
                blob = base64.urlsafe_b64decode(encoded.encode())
                if len(blob) <= 16:
                    return None
                
                nonce, cipher = blob[:16], blob[16:]
                decrypted = self._crypt(cipher, nonce)
                frame = json.loads(decrypted.decode())
                
                if frame.get("team") != self.team_name:
                    return None
                
                return {
                    "kind": frame.get("kind"),
                    "level": int(frame.get("level", 0)),
                    "direction": direction,
                    "leader_id": str(frame.get("leader")),
                    "session_id": str(frame.get("session")),
                    "payload": frame.get("payload", {}),
                    "legacy": False
                }
            except Exception:
                return None

        legacy_prefix = f"{self.team_name}_ELEVATE_"
        if text.startswith(legacy_prefix):
            try:
                level = int(text.replace(legacy_prefix, "", 1))
                return {
                    "kind": "elevation-request",
                    "level": level,
                    "direction": direction,
                    "leader_id": "legacy",
                    "session_id": f"legacy:{self.team_name}:{level}",
                    "payload": {},
                    "legacy": True
                }
            except ValueError:
                return None

        return None

    def should_follow(self, frame, lock_ttl=6.0):
        if not frame or frame.get("kind") != "elevation-request":
            return False

        now = time.time()
        session_id = frame.get("session_id")

        if self.active_session and session_id != self.active_session:
            if now - self.last_seen_session < lock_ttl:
                return False

        self.active_session = session_id
        self.last_seen_session = now
        return True