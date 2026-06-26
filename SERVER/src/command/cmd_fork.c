/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** command_fork
*/

#include "server_config.h"
#include "timer.h"
#include "gui.h"
#include <stdio.h>
#include <string.h>

void cmd_fork(server_t *server, client_t *client)
{
    egg_t *new_egg;
    long long now = get_time_ms();

    if (server->egg_count >= 1024) {
        send_to_client(client, "ko\n");
        return;
    }
    new_egg = &server->eggs[server->egg_count];
    new_egg->id = server->egg_count + 1;
    strncpy(new_egg->team_name, client->team_name, TEAM_NAME_LEN - 1);
    new_egg->pos_x = client->pos_x;
    new_egg->pos_y = client->pos_y;
    new_egg->is_hatched = 0;
    new_egg->hatch_time = now + (600000 / server->config.freq);
    server->egg_count++;
    gui_notify_pfk(server, client);
    gui_notify_enw(server, new_egg, client->id);
    send_to_client(client, "ok\n");
}
