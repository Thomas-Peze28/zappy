/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Input
*/

#include "Input.hpp"

/**
* Initializes the Input class.
**/
Zappy::Input::Input()
{
}

/**
* Destroys the Input class.
**/
Zappy::Input::~Input()
{
}

/**
* Checks if a specific key is currently being pressed.
* @param key The key code to check.
* @return True if the key is pressed, false otherwise.
**/
bool Zappy::Input::isKeyPressed(int key)
{
    return IsKeyPressed(key);
}

/**
* Checks if a specific key is currently being held down.
* @param key The key code to check.
* @return True if the key is held down, false otherwise.
**/
bool Zappy::Input::isKeyDown(int key)
{
    return IsKeyDown(key);
}

/**
* Checks if a specific key has been released.
* @param key The key code to check.
* @return True if the key has been released, false otherwise.
**/
bool Zappy::Input::isKeyReleased(int key)
{
    return IsKeyReleased(key);
}

/**
* Checks if a specific mouse button is currently being pressed.
* @param button The mouse button code to check.
* @return True if the mouse button is pressed, false otherwise.
**/
bool Zappy::Input::isMouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

/**
* Checks if a specific mouse button is currently being held down.
* @param button The mouse button code to check.
* @return True if the mouse button is held down, false otherwise.
**/
bool Zappy::Input::isMouseButtonDown(int button)
{
    return IsMouseButtonDown(button);
}

/**
* Checks if a specific mouse button has been released.
* @param button The mouse button code to check.
* @return True if the mouse button has been released, false otherwise.
**/
bool Zappy::Input::isMouseButtonReleased(int button)
{
    return IsMouseButtonReleased(button);
}

/**
* Gets the current position of the mouse.
* @return The mouse position as a Vector2.
**/
Vector2 Zappy::Input::getMousePosition()
{
    return GetMousePosition();
}

/**
* Gets the current movement of the mouse wheel.
* @return The mouse wheel movement as a float.
**/
float Zappy::Input::getMouseWheelMove()
{
    return GetMouseWheelMove();
}
