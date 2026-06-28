import time

class ZappyRitual:
    STONES = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

    RITUAL_SESSION_TTL = 120.0
    RITUAL_READY_COOLDOWN = 4.0
    RITUAL_SITE_COOLDOWN = 0.6

    def __init__(self, drone_id, team_name, queue_command_fn, broadcast_coordinator, registry, movement):
        self.drone_id = drone_id
        self.team_name = team_name
        self._queue = queue_command_fn
        self.broadcast = broadcast_coordinator
        self.registry = registry
        self.movement = movement
        self.ritual_sessions = {}
        self.my_ritual_session = None
        self.ignored_ritual_sessions = {}
        self.committed_session = None
        self.leader_locked = False
        self.arrived_at_leader = False
        self.last_ritual_site_broadcast = 0.0
        self.last_ritual_ready_broadcast = 0.0
        self.last_wait_on_leader_log = 0.0
        self.last_fast_join_log = 0.0
        self.last_join_move_session = None
        self.last_join_move_seen = 0.0
        self.join_steps_done = 0
        self.last_nonzero_leader_direction = 1
        self.last_zero_probe_time = 0.0
        self.zero_leader_probe_pending = False
        self._current_leader_id_snapshot: str = drone_id

    def best_ritual_session(self):
        self.cleanup()
        if self.my_ritual_session:
            session = self.ritual_sessions.get(self.my_ritual_session)
            if (
                session
                and session.get("target_level") == self._level + 1
                and session.get("required", 0) > 1
            ):
                return self.my_ritual_session, session
            self.my_ritual_session = None

        candidates = [
            (sid, s)
            for sid, s in self.ritual_sessions.items()
            if s.get("target_level") == self._level + 1
            and s.get("required", 0) > 1
            and sid not in self.ignored_ritual_sessions
        ]
        if not candidates:
            return None, None
        candidates.sort(
            key=lambda item: (self._ritual_ready_count(item[0]), item[1].get("last_seen", 0)),
            reverse=True,
        )
        return candidates[0]

    def ritual_ready_count(self, session_id):
        return self._ritual_ready_count(session_id)

    def set_level(self, level):
        self._level = level

    def broadcast_ritual_site(self, req, target_group, leader_food, known_count):
        now = time.time()
        if now - self.last_ritual_site_broadcast < self.RITUAL_SITE_COOLDOWN:
            return

        packet = self.broadcast.build_ritual_site_packet(
            self._level + 1,
            target_group,
            leader_food,
            known_count,
        )
        session_id = f"{self.team_name}:global-ritual:{self.drone_id}"
        self.leader_locked = True
        self.committed_session = session_id
        self._queue(f"Broadcast {packet}")
        self.last_ritual_site_broadcast = now
        self._register_my_ritual_site(session_id, target_group)
        print(
            f"[LEADER] Appel regroupement global session={session_id} "
            f"niveau={self._level}->{self._level + 1} target={target_group}",
            flush=True,
        )
        print("[LEADER] RALLIEMENT VERROUILLE: je ne bouge plus, j'attends les drones sur ma case.", flush=True)

    def broadcast_ritual_ready(self, session_id, leader_food):
        now = time.time()
        if now - self.last_ritual_ready_broadcast < self.RITUAL_READY_COOLDOWN:
            return
        packet = self.broadcast.build_ritual_ready_packet(
            self._level + 1,
            session_id,
            leader_food,
        )
        self._queue(f"Broadcast {packet}")
        self.last_ritual_ready_broadcast = now
        session = self.ritual_sessions.setdefault(
            session_id,
            {
                "leader_id": None,
                "target_level": self._level + 1,
                "required": 0,
                "ready": {},
                "last_direction": 0,
                "last_seen": now,
                "created_at": now,
            },
        )
        session["ready"][self.drone_id] = now
        session["last_seen"] = now

    def handle_ritual_broadcast(self, k, frame, min_drones, current_leader_id):
        if frame.get("leader_id") == self.drone_id:
            return
        target_level = frame.get("level", 0)
        if self.registry.known_drone_count(self._level, {}) < min_drones:
            return
        if frame.get("leader_id") != current_leader_id:
            return

        session_id = frame.get("session_id")
        if not session_id:
            return

        now = time.time()
        if session_id in self.ignored_ritual_sessions:
            if now - self.ignored_ritual_sessions[session_id] < self.RITUAL_SESSION_TTL:
                return
            self.ignored_ritual_sessions.pop(session_id, None)
        kind = frame.get("kind")
        payload = frame.get("payload", {})
        if kind == "ritual-ready":
            session = self.ritual_sessions.get(session_id)
            if not session:
                return
            if self.my_ritual_session and self.my_ritual_session != session_id:
                return
            session["ready"][frame.get("leader_id")] = now
            session["last_seen"] = now
            print(
                f"[IA] Drone prêt pour rituel: session={session_id} ready={self._ritual_ready_count(session_id)}"
            )
            return

        if kind != "ritual-site":
            return

        required_players = max(payload.get("required", 0), 1)

        if self.my_ritual_session and self.my_ritual_session != session_id:
            return
        if not self.broadcast.should_follow(frame, lock_ttl=30.0):
            return

        session = self.ritual_sessions.setdefault(
            session_id,
            {
                "leader_id": frame.get("leader_id"),
                "target_level": target_level,
                "required": required_players,
                "ready": {},
                "last_direction": k,
                "last_seen": now,
                "created_at": now,
            },
        )
        session["leader_id"] = frame.get("leader_id")
        session["target_level"] = target_level
        session["required"] = max(session.get("required", 0), required_players)
        session["last_direction"] = k
        session["last_seen"] = now
        session["direction_updated_at"] = now
        if k != 0:
            self.last_nonzero_leader_direction = k
            self.zero_leader_probe_pending = False
        session["ready"][frame.get("leader_id")] = now

        if not self.my_ritual_session:
            self.my_ritual_session = session_id
        if self.committed_session != session_id:
            self.arrived_at_leader = False
            self.last_join_move_session = None
            self.last_join_move_seen = 0.0
            self.join_steps_done = 0
        self.committed_session = session_id

        print(f"[IA] Ralliement leader: session={session_id} son={k}.", flush=True)
        return True

    def go_to_ritual_site(self, session, deposit_fn, inventory_fresh, queue_fn):
        direction = session.get("last_direction", 0)
        session_id = self.committed_session or self.my_ritual_session
        now = time.time()
        if self.arrived_at_leader:
            queue_fn.clear_queue()
            if now - self.last_wait_on_leader_log > 2.0:
                print("[IA] STOP LEADER: sur la case du leader, je ne bouge plus.", flush=True)
                self.last_wait_on_leader_log = now
            if not inventory_fresh:
                queue_fn.queue_command("Inventory")
                return
            deposit_fn()
            return
        if direction == 0:
            self.arrived_at_leader = True
            queue_fn.clear_queue()
            print("[IA] ARRIVE LEADER: son direction=0, stop definitif et depot des pierres.", flush=True)
            if not inventory_fresh:
                queue_fn.queue_command("Inventory")
                return
            deposit_fn()
            return
        direction_seen = session.get("direction_updated_at", session.get("last_seen", 0))
        already_used = (
            self.last_join_move_session == session_id
            and direction_seen <= self.last_join_move_seen
        )
        if already_used:
            return
        if now - self.last_fast_join_log > 1.0:
            print(f"[IA] SUIVI SON LEADER: direction={direction}.", flush=True)
            self.last_fast_join_log = now
        self.last_nonzero_leader_direction = direction
        self.last_join_move_session = session_id
        self.last_join_move_seen = direction_seen
        self.join_steps_done += 1
        self.movement.move_one_step_to_sound(direction)

    def cleanup(self):
        now = time.time()
        expired = [
            sid
            for sid, s in self.ritual_sessions.items()
            if now - s.get("last_seen", 0) > self.RITUAL_SESSION_TTL
        ]
        for sid in expired:
            self.ritual_sessions.pop(sid, None)
            if self.my_ritual_session == sid:
                self.my_ritual_session = None
        for sid, last_seen in list(self.ignored_ritual_sessions.items()):
            if now - last_seen > self.RITUAL_SESSION_TTL:
                self.ignored_ritual_sessions.pop(sid, None)
        for session in self.ritual_sessions.values():
            ready = session.get("ready", {})
            for drone_id, last_seen in list(ready.items()):
                if now - last_seen > self.RITUAL_SESSION_TTL:
                    ready.pop(drone_id, None)

    def reset_after_level_up(self, level, team_name, drone_id, was_locked_group, was_locked_leader, target_group_fn):
        self.ritual_sessions.clear()
        self.my_ritual_session = None
        self.ignored_ritual_sessions.clear()
        self.last_join_move_session = None
        self.last_join_move_seen = 0.0
        self.join_steps_done = 0
        self.last_nonzero_leader_direction = 1
        self.last_zero_probe_time = 0.0
        self.zero_leader_probe_pending = False

        if was_locked_group and level < 8:
            leader_id = self._current_leader_id_snapshot
            next_session = f"{team_name}:global-ritual:{leader_id}"
            self.committed_session = next_session
            self.my_ritual_session = next_session
            self.arrived_at_leader = True
            self.leader_locked = was_locked_leader
            now = time.time()
            self.ritual_sessions[next_session] = {
                "leader_id": leader_id,
                "target_level": level + 1,
                "required": target_group_fn(),
                "ready": {drone_id: now},
                "last_direction": 0,
                "last_seen": now,
                "direction_updated_at": now,
                "created_at": now,
            }
            return "leader" if was_locked_leader else "follower"
        elif was_locked_group and level >= 8:
            self.arrived_at_leader = True
            return "max_level"
        else:
            self.committed_session = None
            self.leader_locked = False
            self.arrived_at_leader = False
            return "free"

    def _ritual_ready_count(self, session_id):
        session = self.ritual_sessions.get(session_id)
        if not session:
            return 0
        now = time.time()
        return sum(
            1
            for last_seen in session.get("ready", {}).values()
            if now - last_seen <= self.RITUAL_SESSION_TTL
        )

    def _register_my_ritual_site(self, session_id, required):
        now = time.time()
        self.my_ritual_session = session_id
        session = self.ritual_sessions.setdefault(
            session_id,
            {
                "leader_id": self.drone_id,
                "target_level": self._level + 1,
                "required": required,
                "ready": {},
                "last_direction": 0,
                "last_seen": now,
                "created_at": now,
            },
        )
        session["leader_id"] = self.drone_id
        session["target_level"] = self._level + 1
        session["required"] = required
        session["last_direction"] = 0
        session["last_seen"] = now
        session["ready"][self.drone_id] = now
