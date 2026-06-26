/*
** EPITECH PROJECT, 2026
** notify_player.c
** File description:
** player event and end game notifications
*/

#include "gui.h"
#include <stdio.h>

void gui_notify_pnw(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pnw #%d %d %d %d %d %s\n",
        player->id, player->pos_x, player->pos_y,
        player->orientation, player->level, player->team_name);
    send_to_all_gui(server, buf);
}

void gui_notify_pex(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pex #%d\n", player->id);
    send_to_all_gui(server, buf);
}

void gui_notify_pbc(server_t *server, client_t *player,
    const char *msg)
{
    char buf[BUFFER_SIZE];

    snprintf(buf, BUFFER_SIZE, "pbc #%d %s\n", player->id, msg);
    send_to_all_gui(server, buf);
}

void gui_notify_pic(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pic %d %d %d #%d\n",
        player->pos_x, player->pos_y, player->level, player->id);
    send_to_all_gui(server, buf);
}

void gui_notify_pie(server_t *server, client_t *player, int result)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pie %d %d %d\n",
        player->pos_x, player->pos_y, result);
    send_to_all_gui(server, buf);
}
