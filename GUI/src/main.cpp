/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "Core.hpp"

int main(int argc, char *argv[])
{
    try {
        Zappy::Core core(argc, argv);

        return core.run();
    
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
