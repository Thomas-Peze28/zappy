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
    #include "Font.hpp"
    
namespace Zappy
{
    class Text
    {
        public:
            Text(const Font& font, const std::string& text, Vector2 position, float fontSize, Color color);
            ~Text() = default;
            
            void draw() const;
        
            void setString(const std::string& text);
            void setPosition(Vector2 position);
            void setColor(Color color);
            void setFontSize(float size);

            const std::string& getString() const;
            Vector2 getPosition() const;
            Color getColor() const;
            float getFontSize() const;
            
        private:
            const Font& _font;
            std::string _string;
            Vector2 _position;
            float _fontSize;
            Color _color;
    };
};
