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

static long long get_command_time(server_t *server, const char *cmd)
{
    (void)server;
    if (strcmp(cmd, "Fork") == 0)
        return get_time_ms() + (42000 / server->config.freq);
    return get_time_ms() + (7000 / server->config.freq);
}

static void handle_command(server_t *server, client_t *client, char *line)
{
    long long exec_time;

    exec_time = get_command_time(server, line);
    if (queue_command(client, line, exec_time) == -1)
        client->pending_disconnect = 1;
}

void handle_client_read(server_t *server, int i)
{
    client_t *client = &server->clients[i];
    char *line;

    if (buffer_read(client) == -1) {
        disconnect_client(server, i);
        return;
    }
    if (client->type == CLIENT_UNKNOWN) {
        handle_connection(server, i);
        return;
    }
    line = buffer_get_line(client);
    if (line != NULL)
        handle_command(server, client, line);
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
    for (i = 0; i < server->client_count; i++) {
        if (server->polls[i + 1].revents & POLLIN)
            handle_client_read(server, i);
        if (server->polls[i + 1].revents & POLLOUT)
            handle_client_write(server, i);
    }
}
