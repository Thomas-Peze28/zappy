/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_right_left
*/

#include "command.h"
#include "gui.h"

void cmd_right(server_t *server, client_t *client)
{
    switch (client->orientation) {
        case NORTH:
            client->orientation = EAST;
            break;
        case EAST:
            client->orientation = SOUTH;
            break;
        case SOUTH:
            client->orientation = WEST;
            break;
        case WEST:
            client->orientation = NORTH;
            break;
    }
    gui_notify_ppo(server, client);
    send_to_client(client, "ok\n");
}

void cmd_left(server_t *server, client_t *client)
{
    switch (client->orientation) {
        case NORTH:
            client->orientation = WEST;
            break;
        case WEST:
            client->orientation = SOUTH;
            break;
        case SOUTH:
            client->orientation = EAST;
            break;
        case EAST:
            client->orientation = NORTH;
            break;
    }
    gui_notify_ppo(server, client);
    send_to_client(client, "ok\n");
}
