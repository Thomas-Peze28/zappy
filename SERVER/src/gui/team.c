/*
** EPITECH PROJECT, 2026
** team.c
** File description:
** team and player commands
*/

#include "gui.h"
#include <stdio.h>

void gui_send_tna(server_t *server, client_t *gui)
{
    char buf[MAX_CMD_LEN];
    int i = 0;

    while (i < server->config.team_count) {
        snprintf(buf, MAX_CMD_LEN, "tna %s\n",
            server->config.team_names[i]);
        send_to_client(gui, buf);
        i++;
    }
}

void gui_send_ppo(server_t *server, client_t *gui, int player_id)
{
    client_t *player = find_player_by_id(server, player_id);
    char buf[MAX_CMD_LEN];

    if (player == NULL) {
        send_to_client(gui, "sbp\n");
        return;
    }
    snprintf(buf, MAX_CMD_LEN, "ppo #%d %d %d %d\n",
        player->id, player->pos_x, player->pos_y,
        player->orientation);
    send_to_client(gui, buf);
}

void gui_send_plv(server_t *server, client_t *gui, int player_id)
{
    client_t *player = find_player_by_id(server, player_id);
    char buf[MAX_CMD_LEN];

    if (player == NULL) {
        send_to_client(gui, "sbp\n");
        return;
    }
    snprintf(buf, MAX_CMD_LEN, "plv #%d %d\n",
        player->id, player->level);
    send_to_client(gui, buf);
}

void gui_send_pin(server_t *server, client_t *gui, int player_id)
{
    client_t *player = find_player_by_id(server, player_id);
    char buf[MAX_CMD_LEN];

    if (player == NULL) {
        send_to_client(gui, "sbp\n");
        return;
    }
    snprintf(buf, MAX_CMD_LEN,
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->pos_x, player->pos_y,
        player->inventory.food, player->inventory.linemate,
        player->inventory.deraumere, player->inventory.sibur,
        player->inventory.mendiane, player->inventory.phiras,
        player->inventory.thystame);
    send_to_client(gui, buf);
}

void gui_notify_pin(server_t *server, client_t *player)
{
    char buf[MAX_CMD_LEN];
    int i;

    snprintf(buf, MAX_CMD_LEN,
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->pos_x, player->pos_y,
        player->inventory.food, player->inventory.linemate,
        player->inventory.deraumere, player->inventory.sibur,
        player->inventory.mendiane, player->inventory.phiras,
        player->inventory.thystame);
    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_GUI)
            send_to_client(&server->clients[i], buf);
    }
}
