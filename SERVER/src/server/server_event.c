/*
** EPITECH PROJECT, 2026
** server_events.c
** File description:
** Handle poll events and starvation
*/

#include "server_config.h"
#include <unistd.h>
#include <string.h>
#include "command.h"
#include "timer.h"
#include "gui.h"
#include <stdio.h>

static long long get_command_time(server_t *server, const char *cmd)
{
    long long cycles = 7;

    if (strncmp(cmd, "Fork", 4) == 0 || strncmp(cmd, "Incantation", 11) == 0)
        cycles = 42;
    return get_time_ms() + ((cycles * 1000) / server->config.freq);
}

static void notify_incantation_players(server_t *server, client_t *client)
{
    int i;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type == CLIENT_AI &&
            server->clients[i].pos_x == client->pos_x &&
            server->clients[i].pos_y == client->pos_y &&
            server->clients[i].level == client->level) {
            send_to_client(&server->clients[i], "Elevation under way\n");
        }
    }
}

static int pre_check_incantation(server_t *server, client_t *client)
{
    tile_t *tile = &server->map->tiles[client->pos_y][client->pos_x];
    const incantation_req_t *req = &REQS[client->level - 1];
    int p_count = count_players_on_tile(server, client);

    if (client->level < 1 || client->level > 7)
        return 0;
    if (p_count < req->players || !check_tile_resources(tile, req))
        return 0;
    return 1;
}

static void handle_command(server_t *server, client_t *client, char *line)
{
    long long exec_time;

    if (strcmp(line, "Incantation") == 0) {
        if (!pre_check_incantation(server, client)) {
            send_to_client(client, "ko\n");
            return;
        }
        notify_incantation_players(server, client);
        gui_notify_pic(server, client);
    }
    exec_time = get_command_time(server, line);
    if (queue_command(client, line, exec_time) == -1)
        return;
}

static void process_client_lines(server_t *server, client_t *client)
{
    char *line = buffer_get_line(client);

    while (line != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) > 0) {
            printf("[SERVER] Received from AI #%d: '%s'\n", client->id, line);
            handle_command(server, client, line);
        }
        line = buffer_get_line(client);
    }
}

void handle_client_read(server_t *server, int i)
{
    client_t *client = &server->clients[i];

    if (buffer_read(client) == -1) {
        printf("[SERVER] Client #%d disconnected\n", client->id);
        disconnect_client(server, i);
        return;
    }
    if (client->type == CLIENT_UNKNOWN) {
        printf("[SERVER] New unknown client checking in...\n");
        handle_connection(server, i);
        return;
    }
    if (client->type == CLIENT_GUI) {
        handle_gui_command(server, i);
        return;
    }
    process_client_lines(server, client);
}

void handle_client_write(server_t *server, int i)
{
    buffer_write(&server->clients[i]);
    if (server->clients[i].pending_disconnect
        && server->clients[i].write_len == 0)
        disconnect_client(server, i);
}

void handle_poll_events(server_t *server, int nb_events)
{
    int i;

    if (nb_events <= 0)
        return;
    if (server->polls[0].revents & POLLIN)
        accept_client(server);
    for (i = server->client_count - 1; i >= 0; i--) {
        if (server->polls[i + 1].revents & POLLIN)
            handle_client_read(server, i);
        if (i < server->client_count &&
            server->polls[i + 1].revents & POLLOUT)
            handle_client_write(server, i);
    }
}
