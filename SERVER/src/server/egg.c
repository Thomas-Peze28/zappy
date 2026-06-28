/*
** EPITECH PROJECT, 2026
** egg.c
** File description:
** egg management
*/

#include "server_config.h"
#include "timer.h"
#include "gui.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void update_eggs(server_t *server)
{
    long long now = get_time_ms();

    for (int i = 0; i < server->egg_count; i++) {
        if (!server->eggs[i].is_hatched && server->eggs[i].hatch_time > 0
            && now >= server->eggs[i].hatch_time) {
            server->eggs[i].is_hatched = 1;
        }
    }
}

static void create_egg(server_t *server, const char *team,
    int player_id, int hatched)
{
    egg_t *egg = &server->eggs[server->egg_count];

    egg->id = server->egg_count + 1;
    strncpy(egg->team_name, team, TEAM_NAME_LEN - 1);
    egg->pos_x = rand() % server->config.width;
    egg->pos_y = rand() % server->config.height;
    egg->is_hatched = hatched;
    egg->hatch_time = 0;
    server->egg_count++;
    gui_notify_enw(server, egg, player_id);
}

void init_team_eggs(server_t *server)
{
    for (int i = 0; i < server->config.team_count; i++) {
        for (int j = 0; j < server->config.clients_nb; j++)
            create_egg(server, server->config.team_names[i], 0, 1);
    }
}

egg_t *find_available_egg(server_t *server, const char *team)
{
    for (int i = 0; i < server->egg_count; i++) {
        if (server->eggs[i].is_hatched &&
            strcmp(server->eggs[i].team_name, team) == 0)
            return &server->eggs[i];
    }
    return NULL;
}

void hatch_egg(server_t *server, egg_t *egg)
{
    (void)server;
    egg->is_hatched = 1;
}

void gui_send_all_eggs(server_t *server, client_t *gui)
{
    char buf[MAX_CMD_LEN];

    for (int i = 0; i < server->egg_count; i++) {
        snprintf(buf, MAX_CMD_LEN, "enw #%d #%d %d %d\n",
            server->eggs[i].id, 0,
            server->eggs[i].pos_x, server->eggs[i].pos_y);
        send_to_client(gui, buf);
    }
}
