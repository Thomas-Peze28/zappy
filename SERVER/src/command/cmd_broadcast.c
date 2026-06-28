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
#include <math.h>

static int get_toric_delta(int pos_sender, int pos_receiver, int map_max)
{
    int delta = pos_sender - pos_receiver;

    if (delta > map_max / 2)
        delta -= map_max;
    else if (delta < -map_max / 2)
        delta += map_max;
    return delta;
}

int calculate_sound_direction(server_t *server, client_t *src, client_t *dst)
{
    int dx;
    int dy;
    double angle;
    int zappy_abs_dirs[8] = {3, 2, 1, 8, 7, 6, 5, 4};
    int final_dir;

    if (src->pos_x == dst->pos_x && src->pos_y == dst->pos_y)
        return 0;
    dx = get_toric_delta(src->pos_x, dst->pos_x, server->config.width);
    dy = get_toric_delta(dst->pos_y, src->pos_y, server->config.height);
    angle = atan2(dy, dx);
    if (angle < 0)
        angle += 2.0 * M_PI;
    final_dir = zappy_abs_dirs[(int)round(angle / (M_PI / 4.0)) % 8];
    final_dir -= ((dst->orientation - 1) * 2);
    if (final_dir <= 0)
        final_dir += 8;
    return final_dir;
}

void cmd_broadcast(server_t *server, client_t *client, const char *text)
{
    char buf[1024];
    client_t *receiver;
    int dir;
    int i;

    for (i = 0; i < server->client_count; i++) {
        receiver = &server->clients[i];
        if (receiver->type != CLIENT_AI)
            continue;
        dir = calculate_sound_direction(server, client, receiver);
        snprintf(buf, sizeof(buf), "message %d, %s\n", dir, text);
        send_to_client(receiver, buf);
    }
    gui_notify_pbc(server, client, text);
    send_to_client(client, "ok\n");
}
