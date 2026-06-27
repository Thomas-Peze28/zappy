/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** map_generate
*/

#include <stdlib.h>
#include "map.h"
#include "gui.h"
#include <stddef.h>

static void add_res(tile_t *tile, int res_type)
{
    int *res_ptr[7];

    res_ptr[0] = &tile->food;
    res_ptr[1] = &tile->linemate;
    res_ptr[2] = &tile->deraumere;
    res_ptr[3] = &tile->sibur;
    res_ptr[4] = &tile->mendiane;
    res_ptr[5] = &tile->phiras;
    res_ptr[6] = &tile->thystame;
    (*res_ptr[res_type])++;
}

static void fill_tile_res(map_t *map, int count, int res_type)
{
    int i;
    int rand_y;
    int rand_x;

    for (i = 0; i < count; i++) {
        rand_y = rand() % map->height;
        rand_x = rand() % map->width;
        add_res(&map->tiles[rand_y][rand_x], res_type);
    }
}

void spawn_resources(map_t *map, int is_init)
{
    int tot = map->width * map->height;
    int fd = is_init ? (tot * 0.5) : (tot * 0.1);
    int lm = is_init ? (tot * 0.3) : (tot * 0.05);
    int dr = is_init ? (tot * 0.15) : (tot * 0.03);
    int sb = is_init ? (tot * 0.1) : (tot * 0.02);
    int md = is_init ? (tot * 0.1) : (tot * 0.01);
    int pr = is_init ? (tot * 0.08) : (tot * 0.01);
    int ts = is_init ? (tot * 0.05) : (tot * 0.01);

    fill_tile_res(map, fd, 0);
    fill_tile_res(map, lm, 1);
    fill_tile_res(map, dr, 2);
    fill_tile_res(map, sb, 3);
    fill_tile_res(map, md, 4);
    fill_tile_res(map, pr, 5);
    fill_tile_res(map, ts, 6);
}

static void count_tiles(server_t *srv, int *count)
{
    tile_t *t;

    for (int y = 0; y < srv->config.height; y++) {
        for (int x = 0; x < srv->config.width; x++) {
            t = &srv->map->tiles[y][x];
            count[0] += t->food;
            count[1] += t->linemate;
            count[2] += t->deraumere;
            count[3] += t->sibur;
            count[4] += t->mendiane;
            count[5] += t->phiras;
            count[6] += t->thystame;
        }
    }
}

static void add_missing_resources(server_t *srv, int *count, int size)
{
    float densities[7] = {0.1, 0.05, 0.03, 0.02, 0.01, 0.01, 0.01};
    int rand_y;
    int rand_x;
    int res;

    for (int loop = 0; loop < 1000; loop++) {
        res = rand() % 7;
        if (count[res] < (int)(size * densities[res])) {
            rand_y = rand() % srv->config.height;
            rand_x = rand() % srv->config.width;
            add_res(&srv->map->tiles[rand_y][rand_x], res);
            gui_notify_bct(srv, rand_x, rand_y);
            count[res]++;
        }
    }
}

void check_and_refill(server_t *server)
{
    int size = server->config.width * server->config.height;
    int count[7] = {0};

    count_tiles(server, count);
    add_missing_resources(server, count, size);
}
