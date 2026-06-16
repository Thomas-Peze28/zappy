/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Music
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Music
    {
        public:
            Music(const std::string &filePath);
            ~Music();

            void play() const;
            void stop() const;
            void update() const;
            void setVolume(float volume);
            bool isPlaying() const;
            void pause() const;

        private:
            ::Music _music;
    };
};

