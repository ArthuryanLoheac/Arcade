/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#pragma once
#include <memory>

namespace Core {
    class Core;
    class error : public std::exception {
    };
}

class Core {
public:
    Core();
    ~Core() = default;

private:
    bool openGameLib(const std::string &gameLibPath);
    bool openDisplayLib(const std::string &displayLibPath);

    bool closeGameLib();
    bool closeDisplayLib();

    std::unique_ptr<IDisplay> _displayLib;
    std::unique_ptr<IGame> _gameLib;
};
