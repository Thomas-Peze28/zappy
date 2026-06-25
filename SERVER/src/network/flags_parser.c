/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** flag parsing for server config
*/

#include "server_config.h"
#include <stdlib.h>

static void parse_flag_p(char **argv, int i, server_config_t *config)
{
    config->port = atoi(argv[i + 1]);
}

static void parse_flag_x(char **argv, int i, server_config_t *config)
{
    config->width = atoi(argv[i + 1]);
}

static void parse_flag_y(char **argv, int i, server_config_t *config)
{
    config->height = atoi(argv[i + 1]);
}

static void parse_flag_c(char **argv, int i, server_config_t *config)
{
    config->clients_nb = atoi(argv[i + 1]);
}

static void parse_flag_f(char **argv, int i, server_config_t *config)
{
    config->freq = atoi(argv[i + 1]);
}

void parse_single_flag(char **argv, int i, server_config_t *config)
{
    if (argv[i][1] == 'p')
        parse_flag_p(argv, i, config);
    if (argv[i][1] == 'x')
        parse_flag_x(argv, i, config);
    if (argv[i][1] == 'y')
        parse_flag_y(argv, i, config);
    if (argv[i][1] == 'c')
        parse_flag_c(argv, i, config);
    if (argv[i][1] == 'f')
        parse_flag_f(argv, i, config);
}
