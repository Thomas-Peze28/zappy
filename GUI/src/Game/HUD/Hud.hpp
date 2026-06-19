/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#ifndef HUD_HPP_
#define HUD_HPP_

#include "raylib.h"
#include "Input.hpp"
#include <cmath>

namespace Zappy
{
    class Hud {
        public:
            Hud();
            ~Hud();

            void update();
            void draw() const;
            bool isOpen() const;

        protected:
        private:
            bool _open;
            float _panelWidth;
            float _currentX;
            float _targetX;
            float _animSpeed;
            Rectangle _buttonRect;
            Color _panelColor;
    };
};

#endif /* !HUD_HPP_ */
