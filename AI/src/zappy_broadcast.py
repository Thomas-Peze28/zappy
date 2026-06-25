import time


class ZappyBroadcastCoordinator:
    def __init__(self, team_name, leader_id):
        self.team_name = str(team_name)
        self.leader_id = str(leader_id)

        self.last_announce = 0.0
        self.active_session = None
        self.last_seen_session = 0.0

    def can_announce(self, interval=1.4):
        now = time.time()
        if now - self.last_announce < interval:
            return False
        self.last_announce = now
        return True

    def _escape(self, value):
        return str(value).replace(";", "_").replace("=", "_").replace("\n", "_")

    def _build_packet(self, kind, level, fields=None):
        bucket = int(time.time() // 4)
        session = f"{self.team_name}:{kind}:{level}:{self.leader_id}:{bucket}"

        data = {
            "team": self.team_name,
            "kind": kind,
            "level": level,
            "leader": self.leader_id,
            "session": session,
        }

        if fields:
            data.update(fields)

        parts = ["BC1"]
        for key, value in data.items():
            parts.append(f"{key}={self._escape(value)}")

        return ";".join(parts)

    def build_inventory_packet(self, level, inventory):
        return self._build_packet(
            "inventory-status",
            level,
            {
                "food": inventory.get("food", 0),
                "linemate": inventory.get("linemate", 0),
                "deraumere": inventory.get("deraumere", 0),
                "sibur": inventory.get("sibur", 0),
                "mendiane": inventory.get("mendiane", 0),
                "phiras": inventory.get("phiras", 0),
                "thystame": inventory.get("thystame", 0),
            },
        )

    def build_elevation_packet(self, level, payload=None):
        payload = payload or {}

        fields = {
            "players": payload.get("players", 0),
            "required": payload.get("required", 0),
            "food": payload.get("food", 0),
        }

        missing = payload.get("missing", {})
        for stone, amount in missing.items():
            fields[f"missing_{stone}"] = amount

        leader_inventory = payload.get("leader_inventory", {})
        for stone, amount in leader_inventory.items():
            fields[f"inv_{stone}"] = amount

        return self._build_packet("elevation-request", level, fields)

    def parse_incoming(self, raw_message):
        direction = 0
        text = raw_message.strip()

        if text.startswith("message "):
            try:
                parts = text.split(",", 1)
                direction = int(parts[0].split()[1])
                text = parts[1].strip()
            except (IndexError, ValueError):
                return None

        if not text.startswith("BC1;"):
            return None

        try:
            data = {}

            for part in text.split(";")[1:]:
                if "=" not in part:
                    continue
                key, value = part.split("=", 1)
                data[key] = value

            if data.get("team") != self.team_name:
                return None

            kind = data.get("kind")
            level = int(data.get("level", 0))

            payload = {}

            if kind == "inventory-status":
                payload["inventory"] = {
                    "food": int(data.get("food", 0)),
                    "linemate": int(data.get("linemate", 0)),
                    "deraumere": int(data.get("deraumere", 0)),
                    "sibur": int(data.get("sibur", 0)),
                    "mendiane": int(data.get("mendiane", 0)),
                    "phiras": int(data.get("phiras", 0)),
                    "thystame": int(data.get("thystame", 0)),
                }

            elif kind == "elevation-request":
                payload["players"] = int(data.get("players", 0))
                payload["required"] = int(data.get("required", 0))
                payload["food"] = int(data.get("food", 0))

                missing = {}
                for key, value in data.items():
                    if key.startswith("missing_"):
                        stone = key.replace("missing_", "", 1)
                        missing[stone] = int(value)
                payload["missing"] = missing

                leader_inventory = {}
                for key, value in data.items():
                    if key.startswith("inv_"):
                        stone = key.replace("inv_", "", 1)
                        leader_inventory[stone] = int(value)
                payload["leader_inventory"] = leader_inventory

            return {
                "kind": kind,
                "level": level,
                "direction": direction,
                "leader_id": data.get("leader"),
                "session_id": data.get("session"),
                "payload": payload,
                "legacy": False,
            }

        except Exception as e:
            print(f"[BROADCAST] parse error: {e} | raw={text}")
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