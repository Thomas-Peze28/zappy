/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Sound
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Sound
    {
        public:
            Sound(const std::string &filePath);
            ~Sound();

            void play() const;
            void stop() const;
            void setVolume(float volume);

        private:
            ::Sound _sound;
    };
};

