/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Client
*/

#include "server_config.h"
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

static const char WELCOME[] = "WELCOME\n";

void init_client(client_t *client)
{
    client->fd = -1;
    client->read_len = 0;
    client->write_len = 0;
    client->type = CLIENT_UNKNOWN;
    client->is_connected = 0;
    client->pending_disconnect = 0;
    memset(client->read_buf, 0, BUFFER_SIZE);
    memset(client->write_buf, 0, BUFFER_SIZE);
    memset(client->line_buf, 0, BUFFER_SIZE);
    memset(client->team_name, 0, TEAM_NAME_LEN);
}

static void send_welcome(client_t *client)
{
    int len = sizeof(WELCOME) - 1;

    memcpy(client->write_buf, WELCOME, len);
    client->write_len = len;
    buffer_write(client);
}

int accept_client(server_t *server)
{
    int new_fd = accept(server->fd, NULL, NULL);
    int idx = server->client_count;

    if (new_fd == -1)
        return -1;
    if (idx >= MAX_CLIENTS) {
        close(new_fd);
        return -1;
    }
    init_client(&server->clients[idx]);
    server->clients[idx].fd = new_fd;
    server->clients[idx].is_connected = 1;
    server->client_count++;
    send_welcome(&server->clients[idx]);
    return 0;
}

void disconnect_client(server_t *server, int idx)
{
    close(server->clients[idx].fd);
    init_client(&server->clients[idx]);
    server->clients[idx] = server->clients[server->client_count - 1];
    server->client_count--;
}
