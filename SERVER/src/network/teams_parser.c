/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Team names parsing
*/

#include "server_config.h"
#include <stdlib.h>

static int count_team_names(char **argv, int i, int argc)
{
    int count = 0;

    while (i < argc && argv[i][0] != '-') {
        count++;
        i++;
    }
    return count;
}

static char **alloc_team_names(int count)
{
    return malloc(sizeof(char *) * (count + 1));
}

int parse_team_names(char **argv, int i, int argc,
    server_config_t *config)
{
    int count = count_team_names(argv, i, argc);
    int j = 0;

    config->team_names = alloc_team_names(count);
    if (config->team_names == NULL)
        return -1;
    while (j < count) {
        config->team_names[j] = argv[i + j];
        j++;
    }
    config->team_names[count] = NULL;
    config->team_count = count;
    return i + count - 1;
}
