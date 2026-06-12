/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Command
*/

#include "Core.hpp"

int Zappy::Core::handleCommands(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    static const std::map<std::string, Zappy::Core::CommandHandler> commands = {
        {"msz", &Zappy::Core::msz},
        {"bct", &Zappy::Core::bct},
        {"seg", &Zappy::Core::seg},
        // {"tna", &Zappy::Core::tna},
        // {"pnw", &Zappy::Core::pnw},
        // {"ppo", &Zappy::Core::ppo},
        // {"plv", &Zappy::Core::plv},
        // {"pin", &Zappy::Core::pin},
        // {"sgt", &Zappy::Core::sgt},
        // {"sgt", &Zappy::Core::sgt},
        // ...
    };

    for (const auto& [cmd, handler] : commands) {
        if (message.starts_with(cmd)) {
            return (this->*handler)(message, game, server);
        }
    }
    return 0;
}

/**
* Handles the "msz" command for setting the map size.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::msz(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int width;
    int height;
    if (sscanf(message.c_str(), "msz %d %d", &width, &height) == 2) {
        game.getMap().resize(width, height);
        server.sendMessage("msz\n");
    }
    return 0;
}

/**
* Handles the "bct" command for setting the content of a tile.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::bct(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int x;
    int y;
    int q0;
    int q1;
    int q2;
    int q3;
    int q4;
    int q5;
    int q6;
    if (sscanf(message.c_str(), "bct %d %d %d %d %d %d %d %d %d", &x, &y, &q0, &q1, &q2, &q3, &q4, &q5, &q6) == 9) {
        game.getMap().getTile(x, y).setResources(q0, q1, q2, q3, q4, q5, q6);
        server.sendMessage("bct " + std::to_string(x) + " " + std::to_string(y) + "\n");
    }
    return 0;
}

/**
* Handles the "seg" command for the end game.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::seg(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    std::string teamName;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "seg %s", teamName.data()) == 1)
        exit(0);
    return 0;
}
