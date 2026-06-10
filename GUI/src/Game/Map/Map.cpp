/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#include "Map.hpp"

/**
* @brief Constructs a new Map object with default dimensions (0x0).
**/
Zappy::Map::Map()
    : _width(0), _height(0), _tiles()
{
}

/**
* @brief Constructs a new Map object with the specified dimensions.
* @param width The width of the map.
* @param height The height of the map.
**/
Zappy::Map::Map(int width, int height)
    : _width(width), _height(height), _tiles(height, std::vector<Tile>(width))
{
}

/**
* @brief Resizes the map to the specified dimensions.
* @param width The new width of the map.
* @param height The new height of the map.
**/
void Zappy::Map::resize(int width, int height)
{
    _width = width;
    _height = height;
    _tiles.resize(height);

    for (auto &row : _tiles) {
        row.resize(width);
    }
}

/**
* @brief Sets the content of a tile at the specified coordinates.
* @param x The x-coordinate of the tile.
* @param y The y-coordinate of the tile.
* @param content The content to set.
**/
void Zappy::Map::setTileContent(int x, int y, const Tile &content)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw ErrorMinor("Tile coordinates out of bounds");
    }
    _tiles[y][x] = content;
}

/**
* @brief Gets the content of a tile at the specified coordinates.
* @param x The x-coordinate of the tile.
* @param y The y-coordinate of the tile.
* @return A reference to the content of the tile.
**/
const Zappy::Tile &Zappy::Map::getTile(int x, int y) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw ErrorMinor("Tile coordinates out of bounds");
    }
    return _tiles[y][x];
}

/**
* @brief Gets the width of the map.
* @return The width of the map.
**/
int Zappy::Map::getWidth() const
{
    return _width;
}

/**
* @brief Gets the height of the map.
* @return The height of the map.
**/
int Zappy::Map::getHeight() const
{
    return _height;
}
