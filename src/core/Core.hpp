/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#pragma once
#include <memory>
#include <vector>
#include "dlfcn.h"

#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include <chrono>

typedef std::unique_ptr<IDisplayModule> (*getDisplay)();
typedef std::unique_ptr<IGameModule> (*getGame)();

namespace Core {

enum StateCore {
    NONE,
    EXIT_TO_MENU,
    EXIT
};

class Core {
public:
    Core();
    ~Core() = default;
    StateCore update();
    StateCore events();
    void draw();

private:
    void openGameLib(const std::string &gameLibPath);
    void openDisplayLib(const std::string &displayLibPath);

    void closeGameLib();
    void closeDisplayLib();

    std::unique_ptr<IDisplayModule> _display;
    std::unique_ptr<IGameModule> _game;

    void *_displayHandle;
    void *_gameHandle;

    const std::vector<std::string> _displayLibs = {
        "./dynlib/displays/lib_arcade_ncurses.so",
        "./dynlib/displays/lib_arcade_sdl.so",
        "./dynlib/displays/lib_arcade_sfml.so"
    };
    const std::vector<std::string> _gameLibs = {
        "./dynlib/games/lib_arcade_minesweeper.so",
        "./dynlib/games/lib_arcade_pacman.so",
        "./dynlib/games/lib_arcade_centipede.so"
    };

    std::chrono::steady_clock::time_point lastFrameTime;
    int _displayIndex = 0;
    int _gameIndex = 0;

    bool handleEventLibs(const Event &event);
};

class Error : public std::exception {
    public:
    Error(const std::string &what, const std::string &where) : _what(what), _where(where) {}
    ~Error() = default;

    const char *what() const noexcept override { return _what.c_str(); }
    const char *where() const noexcept { return _where.c_str(); }
private:
    std::string _what;
    std::string _where;
};
}
