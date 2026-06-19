/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <vector>
    #include <unordered_map>

    #include "RTCError.hpp"
    #include "Tile.hpp"

namespace Zappy
{
    enum class Direction {
        NORTH = 1,
        EAST = 2,
        SOUTH = 3,
        WEST = 4
    };

    class Player
    {
        public:
            Player();
            Player(int id, const std::string &name, const std::string &teamName, int level, int x, int y, Direction direction);
            ~Player() = default;

            int getId() const;
            const std::string& getName() const;
            const std::string& getTeamName() const;
            int getLevel() const;
            int getX() const;
            int getY() const;
            Direction getDirection() const;
            int getResource(RESSOURCE type) const;
            const std::unordered_map<RESSOURCE, int>& getInventory() const;
            float getXVisual() const;
            float getYVisual() const;
            void updateVisual(float deltaTime);

            void setId(int id);
            void setName(const std::string &name);
            void setTeamName(const std::string &teamName);
            void setPosition(int x, int y, Direction direction); 
            void setLevel(int level);
            void setInventory(int food, int linemate, int deraumere, int sibur, int mendiane, int phiras, int thystame);
            void setRessource(RESSOURCE type, int quantity);
            void setPositionVisual(float xVisual, float yVisual);

        private:
            int _id;
            std::string _name;
            std::string _teamName;
            int _level;
            int _x;
            int _y;
            float _xVisual;
            float _yVisual;
            Direction _direction;
            std::unordered_map<RESSOURCE, int> _inventory;
    };
};

