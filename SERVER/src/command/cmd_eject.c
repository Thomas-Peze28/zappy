/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** cmd_eject
*/

#include "command.h"
#include "gui.h"
#include <stdio.h>

static void get_push_delta(client_t *ejector, int *dx, int *dy)
{
    *dx = 0;
    *dy = 0;
    if (ejector->orientation == NORTH)
        *dy = -1;
    else if (ejector->orientation == EAST)
        *dx = 1;
    else if (ejector->orientation == SOUTH)
        *dy = 1;
    else if (ejector->orientation == WEST)
        *dx = -1;
}

static int get_eject_direction(client_t *ejector, client_t *ejected)
{
    int abs_dir = (ejector->orientation - 1) * 2 + 1;
    int k = abs_dir - (ejected->orientation - 1) * 2;

    while (k <= 0)
        k += 8;
    return k;
}

static void push_player(server_t *server, client_t *ejector, client_t *ejected)
{
    int dx;
    int dy;
    int k;
    char buf[32];

    get_push_delta(ejector, &dx, &dy);
    k = get_eject_direction(ejector, ejected);
    ejected->pos_x = (ejected->pos_x + dx + server->config.width)
        % server->config.width;
    ejected->pos_y = (ejected->pos_y + dy + server->config.height)
        % server->config.height;
    snprintf(buf, sizeof(buf), "eject: %d\n", k);
    send_to_client(ejected, buf);
    gui_notify_ppo(server, ejected);
}

static void destroy_eggs_on_tile(server_t *server, int x, int y)
{
    int i;

    for (i = 0; i < server->egg_count; i++) {
        if (server->eggs[i].pos_x == x && server->eggs[i].pos_y == y
            && server->eggs[i].hatch_time >= 0) {
            gui_notify_edi(server, &server->eggs[i]);
            server->eggs[i].hatch_time = -1;
        }
    }
}

void cmd_connect_nbr(server_t *server, client_t *client)
{
    char buf[32];
    int slots = get_team_slots(server, client->team_name);

    snprintf(buf, sizeof(buf), "%d\n", slots);
    send_to_client(client, buf);
}

void cmd_eject(server_t *server, client_t *client)
{
    int i;

    destroy_eggs_on_tile(server, client->pos_x, client->pos_y);
    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI)
            continue;
        if (server->clients[i].fd == client->fd)
            continue;
        if (server->clients[i].pos_x != client->pos_x)
            continue;
        if (server->clients[i].pos_y != client->pos_y)
            continue;
        push_player(server, client, &server->clients[i]);
    }
    gui_notify_pex(server, client);
    send_to_client(client, "ok\n");
}
