/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Game
*/

#include "Game.hpp"

/**
* @brief Constructs a new Game object.
**/
Zappy::Game::Game()
    : _map(0, 0)
{
}

/**
* @brief Gets the map of the game.
* @return A reference to the map.
**/
Zappy::Map &Zappy::Game::getMap()
{
    return _map;
}

/**
* @brief Gets the map of the game.
* @return A reference to the map.
**/
const Zappy::Map &Zappy::Game::getMap() const
{
    return _map;
}
