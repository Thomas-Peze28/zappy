/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Game
*/

#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
#include <csignal>
#include <unordered_map>
#include <vector>

#include "RTCError.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Egg.hpp"

namespace Zappy
{
    class Game
    {
        public:
            Game();
            ~Game() = default;

            Map &getMap();
            const Map &getMap() const;

            void addPlayer(const Player& player);
            void removePlayer(int id);
            Player &getPlayer(int id);
            const std::unordered_map<int, Player> &getPlayers() const;
            void addNotification(const std::string &message);
            const std::vector<std::string> &getNotifications() const;
        
            void addEgg(const Egg& egg);
            void removeEgg(int id);
            Egg &getEgg(int id);
            const std::unordered_map<int, Egg> &getEggs() const;

            int getTimeUnit() const;
            void setTimeUnit(int timeUnit);

            void update(float deltaTime);
        
        private:
            Map _map;
            std::unordered_map<int, Player> _players;
            std::unordered_map<int, Egg> _eggs;
            std::vector<std::string> _notifications;

            int time_unit;
    };
};
