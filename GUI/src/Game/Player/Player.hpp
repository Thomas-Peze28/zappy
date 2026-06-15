/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <vector>
    #include <unordered_map>

    #include "RTCError.hpp"
    #include "Tile.hpp"

namespace Zappy
{
    enum class Direction {
        NORTH = 1,
        EAST = 2,
        SOUTH = 3,
        WEST = 4
    };

    class Player
    {
        public:
            Player();
            Player(int id, const std::string &name, const std::string &teamName, int level, int x, int y, Direction direction);
            ~Player() = default;
        
        private:
            int _id;
            std::string _name;
            std::string _teamName;
            int _level;
            int _x;
            int _y;
            Direction _direction;
            std::unordered_map<RESSOURCE, int> _inventory;
    };
};

