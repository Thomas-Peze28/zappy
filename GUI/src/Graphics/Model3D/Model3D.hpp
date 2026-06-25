/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Model3D
*/

#pragma once

    #include <string>
    #include "raylib.h"
    #include "raymath.h"

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
            void drawStanding(Vector3 position, float rotationAngleY, float scale, Color color) const;

            float getWidth() const;
            float getDepth() const;

            void applyAnimation(int animIndex, int frame);
            int getAnimFrameCount(int animIndex) const;
            void rotateModelX(float angleDegrees);

        private:
            Model _model;
    
            float _width;
            float _depth;

            ModelAnimation *_animations;
            int _animCount;
            int _currentAnimIndex;
            int _animFrameCounter;
    };
};

