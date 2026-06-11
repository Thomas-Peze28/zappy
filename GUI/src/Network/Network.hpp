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
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <poll.h>
    #include <queue>

    #include "RTCError.hpp"

namespace Zappy
{
    class Network
    {
        public:
            Network(std::string host, int port);
            ~Network();

            void update();
            void sendMessage(const std::string &msg);
            bool hasMessage() const;
            std::string popMessage();

        private:
            void connectToServer();

            std::string _host;
            int _port;

            int _fd;
            struct pollfd _pollFd;

            std::string _readBuffer;
            std::queue<std::string> _messages;
    };
};
