/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#include "Core.hpp"

bool Zappy::Core::_signal = false;

/**
* Initializes the Core.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
**/
Zappy::Core::Core(int argc, char *argv[]) : _argc(argc), _argv(argv)
{
    if (displayHelp(argc, argv)) {
        std::exit(0);
    }
    _port = -1;
    _host = "";
}

/**
* Displays help information for the Zappy GUI.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return True if help was displayed, false otherwise.
**/
bool Zappy::Core::displayHelp(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
        if (std::string(argv[i]) == "--help") {
            std::cout << "USAGE: ./zappy_gui -p <port> -h <host>" << std::endl;
            return true;
        }
    return false;
}

/**
* Parses the command-line arguments for the Zappy GUI.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return True if the arguments were parsed successfully, false otherwise.
**/
bool Zappy::Core::parseArguments(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);

        if (arg == "-p" || arg == "--port") {
            if (i + 1 >= argc || std::string(argv[i + 1]).starts_with("-")) {
                throw ErrorMajor("Error: -p option requires a port number.");
                return false;
            }
            _port = std::atoi(argv[++i]);
        } else if (arg == "-h" || arg == "--host") {
            if (i + 1 >= argc || std::string(argv[i + 1]).starts_with("-")) {
                throw ErrorMajor("Error: -h option requires a host address.");
                return false;
            }
            _host = argv[++i];
        } else {
            throw ErrorMajor("Error: Unknown option '" + arg + "'");
            return false;
        }
    }
    if (_port == -1 || _host.empty()) {
        throw ErrorMajor("Error: Missing required options.\nUSAGE: ./zappy_gui -p <port> -h <host>");
        return false;
    }
    return true;
}

/**
* Handles the signal for interrupting the Zappy GUI.
* @param signal The signal received.
**/
void Zappy::Core::signalHandler(int signal)
{
    if (signal == SIGINT) {
        _signal = true;
    }
}

/**
* Runs the Zappy GUI.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::run()
{
    std::signal(SIGINT, signalHandler);

    if (!parseArguments(_argc, _argv)) {
        return 84;
    }

    Zappy::Network network(_host, _port);
    network.connectToServer();

    Zappy::Engine display(1280, 720, "Zappy - GUI");
    Zappy::Game game;

    game.getMap().resize(5, 5);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::FOOD, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::MENDIANE, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::PHIRAS, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::THYSTAME, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::SIBUR, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::DERAUMERE, 3);
    game.getMap().getTile(2, 2).setResource(Zappy::RESSOURCE::LINEMATE, 3);

    while (display.isOpen() && !_signal) {

        if (display.handleInputs() == 1)
            _signal = true;
        network.update();
        while (network.hasMessage()) {
            std::string message = network.popMessage();
            std::cout << "Received: " << message << std::endl;
            handleCommands(message, game, network); 
        }
        display.render(game);
    }
    return 0;
}

int Zappy::Core::handleCommands(std::string message, Zappy::Game &game, Zappy::Network &server)
{
    static const std::map<std::string, Zappy::Core::CommandHandler> commands = {
        {"msz", &Zappy::Core::msz},
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
