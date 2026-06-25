##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

NAME    =   zappy_server

CC      =   gcc
CFLAGS  =   -Wall -Wextra -Werror
IFLAGS  =   -I include

SRC     =   src/main.c                      \
            src/network/arg_parser.c        \
            src/network/teams_parser.c      \
            src/network/parser.c  \
            src/network/flags_parser.c   \
			src/network/connection.c              \
			src/server.c                    \
			src/client.c                    \
			src/buffer.c


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