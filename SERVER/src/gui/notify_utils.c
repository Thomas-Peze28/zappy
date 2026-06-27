/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** notify_utils
*/

#include "gui.h"
#include <stdio.h>

void gui_notify_bct(server_t *server, int x, int y)
{
    char buf[256];
    tile_t *tile = &server->map->tiles[y][x];

    snprintf(buf, sizeof(buf), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->food,
        tile->linemate,
        tile->deraumere,
        tile->sibur,
        tile->mendiane,
        tile->phiras,
        tile->thystame);
    send_to_all_gui(server, buf);
}
