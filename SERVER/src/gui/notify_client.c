/*
** EPITECH PROJECT, 2026
** notify_client.c
** File description:
** send a unique client to GUI
*/

#include "gui.h"
#include <stdio.h>

void gui_notify_pnw_to(server_t *server, client_t *gui,
    client_t *player)
{
    char buf[MAX_CMD_LEN];

    (void)server;
    snprintf(buf, MAX_CMD_LEN, "pnw #%d %d %d %d %d %s\n",
        player->id, player->pos_x, player->pos_y,
        player->orientation, player->level, player->team_name);
    send_to_client(gui, buf);
}
