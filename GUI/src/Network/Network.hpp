/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Network
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>

    #include "RTCError.hpp"

namespace Zappy
{
    class Network
    {
        public:
            Network(std::string host, int port);
            ~Network() = default;
        
        private:
            std::string _host;
            int _port;
    };
};
