from enum import Enum


class DroneState(Enum):
    SEARCH_FOOD = 1
    SEARCH_STONES = 2
    ELEVATE = 3
    JOIN_ELEVATION = 4
    FORK = 5
    RITUAL_FOOD = 6
