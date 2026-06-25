/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AudioDevice
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class AudioDevice
    {
        public:
            AudioDevice();
            ~AudioDevice();

            bool isReady() const;
            void setMasterVolume(float volume);

        private:
    };
};

