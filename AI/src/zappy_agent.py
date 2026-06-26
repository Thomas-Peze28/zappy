import math
import os
import time
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

    ROLE_PILLAR = "pillar"
    ROLE_FOOD = "food_supplier"
    ROLE_STONE = "stone_collector"

    MAX_PENDING_COMMANDS = 10
    TARGET_DRONES = 8

    CONNECT_NBR_COOLDOWN = 2.0
    INVENTORY_BROADCAST_COOLDOWN = 8.0
    PILLAR_BROADCAST_COOLDOWN = 3.0
    PILLAR_CALL_COOLDOWN = 3.0
    PILLAR_FEED_COOLDOWN = 1.0

    PILLAR_MIN_FOOD_TO_FORK = 20
    PILLAR_MIN_FOOD_TO_FORK_AFTER_TARGET = 35
    PILLAR_MIN_FOOD_TO_BROADCAST = 12

    FOOD_SUPPLIER_RETURN_FOOD = 10
    STONE_COLLECTOR_MIN_FOOD = 15

    MIN_FOOD_TO_FOLLOW = 10
    DRONE_MIN_FOOD_ON_PILLAR = 8
    PILLAR_MIN_FOOD_TO_GIVE = 3
    PILLAR_FOOD_PER_DRONE = 8
    PILLAR_FOOD_RESERVE = 20

    def __init__(self, manager, sock, team_name, remaining_slots, map_x, map_y, role=ROLE_STONE):
        self.manager = manager
        self.sock = sock
        self.team_name = team_name
        self.remaining_slots = remaining_slots
        self.map_x = map_x
        self.map_y = map_y
        self.role = role

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
        self.broadcast = ZappyBroadcastCoordinator(team_name, self.drone_id)

        self.initial_team_capacity = max(1, remaining_slots + 1)
        self.available_slots = remaining_slots

        self.last_connect_nbr_time = 0.0
        self.last_inventory_broadcast = 0.0
        self.last_pillar_broadcast = 0.0
        self.last_pillar_call = 0.0
        self.last_food_drop = 0.0

        self.pillar_direction = None
        self.waiting_food_on_pillar = False

    def queue_command(self, command):
        self.command_queue.append(command)
        self._flush_commands()

    def _flush_commands(self):
        while self.command_queue and len(self.sent_commands) < self.MAX_PENDING_COMMANDS:
            command = self.command_queue.popleft()

            try:
                self.sock.sendall((command + "\n").encode("utf-8"))
                self.sent_commands.append(command)
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
            self._handle_server_line(line.strip())

            if self.manager.client is None:
                return

            self._flush_commands()

    def _handle_server_line(self, line):
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

        if self.sent_commands:
            command = self.sent_commands.popleft()
            self._handle_response(command, line)

    def _handle_message_line(self, line):
        parts = line.replace("message ", "", 1).split(",", 1)

        if len(parts) != 2:
            return

        try:
            direction = int(parts[0].strip())
        except ValueError:
            return

        self._handle_broadcast(direction, parts[1].strip())

    def _handle_level_up(self, line):
        try:
            self.level = int(line.split(":", 1)[1].strip())
        except ValueError:
            return

        print(f"[IA] Niveau {self.level} atteint !")
        self.state = DroneState.SEARCH_FOOD
        self._invalidate_data()

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
            self._handle_connect_nbr_response(response)
            return

        if command == "Fork" and response == "ok":
            print("[IA] Oeuf pondu.")
            self.queue_command("Connect_nbr")
            return

        if command == "Incantation":
            self._handle_incantation_response(response)
            return

        if command.startswith("Take") and response == "ko":
            self.vision_fresh = False
            return

        if command.startswith("Set") and response == "ko":
            resource = command.split(" ", 1)[-1]
            print(f"[IA] Impossible de déposer '{resource}' (ko).")

    def _handle_connect_nbr_response(self, response):
        try:
            self.available_slots = int(response)
        except ValueError:
            return

        print(f"[IA] Slots disponibles: {self.available_slots}")

    def _handle_incantation_response(self, response):
        if response == "Elevation underway":
            print("[IA] Rituel en cours, je me fige.")
            self.command_queue.clear()
            return

        if response == "ko":
            print("[IA] Echec incantation (ko). Reset.")
            self.state = DroneState.SEARCH_FOOD
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

    def _handle_broadcast(self, direction, text):
        if self._handle_plain_broadcast(direction, text):
            return

        frame = self.broadcast.parse_incoming(text)

        if not frame:
            return

        if frame.get("kind") == "inventory-status":
            return

        if frame.get("kind") == "elevation-request":
            self._handle_elevation_broadcast(direction, frame)

    def _handle_plain_broadcast(self, direction, text):
        parts = text.strip().split("|")

        if len(parts) < 3 or parts[0] != self.team_name:
            return False

        sender_role = parts[1]
        action = parts[2]

        if sender_role == "PILLAR":
            self.pillar_direction = direction
            self._handle_pillar_broadcast(direction, action)
            return True

        if sender_role == "DRONE" and action == "NEED_FOOD":
            if self.role == self.ROLE_PILLAR and direction == 0:
                self._pillar_feed_drone()
            return True

        return False

    def _handle_pillar_broadcast(self, direction, action):
        if action == "POS":
            return

        if action == "CALL_STONES" and self.role == self.ROLE_STONE:
            self._go_to_pillar(direction)
            return

        if action == "CALL_FOOD" and self.role == self.ROLE_FOOD:
            self._go_to_pillar(direction)
            return

        if action == "CALL_ALL" and self.role in (self.ROLE_STONE, self.ROLE_FOOD):
            self._go_to_pillar(direction)

    def _handle_elevation_broadcast(self, direction, frame):
        target_level = frame.get("level", 0)

        if target_level != self.level + 1:
            return

        if self.inventory.get("food", 0) <= self.MIN_FOOD_TO_FOLLOW:
            return

        if not self.broadcast.should_follow(frame):
            return

        self.command_queue.clear()

        if direction == 0:
            print("[IA] FOLLOWER : Arrivé sur la case du leader.")
            return

        print(f"[IA] FOLLOWER : Je rejoins le broadcast direction {direction}.")
        self._move_to_sound(direction)

    def _broadcast_inventory(self):
        now = time.time()

        if now - self.last_inventory_broadcast < self.INVENTORY_BROADCAST_COOLDOWN:
            return

        if self.inventory.get("food", 0) <= self.MIN_FOOD_TO_FOLLOW:
            return

        packet = self.broadcast.build_inventory_packet(
            self.level,
            {resource: self.inventory.get(resource, 0) for resource in ["food"] + self.STONES},
        )

        self.queue_command(f"Broadcast {packet}")
        self.last_inventory_broadcast = now

    def _broadcast_pillar_position(self):
        now = time.time()

        if now - self.last_pillar_broadcast < self.PILLAR_BROADCAST_COOLDOWN:
            return

        if self.inventory.get("food", 0) < self.PILLAR_MIN_FOOD_TO_BROADCAST:
            return

        self.queue_command(f"Broadcast {self.team_name}|PILLAR|POS|{self.level}|{self.drone_id}")
        self.last_pillar_broadcast = now

    def _call_stone_collectors(self):
        self._pillar_call("CALL_STONES")

    def _call_food_suppliers(self):
        self._pillar_call("CALL_FOOD")

    def _call_all_drones(self):
        self._pillar_call("CALL_ALL")

    def _pillar_call(self, action):
        now = time.time()

        if now - self.last_pillar_call < self.PILLAR_CALL_COOLDOWN:
            return

        self.queue_command(f"Broadcast {self.team_name}|PILLAR|{action}|{self.level}|{self.drone_id}")
        self.last_pillar_call = now

    def think(self):
        if self.command_queue or self.sent_commands:
            return

        if self._refresh_data_if_needed():
            return

        handlers = {
            self.ROLE_PILLAR: self.think_pillar,
            self.ROLE_FOOD: self.think_food_supplier,
            self.ROLE_STONE: self.think_stone_collector,
        }

        handlers.get(self.role, self.think_stone_collector)()
        self._invalidate_data()

    def _refresh_data_if_needed(self):
        if not self.inventory_fresh:
            self.queue_command("Inventory")
            return True

        if not self.vision_fresh:
            self.queue_command("Look")
            return True

        return False

    def _refresh_connect_nbr_if_needed(self):
        now = time.time()

        if now - self.last_connect_nbr_time < self.CONNECT_NBR_COOLDOWN:
            return False

        self.queue_command("Connect_nbr")
        self.last_connect_nbr_time = now
        return True

    def _invalidate_data(self):
        self.inventory_fresh = False
        self.vision_fresh = False

    def think_pillar(self):
        current_food = self.inventory.get("food", 0)

        if self._refresh_connect_nbr_if_needed():
            return

        if self._connected_drone_estimate() < self.TARGET_DRONES:
            self._grow_team(current_food)
            return

        self._broadcast_pillar_position()
        self._broadcast_inventory()

        if current_food >= self.PILLAR_MIN_FOOD_TO_FORK_AFTER_TARGET and self.available_slots <= 0:
            self.queue_command("Fork")
            return

        if not self._pillar_has_enough_food_for_group():
            self._call_food_suppliers()
            return

        if not self._tile_has_elevation_stones():
            self._call_stone_collectors()
            return

        if not self._enough_players_on_tile_for_elevation():
            self._call_all_drones()
            return

        self._prepare_elevation_from_tile()

    def _grow_team(self, current_food):
        if self.available_slots > 0:
            if current_food < self.PILLAR_MIN_FOOD_TO_FORK:
                self._search_food_behavior()
                return

            self._broadcast_pillar_position()
            return

        if current_food >= self.PILLAR_MIN_FOOD_TO_FORK:
            self.queue_command("Fork")
            return

        self._search_food_behavior()

    def _connected_drone_estimate(self):
        return max(1, self.initial_team_capacity - self.available_slots)

    def _pillar_has_enough_food_for_group(self):
        players_on_tile = self._players_on_current_tile()
        required_food = self.PILLAR_FOOD_RESERVE + players_on_tile * self.PILLAR_FOOD_PER_DRONE
        return self.inventory.get("food", 0) >= required_food

    def _pillar_feed_drone(self):
        now = time.time()

        if now - self.last_food_drop < self.PILLAR_FEED_COOLDOWN:
            return

        if self.inventory.get("food", 0) <= self.PILLAR_MIN_FOOD_TO_GIVE:
            return

        self.queue_command("Set food")
        self.last_food_drop = now


    def think_food_supplier(self):
        if self._take_food_on_current_tile_if_needed():
            return

        current_food = self.inventory.get("food", 0)

        if self.pillar_direction == 0 and current_food > self.DRONE_MIN_FOOD_ON_PILLAR:
            self._drop_food_on_pillar()
            return

        if current_food < self.FOOD_SUPPLIER_RETURN_FOOD:
            self._search_food_behavior()
            return

        if self.pillar_direction is not None:
            self._go_to_pillar(self.pillar_direction)
            return

        self._search_food_behavior()

    def think_stone_collector(self):
        if self._take_food_on_current_tile_if_needed():
            return

        current_food = self.inventory.get("food", 0)

        if self.pillar_direction == 0:
            self._drop_stones_on_pillar()
            return

        if current_food < self.STONE_COLLECTOR_MIN_FOOD:
            self._search_food_behavior()
            return

        self._search_stones_behavior()

    def _go_to_pillar(self, direction):
        self.command_queue.clear()

        if direction == 0:
            if self.inventory.get("food", 0) < self.DRONE_MIN_FOOD_ON_PILLAR:
                self._ask_pillar_for_food()
                self.waiting_food_on_pillar = True
                return

            if self.role == self.ROLE_STONE:
                self._drop_stones_on_pillar()
                return

            if self.role == self.ROLE_FOOD:
                self._drop_food_on_pillar()
                return

        self._move_to_sound(direction)

    def _ask_pillar_for_food(self):
        current_food = self.inventory.get("food", 0)
        self.queue_command(f"Broadcast {self.team_name}|DRONE|NEED_FOOD|{self.drone_id}|{current_food}")

    def _take_food_on_current_tile_if_needed(self):
        if not self.waiting_food_on_pillar:
            return False

        if not self.vision:
            self.queue_command("Look")
            return True

        if "food" in self.vision[0]:
            self.queue_command("Take food")
            self.waiting_food_on_pillar = False
            return True

        self._ask_pillar_for_food()
        return True

    def _drop_food_on_pillar(self):
        if self.inventory.get("food", 0) <= self.DRONE_MIN_FOOD_ON_PILLAR:
            self._ask_pillar_for_food()
            self.waiting_food_on_pillar = True
            return

        self.queue_command("Set food")

    def _drop_stones_on_pillar(self):
        for stone in self.STONES:
            if self.inventory.get(stone, 0) > 0:
                self.queue_command(f"Set {stone}")
                return

        self.queue_command("Look")

    def _search_food_behavior(self):
        food_index = self._find_item_in_vision("food")

        if food_index is not None:
            self.move_to_target(food_index)
            self.queue_command("Take food")
            return

        self.queue_command("Forward")

    def _search_stones_behavior(self):
        missing_stones = self._missing_stones()

        if not missing_stones:
            self.queue_command("Forward")
            return

        for stone in missing_stones:
            stone_index = self._find_item_in_vision(stone)

            if stone_index is not None:
                self.move_to_target(stone_index)
                self.queue_command(f"Take {stone}")
                return

        self.queue_command("Forward")
        self.queue_command("Left")

    def _find_item_in_vision(self, item_name):
        for index, tile in enumerate(self.vision):
            if item_name in tile:
                return index

        return None

    def _missing_stones(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)

        if not requirement:
            return {}

        return {
            stone: requirement[index + 1] - self.inventory.get(stone, 0)
            for index, stone in enumerate(self.STONES)
            if self.inventory.get(stone, 0) < requirement[index + 1]
        }

    def _tile_has_elevation_stones(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)

        if not requirement or not self.vision:
            return False

        current_tile = self.vision[0]

        return all(
            current_tile.count(stone) + self.inventory.get(stone, 0) >= requirement[index + 1]
            for index, stone in enumerate(self.STONES)
        )

    def _enough_players_on_tile_for_elevation(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)

        if not requirement:
            return False

        return self._players_on_current_tile() >= requirement[0]

    def _prepare_elevation_from_tile(self):
        requirement = self.ELEVATION_REQUIREMENTS.get(self.level + 1)

        if not requirement or not self.vision:
            return

        current_tile = self.vision[0]

        for index, stone in enumerate(self.STONES):
            needed = requirement[index + 1]
            already_on_tile = current_tile.count(stone)
            missing_on_tile = max(0, needed - already_on_tile)
            available = self.inventory.get(stone, 0)

            for _ in range(min(missing_on_tile, available)):
                self.queue_command(f"Set {stone}")

        self.queue_command("Incantation")

    def _players_on_current_tile(self):
        if not self.vision:
            return 1

        return self.vision[0].count("player")

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
