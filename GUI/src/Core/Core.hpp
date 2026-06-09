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
    #include "Model3D.hpp"
    #include "Font.hpp"
    #include "Text.hpp"
    #include "Input.hpp"
    #include "Sprite.hpp"
    #include "AudioDevice.hpp"
    #include "Sound.hpp"

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

