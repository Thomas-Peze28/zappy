##
## EPITECH PROJECT, 2026
## Main Makefile
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

.SILENT:

all: zappy_server zappy_ai zappy_gui
	@printf "$(GREEN)[OK]$(RESET) All binaries compiled successfully.\n"

zappy_server:
	$(MAKE) --no-print-directory -C SERVER
	mv SERVER/zappy_server ./
	@printf "$(GREEN)[OK]$(RESET) Server binary compiled successfully.\n"

zappy_ai:
	$(MAKE) --no-print-directory -C AI
	mv AI/zappy_ai ./
	@printf "$(GREEN)[OK]$(RESET) AI binary compiled successfully.\n"

zappy_gui:
	$(MAKE) --no-print-directory -C GUI
	mv GUI/zappy_gui ./
	@printf "$(GREEN)[OK]$(RESET) GUI binary compiled successfully.\n"

clean:
	$(MAKE) --no-print-directory -C SERVER clean
	$(MAKE) --no-print-directory -C AI clean
	$(MAKE) --no-print-directory -C GUI clean
	@printf "$(YELLOW)[INFO]$(RESET) Object files removed.\n"

fclean: clean
	$(MAKE) --no-print-directory -C SERVER fclean
	$(MAKE) --no-print-directory -C AI fclean
	$(MAKE) --no-print-directory -C GUI fclean
	rm -f zappy_server zappy_ai zappy_gui
	@printf "$(YELLOW)[INFO]$(RESET) Binaries removed.\n"

re: fclean all
	@printf "$(BLUE)[INFO]$(RESET) Project rebuilt successfully.\n"

.PHONY: all zappy_server zappy_ai zappy_gui clean fclean re tests_run clean_tests