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

    //game.getMap().resize(10, 10);
    //game.addPlayer(Zappy::Player(1, "Player1", "TeamA", 1, 2, 3, Zappy::Direction::NORTH));
    //game.addPlayer(Zappy::Player(2, "Player2", "TeamB", 2, 4, 5, Zappy::Direction::SOUTH));
    //game.addEgg(Zappy::Egg(1, "Egg1", "TeamA", 1, 4, 5));
    //game.getMap().getTile(4, 5).setResources(1, 2, 3, 4, 5, 6, 7);

    while (display.isOpen() && !_signal) {

        if (display.handleInputs(game, network) == 1)
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
