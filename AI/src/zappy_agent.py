import math
import time
import os
from collections import deque

from .zappy_broadcast import ZappyBroadcastCoordinator
from .zappy_state import DroneState


class ZappyAI:
    ELEVATION_REQUIREMENTS = {
        2: [1, 1, 0, 0, 0, 0, 0],
        3: [2, 1, 1, 1, 0, 0, 0],
        4: [2, 2, 0, 1, 0, 2, 0],
        5: [4, 1, 1, 2, 0, 1, 0],
        6: [4, 1, 2, 1, 3, 0, 0],
        7: [6, 1, 2, 3, 0, 1, 0],
        8: [6, 2, 2, 2, 2, 2, 1],
    }
    STONES = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
    MIN_DRONES = 15
    FORK_COOLDOWN = 18.0
    CONNECT_NBR_COOLDOWN = 4.0
    FOOD_SEARCH_MIN = 18
    FORK_FOOD_MIN = 25
    JOIN_RITUAL_FOOD_MIN = 22
    COLLECTOR_FOOD_TARGET = 50
    COLLECTOR_READY_FOOD_MIN = 45
    LEADER_RITUAL_FOOD_MIN = 70
    EMERGENCY_FOOD_MIN = 8
    RITUAL_FOOD_MIN = JOIN_RITUAL_FOOD_MIN
    RITUAL_SESSION_TTL = 120.0
    RITUAL_READY_COOLDOWN = 4.0
    RITUAL_SITE_COOLDOWN = 0.6
    JOIN_REPEAT_TIMEOUT = 1.2

    def __init__(self, manager, sock, team_name, remaining_slots, map_x, map_y):
        self.manager = manager
        self.sock = sock
        self.team_name = team_name
        self.remaining_slots = remaining_slots
        self.map_x = map_x
        self.map_y = map_y
        self.read_buffer = ""
        self.command_queue = deque()
        self.sent_commands = deque()
        self.inventory = {}
        self.vision = []
        self.inventory_fresh = False
        self.vision_fresh = False
        self.state = DroneState.SEARCH_FOOD
        self.level = 1
        self.food_threshold = self.FOOD_SEARCH_MIN
        self.last_fork_time = 0.0
        self.last_connect_nbr_time = time.time()
        self.drone_id = f"{os.getpid()}-{self.sock.fileno()}-{int(time.time() * 1000)}"
        self.broadcast = ZappyBroadcastCoordinator(team_name, self.drone_id)
        self.known_drones = {}
        self.leader_locked = False
        self.committed_session = None
        self.last_inventory_broadcast = 0.0
        self.INVENTORY_BROADCAST_COOLDOWN = 8.0
        self.ritual_sessions = {}
        self.my_ritual_session = None
        self.ignored_ritual_sessions = {}
        self.last_ritual_site_broadcast = 0.0
        self.last_ritual_ready_broadcast = 0.0
        self.last_leader_block_log = 0.0
        self.last_leader_status_log = 0.0
        self.last_seen_leader_id = None
        self.last_fast_join_log = 0.0
        self.arrived_at_leader = False
        self.last_join_move_session = None
        self.last_join_move_seen = 0.0
        self.join_steps_done = 0
        self.last_wait_on_leader_log = 0.0
        self.last_nonzero_leader_direction = 1
        self.last_zero_probe_time = 0.0
        self.zero_leader_probe_pending = False

    def queue_command(self, command):
        self.command_queue.append(command)
        self._flush_commands()

    def _flush_commands(self):
        while self.command_queue and len(self.sent_commands) < 10:
            cmd = self.command_queue.popleft()
            try:
                self.sock.sendall((cmd + "\n").encode("utf-8"))
                self.sent_commands.append(cmd)
            except Exception:
                self.manager.disconnect()
                return

    def receive_data(self):
        try:
            data = self.sock.recv(4096)
            if not data:
                self.manager.disconnect()
                return
            self.read_buffer += data.decode("utf-8")
            self._process_buffer()
        except BlockingIOError:
            pass
        except Exception:
            self.manager.disconnect()

    def _process_buffer(self):
        while "\n" in self.read_buffer:
            line, self.read_buffer = self.read_buffer.split("\n", 1)
            line = line.strip()

            if line == "dead":
                print("[*] Le drone est mort de faim.")
                self.manager.disconnect()
                return

            if line.startswith("message"):
                parts = line.replace("message ", "").split(",", 1)
                if len(parts) == 2:
                    try:
                        self._handle_broadcast(int(parts[0].strip()), parts[1].strip())
                    except ValueError:
                        pass
                continue

            if line.startswith("eject:"):
                continue

            if line.startswith("Current level:"):
                try:
                    was_locked_group = self.leader_locked or self.committed_session is not None or self.arrived_at_leader
                    was_locked_leader = self.leader_locked and self._is_global_leader()
                    self.level = int(line.split(":")[1].strip())
                    print(f"[IA] Niveau {self.level} atteint !", flush=True)

                    self.command_queue.clear()
                    self.ritual_sessions.clear()
                    self.my_ritual_session = None
                    self.ignored_ritual_sessions.clear()
                    self.inventory_fresh = False
                    self.vision_fresh = False
                    self.last_join_move_session = None
                    self.last_join_move_seen = 0.0
                    self.join_steps_done = 0
                    self.last_nonzero_leader_direction = 1
                    self.last_zero_probe_time = 0.0
                    self.zero_leader_probe_pending = False

                    if was_locked_group and self.level < 8:
                        leader_id = self._current_leader_id()
                        next_session = f"{self.team_name}:global-ritual:{leader_id}"
                        self.committed_session = next_session
                        self.my_ritual_session = next_session
                        self.arrived_at_leader = True
                        self.leader_locked = was_locked_leader
                        now = time.time()
                        self.ritual_sessions[next_session] = {
                            "leader_id": leader_id,
                            "target_level": self.level + 1,
                            "required": self._target_group_size(),
                            "ready": {self.drone_id: now},
                            "last_direction": 0,
                            "last_seen": now,
                            "direction_updated_at": now,
                            "created_at": now,
                        }
                        if self.leader_locked:
                            self.state = DroneState.ELEVATE
                            print("[LEADER] Rituel reussi: je reste bloque sur la case pour enchainer le prochain rituel.", flush=True)
                        else:
                            self.state = DroneState.JOIN_ELEVATION
                            print("[IA] Rituel reussi: je reste sur la case du leader, aucun mouvement.", flush=True)
                    elif was_locked_group and self.level >= 8:
                        self.committed_session = self.committed_session or f"{self.team_name}:global-ritual:{self._current_leader_id()}"
                        self.arrived_at_leader = True
                        self.state = DroneState.JOIN_ELEVATION
                        print("[IA] Niveau max atteint: je reste sur la case, aucun mouvement.", flush=True)
                    else:
                        self.state = DroneState.SEARCH_FOOD
                        self.committed_session = None
                        self.leader_locked = False
                        self.arrived_at_leader = False
                except ValueError:
                    pass
                continue

            if self.sent_commands:
                original_cmd = self.sent_commands.popleft()
                self._handle_response(original_cmd, line)

            self._flush_commands()

    def _handle_response(self, command, response):
        if command == "Inventory":
            clean_text = response.strip("[]")
            parsed = {}
            for item in clean_text.split(","):
                parts = item.strip().split()
                if len(parts) == 2:
                    try:
                        parsed[parts[0]] = int(parts[1])
                    except ValueError:
                        pass
            self.inventory = parsed
            self.inventory_fresh = True
            self._broadcast_inventory()
            return

        if command == "Look":
            clean_text = response.strip("[]")
            self.vision = [tile.strip().split() for tile in clean_text.split(",")]
            self.vision_fresh = True
            return

        if command == "Incantation":
            if response == "Elevation underway":
                print("[IA] Rituel en cours, je me fige.", flush=True)
                self.command_queue.clear()
            elif response == "ko":
                self.inventory_fresh = False
                self.vision_fresh = False
                if self.leader_locked:
                    print("[LEADER] Incantation ko: je reste sur place et je retente quand la case est prete.", flush=True)
                    self.state = DroneState.ELEVATE
                    self.command_queue.clear()
                    return
                print("[IA] Échec incantation (ko). Reset.", flush=True)
                self.state = DroneState.SEARCH_FOOD
                if self.my_ritual_session:
                    self.ignored_ritual_sessions[self.my_ritual_session] = time.time()
                self.ritual_sessions.clear()
                self.my_ritual_session = None
            return

        if command == "Fork" and response == "ok":
            print("[IA] Œuf pondu ! Demande des slots disponibles...")
            self.queue_command("Connect_nbr")
            return

        if command == "Connect_nbr":
            try:
                available_slots = int(response)
                if available_slots > 0:
                    print(f"[IA] {available_slots} slot(s) disponible(s) sur le serveur.")
                    self._try_spawn_child_after_slot(available_slots)
            except ValueError:
                pass
            return

        if command.startswith("Take "):
            resource = command.split(" ", 1)[1]
            if response == "ok":
                self.inventory[resource] = self.inventory.get(resource, 0) + 1
                self.inventory_fresh = True
            else:
                self.vision_fresh = False
            return

        if command.startswith("Set "):
            resource = command.split(" ", 1)[1]
            if response == "ok":
                self.inventory[resource] = max(0, self.inventory.get(resource, 0) - 1)
                self.inventory_fresh = True
            else:
                print(f"[IA] Impossible de déposer '{resource}' (ko).")
            return

    def _register_self(self):
        self.known_drones[self.drone_id] = {
            "level": self.level,
            "inventory": self.inventory.copy() if self.inventory else {"food": 0},
            "last_seen": time.time(),
        }

    def _known_drone_count(self):
        self._register_self()
        now = time.time()
        for drone_id, drone in list(self.known_drones.items()):
            if now - drone.get("last_seen", 0) > 35:
                self.known_drones.pop(drone_id, None)
        return len(self.known_drones)

    def _current_leader_id(self):
        self._register_self()
        alive = sorted(self.known_drones.keys())
        return alive[0] if alive else self.drone_id

    def _is_global_leader(self):
        return self._current_leader_id() == self.drone_id

    def _log_leader_status(self):
        leader_id = self._current_leader_id()
        now = time.time()
        changed = leader_id != self.last_seen_leader_id
        if changed or now - self.last_leader_status_log >= 10.0:
            if leader_id == self.drone_id:
                print(
                    f"[LEADER] Je suis le leader. id={self.drone_id} "
                    f"known={self._known_drone_count()}/{self.MIN_DRONES} "
                    f"food={self.inventory.get('food', 0)}. Je recolte uniquement la food.",
                    flush=True,
                )
            else:
                print(
                    f"[IA] Leader actuel={leader_id}. Mon id={self.drone_id}.",
                    flush=True,
                )
            self.last_seen_leader_id = leader_id
            self.last_leader_status_log = now

    def _total_known_stones(self):
        self._register_self()
        totals = {stone: 0 for stone in self.STONES}
        now = time.time()
        for drone in self.known_drones.values():
            if now - drone.get("last_seen", 0) > 35:
                continue
            inv = drone.get("inventory", {})
            for stone in self.STONES:
                totals[stone] += inv.get(stone, 0)
        return totals

    def _planned_remaining_requirements(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        totals = {stone: 0 for stone in self.STONES}
        if not req:
            return totals
        for i, stone in enumerate(self.STONES):
            totals[stone] = req[i + 1]
        return totals

    def _target_group_size(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return self.MIN_DRONES
        return max(self.MIN_DRONES, req[0])

    def _log_leader_block(self, reason):
        now = time.time()
        if now - self.last_leader_block_log < 5.0:
            return
        print(f"[LEADER] Pas encore prêt: {reason} | id={self.drone_id}", flush=True)
        self.last_leader_block_log = now

    def _global_ritual_conditions_ok(self):
        if not self._is_global_leader():
            return False

        known = self._known_drone_count()
        if known < self.MIN_DRONES:
            self._log_leader_block(f"drones connus {known}/{self.MIN_DRONES}")
            return False

        leader_food = self.inventory.get("food", 0)
        if leader_food < self.LEADER_RITUAL_FOOD_MIN:
            self._log_leader_block(f"food leader {leader_food}/{self.LEADER_RITUAL_FOOD_MIN}")
            return False

        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return False

        now = time.time()
        same_level_known = 0
        same_level_food_ready = 0
        for drone_id, drone in self.known_drones.items():
            if now - drone.get("last_seen", 0) > 35:
                continue
            if drone.get("level", 0) != self.level:
                continue

            same_level_known += 1
            inv = drone.get("inventory", {})
            food = inv.get("food", 0)
            if drone_id == self.drone_id:
                same_level_food_ready += 1
            elif food >= self.COLLECTOR_READY_FOOD_MIN:
                same_level_food_ready += 1

        target_group = self._target_group_size()
        if same_level_known < target_group:
            self._log_leader_block(f"drones meme niveau connus {same_level_known}/{target_group}")
            return False
        if same_level_food_ready < target_group:
            self._log_leader_block(
                f"drones avec food prete {same_level_food_ready}/{target_group} "
                f"min_collecteur={self.COLLECTOR_READY_FOOD_MIN}"
            )
            return False

        have = self._total_known_stones()
        need = self._planned_remaining_requirements()
        for stone in self.STONES:
            if have.get(stone, 0) < need.get(stone, 0):
                self._log_leader_block(f"manque {stone} {have.get(stone, 0)}/{need.get(stone, 0)}")
                return False
        return True

    def _try_spawn_child_after_slot(self, available_slots):
        if available_slots <= 0:
            return
        if self._known_drone_count() >= self.MIN_DRONES:
            return
        if self.manager.spawn_child_drone():
            print(f"[IA] Nouveau processus IA lance automatiquement apres Fork/slot. known={self._known_drone_count()}/{self.MIN_DRONES}")

    def _handle_inventory_broadcast(self, frame):
        drone_id = frame.get("leader_id")
        payload = frame.get("payload", {})
        inventory = payload.get("inventory", {})

        if not drone_id or drone_id == self.drone_id:
            return

        drone_level = frame.get("level", 0)
        self.known_drones[drone_id] = {
            "level": drone_level,
            "inventory": inventory,
            "last_seen": time.time(),
        }

        for session_id, session in list(self.ritual_sessions.items()):
            if session.get("leader_id") == drone_id and drone_level >= session.get("target_level", 99):
                self.ritual_sessions.pop(session_id, None)
                if self.my_ritual_session == session_id:
                    self.my_ritual_session = None

        print(
            f"[IA] Info inventaire reçue de {drone_id}: "
            f"lvl={frame.get('level', 0)} role={payload.get('role', 'drone')} "
            f"known_by_sender={payload.get('known', '?')} inv={inventory}",
            flush=True,
        )

    def _handle_ritual_broadcast(self, k, frame):
        if frame.get("leader_id") == self.drone_id:
            return

        target_level = frame.get("level", 0)
        if self._known_drone_count() < self.MIN_DRONES:
            return
        if frame.get("leader_id") != self._current_leader_id():
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
            print(f"[IA] Drone prêt pour rituel: session={session_id} ready={self._ritual_ready_count(session_id)}")
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
        self.state = DroneState.JOIN_ELEVATION
        self.command_queue.clear()
        if not self.sent_commands:
            self._go_to_ritual_site(session)
        print(f"[IA] Ralliement leader: session={session_id} son={k}.", flush=True)

    def _cleanup_ritual_sessions(self):
        now = time.time()
        expired = [
            session_id
            for session_id, session in self.ritual_sessions.items()
            if now - session.get("last_seen", 0) > self.RITUAL_SESSION_TTL
        ]
        for session_id in expired:
            self.ritual_sessions.pop(session_id, None)
            if self.my_ritual_session == session_id:
                self.my_ritual_session = None

        for session_id, last_seen in list(self.ignored_ritual_sessions.items()):
            if now - last_seen > self.RITUAL_SESSION_TTL:
                self.ignored_ritual_sessions.pop(session_id, None)

        for session in self.ritual_sessions.values():
            ready = session.get("ready", {})
            for drone_id, last_seen in list(ready.items()):
                if now - last_seen > self.RITUAL_SESSION_TTL:
                    ready.pop(drone_id, None)

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

    def _best_ritual_session(self):
        self._cleanup_ritual_sessions()
        if self.my_ritual_session:
            session = self.ritual_sessions.get(self.my_ritual_session)
            if (
                session
                and session.get("target_level") == self.level + 1
                and session.get("required", 0) > 1
            ):
                return self.my_ritual_session, session
            self.my_ritual_session = None

        candidates = [
            (session_id, session)
            for session_id, session in self.ritual_sessions.items()
            if session.get("target_level") == self.level + 1
            and session.get("required", 0) > 1
            and session_id not in self.ignored_ritual_sessions
        ]
        if not candidates:
            return None, None
        candidates.sort(key=lambda item: (self._ritual_ready_count(item[0]), item[1].get("last_seen", 0)), reverse=True)
        return candidates[0]

    def _register_my_ritual_site(self, session_id, required):
        now = time.time()
        self.my_ritual_session = session_id
        session = self.ritual_sessions.setdefault(
            session_id,
            {
                "leader_id": self.drone_id,
                "target_level": self.level + 1,
                "required": required,
                "ready": {},
                "last_direction": 0,
                "last_seen": now,
                "created_at": now,
            },
        )
        session["leader_id"] = self.drone_id
        session["target_level"] = self.level + 1
        session["required"] = required
        session["last_direction"] = 0
        session["last_seen"] = now
        session["ready"][self.drone_id] = now

    def _broadcast_ritual_site(self, req):
        now = time.time()
        if now - self.last_ritual_site_broadcast < self.RITUAL_SITE_COOLDOWN:
            return

        target_group = self._target_group_size()
        packet = self.broadcast.build_ritual_site_packet(
            self.level + 1,
            target_group,
            self.inventory.get("food", 0),
            self._known_drone_count(),
        )
        session_id = f"{self.team_name}:global-ritual:{self.drone_id}"
        self.leader_locked = True
        self.committed_session = session_id
        self.queue_command(f"Broadcast {packet}")
        self.last_ritual_site_broadcast = now
        self._register_my_ritual_site(session_id, target_group)
        print(f"[LEADER] Appel regroupement global session={session_id} niveau={self.level}->{self.level + 1} target={target_group}", flush=True)
        print("[LEADER] RALLIEMENT VERROUILLE: je ne bouge plus, j'attends les drones sur ma case.", flush=True)

    def _broadcast_ritual_ready(self, session_id):
        now = time.time()
        if now - self.last_ritual_ready_broadcast < self.RITUAL_READY_COOLDOWN:
            return

        packet = self.broadcast.build_ritual_ready_packet(
            self.level + 1,
            session_id,
            self.inventory.get("food", 0),
        )
        self.queue_command(f"Broadcast {packet}")
        self.last_ritual_ready_broadcast = now

        session = self.ritual_sessions.setdefault(
            session_id,
            {
                "leader_id": None,
                "target_level": self.level + 1,
                "required": 0,
                "ready": {},
                "last_direction": 0,
                "last_seen": now,
                "created_at": now,
            },
        )
        session["ready"][self.drone_id] = now
        session["last_seen"] = now

    def _search_food_action(self):
        for index, tile in enumerate(self.vision):
            if "food" in tile:
                self.move_to_target(index)
                self.queue_command("Take food")
                return
        self.queue_command("Forward")

    def _tile_resource_count(self, stone):
        if not self.vision:
            return 0
        return self.vision[0].count(stone)

    def _deposit_all_stones_for_ritual(self):
        deposited = False
        for stone in self.STONES:
            amount = self.inventory.get(stone, 0)
            for _ in range(amount):
                self.queue_command(f"Set {stone}")
                deposited = True
        if deposited:
            print("[IA] Pierres deposees sur la case du leader.")
            self.inventory_fresh = False
            self.vision_fresh = False
        return deposited

    def _needed_global_stones_to_collect(self):
        need = self._planned_remaining_requirements()
        have = self._total_known_stones()
        missing = {}
        for stone in self.STONES:
            if have.get(stone, 0) < need.get(stone, 0):
                missing[stone] = need[stone] - have.get(stone, 0)
        return missing

    def _go_to_ritual_site(self, session):
        direction = session.get("last_direction", 0)
        session_id = self.committed_session or self.my_ritual_session
        now = time.time()

        if self.arrived_at_leader:
            self.command_queue.clear()
            if now - self.last_wait_on_leader_log > 2.0:
                print("[IA] STOP LEADER: sur la case du leader, je ne bouge plus.", flush=True)
                self.last_wait_on_leader_log = now
            if not self.inventory_fresh:
                self.queue_command("Inventory")
                return
            self._deposit_all_stones_for_ritual()
            return

        if direction == 0:
            self.arrived_at_leader = True
            self.command_queue.clear()
            print("[IA] ARRIVE LEADER: son direction=0, stop definitif et depot des pierres.", flush=True)
            if not self.inventory_fresh:
                self.queue_command("Inventory")
                return
            self._deposit_all_stones_for_ritual()
            return
        direction_seen = session.get("direction_updated_at", session.get("last_seen", 0))
        already_used_this_broadcast = (
            self.last_join_move_session == session_id
            and direction_seen <= self.last_join_move_seen
        )
        if already_used_this_broadcast:
            return

        if now - self.last_fast_join_log > 1.0:
            print(f"[IA] SUIVI SON LEADER: direction={direction}.", flush=True)
            self.last_fast_join_log = now
        self.last_nonzero_leader_direction = direction
        self.last_join_move_session = session_id
        self.last_join_move_seen = direction_seen
        self.join_steps_done += 1
        self._move_one_step_to_sound(direction)

    def _handle_broadcast(self, k, text):
        frame = self.broadcast.parse_incoming(text)
        if not frame:
            return

        kind = frame.get("kind")

        if kind == "inventory-status":
            self._handle_inventory_broadcast(frame)
            return

        if kind in ("ritual-site", "ritual-ready"):
            self._handle_ritual_broadcast(k, frame)
            return

        return

    def _team_elevation(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return False

        now = time.time()
        same_level_helpers = 1
        total_stones = {
            stone: self.inventory.get(stone, 0)
            for stone in self.STONES
        }

        for drone in self.known_drones.values():
            if now - drone.get("last_seen", 0) > 20:
                continue

            if drone.get("level") != self.level:
                continue

            inv = drone.get("inventory", {})
            if inv.get("food", 0) < self.JOIN_RITUAL_FOOD_MIN:
                continue

            same_level_helpers += 1

            for stone in self.STONES:
                total_stones[stone] += inv.get(stone, 0)

        if same_level_helpers < req[0]:
            return False

        for i, stone in enumerate(self.STONES):
            if total_stones[stone] < req[i + 1]:
                return False

        return True

    def _move_one_step_to_sound(self, k):
        routes = {
            1: ["Forward"],
            2: ["Forward"],
            3: ["Left", "Forward"],
            4: ["Left", "Forward"],
            5: ["Left", "Left", "Forward"],
            6: ["Right", "Forward"],
            7: ["Right", "Forward"],
            8: ["Forward"],
        }
        for command in routes.get(k, []):
            self.queue_command(command)

    def _move_to_sound(self, k):
        if k == 1:
            self.queue_command("Forward")
        elif k == 2:
            self.queue_command("Forward")
            self.queue_command("Right")
            self.queue_command("Forward")
            self.queue_command("Left")
        elif k == 3:
            self.queue_command("Right")
            self.queue_command("Forward")
            self.queue_command("Left")
        elif k == 4:
            self.queue_command("Right")
            self.queue_command("Right")
            self.queue_command("Forward")
            self.queue_command("Right")
            self.queue_command("Forward")
            self.queue_command("Left")
            self.queue_command("Left")
        elif k == 5:
            self.queue_command("Right")
            self.queue_command("Right")
            self.queue_command("Forward")
            self.queue_command("Right")
            self.queue_command("Right")
        elif k == 6:
            self.queue_command("Left")
            self.queue_command("Left")
            self.queue_command("Forward")
            self.queue_command("Left")
            self.queue_command("Forward")
            self.queue_command("Right")
            self.queue_command("Right")
        elif k == 7:
            self.queue_command("Left")
            self.queue_command("Forward")
            self.queue_command("Right")
        elif k == 8:
            self.queue_command("Forward")
            self.queue_command("Left")
            self.queue_command("Forward")
            self.queue_command("Right")

    def move_to_target(self, target_index):
        if target_index == 0:
            return
        r = int(math.sqrt(target_index))
        c = target_index - (r * r) - r
        for _ in range(r):
            self.queue_command("Forward")
        if c < 0:
            self.queue_command("Left")
            for _ in range(abs(c)):
                self.queue_command("Forward")
            self.queue_command("Right")
        elif c > 0:
            self.queue_command("Right")
            for _ in range(abs(c)):
                self.queue_command("Forward")
            self.queue_command("Left")

    def _missing_stones(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return {}
        return {
            stone: req[i + 1] - self.inventory.get(stone, 0)
            for i, stone in enumerate(self.STONES)
            if self.inventory.get(stone, 0) < req[i + 1]
        }

    def _is_ready_for_elevation(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return False
        return all(
            self.inventory.get(stone, 0) >= req[i + 1]
            for i, stone in enumerate(self.STONES)
        )
    
    def _broadcast_inventory(self):
        now = time.time()

        if now - self.last_inventory_broadcast < self.INVENTORY_BROADCAST_COOLDOWN:
            return

        packet = self.broadcast.build_inventory_packet(
            self.level,
            {
                "food": self.inventory.get("food", 0),
                "linemate": self.inventory.get("linemate", 0),
                "deraumere": self.inventory.get("deraumere", 0),
                "sibur": self.inventory.get("sibur", 0),
                "mendiane": self.inventory.get("mendiane", 0),
                "phiras": self.inventory.get("phiras", 0),
                "thystame": self.inventory.get("thystame", 0),
            },
            role="leader" if self._is_global_leader() else "drone",
            known=self._known_drone_count(),
        )

        self.queue_command(f"Broadcast {packet}")
        self.last_inventory_broadcast = now

    def think(self):
        if self.command_queue or self.sent_commands:
            return

        if not self.leader_locked and time.time() - self.last_connect_nbr_time > self.CONNECT_NBR_COOLDOWN:
            self.queue_command("Connect_nbr")
            self.last_connect_nbr_time = time.time()
            return

        if self.committed_session and not (self.leader_locked and self._is_global_leader()):
            self._cleanup_ritual_sessions()
            best_session_id, best_session = self._best_ritual_session()
            if best_session_id and best_session:
                self.committed_session = best_session_id
                direction = best_session.get("last_direction", 0)
                if direction == 0 and self.arrived_at_leader:
                    self._broadcast_ritual_ready(best_session_id)
                self._go_to_ritual_site(best_session)
            else:
                if time.time() - self.last_fast_join_log > 2.0:
                    print("[IA] RALLIEMENT: j'attends le prochain son du leader.", flush=True)
                    self.last_fast_join_log = time.time()
            self.inventory_fresh = False
            self.vision_fresh = False
            return

        needs_refresh = False
        if not self.inventory_fresh:
            self.queue_command("Inventory")
            needs_refresh = True
        if not self.vision_fresh:
            self.queue_command("Look")
            needs_refresh = True
        if needs_refresh:
            return

        self._cleanup_ritual_sessions()
        self._log_leader_status()
        if self.committed_session and self.committed_session not in self.ritual_sessions:
            self.committed_session = None
            self.my_ritual_session = None
        current_food = self.inventory.get("food", 0)
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        best_session_id, best_session = self._best_ritual_session()

        known_count = self._known_drone_count()

        is_leader = self._is_global_leader()

        if self.leader_locked and is_leader:
            self.state = DroneState.ELEVATE
        elif self.committed_session:
            self.state = DroneState.JOIN_ELEVATION
        elif current_food < self.food_threshold:
            self.state = DroneState.SEARCH_FOOD
        elif known_count < self.MIN_DRONES and current_food >= self.FORK_FOOD_MIN and (time.time() - self.last_fork_time) > self.FORK_COOLDOWN:
            self.state = DroneState.FORK
        elif not is_leader and current_food < self.COLLECTOR_FOOD_TARGET:
            self.state = DroneState.SEARCH_FOOD
        elif best_session_id and not is_leader:
            if current_food < self.EMERGENCY_FOOD_MIN:
                self.state = DroneState.RITUAL_FOOD
            else:
                self.committed_session = best_session_id
                self.state = DroneState.JOIN_ELEVATION
        elif req and self._global_ritual_conditions_ok():
            self.state = DroneState.ELEVATE
        elif is_leader:
            self.state = DroneState.SEARCH_FOOD
        else:
            self.state = DroneState.SEARCH_STONES

        if self.state == DroneState.SEARCH_FOOD:
            self._search_food_action()

        elif self.state == DroneState.RITUAL_FOOD:
            self._search_food_action()

        elif self.state == DroneState.SEARCH_STONES:
            if current_food < self.COLLECTOR_FOOD_TARGET + 10:
                food_found = False
                for index, tile in enumerate(self.vision):
                    if "food" in tile:
                        self.move_to_target(index)
                        self.queue_command("Take food")
                        food_found = True
                        break
                if food_found:
                    self.inventory_fresh = False
                    self.vision_fresh = False
                    return
            missing = self._needed_global_stones_to_collect() or {stone: 1 for stone in self.STONES}
            found = False
            for index, tile in enumerate(self.vision):
                for stone in missing:
                    if stone in tile:
                        self.move_to_target(index)
                        self.queue_command(f"Take {stone}")
                        found = True
                        break
                if found:
                    break
            if not found:
                self.queue_command("Forward")
                self.queue_command("Left")

        elif self.state == DroneState.FORK:
            self.queue_command("Fork")
            self.last_fork_time = time.time()
            self.state = DroneState.SEARCH_FOOD

        elif self.state == DroneState.JOIN_ELEVATION:
            if best_session_id:
                self.committed_session = best_session_id
                if best_session.get("last_direction", 0) == 0 and self.arrived_at_leader:
                    self._broadcast_ritual_ready(best_session_id)
                self._go_to_ritual_site(best_session)
            else:
                if time.time() - self.last_fast_join_log > 2.0:
                    print("[IA] RALLIEMENT: j'attends le prochain son du leader.", flush=True)
                    self.last_fast_join_log = time.time()

        elif self.state == DroneState.ELEVATE:
            if not req or not self._is_global_leader():
                self.queue_command("Look")
                return
            if not self.leader_locked and not self._global_ritual_conditions_ok():
                self.queue_command("Look")
                return

            players_on_tile = self.vision[0].count("player") if self.vision else 0

            session_id = f"{self.team_name}:ritual:{self.level + 1}:{self.drone_id}"
            if session_id in self.ignored_ritual_sessions and not self.leader_locked:
                self.state = DroneState.SEARCH_FOOD
                self._search_food_action()
                self.inventory_fresh = False
                self.vision_fresh = False
                return

            self._broadcast_ritual_site(req)
            session_id = self.my_ritual_session
            ready_count = self._ritual_ready_count(session_id) if session_id else 1

            target_group = self._target_group_size()
            required_players = max(self.MIN_DRONES, target_group, req[0])
            enough_stones_on_tile = True
            for i, stone in enumerate(self.STONES):
                required_amount = req[i + 1]
                on_tile = self._tile_resource_count(stone)
                missing_on_tile = max(0, required_amount - on_tile)
                if missing_on_tile > 0:
                    enough_stones_on_tile = False
                    continue

            if players_on_tile >= required_players and enough_stones_on_tile:
                print(f"[LEADER] Conditions locales OK ({players_on_tile} joueurs sur case, ready={ready_count}), lancement incantation groupe.", flush=True)
                self.queue_command("Incantation")
            else:
                print(f"[LEADER] Attente regroupement STRICT joueurs_sur_case={players_on_tile}/{required_players} ready={ready_count} stones={enough_stones_on_tile}", flush=True)
                self.queue_command("Look")

        self.inventory_fresh = False
        self.vision_fresh = False
