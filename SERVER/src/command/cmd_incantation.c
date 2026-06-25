/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_incantation
*/

#include "command.h"
#include <stdio.h>

static int check_tile_resources(tile_t *tile, const incantation_req_t *req)
{
    if (tile->linemate < req->linemate || tile->deraumere < req->deraumere)
        return 0;
    if (tile->sibur < req->sibur || tile->mendiane < req->mendiane)
        return 0;
    if (tile->phiras < req->phiras || tile->thystame < req->thystame)
        return 0;
    return 1;
}

static int count_players_on_tile(server_t *server, client_t *client)
{
    int count = 0;

    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_AI &&
            server->clients[i].pos_x == client->pos_x &&
            server->clients[i].pos_y == client->pos_y &&
            server->clients[i].level == client->level) {
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

void cmd_incantation(server_t *server, client_t *client)
{
    tile_t *tile = &server->map->tiles[client->pos_y][client->pos_x];
    const incantation_req_t *req;
    char buf[64];
    int p_count = count_players_on_tile(server, client);

    if (client->level < 1 || client->level > 7) {
        send_to_client(client, "ko\n");
        return;
    }
    req = &REQS[client->level - 1];
    if (p_count < req->players || !check_tile_resources(tile, req)) {
        send_to_client(client, "ko\n");
        return;
    }
    consume_resources(tile, req);
    client->level++;
    snprintf(buf, sizeof(buf), "Current level: %d\n", client->level);
    send_to_client(client, buf);
}
