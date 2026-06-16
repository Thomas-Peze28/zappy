/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Egg
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
    class Egg
    {
        public:
            Egg();
            Egg(int id, const std::string &name, const std::string &teamName, int level, int x, int y);
            ~Egg() = default;

            int getId() const;
            const std::string& getName() const;
            const std::string& getTeamName() const;
            int getLevel() const;
            int getX() const;
            int getY() const;

            void setId(int id);
            void setName(const std::string &name);
            void setTeamName(const std::string &teamName);
            void setPosition(int x, int y); 
            void setLevel(int level);
        
        private:
            int _id;
            std::string _name;
            std::string _teamName;
            int _level;
            int _x;
            int _y;
    };
};

