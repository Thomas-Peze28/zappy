# Zappy GUI

The **Zappy GUI** is the graphical interface of the Zappy project. It connects to the server, displays the game world in real time, visualizes players, resources, events, and allows spectators to follow the progress of the game.

---

# Usage

```bash
./zappy_gui -p <port> -h <machine>
```

## Options

| Option | Description |
|--------|-------------|
| `-p <port>` | Port number of the server |
| `-h <machine>` | Hostname or IP address of the server |

Example:

```bash
./zappy_gui -p 4242 -h localhost
```

---

# Controls

| Key | Action |
|-----|--------|
| `Esc` | Close the application |
| `F` | Toggle fullscreen |
| `C` | Change camera mode |
| `P` | Pause/Resume music |
| `I` | Increase the server time unit |
| `O` | Decrease the server time unit |
| `L` | Toggle log window |
| `Space` | Play sound effect and add current map content to the log |

---

# GUI Protocol

The GUI communicates with the Zappy server through a dedicated protocol.

## Client Requests

| Command | Description |
|---------|-------------|
| `msz` | Request the map size |
| `bct X Y` | Request the content of a tile |
| `mct` | Request the content of the entire map |
| `tna` | Request all team names |
| `ppo #n` | Request a player's position |
| `plv #n` | Request a player's level |
| `pin #n` | Request a player's inventory |
| `sgt` | Request the current time unit |

---

## Server Events

| Command | Description |
|---------|-------------|
| `msz X Y` | Map size |
| `bct X Y q0 q1 q2 q3 q4 q5 q6` | Tile content |
| `tna N` | Team name |
| `pnw #n X Y O L N` | New player connected |
| `ppo #n X Y O` | Player position |
| `plv #n L` | Player level |
| `pin #n X Y q0 q1 q2 q3 q4 q5 q6` | Player inventory |
| `pex #n` | Player expulsion |
| `pbc #n M` | Player broadcast |
| `pic X Y L #n #n ...` | Incantation started |
| `pie X Y R` | Incantation ended |
| `pfk #n` | Egg laying |
| `pdr #n i` | Resource dropped |
| `pgt #n i` | Resource collected |
| `pdi #n` | Player death |
| `enw #e #n X Y` | Egg laid |
| `ebo #e` | Player connected to egg |
| `edi #e` | Egg death |
| `sgt T` | Current time unit |
| `sst T` | Time unit updated |
| `seg N` | End of game |
| `smg M` | Server message |
| `suc` | Unknown command |
| `sbp` | Invalid command parameters |

---

# Protocol Symbols

## Map & Resources

| Symbol | Meaning |
|--------|---------|
| `X` | Horizontal position / map width |
| `Y` | Vertical position / map height |
| `q0` | Food quantity |
| `q1` | Linemate quantity |
| `q2` | Deraumere quantity |
| `q3` | Sibur quantity |
| `q4` | Mendiane quantity |
| `q5` | Phiras quantity |
| `q6` | Thystame quantity |

## General Symbols

| Symbol | Meaning |
|--------|---------|
| `n` | Player number |
| `O` | Orientation (`1 = North`, `2 = East`, `3 = South`, `4 = West`) |
| `L` | Player or incantation level |
| `e` | Egg number |
| `T` | Time unit |
| `N` | Team name |
| `R` | Incantation result |
| `M` | Message |
| `i` | Resource number |

---

# Features

## Tile & Player Information

The GUI provides an interactive way to inspect both map tiles and players.

### General features

- Real-time map visualization
- Live player tracking
- Resource display
- Incantation and broadcast animations
- Event logging
- Camera switching
- Adjustable server time unit
- Sound effects and background music
- Fullscreen mode

### Tile Inspection

Clicking on a tile displays the resources currently present on that tile in the following format:

```text
<resource_name>: <resource_quantity>
```

Example:

```text
Food: 3
Linemate: 1
Sibur: 2
```

### Player Inspection

If a player is present on the selected tile, the GUI also displays information about that player, including:

- Player ID
- Team name
- Level
- Inventory

Each resource in the player's inventory is displayed as:

```text
<resource_name>: <player_inventory_quantity> (<total_quantity_in_team_inventory>)
```

Example:

```text
Food: 5 (42)
Linemate: 1 (8)
Deraumere: 0 (3)
Sibur: 2 (11)
Mendiane: 0 (2)
Phiras: 1 (4)
Thystame: 0 (1)
```

In this example:
- `5` is the amount carried by the selected player.
- `42` is the total amount of the same resource carried by every player belonging to the same team.

---

# Notes

The GUI is intended to be used **only** as a spectator interface. It receives game state updates from the server and visualizes them without interacting with the game logic.