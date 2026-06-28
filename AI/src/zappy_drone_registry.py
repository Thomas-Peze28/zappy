import time

DRONE_TTL = 35.0

class ZappyDroneRegistry:
    STONES = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

    def __init__(self, drone_id, min_drones, manager):
        self.drone_id = drone_id
        self.min_drones = min_drones
        self.manager = manager
        self.known_drones = {}

    def register_self(self, level, inventory):
        self.known_drones[self.drone_id] = {
            "level": level,
            "inventory": inventory.copy() if inventory else {"food": 0},
            "last_seen": time.time(),
        }

    def known_drone_count(self, level, inventory):
        self.register_self(level, inventory)
        now = time.time()
        for drone_id in list(self.known_drones):
            if now - self.known_drones[drone_id].get("last_seen", 0) > DRONE_TTL:
                self.known_drones.pop(drone_id, None)
        return len(self.known_drones)

    def current_leader_id(self, level, inventory):
        self.register_self(level, inventory)
        alive = sorted(self.known_drones.keys())
        return alive[0] if alive else self.drone_id

    def is_global_leader(self, level, inventory):
        return self.current_leader_id(level, inventory) == self.drone_id

    def total_known_stones(self):
        totals = {stone: 0 for stone in self.STONES}
        now = time.time()
        for drone in self.known_drones.values():
            if now - drone.get("last_seen", 0) > DRONE_TTL:
                continue
            inv = drone.get("inventory", {})
            for stone in self.STONES:
                totals[stone] += inv.get(stone, 0)
        return totals

    def update_from_broadcast(self, drone_id, drone_level, inventory):
        self.known_drones[drone_id] = {
            "level": drone_level,
            "inventory": inventory,
            "last_seen": time.time(),
        }

    def try_spawn_child(self, available_slots, level, inventory):
        if available_slots <= 0:
            return
        if self.known_drone_count(level, inventory) >= self.min_drones:
            return
        if self.manager.spawn_child_drone():
            print(
                f"[IA] Nouveau processus IA lance automatiquement apres Fork/slot. "
                f"known={self.known_drone_count(level, inventory)}/{self.min_drones}"
            )
