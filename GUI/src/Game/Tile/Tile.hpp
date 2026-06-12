/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <vector>
    #include <unordered_map>

    #include "RTCError.hpp"

namespace Zappy
{

    /**
    * @brief Enumeration of resources available in the game.
    */
    enum class RESSOURCE {
        FOOD,
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME
    };

    class Tile
    {
        public:
            Tile();
            ~Tile() = default;

            int getResource(RESSOURCE type) const;
            void setResource(RESSOURCE type, int quantity);

            void setResources(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame);

            bool isEmpty() const;
        
        private:
            std::unordered_map<RESSOURCE, int> _resources;

    };
};

