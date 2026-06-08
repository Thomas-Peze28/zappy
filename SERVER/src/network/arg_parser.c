/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server config
*/

#include "server_config.h"
#include <stddef.h>

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

static int check_required_fields(server_config_t *config)
{
    if (!is_valid_positive(config->port))
        return 0;
    if (!is_valid_positive(config->width))
        return 0;
    if (!is_valid_positive(config->height))
        return 0;
    if (!is_valid_positive(config->clients_nb))
        return 0;
    if (config->team_count == 0 || config->team_names == NULL)
        return 0;
    return 1;
}

int validate_config(server_config_t *config)
{
    return check_required_fields(config);
}
