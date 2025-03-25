/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#include "Core.hpp"

Core::Core::Core()
{
    _display = nullptr;
    _game = nullptr;
}

void Core::Core::openGameLib(const std::string &gameLibPath)
{
    if (_gameHandle)
        closeGameLib();
    _gameHandle = dlopen(gameLibPath.c_str(), RTLD_LAZY);
    if (!_gameHandle)
        throw Error(dlerror(), "openGameLib");
    getGame *getGameModule = (getGame *)dlsym(_gameHandle, "getGameModule");
    if (!getGameModule)
        throw Error(dlerror(), "openGameLib");
    _game = (*getGameModule)();
}

void Core::Core::openDisplayLib(const std::string &displayLibPath)
{
    
}

void Core::Core::closeGameLib()
{
}

void Core::Core::closeDisplayLib()
{
}
