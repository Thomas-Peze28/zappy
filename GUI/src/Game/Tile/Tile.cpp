/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#include "Tile.hpp"

/**
* @brief Constructs a new Tile object.
**/
Zappy::Tile::Tile()
{
    _resources[RESSOURCE::FOOD] = 0;
    _resources[RESSOURCE::LINEMATE] = 0;
    _resources[RESSOURCE::DERAUMERE] = 0;
    _resources[RESSOURCE::SIBUR] = 0;
    _resources[RESSOURCE::MENDIANE] = 0;
    _resources[RESSOURCE::PHIRAS] = 0;
    _resources[RESSOURCE::THYSTAME] = 0;
}

/**
* @brief Gets the quantity of a specific resource on the tile.
* @param type The type of resource to get.
* @return The quantity of the resource.
**/
int Zappy::Tile::getResource(RESSOURCE type) const
{
    auto it = _resources.find(type);
    if (it != _resources.end()) {
        return it->second;
    }
    return 0;
}

/**
* @brief Sets the quantity of a specific resource on the tile.
* @param type The type of resource to set.
* @param quantity The quantity of the resource.
**/
void Zappy::Tile::setResource(RESSOURCE type, int quantity)
{
    _resources[type] = quantity;
}

/**
* @brief Sets the quantities of all resources on the tile.
* @param food The quantity of food.
* @param linemate The quantity of linemate.
* @param deraumere The quantity of deraumere.
* @param sibur The quantity of sibur.
* @param mendiane The quantity of mendiane.
* @param phiras The quantity of phiras.
* @param thystame The quantity of thystame.
**/
void Zappy::Tile::setResources(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame)
{
    _resources[RESSOURCE::FOOD] = food;
    _resources[RESSOURCE::LINEMATE] = linemate;
    _resources[RESSOURCE::DERAUMERE] = deraumere;
    _resources[RESSOURCE::SIBUR] = sibur;
    _resources[RESSOURCE::MENDIANE] = mendiane;
    _resources[RESSOURCE::PHIRAS] = phiras;
    _resources[RESSOURCE::THYSTAME] = thystame;
}

/**
* @brief Checks if the tile is empty (no resources).
* @return True if the tile is empty, false otherwise.
**/
bool Zappy::Tile::isEmpty() const
{
    for (const auto &pair : _resources) {
        if (pair.second > 0) {
            return false;
        }
    }
    return true;
}