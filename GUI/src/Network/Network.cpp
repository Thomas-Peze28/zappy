/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Network
*/

#include "Network.hpp"

/**
* Initializes the Network with the specified host and port.
* @param host The host to connect to.
* @param port The port to connect to.
**/
Zappy::Network::Network(std::string host, int port)
{
    if (host.empty() || port <= 0 || port > 65535) {
        throw ErrorMajor("Invalid host or port");
    }
    if (host.length() > 255) {
        throw ErrorMajor("Host name too long");
    }
    _host = std::move(host);
    _port = port;
}
