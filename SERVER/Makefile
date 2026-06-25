##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

NAME    =   zappy_server

CC      =   epiclang
CFLAGS  =   -Wall -Wextra -Werror
IFLAGS  =   -I include

SRC     =   src/main.c                      \
            src/network/arg_parser.c        \
            src/network/teams_parser.c      \
            src/network/parser.c  \
            src/network/flags_parser.c   \
			src/network/connection.c              \
			src/server/server.c                    \
			src/server/server_utils.c                     \
			src/server/server_event.c               \
			src/server/client.c                    \
			src/buffer.c					\
			src/map/map_create.c                 \
			src/map/map_destroy.c                \
			src/map/map_generate.c                  \
			src/command/command_exec.c                 \
			src/command/cmd_fork.c                 \
			src/command/cmd_forward.c                 \
			src/command/cmd_right_left.c             \
			src/command/cmd_inventory.c                 \
			src/command/cmd_take_set.c                 \
			src/command/cmd_broadcast.c                 \
			src/command/cmd_look.c                 \
			src/command/cmd_incantation.c                 \
			src/command/command_queue.c			\
			src/timer/timer.c	 \
			src/gui/gui_handler.c \
			src/gui/gui_parser.c \
			src/gui/gui_utils.c \
			src/gui/map.c \
			src/gui/notify_action.c \
			src/gui/notify_egg.c \
			src/gui/notify_player.c \
			src/gui/team.c \
			src/gui/time.c

OBJ     =   $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(IFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re