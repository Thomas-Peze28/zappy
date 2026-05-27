##
## EPITECH PROJECT, 2026
## Server Makefile
## File description:
## Zappy
##

RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
MAGENTA=\033[0;35m
CYAN=\033[0;36m
RESET=\033[0m

SRC_SERVER     =   src/main.c \

C     =   epiclang

OBJ     =   $(SRC_SERVER:.c=.o)

NAME    =   zappy_server

CFLAGS = -Wextra -Wall -I ./include/ -lm -std=gnu17

LDLIBS   = -lm

.SILENT:

all:    $(NAME)

$(NAME):    $(OBJ)
	$(C) -o $(NAME) $(OBJ) $(CFLAGS) $(LDLIBS)
	@printf "$(GREEN)[OK]$(RESET) Binary $(CYAN)'$(NAME)'$(RESET) compiled successfully.\n"

clean:
	rm -f $(OBJ)
	@printf "$(YELLOW)[INFO]$(RESET) Server object files removed.\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(YELLOW)[INFO]$(RESET) Server binary $(CYAN)'$(NAME)'$(RESET) removed.\n"

re:     fclean all
		@printf "$(BLUE)[INFO]$(RESET) Server rebuilt successfully.\n"

.PHONY: all clean fclean re tests_run clean_tests