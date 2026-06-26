/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_take_set
*/

#include "command.h"
#include "gui.h"
#include <string.h>

static int get_res_id(const char *res)
{
    const char *names[] = {"food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame"};

    for (int i = 0; i < 7; i++) {
        if (strcmp(res, names[i]) == 0)
            return i;
    }
    return -1;
}

static int *get_resource_ptr_tile(tile_t *tile, const char *res)
{
    if (strcmp(res, "food") == 0)
        return &tile->food;
    if (strcmp(res, "linemate") == 0)
        return &tile->linemate;
    if (strcmp(res, "deraumere") == 0)
        return &tile->deraumere;
    if (strcmp(res, "sibur") == 0)
        return &tile->sibur;
    if (strcmp(res, "mendiane") == 0)
        return &tile->mendiane;
    if (strcmp(res, "phiras") == 0)
        return &tile->phiras;
    if (strcmp(res, "thystame") == 0)
        return &tile->thystame;
    return NULL;
}

static int *get_resource_ptr_player(client_t *client, const char *res)
{
    if (strcmp(res, "food") == 0)
        return &client->inventory.food;
    if (strcmp(res, "linemate") == 0)
        return &client->inventory.linemate;
    if (strcmp(res, "deraumere") == 0)
        return &client->inventory.deraumere;
    if (strcmp(res, "sibur") == 0)
        return &client->inventory.sibur;
    if (strcmp(res, "mendiane") == 0)
        return &client->inventory.mendiane;
    if (strcmp(res, "phiras") == 0)
        return &client->inventory.phiras;
    if (strcmp(res, "thystame") == 0)
        return &client->inventory.thystame;
    return NULL;
}

void cmd_take(server_t *server, client_t *client, const char *arg)
{
    tile_t *tile = &server->map->tiles[client->pos_y][client->pos_x];
    int *tile_res = get_resource_ptr_tile(tile, arg);
    int *player_res = get_resource_ptr_player(client, arg);
    int res_id = get_res_id(arg);

    if (tile_res && player_res && *tile_res > 0) {
        (*tile_res)--;
        (*player_res)++;
        send_to_client(client, "ok\n");
        if (res_id != -1)
            gui_notify_pgt(server, client, res_id);
    } else {
        send_to_client(client, "ko\n");
    }
}

void cmd_set(server_t *server, client_t *client, const char *arg)
{
    tile_t *tile = &server->map->tiles[client->pos_y][client->pos_x];
    int *tile_res = get_resource_ptr_tile(tile, arg);
    int *player_res = get_resource_ptr_player(client, arg);

    if (tile_res && player_res && *player_res > 0) {
        (*player_res)--;
        (*tile_res)++;
        send_to_client(client, "ok\n");
        gui_notify_pdr(server, client, get_res_id(arg));
    } else {
        send_to_client(client, "ko\n");
    }
}
