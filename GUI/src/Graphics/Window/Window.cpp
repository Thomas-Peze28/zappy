/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Window
*/

#include "Window.hpp"

/**
* Initializes a window with default settings. 
**/
Zappy::Window::Window() : _width(800), _height(800), _title("Raylib - Basic Window")
{
    InitWindow(_width, _height, _title.c_str());
    SetTargetFPS(60);
}

/**
* Initializes a window with custom settings.
* @param width The width of the window.
* @param height The height of the window.
* @param title The title of the window.
* @param fps The target frames per second for the window.
**/
Zappy::Window::Window(int width, int height, const std::string &title, int fps)
    : _width(width), _height(height), _title(title)
{
    InitWindow(_width, _height, _title.c_str());
    SetTargetFPS(fps);
}

/**
* Destructor for the Window class.
**/
Zappy::Window::~Window()
{
    CloseWindow();
}

/**
* Clears the window with a specified background color.
* @param color The color to clear the window with.
**/
void Zappy::Window::clear(Color color) const
{
    ClearBackground(color);
}

/**
* Checks if the window is open.
* @return True if the window is open, false otherwise.
**/
bool Zappy::Window::isOpen() const
{
    return !WindowShouldClose();
}

/**
* Begins the drawing process.
**/
void Zappy::Window::beginDrawing() const
{
    BeginDrawing();
}

/**
* Ends the drawing process.
**/
void Zappy::Window::endDrawing() const
{
    EndDrawing();
}

/**
* Draws the frames per second (FPS) counter.
* @param x The x-coordinate of the FPS counter.
* @param y The y-coordinate of the FPS counter.
**/
void Zappy::Window::drawFPS(int x, int y) const
{
    DrawFPS(x, y);
}

/**
* Checks if the window is currently in fullscreen mode.
* @return True if the window is in fullscreen mode, false otherwise.
**/
bool Zappy::Window::isWindowFullscreen() const
{
    return IsWindowFullscreen();
}

/**
* Toggles the fullscreen mode of the window.
**/
void Zappy::Window::toggleFullscreen()
{
    ToggleFullscreen();
}
