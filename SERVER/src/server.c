/*
** EPITECH PROJECT, 2026
** server.c
** File description:
** server.c
*/

#include "server_config.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

static int setup_socket(int port)
{
    struct sockaddr_in addr;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (fd == -1)
        return -1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        return -1;
    return fd;
}

int create_server(server_t *server)
{
    int fd = setup_socket(server->config.port);

    if (fd == -1)
        return -1;
    if (listen(fd, MAX_CLIENTS) == -1)
        return -1;
    server->fd = fd;
    server->client_count = 0;
    server->polls[0].fd = fd;
    server->polls[0].events = POLLIN;
    return 0;
}

static void handle_client_read(server_t *server, int i)
{
    int ret = buffer_read(&server->clients[i]);

    if (ret == -1) {
        disconnect_client(server, i);
        return;
    }
    if (server->clients[i].type == CLIENT_UNKNOWN)
        handle_connection(server, i);
}

static void handle_client_write(server_t *server, int i)
{
    buffer_write(&server->clients[i]);
    if (server->clients[i].pending_disconnect
        && server->clients[i].write_len == 0)
        disconnect_client(server, i);
}

static void handle_poll_events(server_t *server, int nb_events)
{
    if (nb_events <= 0)
        return;
    if (server->polls[0].revents & POLLIN)
        accept_client(server);
    for (int i = 0; i < server->client_count; i++) {
        if (server->polls[i + 1].revents & POLLIN)
            handle_client_read(server, i);
        if (server->polls[i + 1].revents & POLLOUT)
            handle_client_write(server, i);
    }
}

static void setup_polls(server_t *server)
{
    for (int i = 0; i < server->client_count; i++) {
        server->polls[i + 1].fd = server->clients[i].fd;
        server->polls[i + 1].events = POLLIN;
        if (server->clients[i].write_len > 0)
            server->polls[i + 1].events |= POLLOUT;
    }
}

void run_server(server_t *server)
{
    int nb_events = 0;

    while (1) {
        setup_polls(server);
        nb_events = poll(server->polls,
            server->client_count + 1, -1);
        handle_poll_events(server, nb_events);
    }
}

void close_server(server_t *server)
{
    for (int i = 0; i < server->client_count; i++)
        disconnect_client(server, i);
    close(server->fd);
}
