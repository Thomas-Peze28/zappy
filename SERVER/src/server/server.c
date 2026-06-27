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
#include "command.h"
#include "gui.h"

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
    server->map = create_map(server->config.width, server->config.height);
    if (server->map == NULL)
        return -1;
    spawn_resources(server->map, 1);
    server->fd = fd;
    server->client_count = 0;
    server->next_client_id = 1;
    server->polls[0].fd = fd;
    server->polls[0].events = POLLIN;
    server->next_refill = get_time_ms() + 20000;
    return 0;
}

void run_server(server_t *server)
{
    int nb_events;
    int timeout;

    while (1) {
        setup_polls(server);
        timeout = get_next_timeout(server);
        nb_events = poll(server->polls, server->client_count + 1, timeout);
        handle_poll_events(server, nb_events);
        check_starvation(server);
        update_game(server);
    }
}

void close_server(server_t *server)
{
    int i;

    for (i = 0; i < server->client_count; i++)
        disconnect_client(server, i);
    destroy_map(server->map);
    close(server->fd);
}
