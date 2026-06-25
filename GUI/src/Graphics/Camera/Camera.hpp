/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Camera
    {
        public:
            Camera();
            Camera(Vector3 position, Vector3 target, Vector3 up, float fovy);
            ~Camera() = default;

            void update();

            void beginMode() const;
            void endMode() const;

            void setPosition(Vector3 position);
            void setTarget(Vector3 target);
            void setMode(int mode);
            Vector3 getPosition() const;
            Vector3 getTarget() const;
            int getMode() const;

        private:
            Camera3D _camera;
            int _mode;
    };
};
