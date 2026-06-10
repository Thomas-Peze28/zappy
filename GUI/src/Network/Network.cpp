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
Zappy::Network::Network(std::string host, int port) : _host(host), _port(port)
{
}
