/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Sprite
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"

namespace Zappy
{
    class Sprite
    {
        public:
            Sprite(const std::string &filePath);
            ~Sprite();

            void draw(const Vector3 &position, float scale, Color tint) const;

            float getWidth() const;
            float getHeight() const;

        private:
            Texture _texture;
            float _width;
            float _height;
    };
};

