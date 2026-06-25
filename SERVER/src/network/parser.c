/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Main argument parsing loop
*/

#include "server_config.h"
#include <stdlib.h>
#include <unistd.h>

static const char ERR_USAGE[] =
    "USAGE: ./zappy_server -p port -x width -y height"
    " -n name1 name2 ... -c clientsNb -f freq\n";

static void print_usage(void)
{
    write(2, ERR_USAGE, sizeof(ERR_USAGE) - 1);
}

static int handle_flag_n(char **argv, int i, int argc,
    server_config_t *config)
{
    int new_i = parse_team_names(argv, i + 1, argc, config);

    if (new_i == -1)
        return -1;
    return new_i;
}

static int handle_argument(char **argv, int *i,
    int argc, server_config_t *config)
{
    if (argv[*i][0] != '-')
        return 0;
    if (argv[*i][1] != 'n') {
        parse_single_flag(argv, *i, config);
        return 0;
    }
    *i = handle_flag_n(argv, *i, argc, config);
    return (*i == -1) ? -1 : 0;
}

static int parse_loop(int argc, char **argv, server_config_t *config)
{
    int i = 1;

    while (i < argc) {
        if (handle_argument(argv, &i, argc, config) == -1)
            return -1;
        i++;
    }
    return 0;
}

int parse_args(int argc, char **argv, server_config_t *config)
{
    if (argc < 2) {
        print_usage();
        return 84;
    }
    init_config(config);
    if (parse_loop(argc, argv, config) == -1)
        return 84;
    if (!validate_config(config)) {
        print_usage();
        return 84;
    }
    return 0;
}
