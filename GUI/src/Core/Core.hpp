/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <map>

    #include "RTCError.hpp"

    #include "Engine.hpp"
    #include "Game.hpp"
    #include "Network.hpp"

namespace Zappy
{
    class Core
    {
        public:
            using CommandHandler = int (Core::*)(std::string, Game&, Network&);

            Core(int argc, char *argv[]);
            ~Core() = default;
        
            int run();

            bool displayHelp(int argc, char *argv[]);

            static void signalHandler(int signal);
            static bool _signal;

            bool parseArguments(int argc, char *argv[]);

            int handleCommands(std::string message, Game &game, Zappy::Network &server);
            int msz(std::string message, Zappy::Game &game, Zappy::Network &server);
            int bct(std::string message, Zappy::Game &game, Zappy::Network &server);
            int seg(std::string message, Zappy::Game &game, Zappy::Network &server);
            int tna(std::string message, Zappy::Game &game, Zappy::Network &server);
            int pnw(std::string message, Zappy::Game &game, Zappy::Network &server);
            int ppo(std::string message, Zappy::Game &game, Zappy::Network &server);
            int plv(std::string message, Zappy::Game &game, Zappy::Network &server);
            int pin(std::string message, Zappy::Game &game, Zappy::Network &server);
            int pex(std::string message, Zappy::Game &game, Zappy::Network &server);
            int pic(std::string message, Zappy::Game &game, Zappy::Network &server);

        private:
            int _argc;
            char **_argv;
            int _port;
            std::string _host;
    };
};
