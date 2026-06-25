/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#ifndef HUD_HPP_
#define HUD_HPP_

#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

namespace Zappy
{
    class Hud {
        public:
            Hud();
            ~Hud();

            void update();
            void draw() const;
            void setMessages(const std::vector<std::string> &messages);
            bool isOpen() const;
// TODO: essayer de fix le toggle bar au moment du passage en fullscreen
            // void setPanelWidth(float width);
            // void setButtonRectX(const Rectangle &rect);
            // void setCurrentX(const Rectangle &rect);
            // const Rectangle &getButtonRect(void) const;

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

#endif /* !HUD_HPP_ */
