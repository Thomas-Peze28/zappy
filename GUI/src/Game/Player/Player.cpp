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
    : _id(0), _name(""), _teamName(""), _level(1), _x(0), _y(0), _xVisual(0), _yVisual(0), _direction(Direction::NORTH), _inventory()
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
    : _id(id), _name(name), _teamName(teamName), _level(level), _x(x), _y(y), _xVisual(x), _yVisual(y), _direction(direction), _inventory()
{
}

/**
* @brief Gets the ID of the player.
* @return The ID of the player.
**/
int Zappy::Player::getId() const
{
    return _id;
}

/**
* @brief Gets the name of the player.
* @return The name of the player.
**/
const std::string& Zappy::Player::getName() const
{
    return _name;
}

/**
* @brief Gets the name of the team to which the player belongs.
* @return The name of the team.
**/
const std::string& Zappy::Player::getTeamName() const
{
    return _teamName;
}

/**
* @brief Gets the level of the player.
* @return The level of the player.
**/
int Zappy::Player::getLevel() const
{
    return _level;
}

/**
* @brief Gets the x-coordinate of the player's position.
* @return The x-coordinate of the player's position.
**/
int Zappy::Player::getX() const
{
    return _x;
}

/**
* @brief Gets the y-coordinate of the player's position.
* @return The y-coordinate of the player's position.
**/
int Zappy::Player::getY() const
{
    return _y;
}

/**
* @brief Gets the x-coordinate of the player's visual position.
* @return The x-coordinate of the player's visual position.
**/
float Zappy::Player::getXVisual() const
{
    return _xVisual;
}

/**
* @brief Gets the y-coordinate of the player's visual position.
* @return The y-coordinate of the player's visual position.
**/
float Zappy::Player::getYVisual() const
{
    return _yVisual;
}

void Zappy::Player::updateVisual(float deltaTime)
{
    float speed = 5.0f;

    _xVisual += (_x - _xVisual) * speed * deltaTime;
    _yVisual += (_y - _yVisual) * speed * deltaTime;
}

/**
* @brief Gets the direction in which the player is facing.
* @return The direction in which the player is facing.
**/
Zappy::Direction Zappy::Player::getDirection() const
{
    return _direction;
}

/**
* @brief Gets the amount of a specific resource the player has.
* @param type The type of resource to get.
* @return The amount of the specified resource.
**/
int Zappy::Player::getResource(RESSOURCE type) const
{
    auto it = _inventory.find(type);
    if (it != _inventory.end()) {
        return it->second;
    }
    return 0;
}

/**
* @brief Gets the inventory of the player.
* @return The inventory of the player.
**/
const std::unordered_map<Zappy::RESSOURCE, int>& Zappy::Player::getInventory() const
{
    return _inventory;
}

void Zappy::Player::setRessource(RESSOURCE type, int quantity)
{
    _inventory[type] = quantity;
}

/**
* @brief Sets the ID of the player.
* @param id The ID of the player.
**/
void Zappy::Player::setId(int id)
{
    _id = id;
}

/**
* @brief Sets the name of the player.
* @param name The name of the player.
**/
void Zappy::Player::setName(const std::string &name)
{
    _name = name;
}

/**
* @brief Sets the name of the team to which the player belongs.
* @param teamName The name of the team.
**/
void Zappy::Player::setTeamName(const std::string &teamName)
{
    _teamName = teamName;
}

/**
* @brief Sets the position of the player.
* @param x The x-coordinate of the player's position.
* @param y The y-coordinate of the player's position.
* @param direction The direction in which the player is facing.
**/
void Zappy::Player::setPosition(int x, int y, Direction direction)
{
    _x = x;
    _y = y;
    _direction = direction;
}

/**
* @brief Sets the visual position of the player.
* @param xVisual The x-coordinate of the player's visual position.
* @param yVisual The y-coordinate of the player's visual position.
**/
void Zappy::Player::setPositionVisual(float xVisual, float yVisual)
{
    _xVisual = xVisual;
    _yVisual = yVisual;
}

/**
* @brief Sets the level of the player.
* @param level The level of the player.
**/
void Zappy::Player::setLevel(int level)
{
    _level = level;
}

/**
* @brief Sets the inventory of the player.
* @param food The amount of food the player has.
* @param linemate The amount of linemate the player has.
* @param deraumere The amount of deraumere the player has.
* @param sibur The amount of sibur the player has.
* @param mendiane The amount of mendiane the player has.
* @param phiras The amount of phiras the player has.
* @param thystame The amount of thystame the player has.
**/
void Zappy::Player::setInventory(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame)
{
    _inventory[RESSOURCE::FOOD] = food;
    _inventory[RESSOURCE::LINEMATE] = linemate;
    _inventory[RESSOURCE::DERAUMERE] = deraumere;
    _inventory[RESSOURCE::SIBUR] = sibur;
    _inventory[RESSOURCE::MENDIANE] = mendiane;
    _inventory[RESSOURCE::PHIRAS] = phiras;
    _inventory[RESSOURCE::THYSTAME] = thystame;
}
