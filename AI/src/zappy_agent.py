import math
import time
from collections import deque

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
    FORK_COOLDOWN = 30.0
    CONNECT_NBR_COOLDOWN = 10.0

    def __init__(self, manager, sock, team_name, initial_slots, map_x, map_y):
        self.manager = manager
        self.sock = sock
        self.team_name = team_name
        self.initial_slots = initial_slots
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
        self.food_threshold = 15
        self.last_fork_time = 0.0
        self.last_connect_nbr_time = time.time()

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
                self.manager.remove_client(self.sock.fileno())
                return

    def receive_data(self):
        try:
            data = self.sock.recv(4096)
            if not data:
                self.manager.remove_client(self.sock.fileno())
                return
            self.read_buffer += data.decode("utf-8")
            self._process_buffer()
        except BlockingIOError:
            pass
        except Exception:
            self.manager.remove_client(self.sock.fileno())

    def _process_buffer(self):
        while "\n" in self.read_buffer:
            line, self.read_buffer = self.read_buffer.split("\n", 1)
            line = line.strip()
            fd = self.sock.fileno()

            if line == "dead":
                print(f"[*] [FD {fd}] Le drone est mort de faim.")
                self.manager.remove_client(fd)
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
                    self.level = int(line.split(":")[1].strip())
                    print(f"[IA - FD {fd}] Niveau {self.level} atteint !")
                    self.state = DroneState.SEARCH_FOOD
                    self.inventory_fresh = False
                    self.vision_fresh = False
                except ValueError:
                    pass
                continue

            if self.sent_commands:
                original_cmd = self.sent_commands.popleft()
                self._handle_response(original_cmd, line)

            self._flush_commands()

    def _handle_response(self, command, response):
        fd = self.sock.fileno()

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
            return

        if command == "Look":
            clean_text = response.strip("[]")
            self.vision = [tile.strip().split() for tile in clean_text.split(",")]
            self.vision_fresh = True
            return

        if command == "Incantation":
            if response == "Elevation underway":
                print(f"[IA - FD {fd}] Rituel en cours, je me fige.")
                self.command_queue.clear()
            elif response == "ko":
                print(f"[IA - FD {fd}] Echec incantation (ko). Reset.")
                self.inventory_fresh = False
                self.vision_fresh = False
                self.state = DroneState.SEARCH_FOOD
            return

        if command == "Fork" and response == "ok":
            print(f"[IA - FD {fd}] Oeuf pondu ! Demande des slots disponibles...")
            self.queue_command("Connect_nbr")
            return

        if command == "Connect_nbr":
            try:
                available_slots = int(response)
                if available_slots > 0:
                    print(f"[IA - FD {fd}] {available_slots} slot(s) disponible(s). Éclosion en cours...")
                    for _ in range(available_slots):
                        self.manager.spawn_client()
            except ValueError:
                pass
            return

        if command.startswith("Take") and response == "ko":
            self.vision_fresh = False
            return

        if command.startswith("Set") and response == "ko":
            resource = command.split(" ", 1)[-1]
            print(f"[IA - FD {fd}] Impossible de deposer '{resource}' (ko).")

    def _handle_broadcast(self, k, text):
        expected_msg = f"{self.team_name}_ELEVATE_{self.level + 1}"
        if text == expected_msg and self.inventory.get("food", 0) > 10:
            self.command_queue.clear()
            self.state = DroneState.JOIN_ELEVATION
            if k == 0:
                print(f"[IA - FD {self.sock.fileno()}] FOLLOWER : Arrive sur la case du leader.")
            else:
                self._move_to_sound(k)

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

    def think(self):
        if self.command_queue or self.sent_commands:
            return

        if time.time() - self.last_connect_nbr_time > self.CONNECT_NBR_COOLDOWN:
            self.queue_command("Connect_nbr")
            self.last_connect_nbr_time = time.time()
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

        current_food = self.inventory.get("food", 0)

        if current_food < self.food_threshold:
            self.state = DroneState.SEARCH_FOOD
        elif self.state == DroneState.JOIN_ELEVATION:
            pass
        elif self._is_ready_for_elevation():
            self.state = DroneState.ELEVATE
        elif current_food > 35 and self.level < 8 and (time.time() - self.last_fork_time) > self.FORK_COOLDOWN:
            self.state = DroneState.FORK
        else:
            self.state = DroneState.SEARCH_STONES

        if self.state == DroneState.SEARCH_FOOD:
            for index, tile in enumerate(self.vision):
                if "food" in tile:
                    self.move_to_target(index)
                    self.queue_command("Take food")
                    break
            else:
                self.queue_command("Forward")
        elif self.state == DroneState.SEARCH_STONES:
            missing = self._missing_stones()
            if not missing:
                self.queue_command("Forward")
            else:
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
        elif self.state == DroneState.ELEVATE:
            req = self.ELEVATION_REQUIREMENTS.get(self.level + 1)
            if not req:
                return
            players_on_tile = (self.vision[0].count("player") if self.vision else 0) + 1
            if players_on_tile < req[0]:
                self.queue_command(f"Broadcast {self.team_name}_ELEVATE_{self.level + 1}")
            else:
                for i, stone in enumerate(self.STONES):
                    for _ in range(req[i + 1]):
                        self.queue_command(f"Set {stone}")
                self.queue_command("Incantation")

        self.inventory_fresh = False
        self.vision_fresh = False
