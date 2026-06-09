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

    #include "RTCError.hpp"
    #include "Window.hpp"
    #include "Camera.hpp"

namespace Zappy
{
    class Core
    {
        public:
            Core() = default;
            ~Core() = default;
        
            int run();
            int display();

            bool displayHelp(int argc, char *argv[]);

            static void signalHandler(int signal);
            static bool _signal;

        private:
    };
};

