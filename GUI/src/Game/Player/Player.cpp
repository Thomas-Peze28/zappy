/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#include "Player.hpp"

/**
* @brief Constructs a new Player object with default values.
**/
Zappy::Player::Player()
    : _id(0), _name(""), _teamName(""), _level(1), _x(0), _y(0), _direction(Direction::NORTH), _inventory()
{
}

/**
* @brief Constructs a new Player object with the specified values.
* @param id The ID of the player.
* @param name The name of the player.
* @param teamName The name of the team to which the player belongs.
* @param level The level of the player.
* @param x The x-coordinate of the player's position.
* @param y The y-coordinate of the player's position.
* @param direction The direction in which the player is facing.
**/
Zappy::Player::Player(int id, const std::string &name, const std::string &teamName, int level, int x, int y, Direction direction)
    : _id(id), _name(name), _teamName(teamName), _level(level), _x(x), _y(y), _direction(direction), _inventory()
{
}