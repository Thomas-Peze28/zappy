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

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/System.hpp>

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Core
    {
        public:
            Core() = default;
            ~Core() = default;
        
            int run();

            bool displayHelp(int argc, char *argv[]);

            static void signalHandler(int signal);
            static bool _signal;

        private:
    };
};

