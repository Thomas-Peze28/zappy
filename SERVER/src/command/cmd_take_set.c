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

static int *get_res_ptr(tile_t *tile, client_t *cl, const char *res, int tk)
{
    if (strcmp(res, "food") == 0)
        return tk ? &tile->food : &cl->inventory.food;
    if (strcmp(res, "linemate") == 0)
        return tk ? &tile->linemate : &cl->inventory.linemate;
    if (strcmp(res, "deraumere") == 0)
        return tk ? &tile->deraumere : &cl->inventory.deraumere;
    if (strcmp(res, "sibur") == 0)
        return tk ? &tile->sibur : &cl->inventory.sibur;
    if (strcmp(res, "mendiane") == 0)
        return tk ? &tile->mendiane : &cl->inventory.mendiane;
    if (strcmp(res, "phiras") == 0)
        return tk ? &tile->phiras : &cl->inventory.phiras;
    if (strcmp(res, "thystame") == 0)
        return tk ? &tile->thystame : &cl->inventory.thystame;
    return NULL;
}

static void clean_resource_name(const char *arg, char *dest)
{
    int i = 0;

    while (*arg == ' ' || *arg == '\t')
        arg++;
    while (arg[i] && arg[i] != ' ' && arg[i] != '\r' &&
        arg[i] != '\n' && i < 63) {
        dest[i] = arg[i];
        i++;
    }
    dest[i] = '\0';
}

void cmd_take(server_t *server, client_t *client, const char *arg)
{
    tile_t *tl = &server->map->tiles[client->pos_y][client->pos_x];
    char clean_res[64];
    int *t_res;
    int *p_res;

    clean_resource_name(arg, clean_res);
    t_res = get_res_ptr(tl, client, clean_res, 1);
    p_res = get_res_ptr(tl, client, clean_res, 0);
    if (t_res && p_res && *t_res > 0) {
        (*t_res)--;
        (*p_res)++;
        send_to_client(client, "ok\n");
        gui_notify_pgt(server, client, get_res_id(clean_res));
        gui_notify_bct(server, client->pos_x, client->pos_y);
        gui_notify_pin(server, client);
    } else
        send_to_client(client, "ko\n");
}

void cmd_set(server_t *server, client_t *client, const char *arg)
{
    tile_t *tl = &server->map->tiles[client->pos_y][client->pos_x];
    char clean_res[64];
    int *t_res;
    int *p_res;

    clean_resource_name(arg, clean_res);
    t_res = get_res_ptr(tl, client, clean_res, 1);
    p_res = get_res_ptr(tl, client, clean_res, 0);
    if (t_res && p_res && *p_res > 0) {
        (*p_res)--;
        (*t_res)++;
        send_to_client(client, "ok\n");
        gui_notify_pdr(server, client, get_res_id(clean_res));
        gui_notify_bct(server, client->pos_x, client->pos_y);
        gui_notify_pin(server, client);
    } else
        send_to_client(client, "ko\n");
}
