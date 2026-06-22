/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server config
*/

#include "server_config.h"
#include <stddef.h>
#include <unistd.h>

void init_config(server_config_t *config)
{
    config->port = -1;
    config->width = -1;
    config->height = -1;
    config->clients_nb = -1;
    config->freq = DEFAULT_FREQ;
    config->team_names = NULL;
    config->team_count = 0;
}

static int is_valid_positive(int value)
{
    return value > 0;
}

static int check_map_and_teams(server_config_t *config)
{
    if (!is_valid_positive(config->width) ||
        !is_valid_positive(config->height)) {
        write(2, "Error: Invalid map dimensions\n", 30);
        return 0;
    }
    if (config->team_count == 0 || config->team_names == NULL) {
        write(2, "Error: No team names provided\n", 31);
        return 0;
    }
    return 1;
}

int validate_config(server_config_t *config)
{
    if (!is_valid_positive(config->port)) {
        write(2, "Error: Invalid port number\n", 28);
        return 0;
    }
    if (!is_valid_positive(config->clients_nb)) {
        write(2, "Error: Invalid client count\n", 28);
        return 0;
    }
    return check_map_and_teams(config);
}
