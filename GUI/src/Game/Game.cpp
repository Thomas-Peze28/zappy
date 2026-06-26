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
    : _map(0, 0), time_unit(0), _winningTeam(""), _gameOver(false)
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

void Zappy::Game::addNotification(const std::string &message)
{
    _notifications.push_back(message);
    if (_notifications.size() > 20)
        _notifications.erase(_notifications.begin());
}

const std::vector<std::string> &Zappy::Game::getNotifications() const
{
    return _notifications;
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

/**
* @brief Updates the visual game state.
* @param deltaTime The time elapsed since the last update.
**/
void Zappy::Game::update(float deltaTime)
{
    for (auto &[id, player] : _players) {
        player.updateVisual(deltaTime);
    }
}

/**
* @brief Gets the total amount of a resource for a team.
* @param teamName The name of the team.
* @param type The type of resource.
* @return The total amount of the resource for the team.
**/
int Zappy::Game::getTeamRessource(const std::string &teamName, RESSOURCE type) const
{
    int total = 0;

    for (const auto &[id, player] : _players) {
        if (player.getTeamName() == teamName) {
            total += player.getResource(type);
        }
    }
    return total;
}

/**
* @brief Gets the winning team.
* @return The name of the winning team.
**/
std::string Zappy::Game::getWinningTeam() const
{
    return _winningTeam;
}

/**
* @brief Sets the winning team.
* @param teamName The name of the winning team.
**/
void Zappy::Game::setWinningTeam(const std::string &teamName)
{
    _winningTeam = teamName;
}

/**
* @brief Checks if the game is over.
* @return True if the game is over, false otherwise.
**/
bool Zappy::Game::isGameOver() const
{
    return _gameOver;
}

/**
* @brief Sets the game over status.
* @param gameOver The new game over status.
**/
void Zappy::Game::setGameOver(bool gameOver)
{
    _gameOver = gameOver;
}
