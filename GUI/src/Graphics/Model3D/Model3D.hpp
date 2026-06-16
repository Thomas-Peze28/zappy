/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Model3D
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"

    #define X_AXIS {1.0f, 0.0f, 0.0f}
    #define Y_AXIS {0.0f, 0.0f, 1.0f}
    #define Z_AXIS {0.0f, 1.0f, 0.0f}

namespace Zappy
{
    class Model3D
    {
        public:
            Model3D(const std::string &filePath);
            ~Model3D();

            void draw(const Vector3 &position, float scale, Color tint) const;
            void draw(const Vector3 &position, const Vector3 &rotationAxis, float rotationAngle, float scale, Color tint) const;

            float getWidth() const;
            float getDepth() const;

        private:
            Model _model;
            float _width;
            float _depth;
    };
};

