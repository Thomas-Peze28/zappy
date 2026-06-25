/*
** EPITECH PROJECT, 2026
** gui_handler.c
** File description:
** command handler and dispatch table
*/

#include "gui.h"
#include <string.h>

void dispatch_msz(server_t *server, client_t *gui, char *line)
{
    (void)line;
    gui_send_msz(server, gui);
}

void dispatch_mct(server_t *server, client_t *gui, char *line)
{
    (void)line;
    gui_send_mct(server, gui);
}

void dispatch_tna(server_t *server, client_t *gui, char *line)
{
    (void)line;
    gui_send_tna(server, gui);
}

void dispatch_sgt(server_t *server, client_t *gui, char *line)
{
    (void)line;
    gui_send_sgt(server, gui);
}

static const gui_cmd_t GUI_COMMANDS[] = {
    {"msz", dispatch_msz},
    {"mct", dispatch_mct},
    {"tna", dispatch_tna},
    {"sgt", dispatch_sgt},
    {"bct", handle_bct_cmd},
    {"ppo", handle_ppo_cmd},
    {"plv", handle_plv_cmd},
    {"pin", handle_pin_cmd},
    {"sst", handle_sst_cmd},
    {NULL, NULL}
};

static int route_gui_command(server_t *server, client_t *gui, char *line)
{
    int i = 0;

    while (GUI_COMMANDS[i].cmd != NULL) {
        if (strncmp(line, GUI_COMMANDS[i].cmd, 3) == 0) {
            GUI_COMMANDS[i].handler(server, gui, line);
            return 1;
        }
        i++;
    }
    return 0;
}

void handle_gui_command(server_t *server, int idx)
{
    client_t *gui = &server->clients[idx];
    char *line = buffer_get_line(gui);

    if (line == NULL)
        return;
    if (!route_gui_command(server, gui, line))
        send_to_client(gui, "suc\n");
}
