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

/**
* @brief Adds a player to the game.
* @param player The player to add.
**/
void Zappy::Game::addPlayer(const Player& player)
{
    _players[player.getId()] = player;
}

/**
* @brief Removes a player from the game.
* @param id The ID of the player to remove.
**/
void Zappy::Game::removePlayer(int id)
{
    _players.erase(id);
}

/**
* @brief Gets a player from the game.
* @param id The ID of the player to get.
* @return A reference to the requested player.
*/
Zappy::Player &Zappy::Game::getPlayer(int id)
{
    return _players.at(id);
}

/**
* @brief Gets all players in the game.
* @return A reference to the map of players.
*/
const std::unordered_map<int, Zappy::Player> &Zappy::Game::getPlayers() const
{
    return _players;
}
