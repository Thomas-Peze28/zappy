/*
** EPITECH PROJECT, 2026
** gui.h
** File description:
** gui.h
*/

#ifndef GUI_H
    #define GUI_H
    #include "server_config.h"

typedef struct {
    const char *cmd;
    void (*handler)(server_t *, client_t *, char *);
} gui_cmd_t;

void handle_gui_command(server_t *server, int idx);
void gui_send_msz(server_t *server, client_t *gui);
void gui_send_bct(server_t *server, client_t *gui, int x, int y);
void gui_send_mct(server_t *server, client_t *gui);
void gui_send_tna(server_t *server, client_t *gui);
void gui_send_ppo(server_t *server, client_t *gui, int player_id);
void gui_send_plv(server_t *server, client_t *gui, int player_id);
void gui_send_pin(server_t *server, client_t *gui, int player_id);
void gui_send_sgt(server_t *server, client_t *gui);
void gui_send_sst(server_t *server, client_t *gui, int freq);
void gui_send_all_eggs(server_t *server, client_t *gui);
void gui_notify_pnw(server_t *server, client_t *player);
void gui_notify_pex(server_t *server, client_t *player);
void gui_notify_pbc(server_t *server, client_t *player, const char *msg);
void gui_notify_pic(server_t *server, client_t *player);
void gui_notify_pie(server_t *server, client_t *player, int result);
void gui_notify_pfk(server_t *server, client_t *player);
void gui_notify_bct(server_t *server, int x, int y);
void gui_notify_pdr(server_t *server, client_t *player, int resource);
void gui_notify_pgt(server_t *server, client_t *player, int resource);
void gui_notify_pdi(server_t *server, client_t *player);
void gui_notify_pin(server_t *server, client_t *client);
void gui_notify_enw(server_t *server, egg_t *egg, int player_id);
void gui_notify_ebo(server_t *server, egg_t *egg);
void gui_notify_edi(server_t *server, egg_t *egg);
void gui_notify_seg(server_t *server, const char *team);
void gui_notify_plv(server_t *server, client_t *player);
void gui_notify_ppo(server_t *server, client_t *player);
void gui_notify_pnw_to(server_t *server, client_t *gui, client_t *player);
void dispatch_msz(server_t *s, client_t *g, char *l);
void dispatch_mct(server_t *s, client_t *g, char *l);
void dispatch_tna(server_t *s, client_t *g, char *l);
void dispatch_sgt(server_t *s, client_t *g, char *l);
void handle_bct_cmd(server_t *server, client_t *gui, char *line);
void handle_ppo_cmd(server_t *server, client_t *gui, char *line);
void handle_plv_cmd(server_t *server, client_t *gui, char *line);
void handle_pin_cmd(server_t *server, client_t *gui, char *line);
void handle_sst_cmd(server_t *server, client_t *gui, char *line);
void send_to_all_gui(server_t *server, const char *msg);
client_t *find_player_by_id(server_t *server, int id);

#endif
