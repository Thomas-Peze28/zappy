/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Game
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>

    #include "RTCError.hpp"
    #include "Map.hpp"

namespace Zappy
{
    class Game
    {
        public:
            Game();
            ~Game() = default;

            Map &getMap();
            const Map &getMap() const;
        
        private:
            Map _map;
    };
};
