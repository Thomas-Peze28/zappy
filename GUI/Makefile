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
BOLD=\033[1m
RESET=\033[0m

SRC_GRAPHICS =  src/Graphics/Camera/Camera.cpp \
				src/Graphics/Window/Window.cpp \
				src/Graphics/Model3D/Model3D.cpp \
				src/Graphics/Font/Font.cpp \
				src/Graphics/Text/Text.cpp \
				src/Graphics/Input/Input.cpp \
				src/Graphics/Sprite/Sprite.cpp \
				src/Graphics/Audio/AudioDevice/AudioDevice.cpp \
				src/Graphics/Audio/Sound/Sound.cpp \
				src/Graphics/Audio/Music/Music.cpp \
				src/Graphics/Shape3D/Shape3D.cpp \

INCLUDE_GRAPHICS =   src/Graphics/Camera/Camera.hpp \
					 src/Graphics/Window/Window.hpp \
					 src/Graphics/Model3D/Model3D.hpp \
					 src/Graphics/Font/Font.hpp \
					 src/Graphics/Text/Text.hpp \
					 src/Graphics/Input/Input.hpp \
					 src/Graphics/Sprite/Sprite.hpp \
					 src/Graphics/Audio/AudioDevice/AudioDevice.hpp \
					 src/Graphics/Audio/Sound/Sound.hpp \
					 src/Graphics/Audio/Music/Music.hpp \
					 src/Graphics/Shape3D/Shape3D.hpp \

SRC_GAME	=   src/Game/Game.cpp \
				src/Game/Tile/Tile.cpp \
				src/Game/Map/Map.cpp \
				src/Game/Player/Player.cpp \
				src/Game/Egg/Egg.cpp \
				src/Game/HUD/Hud.cpp \

INCLUDE_GAME =  src/Game/Game.hpp \
				src/Game/Tile/Tile.hpp \
				src/Game/Map/Map.hpp \
				src/Game/Player/Player.hpp \
				src/Game/Egg/Egg.hpp \
				src/Game/HUD/Hud.hpp \

SRC_GUI     =   src/main.cpp \
				src/Core/Core.cpp \
				src/Core/Command.cpp \
				$(SRC_GRAPHICS) \
				$(SRC_GAME) \
				src/Engine/Engine.cpp \
				src/Network/Network.cpp \
			
INCLUDE_GUI =  	src/Core/Core.hpp \
				src/Include/RTCError.hpp \
				$(INCLUDE_GRAPHICS) \
				$(INCLUDE_GAME) \
				src/Engine/Engine.hpp \
				src/Network/Network.hpp \

CXX     =   g++

OBJ     =   $(SRC_GUI:.cpp=.o)

NAME    =   zappy_gui

CXXFLAGS = -Wextra -Wall $(addprefix -I, $(sort $(dir $(INCLUDE_GUI)))) -std=c++20

LDLIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

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

test_gui:
	${MAKE} --no-print-directory -C ./
	@set -e; \
	trap 'kill $$ai1_pid $$ai2_pid $$server_pid 2>/dev/null || true' EXIT; \
	printf "$(MAGENTA)[TEST]$(RESET) Launching GUI tests...\n"; \
	printf "$(MAGENTA)[TEST]$(RESET) Run test server on port $(BOLD)5252$(RESET)...\n"; \
	./test/zappy_test_server -p 5252 -x 20 -y 20 -n Team1 Team2 -c 5 -f 50 >test/zappy_test_server.log 2>&1 & server_pid=$$!; \
	sleep 1; \
	printf "$(MAGENTA)[TEST]$(RESET) Launching AI binaries...\n"; \
	$(MAKE) --no-print-directory -C ../AI; \
	./zappy_ai -p 5252 -n Team1 > ai_team1.log 2>&1 & ai1_pid=$$!; \
	./zappy_ai -p 5252 -n Team2 > ai_team2.log 2>&1 & ai2_pid=$$!; \
	sleep 1; \
	printf "$(MAGENTA)[TEST]$(RESET) Launching GUI binary...\n"; \
	./zappy_gui -h localhost -p 5252
	${MAKE} --no-print-directory -C ../AI fclean
	${MAKE} --no-print-directory -C ./ fclean

clean_tests:
	${MAKE} --no-print-directory -C ../AI fclean
	${MAKE} --no-print-directory -C ./ fclean
	rm -f test/zappy_test_server.log ai_team1.log ai_team2.log

.PHONY: all clean fclean re test_gui clean_tests