/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Shaoe3D
*/

#pragma once

    #include <string>
    #include "raylib.h"
    #include "raymath.h"

    #include "RTCError.hpp"

namespace Zappy {
    class Shape3D {
        public:
            Shape3D() = default;
            ~Shape3D() = default;

            void drawSphereWires(Vector3 position, float radius, int rings, int slices, Color color);
            void drawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color);
    };
}

