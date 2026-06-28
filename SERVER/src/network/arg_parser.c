/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Server config
*/

#include "server_config.h"
#include <stddef.h>
#include <stdio.h>

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

static int print_err(const char *msg)
{
    fprintf(stderr, "%s", msg);
    return 0;
}

static int check_required_fields(server_config_t *config)
{
    if (!is_valid_positive(config->port))
        return print_err("[ERROR] Invalid or missing port (-p).\n");
    if (!is_valid_positive(config->width))
        return print_err("[ERROR] Invalid or missing width (-x).\n");
    if (!is_valid_positive(config->height))
        return print_err("[ERROR] Invalid or missing height (-y).\n");
    if (!is_valid_positive(config->clients_nb))
        return print_err("[ERROR] Invalid clients number (-c).\n");
    if (!is_valid_positive(config->freq))
        return print_err("[ERROR] Invalid or missing frequency (-f).\n");
    if (config->team_count == 0 || config->team_names == NULL)
        return print_err("[ERROR] At least one team name is required (-n).\n");
    return 1;
}

int validate_config(server_config_t *config)
{
    return check_required_fields(config);
}
