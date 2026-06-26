/*
** EPITECH PROJECT, 2026
** server_utils.c
** File description:
** Server loop tools
*/

#include "server_config.h"
#include "command.h"
#include "timer.h"
#include "gui.h"

void setup_polls(server_t *server)
{
    int i;

    for (i = 0; i < server->client_count; i++) {
        server->polls[i + 1].fd = server->clients[i].fd;
        server->polls[i + 1].events = POLLIN;
        if (server->clients[i].write_len > 0)
            server->polls[i + 1].events |= POLLOUT;
    }
}

static void check_client_events(client_t *client, long long *next_event)
{
    long long cmd_time;

    if (client->type == CLIENT_AI && client->command_count > 0) {
        cmd_time = client->commands[0].execute_at;
        if (*next_event == -1 || cmd_time < *next_event)
            *next_event = cmd_time;
    }
    if (client->type == CLIENT_AI && !client->pending_disconnect) {
        if (*next_event == -1 || client->death_time < *next_event)
            *next_event = client->death_time;
    }
}

int get_next_timeout(server_t *server)
{
    long long now = get_time_ms();
    long long next_event = -1;
    int timeout;
    int i;

    for (i = 0; i < server->client_count; i++)
        check_client_events(&server->clients[i], &next_event);
    if (next_event == -1)
        return -1;
    timeout = (int)(next_event - now);
    return (timeout < 0) ? 0 : timeout;
}

void check_starvation(server_t *server)
{
    long long now = get_time_ms();
    int i;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i].type != CLIENT_AI ||
            server->clients[i].pending_disconnect)
            continue;
        if (now < server->clients[i].death_time)
            continue;
        if (server->clients[i].inventory.food > 0) {
            server->clients[i].inventory.food--;
            server->clients[i].death_time += (126000 / server->config.freq);
        } else {
            send_to_client(&server->clients[i], "dead\n");
            gui_notify_pdi(server, &server->clients[i]);
            server->clients[i].pending_disconnect = 1;
        }
    }
}
