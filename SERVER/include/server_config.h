/*
** EPITECH PROJECT, 2026
** server.h
** File description:
** server.h
*/

#include <sys/poll.h>
#include "map.h"

#ifndef SERVER_CONFIG_H
    #define SERVER_CONFIG_H
    #define DEFAULT_FREQ 100
    #define MIN_MAP_SIZE 10
    #define MAX_TEAMS 20
    #define MAX_CLIENTS 1024
    #define BUFFER_SIZE 4096
    #define MAX_CMD_LEN 128
    #define TEAM_NAME_LEN 64
    #define MAX_COMMAND_LEN 256
    #define MAX_QUEUE_SIZE 10

typedef struct server_config_s {
    int port;
    int width;
    int height;
    int clients_nb;
    int freq;
    char **team_names;
    int team_count;
} server_config_t;

typedef struct egg_s {
    int id;
    char team_name[64];
    int pos_x;
    int pos_y;
    long long hatch_time;
    int is_hatched;
} egg_t;

typedef enum client_type_e {
    CLIENT_UNKNOWN,
    CLIENT_AI,
    CLIENT_GUI
} client_type_t;

typedef struct command_s {
    char command[MAX_COMMAND_LEN];
    long long execute_at;
} command_t;

typedef enum orientation_e {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} orientation_t;

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct client_s {
    int fd;
    char read_buf[BUFFER_SIZE];
    int read_len;
    char write_buf[BUFFER_SIZE];
    int write_len;
    char line_buf[BUFFER_SIZE];
    client_type_t type;
    int is_connected;
    int pending_disconnect;
    char team_name[TEAM_NAME_LEN];
    command_t commands[MAX_QUEUE_SIZE];
    int command_count;
    int id;
    int pos_x;
    int pos_y;
    orientation_t orientation;
    int level;
    inventory_t inventory;
    long long death_time;
} client_t;

typedef struct server_s {
    int fd;
    server_config_t config;
    client_t clients[MAX_CLIENTS];
    struct pollfd polls[MAX_CLIENTS + 1];
    int client_count;
    int next_client_id;
    map_t *map;
    egg_t eggs[1024];
    int egg_count;
} server_t;

int parse_args(int argc, char **argv, server_config_t *config);
void init_config(server_config_t *config);
int validate_config(server_config_t *config);
void free_config(server_config_t *config);
int parse_team_names(char **argv, int i, int argc, server_config_t *config);
void parse_single_flag(char **argv, int i, server_config_t *config);

int create_server(server_t *server);
void run_server(server_t *server);
void close_server(server_t *server);

void init_client(client_t *client);
void disconnect_client(server_t *server, int idx);
int accept_client(server_t *server);

int buffer_read(client_t *client);
int buffer_write(client_t *client);
char *buffer_get_line(client_t *client);

void handle_connection(server_t *server, int idx);
int find_team(server_config_t *config, const char *name);
int get_team_slots(server_t *server, const char *team);
void send_to_client(client_t *client, const char *msg);
void setup_polls(server_t *server);
int get_next_timeout(server_t *server);
void handle_poll_events(server_t *server, int nb_events);
void check_starvation(server_t *server);

#endif
