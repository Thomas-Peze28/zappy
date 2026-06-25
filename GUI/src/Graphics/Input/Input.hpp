/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Input
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Input
    {
        public:
            Input() = delete;
            ~Input() = delete;
            
            static bool isKeyPressed(int key);
            static bool isKeyDown(int key);
            static bool isKeyReleased(int key);

            static bool isMouseButtonPressed(int button);
            static bool isMouseButtonDown(int button);
            static bool isMouseButtonReleased(int button);
            static Vector2 getMousePosition();
            static float getMouseWheelMove();

        private:
    };
};

