/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <vector>

    #include "RTCError.hpp"

namespace Zappy
{

    struct Tile {
        int food = 0;
        int linemate = 0;
        int deraumere = 0;
        int sibur = 0;
        int mendiane = 0;
        int phiras = 0;
        int thystame = 0;
    };

    class Map
    {
        public:
            Map();
            Map(int width, int height);
            ~Map() = default;

            void resize(int width, int height);

            void setTileContent(int x, int y, const Tile &content);
            const Tile &getTile(int x, int y) const;

            int getWidth() const;
            int getHeight() const;
        
        private:
            int _width;
            int _height;
            std::vector<std::vector<Tile>> _tiles;
    };
};

