/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_inventory
*/

#include "command.h"
#include <stdio.h>

void cmd_inventory(server_t *server, client_t *client)
{
    char buf[512];

    (void)server;
    snprintf(buf, sizeof(buf),
        "[food %d, linemate %d, deraumere %d, sibur %d, "
        "mendiane %d, phiras %d, thystame %d]\n",
        client->inventory.food,
        client->inventory.linemate,
        client->inventory.deraumere,
        client->inventory.sibur,
        client->inventory.mendiane,
        client->inventory.phiras,
        client->inventory.thystame);
    send_to_client(client, buf);
}
