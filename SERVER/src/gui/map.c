/*
** EPITECH PROJECT, 2026
** map.c
** File description:
** map commands
*/

#include "gui.h"
#include <stdio.h>

void gui_send_msz(server_t *server, client_t *gui)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "msz %d %d\n",
        server->config.width, server->config.height);
    send_to_client(gui, buf);
}

void gui_send_bct(server_t *server, client_t *gui, int x, int y)
{
    tile_t *tile = &server->map->tiles[y][x];
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN,
        "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        tile->food, tile->linemate, tile->deraumere,
        tile->sibur, tile->mendiane, tile->phiras, tile->thystame);
    send_to_client(gui, buf);
}

void gui_send_mct(server_t *server, client_t *gui)
{
    int x;
    int y = 0;

    while (y < server->config.height) {
        x = 0;
        while (x < server->config.width) {
            gui_send_bct(server, gui, x, y);
            x++;
        }
        y++;
    }
}
