import math

class ZappyMovement:
    def __init__(self, queue_command_fn):
        self._queue = queue_command_fn

    def move_to_target(self, target_index):
        """Move to a tile index in the vision grid."""
        if target_index == 0:
            return
        r = int(math.sqrt(target_index))
        c = target_index - (r * r) - r
        for _ in range(r):
            self._queue("Forward")
        if c < 0:
            self._queue("Left")
            for _ in range(abs(c)):
                self._queue("Forward")
            self._queue("Right")
        elif c > 0:
            self._queue("Right")
            for _ in range(abs(c)):
                self._queue("Forward")
            self._queue("Left")

    def move_one_step_to_sound(self, k):
        """Move one step toward the origin of a sound (direction k, 1-8)."""
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
            self._queue(command)
