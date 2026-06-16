/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Text
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Font
    {
        public:
            Font(const std::string &fontPath);
            ~Font();

            void draw(const std::string &text, Vector2 position, float fontSize, float spacing, Color color) const;
            Vector2 measure(const std::string &text, int fontSize, float spacing) const;
            
        private:
            ::Font _font;
    };
};

