/*
** EPITECH PROJECT, 2026
** gui_parser.c
** File description:
** command parsers
*/

#include "gui.h"
#include <stdio.h>

void handle_bct_cmd(server_t *server, client_t *gui, char *line)
{
    int x;
    int y;

    if (sscanf(line, "bct %d %d", &x, &y) != 2) {
        send_to_client(gui, "sbp\n");
        return;
    }
    gui_send_bct(server, gui, x, y);
}

void handle_ppo_cmd(server_t *server, client_t *gui, char *line)
{
    int id;

    if (sscanf(line, "ppo #%d", &id) != 1) {
        send_to_client(gui, "sbp\n");
        return;
    }
    gui_send_ppo(server, gui, id);
}

void handle_plv_cmd(server_t *server, client_t *gui, char *line)
{
    int id;

    if (sscanf(line, "plv #%d", &id) != 1) {
        send_to_client(gui, "sbp\n");
        return;
    }
    gui_send_plv(server, gui, id);
}

void handle_pin_cmd(server_t *server, client_t *gui, char *line)
{
    int id;

    if (sscanf(line, "pin #%d", &id) != 1) {
        send_to_client(gui, "sbp\n");
        return;
    }
    gui_send_pin(server, gui, id);
}

void handle_sst_cmd(server_t *server, client_t *gui, char *line)
{
    int freq;

    if (sscanf(line, "sst %d", &freq) != 1 || freq <= 0) {
        send_to_client(gui, "sbp\n");
        return;
    }
    gui_send_sst(server, gui, freq);
}
