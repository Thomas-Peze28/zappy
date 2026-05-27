/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** RTCError
*/

#pragma once

    #include <exception>
    #include <string>

namespace Zappy
{
    class AError : public std::exception
    {
        public:
            AError(std::string message) : _message(message) {};
            const char *what() const noexcept override {return _message.c_str();};

        private:
            std::string _message;
    };

    class ErrorMinor : public AError
    {
        public:
            ErrorMinor(std::string message) : AError(std::move(message)) {};
    };

    class ErrorMajor : public AError
    {
        public:
            ErrorMajor(std::string message) : AError(std::move(message)) {};
    };

    class ErrorCritical : public AError
    {
        public:
            ErrorCritical(std::string message) : AError(std::move(message)) {};
    };
};
