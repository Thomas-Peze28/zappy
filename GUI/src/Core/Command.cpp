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
        {"tna", &Zappy::Core::tna},
        {"pnw", &Zappy::Core::pnw},
        {"ppo", &Zappy::Core::ppo},
        {"plv", &Zappy::Core::plv},
        {"pin", &Zappy::Core::pin},
        {"pex", &Zappy::Core::pex},
        {"pic", &Zappy::Core::pic},
        {"pie", &Zappy::Core::pie},
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
    if (sscanf(message.c_str(), "msz %d %d", &width, &height) == 2)
        game.getMap().resize(width, height);
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
    if (sscanf(message.c_str(), "bct %d %d %d %d %d %d %d %d %d", &x, &y, &q0, &q1, &q2, &q3, &q4, &q5, &q6) == 9)
        game.getMap().getTile(x, y).setResources(q0, q1, q2, q3, q4, q5, q6);
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

int Zappy::Core::tna(std::string message, Zappy::Game &game, Zappy::Network &server)
{
//     std::string teamName;

    (void)game;
    (void)server;
//     if (sscanf(message.c_str(), "tna %s", teamName.data()) == 1)
// TODO: Ajoute teamName à la liste des équipes et y assigne une couleur
//         exit(0);
    return 0;
}

int Zappy::Core::pnw(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int x;
    int y;
    Direction d;
    int l;
    std::string teamName;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pnw #%d %d %d %d %d %s", &p_nbr, &x, &y, &d, &l, teamName.data()) == 6)
        game.addPlayer(Zappy::Player(p_nbr, "Player" + std::to_string(p_nbr), teamName, l, x, y, d));
    return 0;
}

int Zappy::Core::ppo(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int x;
    int y;
    Direction d;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "ppo #%d %d %d %d", &p_nbr, &x, &y, &d) == 4)
        game.getPlayer(p_nbr).setPosition(x, y, d);
    return 0;
}

int Zappy::Core::plv(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int l;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "plv #%d %d", &p_nbr, &l) == 2)
        game.getPlayer(p_nbr).setLevel(l);
    return 0;
}

int Zappy::Core::pin(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int x;
    int y;
    int q0;
    int q1;
    int q2;
    int q3;
    int q4;
    int q5;
    int q6;
    std::string teamName;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pin #%d %d %d %d %d %d %d %d %d %s", &p_nbr, &x, &y, &q0, &q1, &q2, &q3, &q4, &q5, &q6, teamName.data()) == 11)
        game.getPlayer(p_nbr).setInventory(q0, q1, q2, q3, q4, q5, q6);
    return 0;
}

int Zappy::Core::pex(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pex #%d", &p_nbr) == 1)
    // TODO: Joue une animation d'expulsion pour le joueur p_nbr
        server.sendMessage("ppo #" + std::to_string(p_nbr) + "\n");
    return 0;
}

int Zappy::Core::pic(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    // int x;
    // int y;
    // int l;
    // int p_nbr;

    (void)game;
    (void)server;
    // if (sscanf(message.c_str(), "pin %d %d %d #%d", &x, &y, &l, &p_nbr))
    // TODO: Joue une animation d'incantation pour le joueur p_nbr
    return 0;
}

int Zappy::Core::pie(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    // int x;
    // int y;
    // int r;

    (void)game;
    (void)server;
    // if (sscanf(message.c_str(), "pie %d %d %d", &x, &y, &r))
    // TODO: Arrêter l'animation d'incantation pour le joueur qui est en incantation
    return 0;
}
