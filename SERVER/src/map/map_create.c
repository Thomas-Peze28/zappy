/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** map_create
*/

#include <stdlib.h>
#include "map.h"

static void free_map_tiles(map_t *map, int max_y)
{
    int i;

    for (i = 0; i < max_y; i++)
        free(map->tiles[i]);
    free(map->tiles);
    free(map);
}

map_t *create_map(int width, int height)
{
    map_t *map = malloc(sizeof(map_t));
    int y;

    if (map == NULL)
        return NULL;
    map->width = width;
    map->height = height;
    map->tiles = malloc(sizeof(tile_t *) * height);
    if (map->tiles == NULL) {
        free(map);
        return NULL;
    }
    for (y = 0; y < height; y++) {
        map->tiles[y] = calloc(width, sizeof(tile_t));
        if (map->tiles[y] == NULL) {
            free_map_tiles(map, y);
            return NULL;
        }
    }
    return map;
}
