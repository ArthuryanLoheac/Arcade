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
#include <chrono>
#include <iostream>
#include <dirent.h>

#include "IDisplayModule.hpp"
#include "IGameModule.hpp"

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
    
    std::vector<std::string> getDisplayLibs() const { return _displayLibs; }
    std::vector<std::string> getGameLibs() const { return _gameLibs; }
    std::string getGameLibPath() const { return _gameLibs[_gameIndex]; }
    std::string getDisplayLibPath() const { return _displayLibs[_displayIndex]; }

    void closeDisplay();
    void openDisplay(const std::string &displayLibPath);
private:
    void openGame(const std::string &gameLibPath);
    void closeGame();

    std::unique_ptr<IDisplayModule> _display;
    std::unique_ptr<IGameModule> _game;

    void *_displayHandle;
    void *_gameHandle;

    std::vector<std::string> _displayLibs;
    std::vector<std::string> _gameLibs;

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
