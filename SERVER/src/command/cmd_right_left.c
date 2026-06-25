/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_right_left
*/

#include "command.h"

void cmd_right(server_t *server, client_t *client)
{
    (void)server;
    client->orientation = (client->orientation % 4) + 1;
    send_to_client(client, "ok\n");
}

void cmd_left(server_t *server, client_t *client)
{
    (void)server;
    client->orientation = client->orientation - 1;
    if (client->orientation < NORTH)
        client->orientation = WEST;
    send_to_client(client, "ok\n");
}
