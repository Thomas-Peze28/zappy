/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_incantation
*/

#include "command.h"
#include "gui.h"
#include <stdio.h>

int check_tile_resources(tile_t *tile, const incantation_req_t *req)
{
    if (tile->linemate < req->linemate || tile->deraumere < req->deraumere)
        return 0;
    if (tile->sibur < req->sibur || tile->mendiane < req->mendiane)
        return 0;
    if (tile->phiras < req->phiras || tile->thystame < req->thystame)
        return 0;
    return 1;
}

int count_players_on_tile(server_t *server, client_t *client)
{
    int count = 0;
    int lvl = client->level;

    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_AI &&
            server->clients[i].pos_x == client->pos_x &&
            server->clients[i].pos_y == client->pos_y &&
            server->clients[i].level == lvl) {
            count++;
        }
    }
    return count;
}

static void consume_resources(tile_t *tile, const incantation_req_t *req)
{
    tile->linemate -= req->linemate;
    tile->deraumere -= req->deraumere;
    tile->sibur -= req->sibur;
    tile->mendiane -= req->mendiane;
    tile->phiras -= req->phiras;
    tile->thystame -= req->thystame;
}

static void elevate_players_on_tile(server_t *server, client_t *client)
{
    int i;
    char buf[64];
    int orig_level = client->level;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI)
            continue;
        if (server->clients[i].pos_x != client->pos_x)
            continue;
        if (server->clients[i].pos_y != client->pos_y)
            continue;
        if (server->clients[i].level != orig_level)
            continue;
        server->clients[i].level++;
        snprintf(buf, sizeof(buf), "Current level: %d\n",
            server->clients[i].level);
        send_to_client(&server->clients[i], buf);
        gui_notify_plv(server, &server->clients[i]);
    }
}

static void unfreeze_tile_players(server_t *server, client_t *client)
{
    int i;
    int orig_level = client->level;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI)
            continue;
        if (server->clients[i].pos_x != client->pos_x)
            continue;
        if (server->clients[i].pos_y != client->pos_y)
            continue;
        if (server->clients[i].level != orig_level)
            continue;
        server->clients[i].is_incanting = 0;
    }
}

static void notify_ko_participants(server_t *server, client_t *client)
{
    int i;
    int orig_level = client->level;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI)
            continue;
        if (&server->clients[i] == client)
            continue;
        if (server->clients[i].pos_x != client->pos_x)
            continue;
        if (server->clients[i].pos_y != client->pos_y)
            continue;
        if (server->clients[i].level != orig_level)
            continue;
        send_to_client(&server->clients[i], "ko\n");
    }
}

void cmd_incantation(server_t *server, client_t *client)
{
    tile_t *tile = &server->map->tiles[client->pos_y][client->pos_x];
    const incantation_req_t *req = &REQS[client->level - 1];
    int p_count = count_players_on_tile(server, client);

    unfreeze_tile_players(server, client);
    if (p_count < req->players || !check_tile_resources(tile, req)) {
        send_to_client(client, "ko\n");
        notify_ko_participants(server, client);
        gui_notify_pie(server, client, 0);
        return;
    }
    consume_resources(tile, req);
    elevate_players_on_tile(server, client);
    gui_notify_pie(server, client, 1);
    check_end_game(server);
}
