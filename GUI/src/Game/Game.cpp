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
    : _map(0, 0), time_unit(1)
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
**/
Zappy::Player &Zappy::Game::getPlayer(int id)
{
    return _players.at(id);
}

/**
* @brief Gets all players in the game.
* @return A reference to the map of players.
**/
const std::unordered_map<int, Zappy::Player> &Zappy::Game::getPlayers() const
{
    return _players;
}

/**
* @brief Adds an egg to the game.
* @param egg The egg to add.
**/
void Zappy::Game::addEgg(const Egg& egg)
{
    _eggs[egg.getId()] = egg;
}

/**
* @brief Removes an egg from the game.
* @param id The ID of the egg to remove.
**/
void Zappy::Game::removeEgg(int id)
{
    _eggs.erase(id);
}

/**
* @brief Gets an egg from the game.
* @param id The ID of the egg to get.
* @return A reference to the requested egg.
*/
Zappy::Egg &Zappy::Game::getEgg(int id)
{
    return _eggs.at(id);
}

/**
* @brief Gets all eggs in the game.
* @return A reference to the map of eggs.
**/
const std::unordered_map<int, Zappy::Egg> &Zappy::Game::getEggs() const
{
    return _eggs;
}

/**
* @brief Gets the time unit of the game.
* @return The time unit.
**/
int Zappy::Game::getTimeUnit() const
{
    return time_unit;
}

/**
* @brief Sets the time unit of the game.
* @param timeUnit The new time unit.
**/
void Zappy::Game::setTimeUnit(int timeUnit)
{
    time_unit = timeUnit;
}
