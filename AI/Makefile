##
## EPITECH PROJECT, 2026
## AI Makefile
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

NAME    =   zappy_ai

.SILENT:

all:    $(NAME)

$(NAME):    $(OBJ)
	cp src/zappy_ai.py $(NAME)
	chmod +x $(NAME)
	@printf "$(GREEN)[OK]$(RESET) Binary $(CYAN)'$(NAME)'$(RESET) compiled successfully.\n"

clean:
	rm -rf __pycache__ src/__pycache__ src/*/__pycache_
	@printf "$(YELLOW)[INFO]$(RESET) AI object files removed.\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(YELLOW)[INFO]$(RESET) AI binary $(CYAN)'$(NAME)'$(RESET) removed.\n"

re:     fclean all
		@printf "$(BLUE)[INFO]$(RESET) AI rebuilt successfully.\n"

.PHONY: all clean fclean re tests_run clean_tests