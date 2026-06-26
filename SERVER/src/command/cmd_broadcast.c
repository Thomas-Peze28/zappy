/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** cmd_broadcast
*/

#include "command.h"
#include "gui.h"
#include <stdlib.h>
#include <stdio.h>

static int get_shortest_delta(int pos_sender, int pos_receiver, int map_max)
{
    int delta = pos_sender - pos_receiver;

    if (abs(delta) > map_max / 2) {
        if (delta > 0)
            delta -= map_max;
        else
            delta += map_max;
    }
    if (delta > 0)
        return 1;
    if (delta < 0)
        return -1;
    return 0;
}

int calculate_sound_direction(server_t *server, client_t *src, client_t *dst)
{
    int dx;
    int dy;
    int final_dir;
    const int dirs[3][3] = {
        {2, 3, 4},
        {1, 0, 5},
        {8, 7, 6}
    };

    if (src->pos_x == dst->pos_x && src->pos_y == dst->pos_y)
        return 0;
    dx = get_shortest_delta(src->pos_x, dst->pos_x, server->config.width);
    dy = get_shortest_delta(src->pos_y, dst->pos_y, server->config.height);
    final_dir = dirs[dy + 1][dx + 1] - ((dst->orientation - 1) * 2);
    return (final_dir <= 0) ? (final_dir + 8) : final_dir;
}

void cmd_broadcast(server_t *server, client_t *client, const char *text)
{
    char buf[1024];
    client_t *receiver;
    int dir;
    int i;

    for (i = 0; i < server->client_count; i++) {
        receiver = &server->clients[i];
        if (receiver->type == CLIENT_AI && receiver->fd != client->fd) {
            dir = calculate_sound_direction(server, client, receiver);
            snprintf(buf, sizeof(buf), "message %d, %s\n", dir, text);
            send_to_client(receiver, buf);
        }
    }
    gui_notify_pbc(server, client, text);
    send_to_client(client, "ok\n");
}
