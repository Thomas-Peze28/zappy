/*
** EPITECH PROJECT, 2026
** notify_egg.c
** File description:
** egg notifications
*/

#include "gui.h"
#include <stdio.h>

void gui_notify_enw(server_t *server, egg_t *egg, int player_id)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "enw %d %d %d %d\n",
        egg->id, player_id, egg->pos_x, egg->pos_y);
    send_to_all_gui(server, buf);
}

void gui_notify_ebo(server_t *server, egg_t *egg)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "ebo %d\n", egg->id);
    send_to_all_gui(server, buf);
}

void gui_notify_edi(server_t *server, egg_t *egg)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "edi %d\n", egg->id);
    send_to_all_gui(server, buf);
}

void gui_notify_pfk(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pfk %d\n", player->id);
    send_to_all_gui(server, buf);
}

void gui_notify_ppo(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "ppo %d %d %d %d\n",
        player->id, player->pos_x, player->pos_y,
        player->orientation);
    send_to_all_gui(server, buf);
}
