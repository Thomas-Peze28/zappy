/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Window
*/

#pragma once

    #include <string>
    #include "raylib.h"

    #include "RTCError.hpp"
    

namespace Zappy
{
    class Window
    {
        public:
            Window();
            Window(int width, int height, const std::string &title, int fps);
            ~Window();

            void clear(Color color) const;
            bool isOpen() const;
            void beginDrawing() const;
            void endDrawing() const;
            void drawFPS(int x, int y) const;

            bool isWindowFullscreen() const;
            void toggleFullscreen();

        private:
            int _width;
            int _height;
            std::string _title;
    };
};

