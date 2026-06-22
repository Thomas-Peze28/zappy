/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_look
*/

#include "command.h"
#include <string.h>

static void append_res(tile_t *tile, char *buf, size_t m)
{
    const char *names[] = {" food", " linemate", " deraumere", " sibur",
        " mendiane", " phiras", " thystame"};
    int counts[] = {tile->food, tile->linemate, tile->deraumere, tile->sibur,
        tile->mendiane, tile->phiras, tile->thystame};

    for (int res = 0; res < 7; res++) {
        for (int i = 0; i < counts[res]; i++) {
            strncat(buf, names[res], m);
        }
    }
}

static void append_tile_content(server_t *server, int x, int y, char *buf)
{
    tile_t *tile = &server->map->tiles[y][x];
    size_t m = 16384 - strlen(buf) - 1;

    strncat(buf, ",", m);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_AI &&
            server->clients[i].pos_x == x && server->clients[i].pos_y == y) {
            strncat(buf, " player", m);
        }
    }
    append_res(tile, buf, m);
}

static void get_look_vectors(enum orientation_e orientation, int v[4])
{
    if (orientation == NORTH) {
        v[1] = -1;
        v[2] = 1;
    }
    if (orientation == EAST) {
        v[0] = 1;
        v[3] = 1;
    }
    if (orientation == SOUTH) {
        v[1] = 1;
        v[2] = -1;
    }
    if (orientation == WEST) {
        v[0] = -1;
        v[3] = -1;
    }
}

static void handle_look_loop(server_t *server, client_t *cl, char *buf, int dp)
{
    int v[4] = {0, 0, 0, 0};
    int cx;
    int cy;
    int i;

    get_look_vectors(cl->orientation, v);
    for (i = 0; i < (2 * dp + 1); i++) {
        cx = (cl->pos_x + (v[0] * dp) - (v[2] * dp) + (v[2] * i)
            + server->config.width * dp) % server->config.width;
        cy = (cl->pos_y + (v[1] * dp) - (v[3] * dp) + (v[3] * i)
            + server->config.height * dp) % server->config.height;
        append_tile_content(server, cx, cy, buf);
    }
}

void cmd_look(server_t *server, client_t *client)
{
    char buf[16384] = "[";
    size_t m = sizeof(buf) - strlen(buf) - 1;

    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_AI &&
            server->clients[i].pos_x == client->pos_x &&
            server->clients[i].pos_y == client->pos_y) {
            strncat(buf, " player", m);
        }
    }
    append_res(&server->map->tiles[client->pos_y][client->pos_x], buf, m);
    for (int depth = 1; depth <= client->level; depth++) {
        handle_look_loop(server, client, buf, depth);
    }
    strncat(buf, "]\n", sizeof(buf) - strlen(buf) - 1);
    send_to_client(client, buf);
}
