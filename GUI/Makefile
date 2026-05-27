##
## EPITECH PROJECT, 2026
## GUI Makefile
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

SRC_GUI     =   src/main.cpp \
			src/Core/Core.cpp \
			
INCLUDE_GUI =  	src/Core/Core.hpp \
			src/Include/RTCError.hpp \
			

CXX     =   g++

OBJ     =   $(SRC_GUI:.cpp=.o)

NAME    =   zappy_gui

CXXFLAGS = -Wextra -Wall $(addprefix -I, $(sort $(dir $(INCLUDE_GUI)))) -std=c++20

LDLIBS   = -lm -lsfml-graphics -lsfml-window -lsfml-system

.SILENT:

all:    $(NAME)

$(NAME):    $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS) $(LDLIBS)
	@printf "$(GREEN)[OK]$(RESET) Binary $(CYAN)'$(NAME)'$(RESET) compiled successfully.\n"

clean:
	rm -f $(OBJ)
	@printf "$(YELLOW)[INFO]$(RESET) GUI object files removed.\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(YELLOW)[INFO]$(RESET) GUI binary $(CYAN)'$(NAME)'$(RESET) removed.\n"

re:     fclean all
		@printf "$(BLUE)[INFO]$(RESET) GUI rebuilt successfully.\n"

.PHONY: all clean fclean re tests_run clean_tests