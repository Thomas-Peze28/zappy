/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** command queue
*/

#include "server_config.h"
#include "command.h"
#include "timer.h"

static void fill_command(command_t *cmd, const char *name)
{
    int i = 0;

    while (name[i]) {
        cmd->command[i] = name[i];
        i++;
    }
    cmd->command[i] = '\0';
}

int queue_command(client_t *client, const char *name, long long execute_at)
{
    long long delay = 0;

    if (client->command_count >= MAX_QUEUE_SIZE)
        return -1;
    fill_command(&client->commands[client->command_count], name);
    if (client->command_count > 0) {
        delay = execute_at - get_time_ms();
        if (delay < 0) {
            delay = 0;
        }
        client->commands[client->command_count].execute_at =
            client->commands[client->command_count - 1].execute_at + delay;
    } else {
        client->commands[client->command_count].execute_at = execute_at;
    }
    client->command_count++;
    return 0;
}

void pop_command(client_t *client)
{
    if (client->command_count <= 0)
        return;
    for (int i = 0; i < client->command_count - 1; i++) {
        client->commands[i] = client->commands[i + 1];
    }
    client->command_count--;
}
