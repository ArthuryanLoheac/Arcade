/*
** EPITECH PROJECT, 2025
** Test
** File description:
** main
*/

#include <iostream>
#include "src/core/Core.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <path_to_display_library>" << std::endl;
        return 84;
    }

    Core::Core core;

    core.openDisplay(av[1]);
    while (core.events() != Core::StateCore::EXIT) {
        core.update();
        core.draw();
    }
    return 0;
}
