/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** map
*/

#ifndef MAP_H
    #define MAP_H

typedef struct tile_s
{
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} tile_t;

typedef struct map_s
{
    int width;
    int height;

    tile_t **tiles;

} map_t;

map_t *create_map(int width, int height);
void destroy_map(map_t *map);
void spawn_resources(map_t *map, int is_initial);

#endif
