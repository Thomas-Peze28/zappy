/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** cmd_forward
*/

#include "command.h"
#include "gui.h"

void cmd_forward(server_t *server, client_t *client)
{
    if (client->orientation == NORTH)
        client->pos_y = (client->pos_y - 1 + server->config.height)
            % server->config.height;
    if (client->orientation == EAST)
        client->pos_x = (client->pos_x + 1) % server->config.width;
    if (client->orientation == SOUTH)
        client->pos_y = (client->pos_y + 1) % server->config.height;
    if (client->orientation == WEST)
        client->pos_x = (client->pos_x - 1 + server->config.width)
            % server->config.width;
    gui_notify_ppo(server, client);
    send_to_client(client, "ok\n");
}
