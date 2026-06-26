/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#pragma once

#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include "Tile.hpp"
#include "Shape3D.hpp"
#include "Player.hpp"

namespace Zappy
{
    class Hud {
        public:
            Hud();
            ~Hud() = default;

            void update();
            void draw() const;
            void setMessages(const std::vector<std::string> &messages);
            bool isOpen() const;
            void toggleOpen();
            void setOpening(bool open);
            void drawTileInventory(int x, int y, const class Tile &tile) const;
            void drawPlayerInventory(const Player &player) const;

        protected:
        private:
            bool _open;
            float _panelWidth;
            float _currentX;
            float _targetX;
            float _animSpeed;
            Rectangle _buttonRect;
            Color _panelColor;
            std::vector<std::string> _messages;
    };
};
