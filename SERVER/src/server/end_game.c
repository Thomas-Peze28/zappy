/*
** EPITECH PROJECT, 2026
** end_game
** File description:
** end_game.c
*/

#include "server_config.h"
#include "gui.h"
#include <string.h>

static int count_level8_in_team(server_t *server, const char *team)
{
    int count = 0;

    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI)
            continue;
        if (strcmp(server->clients[i].team_name, team) != 0)
            continue;
        if (server->clients[i].level == 8)
            count++;
    }
    return count;
}

static const char *find_winning_team(server_t *server)
{
    for (int i = 0; i < server->config.team_count; i++) {
        if (count_level8_in_team(server,
            server->config.team_names[i]) >= 6)
            return server->config.team_names[i];
    }
    return NULL;
}

void check_end_game(server_t *server)
{
    const char *winner = find_winning_team(server);

    if (winner == NULL)
        return;
    gui_notify_seg(server, winner);
}
