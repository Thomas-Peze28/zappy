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
    _fd = -1;
    _poll.fd = -1;
    _poll.events = 0;
}

/**
* Cleans up the Network resources.
**/
Zappy::Network::~Network()
{
    if (_fd > 0) {
        close(_fd);
    }
}

/**
* Updates the Network by checking for incoming messages and processing them.
**/
void Zappy::Network::update()
{
    if (_fd <= 0)
        connectToServer();
    
    int result = poll(&_poll, 1, 0);

    if (result < 0)
        throw ErrorMajor("Poll error: " + std::string(strerror(errno)));

    if (result > 0 && (_poll.revents & POLLIN)) {
        char buffer[4096];
        ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead < 0) {
            throw ErrorMajor("Receive error: " + std::string(strerror(errno)));
        } else if (bytesRead == 0) {
            throw ErrorMajor("Connection closed by server");
        } else {
            buffer[bytesRead] = '\0';
            _readBuffer += buffer;
            size_t pos;
            while ((pos = _readBuffer.find('\n')) != std::string::npos) {
                std::string message = _readBuffer.substr(0, pos);
                _messages.push(message);
                _readBuffer.erase(0, pos + 1);
            }
        }
    }
}

/**
* Sends a message to the server.
* @param msg The message to send.
**/
void Zappy::Network::sendMessage(const std::string &msg)
{
    if (_fd <= 0)
        connectToServer();
    send(_fd, msg.c_str(), msg.length(), 0);
}

/**
* Checks if there are any messages received from the server.
* @return True if there are messages, false otherwise.
**/
bool Zappy::Network::hasMessage() const
{
    return !_messages.empty();
}

/**
* Pops the next message from the message queue.
* @return The next message, or an empty string if there are no messages.
**/
std::string Zappy::Network::popMessage()
{
    if (_messages.empty())
        return "";
    std::string message = _messages.front();
    _messages.pop();
    return message;
}

/**
* Connects to the server using the specified host and port.
**/
void Zappy::Network::connectToServer()
{
    struct addrinfo hints;
    struct addrinfo *result = NULL;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(_host.c_str(), std::to_string(_port).c_str(), &hints, &result);
    if (status != 0)
        throw ErrorMajor("Invalid address " + _host + ": " + std::string(gai_strerror(status)));

    _fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (_fd < 0) {
        freeaddrinfo(result);
        throw ErrorMajor("Socket creation error: " + std::string(strerror(errno)));
    }

    if (connect(_fd, result->ai_addr, result->ai_addrlen) < 0) {
        close(_fd);
        _fd = -1;
        freeaddrinfo(result);
        throw ErrorMajor("Connection error: " + std::string(strerror(errno)));
    }

    freeaddrinfo(result);
    _poll.fd = _fd;
    _poll.events = POLLIN;
    std::cout << "Connected to " << _host << " on port " << _port << std::endl;
    sendMessage("GRAPHIC\n");
}