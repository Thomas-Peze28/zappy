/*
** EPITECH PROJECT, 2026
** gui_utils.c
** File description:
** gui_utils.c
*/

#include "gui.h"
#include <stdio.h>

void send_to_all_gui(server_t *server, const char *msg)
{
    int i = 0;

    while (i < server->client_count) {
        if (server->clients[i].type == CLIENT_GUI)
            send_to_client(&server->clients[i], msg);
        i++;
    }
}

client_t *find_player_by_id(server_t *server, int id)
{
    int i = 0;

    while (i < server->client_count) {
        if (server->clients[i].id == id)
            return &server->clients[i];
        i++;
    }
    return NULL;
}
