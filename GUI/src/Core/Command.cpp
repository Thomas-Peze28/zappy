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
        {"pbc", &Zappy::Core::pbc},
        {"pic", &Zappy::Core::pic},
        {"pie", &Zappy::Core::pie},
        {"pfk", &Zappy::Core::pfk},
        {"pdr", &Zappy::Core::pdr},
        {"pgt", &Zappy::Core::pgt},
        {"pdi", &Zappy::Core::pdi},
        {"enw", &Zappy::Core::enw},
        {"ebo", &Zappy::Core::ebo},
        {"edi", &Zappy::Core::edi},
        {"sgt", &Zappy::Core::sgt},
        {"sst", &Zappy::Core::sst},
        {"smg", &Zappy::Core::smg},
        {"sbp", &Zappy::Core::sbp},
        {"suc", &Zappy::Core::suc}
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

    (void)server;
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

    (void)server;
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
    // TODO: Affiche un message de fin de partie avec le nom de l'équipe gagnante
        exit(0);
    return 0;
}

/**
* Handles the "tna" command for getting the list of teams.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::tna(std::string message, Zappy::Game &game, Zappy::Network &server)
{
//     std::string teamName;

    (void)game;
    (void)server;
    (void)message;
//     if (sscanf(message.c_str(), "tna %s", teamName.data()) == 1)
//         TODO: Ajoute teamName à la liste des équipes et y assigne une couleur
    return 0;
}

/**
* Handles the "pnw" command for player connection.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pnw(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int x;
    int y;
    int d;
    int l;
    std::string teamName;
    Direction direction;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pnw #%d %d %d %d %d %s", &p_nbr, &x, &y, &d, &l, teamName.data()) == 6) {
        direction = static_cast<Direction>(d);
        game.addPlayer(Zappy::Player(p_nbr, "Player" + std::to_string(p_nbr), teamName, l, x, y, direction));
    }
    return 0;
}

/**
* Handles the "ppo" command for player position update.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::ppo(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int x;
    int y;
    int d;
    Direction direction;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "ppo #%d %d %d %d", &p_nbr, &x, &y, &d) == 4) {
        direction = static_cast<Direction>(d);
        if (game.getPlayer(p_nbr).getX() != x || game.getPlayer(p_nbr).getY() != y || game.getPlayer(p_nbr).getDirection() != direction) {
            // TODO: Joue une animation de déplacement pour le joueur p_nbr
            game.getPlayer(p_nbr).setPosition(x, y, direction);
        }
    }
    return 0;
}

/**
* Handles the "plv" command for player level update.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::plv(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int l;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "plv #%d %d", &p_nbr, &l) == 2) {
        if (l != game.getPlayer(p_nbr).getLevel()) {
            // TODO: Joue une animation de level up pour le joueur p_nbr
            game.getPlayer(p_nbr).setLevel(l);
        }
    }
    return 0;
}

/**
* Handles the "pin" command for player inventory update.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
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

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pin #%d %d %d %d %d %d %d %d %d %d", &p_nbr, &x, &y, &q0, &q1, &q2, &q3, &q4, &q5, &q6) == 10)
        game.getPlayer(p_nbr).setInventory(q0, q1, q2, q3, q4, q5, q6);
    return 0;
}

/**
* Handles the "pex" command for player expulsion.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
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

/**
* Handles the "pbc" command for player broadcast.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pbc(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    std::string msg;

    (void)game;
    (void)server;
    if (sscanf(message.c_str(), "pbc #%d %s", &p_nbr, msg.data()) == 2)
    // TODO: Mettre le message dans l'HUD
        printf("Player %d broadcasted a message : %s\n", p_nbr, msg.data());
    return 0;
}

/**
* Handles the "pic" command for player incantation (start of an incantation by the ﬁrst
player).
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pic(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    // int x;
    // int y;
    // int l;
    // int p_nbr;

    (void)game;
    (void)server;
    (void)message;
    // if (sscanf(message.c_str(), "pin %d %d %d #%d", &x, &y, &l, &p_nbr))
    // TODO: Joue une animation d'incantation pour le joueur p_nbr
    return 0;
}

/**
* Handles the "pie" command for player incantation (end of an incantation).
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pie(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    // int x;
    // int y;
    // int r;

    (void)game;
    (void)server;
    (void)message;
    // if (sscanf(message.c_str(), "pie %d %d %d", &x, &y, &r) == 3)
    // TODO: Arrêter l'animation d'incantation pour le joueur qui est en incantation
    return 0;
}

/**
* Handles the "pfk" command for egg laying by the player.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pfk(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    // int p_nbr;

    (void)game;
    (void)server;
    (void)message;
    // if (sscanf(message.c_str(), "pfk %d", &p_nbr) == 1)
    // TODO: Joue l'animation de ponte d'œuf pour le joueur p_nbr
    return 0;
}

/**
* Handles the "pdr" command for player dropping a resource.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pdr(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int i;
    Zappy::RESSOURCE res;
    int x;
    int y;

    (void)server;
    if (sscanf(message.c_str(), "pdr #%d %d", &p_nbr, &i) == 2) {
        res = static_cast<Zappy::RESSOURCE>(i);
        x = game.getPlayer(p_nbr).getX();
        y = game.getPlayer(p_nbr).getY();
        game.getMap().getTile(x, y).setResource(res, game.getMap().getTile(x, y).getResource(res) + 1);
        game.getPlayer(p_nbr).setRessource(res, game.getPlayer(p_nbr).getInventory().at(res) - 1);
    }
    return 0;
}

/**
* Handles the "pgt" command for player getting a resource.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pgt(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int i;
    Zappy::RESSOURCE res;
    int x;
    int y;

    (void)server;
    if (sscanf(message.c_str(), "pgt #%d %d", &p_nbr, &i) == 2) {
        res = static_cast<Zappy::RESSOURCE>(i);
        x = game.getPlayer(p_nbr).getX();
        y = game.getPlayer(p_nbr).getY();
        game.getMap().getTile(x, y).setResource(res, game.getMap().getTile(x, y).getResource(res) - 1);
        game.getPlayer(p_nbr).setRessource(res, game.getPlayer(p_nbr).getInventory().at(res) + 1);
    }
    return 0;
}

/**
* Handles the "pdi" command for player death.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::pdi(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;

    (void)server;
    if (sscanf(message.c_str(), "pdi #%d", &p_nbr) == 1)
        game.removePlayer(p_nbr);
    return 0;
}

/**
* Handles the "enw" command for egg laid by a player.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::enw(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int p_nbr;
    int e_nbr;
    int x;
    int y;

    (void)server;
    if (sscanf(message.c_str(), "enw #%d #%d %d %d", &e_nbr, &p_nbr, &x, &y) == 4) {
        std::string teamName = "";
        int level = 1;

        const auto &players = game.getPlayers();
        if (p_nbr >= 0 && players.find(p_nbr) != players.end()) {
            teamName = game.getPlayer(p_nbr).getTeamName();
            level = game.getPlayer(p_nbr).getLevel();
        }

    game.addEgg(Zappy::Egg(e_nbr, "", teamName, level, x, y));
    }
    return 0;
}

/**
* Handles the "ebo" command for egg hatching.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::ebo(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int e_nbr;

    (void)server;
    if (sscanf(message.c_str(), "ebo #%d", &e_nbr) == 1)
        game.removeEgg(e_nbr);
    return 0;
}

/**
* Handles the "edi" command for egg destruction.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::edi(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int e_nbr;

    (void)server;
    if (sscanf(message.c_str(), "edi #%d", &e_nbr) == 1)
        game.removeEgg(e_nbr);
    return 0;
}

/**
* Handles the "sgt" command for getting the game time unit.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::sgt(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int t;

    (void)server;
    if (sscanf(message.c_str(), "sgt %d", &t) == 1)
        game.setTimeUnit(t);
    return 0;
}

/**
* Handles the "sst" command for setting the game time unit.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::sst(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int t;

    (void)server;
    if (sscanf(message.c_str(), "sst %d", &t) == 1)
        game.setTimeUnit(t);
    return 0;
}

/**
* Handles the "smg" command for the messages of the server.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::smg(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    int m;

    (void)server;
    (void)game;
    if (sscanf(message.c_str(), "smg #%d", &m) == 1)
        printf("Message from server: %d\n", m);
    return 0;
}

/**
* Handles the "suc" command for unknown command.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::suc(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    (void)server;
    (void)game;
    if (sscanf(message.c_str(), "suc") == 0)
        printf("Unknown command sent to server\n");
    return 0;
}

/**
* Handles the "sbp" command for bad parameters.
* @param message The command message.
* @param game The game instance.
* @param server The network instance.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::sbp(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    (void)server;
    (void)game;
    if (sscanf(message.c_str(), "sbp") == 0)
        printf("Bad parameters sent to server on last command\n");
    return 0;
}
