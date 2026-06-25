/*
** EPITECH PROJECT, 2026
** time.c
** File description:
** time commands
*/

#include "gui.h"
#include <stdio.h>

void gui_send_sgt(server_t *server, client_t *gui)
{
    char buf[MAX_CMD_LEN];

    snprintf(buf, MAX_CMD_LEN, "sgt %d\n", server->config.freq);
    send_to_client(gui, buf);
}

void gui_send_sst(server_t *server, client_t *gui, int freq)
{
    char buf[MAX_CMD_LEN];

    server->config.freq = freq;
    snprintf(buf, MAX_CMD_LEN, "sst %d\n", freq);
    send_to_client(gui, buf);
}
