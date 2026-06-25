/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include <stdio.h>
#include <stdlib.h>
#include "server_config.h"
#include <time.h>

void free_config(server_config_t *config)
{
    if (config->team_names == NULL)
        return;
    free(config->team_names);
    config->team_names = NULL;
    config->team_count = 0;
}

int run(int argc, char **argv)
{
    server_t *server = malloc(sizeof(server_t));

    if (server == NULL)
        return 84;
    if (parse_args(argc, argv, &server->config) == 84) {
        free(server);
        return 84;
    }
    if (create_server(server) == -1) {
        free_config(&server->config);
        free(server);
        return 84;
    }
    run_server(server);
    close_server(server);
    free_config(&server->config);
    free(server);
    return 0;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    return run(argc, argv);
}
