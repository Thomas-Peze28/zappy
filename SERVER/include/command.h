/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** command
*/

#include "server_config.h"

#ifndef COMMAND_H
    #define COMMAND_H

typedef struct incantation_req_s {
    int players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} incantation_req_t;

static const incantation_req_t REQS[8] = {
    {0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1}
};

typedef struct command_entry_s {
    const char *name;
    int cost;
    void (*handler)(server_t *, client_t *);
} command_entry_t;

int queue_command(client_t *client,
    const char *name,
    long long execute_at);
void pop_command(client_t *client);
void update_game(server_t *server);
int command_ready(command_t *cmd);
int count_players_on_tile(server_t *server, client_t *client);
int check_tile_resources(tile_t *tile, const incantation_req_t *req);

void cmd_forward(server_t *server, client_t *client);
void cmd_right(server_t *server, client_t *client);
void cmd_left(server_t *server, client_t *client);
void cmd_inventory(server_t *server, client_t *client);
void cmd_take(server_t *server, client_t *client, const char *arg);
void cmd_set(server_t *server, client_t *client, const char *arg);
void cmd_broadcast(server_t *server, client_t *client, const char *text);
void cmd_look(server_t *server, client_t *client);
void cmd_incantation(server_t *server, client_t *client);
void cmd_fork(server_t *server, client_t *client);

#endif
