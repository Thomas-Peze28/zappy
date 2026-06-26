/*
** EPITECH PROJECT, 2026
** notify_action.c
** File description:
** action event notifications
*/

#include "gui.h"
#include <stdio.h>

void gui_notify_pdr(server_t *server, client_t *player, int resource)
{
    char buf[MAX_CMD_LEN];

    if (resource < 0 || resource > 6)
        return;
    snprintf(buf, MAX_CMD_LEN, "pdr %d %d\n", player->id, resource);
    send_to_all_gui(server, buf);
}

void gui_notify_pgt(server_t *server, client_t *player, int resource)
{
    char buf[MAX_CMD_LEN];

    if (resource < 0 || resource > 6)
        return;
    snprintf(buf, MAX_CMD_LEN, "pgt %d %d\n", player->id, resource);
    send_to_all_gui(server, buf);
}

void gui_notify_pdi(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "pdi #%d\n", player->id);
    send_to_all_gui(server, buf);
}

void gui_notify_seg(server_t *server, const char *team)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "seg %s\n", team);
    send_to_all_gui(server, buf);
}

void gui_notify_plv(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "plv #%d %d\n",
        player->id, player->level);
    send_to_all_gui(server, buf);
}
