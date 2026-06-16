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
    #include "Tile.hpp"

namespace Zappy
{

    class Map
    {
        public:
            Map();
            Map(int width, int height);
            ~Map() = default;

            void resize(int width, int height);

            void setTileContent(int x, int y, const Tile &content);
            const Tile &getTile(int x, int y) const;
            Tile &getTile(int x, int y);

            int getWidth() const;
            int getHeight() const;
        
        private:
            int _width;
            int _height;
            std::vector<std::vector<Tile>> _tiles;
    };
};

