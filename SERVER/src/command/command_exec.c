/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** command exec router
*/

#include "command.h"
#include "timer.h"
#include <string.h>

int command_ready(command_t *cmd)
{
    return (get_time_ms() >= cmd->execute_at);
}

static int execute_ai_interactions(server_t *srv, client_t *cl,
    const char *cmd)
{
    const char *cmds[] = {"Look", "Incantation", "Fork", NULL};
    void (*funcs[])(server_t *, client_t *) = {cmd_look, cmd_incantation,
        cmd_fork};

    for (int i = 0; cmds[i] != NULL; i++) {
        if (strcmp(cmd, cmds[i]) == 0) {
            funcs[i](srv, cl);
            return 1;
        }
    }
    if (strncmp(cmd, "Take ", 5) == 0) {
        cmd_take(srv, cl, cmd + 5);
        return 1;
    }
    if (strncmp(cmd, "Set ", 4) == 0) {
        cmd_set(srv, cl, cmd + 4);
        return 1;
    }
    return 0;
}

void execute_ai_command(server_t *server, client_t *client, const char *cmd)
{
    const char *cmds[] = {"Forward", "Right", "Left", "Inventory", NULL};
    void (*funcs[])(server_t *, client_t *) = {cmd_forward, cmd_right,
        cmd_left, cmd_inventory};

    for (int i = 0; cmds[i] != NULL; i++) {
        if (strcmp(cmd, cmds[i]) == 0) {
            funcs[i](server, client);
            return;
        }
    }
    if (strncmp(cmd, "Broadcast ", 10) == 0) {
        cmd_broadcast(server, client, cmd + 10);
        return;
    }
    if (execute_ai_interactions(server, client, cmd))
        return;
    send_to_client(client, "ko\n");
}

void update_game(server_t *server)
{
    client_t *cl;

    for (int i = 0; i < server->client_count; i++) {
        cl = &server->clients[i];
        if (cl->type == CLIENT_AI && !cl->pending_disconnect &&
            cl->command_count > 0 && command_ready(&cl->commands[0])) {
            execute_ai_command(server, cl, cl->commands[0].command);
            pop_command(cl);
        }
    }
    refill_map(server);
}
