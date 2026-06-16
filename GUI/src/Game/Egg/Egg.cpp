/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"

/**
* @brief Constructs a new Egg object with default values.
**/
Zappy::Egg::Egg()
    : _id(0), _name(""), _teamName(""), _level(1), _x(0), _y(0)
{
}

/**
* @brief Constructs a new Egg object with the specified values.
* @param id The ID of the egg.
* @param name The name of the egg.
* @param teamName The name of the team to which the egg belongs.
* @param level The level of the egg.
* @param x The x-coordinate of the egg's position.
* @param y The y-coordinate of the egg's position.
**/
Zappy::Egg::Egg(int id, const std::string &name, const std::string &teamName, int level, int x, int y)
    : _id(id), _name(name), _teamName(teamName), _level(level), _x(x), _y(y)
{
}

/**
* @brief Gets the ID of the egg.
* @return The ID of the egg.
**/
int Zappy::Egg::getId() const
{
    return _id;
}

/**
* @brief Gets the name of the egg.
* @return The name of the egg.
**/
const std::string& Zappy::Egg::getName() const
{
    return _name;
}

/**
* @brief Gets the name of the team to which the egg belongs.
* @return The name of the team.
**/
const std::string& Zappy::Egg::getTeamName() const
{
    return _teamName;
}

/**
* @brief Gets the level of the egg.
* @return The level of the egg.
**/
int Zappy::Egg::getLevel() const
{
    return _level;
}

/**
* @brief Gets the x-coordinate of the egg's position.
* @return The x-coordinate of the egg's position.
**/
int Zappy::Egg::getX() const
{
    return _x;
}

/**
* @brief Gets the y-coordinate of the egg's position.
* @return The y-coordinate of the egg's position.
**/
int Zappy::Egg::getY() const
{
    return _y;
}

/**
* @brief Sets the ID of the egg.
* @param id The ID of the egg.
**/
void Zappy::Egg::setId(int id)
{
    _id = id;
}

/**
* @brief Sets the name of the egg.
* @param name The name of the egg.
**/
void Zappy::Egg::setName(const std::string &name)
{
    _name = name;
}

/**
* @brief Sets the name of the team to which the player belongs.
* @param teamName The name of the team.
**/
void Zappy::Egg::setTeamName(const std::string &teamName)
{
    _teamName = teamName;
}

/**
* @brief Sets the position of the egg.
* @param x The x-coordinate of the egg's position.
* @param y The y-coordinate of the egg's position.
**/
void Zappy::Egg::setPosition(int x, int y)
{
    _x = x;
    _y = y;
}

/**
* @brief Sets the level of the egg.
* @param level The level of the egg.
**/
void Zappy::Egg::setLevel(int level)
{
    _level = level;
}
