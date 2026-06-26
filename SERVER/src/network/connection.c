/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client connection handling
*/

#include "server_config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include "gui.h"
#include "command.h"
#include <unistd.h>

int find_team(server_config_t *config, const char *name)
{
    for (int i = 0; i < config->team_count; i++) {
        if (strcmp(config->team_names[i], name) == 0)
            return i;
    }
    return -1;
}

void send_to_client(client_t *client, const char *msg)
{
    int len = strlen(msg);
    int available = BUFFER_SIZE - client->write_len;

    if (len > available) {
        return;
    }
    memcpy(client->write_buf + client->write_len, msg, len);
    client->write_len += len;
}

int get_team_slots(server_t *server, const char *team)
{
    int slots = server->config.clients_nb;

    for (int i = 0; i < server->client_count; i++) {
        if (strcmp(server->clients[i].team_name, team) == 0)
            slots--;
    }
    return slots;
}

static void handle_gui(server_t *server, int idx)
{
    client_t *gui = &server->clients[idx];

    gui->type = CLIENT_GUI;
    strncpy(gui->team_name, "GRAPHIC", TEAM_NAME_LEN - 1);
    gui_send_msz(server, gui);
    gui_send_mct(server, gui);
}

static void send_ai_info(server_t *server, int idx, const char *team)
{
    char buf[MAX_CMD_LEN];
    int slots = get_team_slots(server, team);

    snprintf(buf, MAX_CMD_LEN, "%d\n", slots);
    send_to_client(&server->clients[idx], buf);
    snprintf(buf, MAX_CMD_LEN, "%d %d\n",
        server->config.width, server->config.height);
    send_to_client(&server->clients[idx], buf);
}

static void spawn_player(server_t *server, client_t *client)
{
    long long now = get_time_ms();

    client->id = server->next_client_id;
    server->next_client_id += 1;
    client->pos_x = rand() % server->config.width;
    client->pos_y = rand() % server->config.height;
    client->orientation = (rand() % 4) + 1;
    client->level = 1;
    client->inventory.food = 10;
    client->death_time = now + (1260000 / server->config.freq);
}

static void handle_ai(server_t *server, int idx, const char *team)
{
    server->clients[idx].type = CLIENT_AI;
    strncpy(server->clients[idx].team_name, team, TEAM_NAME_LEN - 1);
    spawn_player(server, &server->clients[idx]);
    send_ai_info(server, idx, team);
    gui_notify_pnw(server, &server->clients[idx]);
}

void handle_connection(server_t *server, int idx)
{
    char *line = buffer_get_line(&server->clients[idx]);
    int team_idx = 0;

    if (line == NULL)
        return;
    if (strcmp(line, "GRAPHIC") == 0) {
        handle_gui(server, idx);
        return;
    }
    team_idx = find_team(&server->config, line);
    if (team_idx == -1) {
        send_to_client(&server->clients[idx], "ko\n");
        server->clients[idx].pending_disconnect = 1;
        return;
    }
    handle_ai(server, idx, line);
}
