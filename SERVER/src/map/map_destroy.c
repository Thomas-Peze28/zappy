/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** map_destroy
*/

#include <stdlib.h>
#include "map.h"

void destroy_map(map_t *map)
{
    if (map == NULL)
        return;
    if (map->tiles != NULL) {
        for (int y = 0; y < map->height; y++) {
            free(map->tiles[y]);
        }
        free(map->tiles);
    }
    free(map);
}
