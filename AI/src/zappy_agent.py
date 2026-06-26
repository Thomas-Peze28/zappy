import math
import os
import time
from collections import deque

from .zappy_state import DroneState


class ZappyAI:
    """
    Strategie simple objectif level 8:
    - plus de livraison de stones au pillar
    - tous les drones survivent, ramassent food + stones utiles
    - level 1 -> 2 se fait seul
    - a partir du level 2, le drone qui a les stones appelle les drones du meme level
    - le leader pose les stones puis lance Incantation
    """

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

    # Gardes pour compatibilite avec ton manager/spawner, mais le comportement est le meme.
    ROLE_PILLAR = "pillar"
    ROLE_STONE = "stone_collector"

    MAX_PENDING_COMMANDS = 10

    EMERGENCY_FOOD = 8
    FOOD_TARGET = 28
    FOOD_BEFORE_CALL = 24
    FOOD_BEFORE_FOLLOW = 18
    FOOD_BEFORE_INCANT = 20
    FOOD_FEED_RESERVE = 22

    CALL_COOLDOWN = 3.0
    CALL_TIMEOUT = 18.0
    FEED_COOLDOWN = 2.0
    INCANT_FAIL_COOLDOWN = 6.0
    CONNECT_NBR_COOLDOWN = 5.0
    FORK_MIN_FOOD = 45
    TARGET_DRONES = 10

    def __init__(self, manager, sock, team_name, remaining_slots, map_x, map_y, role=ROLE_STONE):
        self.manager = manager
        self.sock = sock
        self.team_name = team_name
        self.remaining_slots = remaining_slots
        self.map_x = map_x
        self.map_y = map_y
        self.role = role if role in (self.ROLE_PILLAR, self.ROLE_STONE) else self.ROLE_STONE

        self.read_buffer = ""
        self.command_queue = deque()
        self.sent_commands = deque()

        self.inventory = {}
        self.vision = []
        self.inventory_fresh = False
        self.vision_fresh = False

        self.state = DroneState.SEARCH_FOOD
        self.level = 1
        self.drone_id = f"{os.getpid()}-{self.sock.fileno()}-{int(time.time() * 1000)}"

        self.available_slots = remaining_slots
        self.initial_team_capacity = max(1, remaining_slots + 1)

        self.search_turn = 0
        self.last_call_time = 0.0
        self.last_connect_nbr_time = 0.0
        self.last_feed_time = 0.0
        self.incant_blocked_until = 0.0

        self.leading_elevation = False
        self.joining_elevation = False
        self.elevation_direction = None
        self.elevation_deadline = 0.0
        self.elevation_leader_id = None

        print(f"[IA] Role: {self.role} | Strategie: level8_swarm")

    def queue_command(self, command):
        if len(self.command_queue) + len(self.sent_commands) >= self.MAX_PENDING_COMMANDS:
            return False
        self.command_queue.append(command)
        self._flush_commands()
        return True

    def _flush_commands(self):
        while self.command_queue and len(self.sent_commands) < self.MAX_PENDING_COMMANDS:
            command = self.command_queue.popleft()
            try:
                self.sock.sendall((command + "\n").encode("utf-8"))
                self.sent_commands.append(command)
            except Exception as error:
                print(f"[IA] Erreur send '{command}': {error}")
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
        except Exception as error:
            print(f"[IA] Erreur receive_data: {error}")
            self.manager.disconnect()

    def _process_buffer(self):
        while "\n" in self.read_buffer:
            line, self.read_buffer = self.read_buffer.split("\n", 1)
            self._handle_server_line(line.strip())
            if self.manager.client is None:
                return
            self._flush_commands()

    def _handle_server_line(self, line):
        if not line:
            return

        if line == "dead":
            print("[*] Le drone est mort de faim.")
            self.manager.disconnect()
            return

        if line.startswith("message"):
            self._handle_message_line(line)
            return

        if line.startswith("eject:"):
            return

        if line.startswith("Current level:"):
            self._handle_level_up(line)
            return

        if not self.sent_commands:
            return

        command = self.sent_commands.popleft()
        self._handle_response(command, line)

    def _handle_response(self, command, response):
        if command == "Inventory":
            self.inventory = self._parse_inventory(response)
            self.inventory_fresh = True
            return

        if command == "Look":
            self.vision = self._parse_vision(response)
            self.vision_fresh = True
            return

        if command == "Connect_nbr":
            try:
                self.available_slots = int(response)
            except ValueError:
                pass
            return

        if command == "Fork":
            if response == "ok":
                print("[IA] Fork ok.")
            return

        if command == "Incantation":
            self._handle_incantation_response(response)
            return

        if command.startswith("Take"):
            self.inventory_fresh = False
            self.vision_fresh = False
            return

        if command.startswith("Set"):
            if response == "ko":
                resource = command.split(" ", 1)[-1]
                print(f"[IA] Set {resource} ko, refresh.")
            self.inventory_fresh = False
            self.vision_fresh = False
            return

    def _handle_level_up(self, line):
        try:
            self.level = int(line.split(":", 1)[1].strip())
        except ValueError:
            return

        print(f"[IA] Niveau {self.level} atteint !")
        self.leading_elevation = False
        self.joining_elevation = False
        self.elevation_direction = None
        self.elevation_deadline = 0.0
        self.elevation_leader_id = None
        self.state = DroneState.SEARCH_FOOD
        self._invalidate_data()

    def _handle_incantation_response(self, response):
        if response == "Elevation underway":
            print("[IA] Incantation en cours.")
            self.command_queue.clear()
            return

        if response == "ko":
            print("[IA] Incantation ko, reset + cooldown.")
            self.leading_elevation = False
            self.joining_elevation = False
            self.elevation_direction = None
            self.elevation_deadline = 0.0
            self.incant_blocked_until = time.time() + self.INCANT_FAIL_COOLDOWN
            self._invalidate_data()

    def _parse_inventory(self, response):
        inventory = {}
        for item in response.strip("[]").split(","):
            parts = item.strip().split()
            if len(parts) != 2:
                continue
            try:
                inventory[parts[0]] = int(parts[1])
            except ValueError:
                pass
        return inventory

    def _parse_vision(self, response):
        return [tile.strip().split() for tile in response.strip("[]").split(",")]

    def _handle_message_line(self, line):
        parts = line.replace("message ", "", 1).split(",", 1)
        if len(parts) != 2:
            return

        try:
            direction = int(parts[0].strip())
        except ValueError:
            return

        self._handle_broadcast(direction, parts[1].strip())

    def _handle_broadcast(self, direction, text):
        parts = text.split("|")
        if len(parts) < 5:
            return
        if parts[0] != self.team_name:
            return
        if parts[1] != "ELEVATE":
            return

        try:
            call_level = int(parts[2])
        except ValueError:
            return

        leader_id = parts[3]
        if leader_id == self.drone_id:
            return
        if call_level != self.level:
            return
        if self.inventory.get("food", 0) < self.FOOD_BEFORE_FOLLOW:
            return
        if self.leading_elevation:
            return

        now = time.time()
        if self.joining_elevation and now < self.elevation_deadline:
            if self.elevation_leader_id != leader_id:
                return

        self.joining_elevation = True
        self.elevation_direction = direction
        self.elevation_deadline = now + self.CALL_TIMEOUT
        self.elevation_leader_id = leader_id

        print(f"[IA] Appel elevation level {call_level}, direction {direction}.")

    def _broadcast_elevation_call(self):
        now = time.time()
        if now - self.last_call_time < self.CALL_COOLDOWN:
            return False

        target = self.level + 1
        requirement = self.ELEVATION_REQUIREMENTS.get(target)
        if not requirement:
            return False

        msg = f"{self.team_name}|ELEVATE|{self.level}|{self.drone_id}|{requirement[0]}"
        self.queue_command(f"Broadcast {msg}")
        self.last_call_time = now
        self.leading_elevation = True
        self.elevation_deadline = now + self.CALL_TIMEOUT
        print(f"[IA] Leader: appel elevation vers niveau {target}.")
        return True

    def think(self):
        if self.command_queue or self.sent_commands:
            return

        if self._refresh_data_if_needed():
            return

        if self.level >= 8:
            print("[IA] Objectif atteint: niveau 8.")
            self.queue_command("Look")
            self._invalidate_data()
            return

        if self._survival_override():
            self._invalidate_data()
            return

        if self.joining_elevation:
            self._joining_elevation_behavior()
            self._invalidate_data()
            return

        self._main_level8_behavior()
        self._invalidate_data()

    def _refresh_data_if_needed(self):
        if not self.inventory_fresh:
            self.queue_command("Inventory")
            return True
        if not self.vision_fresh:
            self.queue_command("Look")
            return True
        return False

    def _invalidate_data(self):
        self.inventory_fresh = False
        self.vision_fresh = False

    def _survival_override(self):
        food = self.inventory.get("food", 0)
        if food > self.EMERGENCY_FOOD:
            return False

        print(f"[IA] SURVIE food={food}.")
        self.leading_elevation = False
        self.joining_elevation = False
        self.elevation_direction = None

        food_index = self._find_item_in_vision("food")
        if food_index is not None:
            self.move_to_target(food_index)
            self.queue_command("Take food")
            return True

        self._explore_for_food()
        return True

    def _main_level8_behavior(self):
        food = self.inventory.get("food", 0)

        if self._should_fork():
            self.queue_command("Fork")
            return

        if food < self.FOOD_TARGET:
            self._search_food_behavior()
            return

        if time.time() < self.incant_blocked_until:
            if food < self.FOOD_TARGET:
                self._search_food_behavior()
                return
            self._search_needed_stones_behavior()
            return

        if self._has_required_stones_for_next_level():
            self._ready_for_elevation_behavior()
            return

        self._search_needed_stones_behavior()

    def _ready_for_elevation_behavior(self):
        target = self.level + 1
        requirement = self.ELEVATION_REQUIREMENTS.get(target)
        if not requirement:
            return

        needed_players = requirement[0]
        players = self._players_on_current_tile()

        if self._clean_extra_stone_from_tile():
            return

        # Level 1 -> 2: exact 1 joueur, sinon le serveur peut refuser.
        if self.level == 1:
            if players != 1:
                print("[IA] Level 1: trop de monde sur ma case, je m'isole.")
                self.queue_command("Forward")
                return
            if self.inventory.get("food", 0) < self.FOOD_BEFORE_INCANT:
                self._search_food_behavior()
                return
            print("[IA] Level 1: incantation solo.")
            self._prepare_elevation_from_inventory()
            return

        if self.inventory.get("food", 0) < self.FOOD_BEFORE_CALL:
            self._search_food_behavior()
            return

        if players > needed_players:
            print(f"[IA] Trop de joueurs ({players}/{needed_players}), je change de case.")
            self.leading_elevation = False
            self.joining_elevation = False
            self.queue_command("Forward")
            return

        if players == needed_players:
            if self.inventory.get("food", 0) < self.FOOD_BEFORE_INCANT:
                self._search_food_behavior()
                return
            print(f"[IA] Joueurs exacts ({players}/{needed_players}), incantation.")
            self._prepare_elevation_from_inventory()
            return

        self._broadcast_elevation_call()
        self._feed_group_if_needed()
        self.queue_command("Look")

    def _joining_elevation_behavior(self):
        if time.time() > self.elevation_deadline:
            print("[IA] Appel elevation expire, je reprends la collecte.")
            self.joining_elevation = False
            self.elevation_direction = None
            return

        if self.inventory.get("food", 0) <= self.EMERGENCY_FOOD:
            print("[IA] Trop faim pour suivre elevation.")
            self.joining_elevation = False
            self.elevation_direction = None
            self._search_food_behavior()
            return

        if self.elevation_direction == 0:
            if self.vision and "food" in self.vision[0] and self.inventory.get("food", 0) < self.FOOD_TARGET:
                self.queue_command("Take food")
                return
            if self.inventory.get("food", 0) < self.FOOD_BEFORE_FOLLOW:
                print("[IA] Plus assez de food pour attendre, je repars chercher.")
                self.joining_elevation = False
                self.elevation_direction = None
                self._search_food_behavior()
                return
            print("[IA] Sur la case elevation, attente.")
            self.queue_command("Look")
            return

        if self.elevation_direction is not None:
            direction = self.elevation_direction
            self.elevation_direction = None
            self._move_to_sound(direction)
            return

        self.queue_command("Look")

    def _feed_group_if_needed(self):
        if not self.vision:
            return False
        if time.time() - self.last_feed_time < self.FEED_COOLDOWN:
            return False
        if self.inventory.get("food", 0) <= self.FOOD_FEED_RESERVE:
            return False

        players = self._players_on_current_tile()
        food_on_tile = self.vision[0].count("food")
        if players <= 1:
            return False
        if food_on_tile >= 1:
            return False

        print("[IA] Je pose food pour le groupe.")
        self.queue_command("Set food")
        self.last_feed_time = time.time()
        return True

    def _should_fork(self):
        now = time.time()
        if now - self.last_connect_nbr_time >= self.CONNECT_NBR_COOLDOWN:
            self.queue_command("Connect_nbr")
            self.last_connect_nbr_time = now
            return False

        if self.inventory.get("food", 0) < self.FORK_MIN_FOOD:
            return False

        connected = max(1, self.initial_team_capacity - self.available_slots)
        if connected >= self.TARGET_DRONES:
            return False

        return self.available_slots <= 0

    def _search_food_behavior(self):
        food_index = self._find_item_in_vision("food")
        if food_index is not None:
            self.move_to_target(food_index)
            self.queue_command("Take food")
            return
        self._explore_for_food()

    def _explore_for_food(self):
        self.search_turn += 1
        if self.search_turn % 7 == 0:
            self.queue_command("Right")
            self.queue_command("Forward")
            return
        if self.search_turn % 4 == 0:
            self.queue_command("Left")
            self.queue_command("Forward")
            return
        self.queue_command("Forward")

    def _search_needed_stones_behavior(self):
        missing = self._missing_stones_for_next_level()
        if not missing:
            self.queue_command("Look")
            return

        stone_index, stone_name = self._find_best_missing_stone(missing)
        if stone_index is not None and stone_name is not None:
            self.move_to_target(stone_index)
            self.queue_command(f"Take {stone_name}")
            return

        self._explore_for_stones()

    def _explore_for_stones(self):
        self.search_turn += 1
        self.queue_command("Forward")
        if self.search_turn % 5 == 0:
            self.queue_command("Left")
        elif self.search_turn % 9 == 0:
            self.queue_command("Right")

    def _find_best_missing_stone(self, missing):
        best_index = None
        best_stone = None
        best_distance = None

        for index, tile in enumerate(self.vision):
            for stone in self.STONES:
                if missing.get(stone, 0) <= 0:
                    continue
                if stone not in tile:
                    continue

                distance = self._vision_distance(index)
                if best_distance is None or distance < best_distance:
                    best_distance = distance
                    best_index = index
                    best_stone = stone

        return best_index, best_stone

    def _vision_distance(self, index):
        row = int(math.sqrt(index))
        column = abs(index - (row * row) - row)
        return row + column

    def _has_required_stones_for_next_level(self):
        return not self._missing_stones_for_next_level()

    def _missing_stones_for_next_level(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not requirement:
            return {}

        missing = {}
        current_tile = self.vision[0] if self.vision else []
        for index, stone in enumerate(self.STONES):
            needed = requirement[index + 1]
            owned = self.inventory.get(stone, 0) + current_tile.count(stone)
            if owned < needed:
                missing[stone] = needed - owned
        return missing

    def _clean_extra_stone_from_tile(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not requirement or not self.vision:
            return False

        current_tile = self.vision[0]
        for index, stone in enumerate(self.STONES):
            needed = requirement[index + 1]
            if current_tile.count(stone) > needed:
                print(f"[IA] Trop de {stone} sur la case, je nettoie.")
                self.queue_command(f"Take {stone}")
                return True
        return False

    def _prepare_elevation_from_inventory(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
        if not requirement or not self.vision:
            return

        current_tile = self.vision[0]
        for index, stone in enumerate(self.STONES):
            needed = requirement[index + 1]
            on_tile = current_tile.count(stone)
            to_set = max(0, needed - on_tile)
            available = self.inventory.get(stone, 0)
            for _ in range(min(to_set, available)):
                self.queue_command(f"Set {stone}")

        self.queue_command("Incantation")

    def _players_on_current_tile(self):
        if not self.vision:
            return 1
        return self.vision[0].count("player")

    def _find_item_in_vision(self, item_name):
        best_index = None
        best_distance = None
        for index, tile in enumerate(self.vision):
            if item_name not in tile:
                continue
            distance = self._vision_distance(index)
            if best_distance is None or distance < best_distance:
                best_distance = distance
                best_index = index
        return best_index

    def move_to_target(self, target_index):
        if target_index == 0:
            return

        row = int(math.sqrt(target_index))
        column = target_index - (row * row) - row

        for _ in range(row):
            self.queue_command("Forward")

        if column < 0:
            self.queue_command("Left")
            for _ in range(abs(column)):
                self.queue_command("Forward")
            self.queue_command("Right")
        elif column > 0:
            self.queue_command("Right")
            for _ in range(column):
                self.queue_command("Forward")
            self.queue_command("Left")

    def _move_to_sound(self, direction):
        moves_by_direction = {
            1: ["Forward"],
            2: ["Forward", "Right", "Forward", "Left"],
            3: ["Right", "Forward", "Left"],
            4: ["Right", "Right", "Forward", "Right", "Forward", "Left", "Left"],
            5: ["Right", "Right", "Forward", "Right", "Right"],
            6: ["Left", "Left", "Forward", "Left", "Forward", "Right", "Right"],
            7: ["Left", "Forward", "Right"],
            8: ["Forward", "Left", "Forward", "Right"],
        }
        for command in moves_by_direction.get(direction, []):
            self.queue_command(command)
