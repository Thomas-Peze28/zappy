import os
import time

from .zappy_broadcast import ZappyBroadcastCoordinator
from .zappy_drone_registry import ZappyDroneRegistry
from .zappy_movement import ZappyMovement
from .zappy_network import ZappyNetwork
from .zappy_ritual import ZappyRitual
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
    INVENTORY_BROADCAST_COOLDOWN = 8.0
    def __init__(self, manager, sock, team_name, remaining_slots, map_x, map_y):
        self.manager = manager
        self.team_name = team_name
        self.remaining_slots = remaining_slots
        self.map_x = map_x
        self.map_y = map_y
        self.sock = sock
        self.inventory = {}
        self.vision = []
        self.inventory_fresh = False
        self.vision_fresh = False
        self.state = DroneState.SEARCH_FOOD
        self.level = 1
        self.food_threshold = self.FOOD_SEARCH_MIN
        self.last_fork_time = 0.0
        self.last_connect_nbr_time = time.time()
        self.last_inventory_broadcast = 0.0
        self.last_leader_block_log = 0.0
        self.last_leader_status_log = 0.0
        self.last_seen_leader_id = None
        self.last_fast_join_log = 0.0
        self.drone_id = f"{os.getpid()}-{sock.fileno()}-{int(time.time() * 1000)}"
        self.broadcast = ZappyBroadcastCoordinator(team_name, self.drone_id)
        self.registry = ZappyDroneRegistry(self.drone_id, self.MIN_DRONES, manager)
        self.net = ZappyNetwork(sock, manager, self._handle_response)
        self.net.set_broadcast_handler(self._handle_broadcast)
        self.net.set_level_up_handler(self._handle_level_up)
        self.movement = ZappyMovement(self.net.queue_command)
        self.ritual = ZappyRitual(
            self.drone_id, team_name,
            self.net.queue_command,
            self.broadcast, self.registry, self.movement,
        )

    def _handle_level_up(self, line):
        try:
            was_locked_group = (
                self.ritual.leader_locked
                or self.ritual.committed_session is not None
                or self.ritual.arrived_at_leader
            )
            was_locked_leader = self.ritual.leader_locked and self._is_global_leader()
            self.level = int(line.split(":")[1].strip())
            print(f"[IA] Niveau {self.level} atteint !", flush=True)
            self.net.clear_queue()
            self.inventory_fresh = False
            self.vision_fresh = False
            self.ritual.set_level(self.level)
            self.ritual._current_leader_id_snapshot = self.registry.current_leader_id(self.level, self.inventory)
            outcome = self.ritual.reset_after_level_up(
                self.level, self.team_name, self.drone_id,
                was_locked_group, was_locked_leader,
                self._target_group_size,
            )

            if outcome == "leader":
                self.state = DroneState.ELEVATE
                print("[LEADER] Rituel reussi: je reste bloque sur la case pour enchainer le prochain rituel.", flush=True)
            elif outcome == "follower":
                self.state = DroneState.JOIN_ELEVATION
                print("[IA] Rituel reussi: je reste sur la case du leader, aucun mouvement.", flush=True)
            elif outcome == "max_level":
                self.state = DroneState.JOIN_ELEVATION
                print("[IA] Niveau max atteint: je reste sur la case, aucun mouvement.", flush=True)
            else:
                self.state = DroneState.SEARCH_FOOD
        except ValueError:
            pass

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
                self.net.clear_queue()
            elif response == "ko":
                self.inventory_fresh = False
                self.vision_fresh = False
                if self.ritual.leader_locked:
                    print("[LEADER] Incantation ko: je reste sur place et je retente quand la case est prete.", flush=True)
                    self.state = DroneState.ELEVATE
                    self.net.clear_queue()
                    return
                print("[IA] Échec incantation (ko). Reset.", flush=True)
                self.state = DroneState.SEARCH_FOOD
                if self.ritual.my_ritual_session:
                    self.ritual.ignored_ritual_sessions[self.ritual.my_ritual_session] = time.time()
                self.ritual.ritual_sessions.clear()
                self.ritual.my_ritual_session = None
            return

        if command == "Fork" and response == "ok":
            print("[IA] Œuf pondu ! Demande des slots disponibles...")
            self.net.queue_command("Connect_nbr")
            return

        if command == "Connect_nbr":
            try:
                available_slots = int(response)
                if available_slots > 0:
                    print(f"[IA] {available_slots} slot(s) disponible(s) sur le serveur.")
                    self.registry.try_spawn_child(available_slots, self.level, self.inventory)
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

    def _handle_broadcast(self, k, text):
        frame = self.broadcast.parse_incoming(text)
        if not frame:
            return

        kind = frame.get("kind")

        if kind == "inventory-status":
            self._handle_inventory_broadcast(frame)
            return

        if kind in ("ritual-site", "ritual-ready"):
            switched = self.ritual.handle_ritual_broadcast(
                k, frame,
                self.MIN_DRONES,
                self.registry.current_leader_id(self.level, self.inventory),
            )
            if switched:
                self.state = DroneState.JOIN_ELEVATION
                self.net.clear_queue()
                if not self.net.sent_commands:
                    best_id, best_session = self.ritual.best_ritual_session()
                    if best_session is not None:
                        self.ritual.go_to_ritual_site(
                            best_session,
                            self._deposit_all_stones_for_ritual,
                            self.inventory_fresh,
                            self.net,
                        )

    def _handle_inventory_broadcast(self, frame):
        drone_id = frame.get("leader_id")
        payload = frame.get("payload", {})
        inventory = payload.get("inventory", {})

        if not drone_id or drone_id == self.drone_id:
            return

        drone_level = frame.get("level", 0)
        self.registry.update_from_broadcast(drone_id, drone_level, inventory)

        for session_id, session in list(self.ritual.ritual_sessions.items()):
            if session.get("leader_id") == drone_id and drone_level >= session.get("target_level", 99):
                self.ritual.ritual_sessions.pop(session_id, None)
                if self.ritual.my_ritual_session == session_id:
                    self.ritual.my_ritual_session = None

        print(
            f"[IA] Info inventaire reçue de {drone_id}: "
            f"lvl={frame.get('level', 0)} role={payload.get('role', 'drone')} "
            f"known_by_sender={payload.get('known', '?')} inv={inventory}",
            flush=True,
        )

    def _broadcast_inventory(self):
        now = time.time()
        if now - self.last_inventory_broadcast < self.INVENTORY_BROADCAST_COOLDOWN:
            return

        packet = self.broadcast.build_inventory_packet(
            self.level,
            {stone: self.inventory.get(stone, 0) for stone in self.STONES + ["food"]},
            role="leader" if self._is_global_leader() else "drone",
            known=self._known_drone_count(),
        )
        self.net.queue_command(f"Broadcast {packet}")
        self.last_inventory_broadcast = now

    def _known_drone_count(self):
        return self.registry.known_drone_count(self.level, self.inventory)

    def _is_global_leader(self):
        return self.registry.is_global_leader(self.level, self.inventory)

    def _target_group_size(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not req:
            return self.MIN_DRONES
        return max(self.MIN_DRONES, req[0])

    def _log_leader_status(self):
        leader_id = self.registry.current_leader_id(self.level, self.inventory)
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
                print(f"[IA] Leader actuel={leader_id}. Mon id={self.drone_id}.", flush=True)
            self.last_seen_leader_id = leader_id
            self.last_leader_status_log = now

    def _log_leader_block(self, reason):
        now = time.time()
        if now - self.last_leader_block_log < 5.0:
            return
        print(f"[LEADER] Pas encore prêt: {reason} | id={self.drone_id}", flush=True)
        self.last_leader_block_log = now

    def _planned_remaining_requirements(self):
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        totals = {stone: 0 for stone in self.STONES}
        if not req:
            return totals
        for i, stone in enumerate(self.STONES):
            totals[stone] = req[i + 1]
        return totals

    def _needed_global_stones_to_collect(self):
        need = self._planned_remaining_requirements()
        have = self.registry.total_known_stones()
        return {
            stone: need[stone] - have.get(stone, 0)
            for stone in self.STONES
            if have.get(stone, 0) < need.get(stone, 0)
        }

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
        for drone_id, drone in self.registry.known_drones.items():
            if now - drone.get("last_seen", 0) > 35:
                continue
            if drone.get("level", 0) != self.level:
                continue
            same_level_known += 1
            food = drone.get("inventory", {}).get("food", 0)
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

        have = self.registry.total_known_stones()
        need = self._planned_remaining_requirements()
        for stone in self.STONES:
            if have.get(stone, 0) < need.get(stone, 0):
                self._log_leader_block(f"manque {stone} {have.get(stone, 0)}/{need.get(stone, 0)}")
                return False
        return True

    def _tile_resource_count(self, stone):
        if not self.vision:
            return 0
        return self.vision[0].count(stone)

    def _deposit_all_stones_for_ritual(self):
        deposited = False
        for stone in self.STONES:
            for _ in range(self.inventory.get(stone, 0)):
                self.net.queue_command(f"Set {stone}")
                deposited = True
        if deposited:
            print("[IA] Pierres deposees sur la case du leader.")
            self.inventory_fresh = False
            self.vision_fresh = False
        return deposited

    def _search_food_action(self):
        for index, tile in enumerate(self.vision):
            if "food" in tile:
                self.movement.move_to_target(index)
                self.net.queue_command("Take food")
                return
        self.net.queue_command("Forward")

    def think(self):
        if self.net.has_pending():
            return

        self.ritual.set_level(self.level)

        if not self.ritual.leader_locked and time.time() - self.last_connect_nbr_time > self.CONNECT_NBR_COOLDOWN:
            self.net.queue_command("Connect_nbr")
            self.last_connect_nbr_time = time.time()
            return

        if self.ritual.committed_session and not (self.ritual.leader_locked and self._is_global_leader()):
            self.ritual.cleanup()
            best_session_id, best_session = self.ritual.best_ritual_session()
            if best_session_id and best_session is not None:
                self.ritual.committed_session = best_session_id
                direction = best_session.get("last_direction", 0)
                if direction == 0 and self.ritual.arrived_at_leader:
                    self.ritual.broadcast_ritual_ready(
                        best_session_id, self.inventory.get("food", 0)
                    )
                self.ritual.go_to_ritual_site(
                    best_session,
                    self._deposit_all_stones_for_ritual,
                    self.inventory_fresh,
                    self.net,
                )
            else:
                if time.time() - self.last_fast_join_log > 2.0:
                    print("[IA] RALLIEMENT: j'attends le prochain son du leader.", flush=True)
                    self.last_fast_join_log = time.time()
            self.inventory_fresh = False
            self.vision_fresh = False
            return

        needs_refresh = False
        if not self.inventory_fresh:
            self.net.queue_command("Inventory")
            needs_refresh = True
        if not self.vision_fresh:
            self.net.queue_command("Look")
            needs_refresh = True
        if needs_refresh:
            return

        self.ritual.cleanup()
        self._log_leader_status()
        if self.ritual.committed_session and self.ritual.committed_session not in self.ritual.ritual_sessions:
            self.ritual.committed_session = None
            self.ritual.my_ritual_session = None

        current_food = self.inventory.get("food", 0)
        req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        best_session_id, best_session = self.ritual.best_ritual_session()
        known_count = self._known_drone_count()
        is_leader = self._is_global_leader()
        if self.ritual.leader_locked and is_leader:
            self.state = DroneState.ELEVATE
        elif self.ritual.committed_session:
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
                self.ritual.committed_session = best_session_id
                self.state = DroneState.JOIN_ELEVATION
        elif req and self._global_ritual_conditions_ok():
            self.state = DroneState.ELEVATE
        elif is_leader:
            self.state = DroneState.SEARCH_FOOD
        else:
            self.state = DroneState.SEARCH_STONES
        if self.state in (DroneState.SEARCH_FOOD, DroneState.RITUAL_FOOD):
            self._search_food_action()

        elif self.state == DroneState.SEARCH_STONES:
            if current_food < self.COLLECTOR_FOOD_TARGET + 10:
                for index, tile in enumerate(self.vision):
                    if "food" in tile:
                        self.movement.move_to_target(index)
                        self.net.queue_command("Take food")
                        self.inventory_fresh = False
                        self.vision_fresh = False
                        return
            missing = self._needed_global_stones_to_collect() or {stone: 1 for stone in self.STONES}
            found = False
            for index, tile in enumerate(self.vision):
                for stone in missing:
                    if stone in tile:
                        self.movement.move_to_target(index)
                        self.net.queue_command(f"Take {stone}")
                        found = True
                        break
                if found:
                    break
            if not found:
                self.net.queue_command("Forward")
                self.net.queue_command("Left")

        elif self.state == DroneState.FORK:
            self.net.queue_command("Fork")
            self.last_fork_time = time.time()
            self.state = DroneState.SEARCH_FOOD

        elif self.state == DroneState.JOIN_ELEVATION:
            if best_session_id and best_session is not None:
                self.ritual.committed_session = best_session_id
                if best_session.get("last_direction", 0) == 0 and self.ritual.arrived_at_leader:
                    self.ritual.broadcast_ritual_ready(best_session_id, self.inventory.get("food", 0))
                self.ritual.go_to_ritual_site(
                    best_session,
                    self._deposit_all_stones_for_ritual,
                    self.inventory_fresh,
                    self.net,
                )
            else:
                if time.time() - self.last_fast_join_log > 2.0:
                    print("[IA] RALLIEMENT: j'attends le prochain son du leader.", flush=True)
                    self.last_fast_join_log = time.time()

        elif self.state == DroneState.ELEVATE:
            if not req or not self._is_global_leader():
                self.net.queue_command("Look")
                return
            if not self.ritual.leader_locked and not self._global_ritual_conditions_ok():
                self.net.queue_command("Look")
                return

            players_on_tile = self.vision[0].count("player") if self.vision else 0
            session_id = f"{self.team_name}:ritual:{self.level + 1}:{self.drone_id}"
            if session_id in self.ritual.ignored_ritual_sessions and not self.ritual.leader_locked:
                self.state = DroneState.SEARCH_FOOD
                self._search_food_action()
                self.inventory_fresh = False
                self.vision_fresh = False
                return

            self.ritual.broadcast_ritual_site(
                req, self._target_group_size(),
                self.inventory.get("food", 0),
                self._known_drone_count(),
            )
            session_id = self.ritual.my_ritual_session
            ready_count = self.ritual.ritual_ready_count(session_id) if session_id else 1
            target_group = self._target_group_size()
            required_players = max(self.MIN_DRONES, target_group, req[0])

            enough_stones_on_tile = all(
                self._tile_resource_count(stone) >= req[i + 1]
                for i, stone in enumerate(self.STONES)
            )

            if players_on_tile >= required_players and enough_stones_on_tile:
                print(
                    f"[LEADER] Conditions locales OK ({players_on_tile} joueurs sur case, ready={ready_count}), "
                    f"lancement incantation groupe.",
                    flush=True,
                )
                self.net.queue_command("Incantation")
            else:
                print(
                    f"[LEADER] Attente regroupement STRICT joueurs_sur_case={players_on_tile}/{required_players} "
                    f"ready={ready_count} stones={enough_stones_on_tile}",
                    flush=True,
                )
                self.net.queue_command("Look")

        self.inventory_fresh = False
        self.vision_fresh = False
