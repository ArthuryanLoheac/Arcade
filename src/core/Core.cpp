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

void Core::Core::update()
{
    float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastFrameTime).count();

    if (_game->update(deltaTime))
        //Stop le jeu
    lastFrameTime = std::chrono::steady_clock::now();
}

void Core::Core::events()
{
    Event event = _display->getEvent();

    while (event.key != Key::KeyCode::NONE) {
        //Gestion des events pour le core
        _game->event(event);
        Event event = _display->getEvent();
    }
}

void Core::Core::draw()
{
    _display->clear();
    for (auto drawable : _game->getDrawables())
        _display->draw(drawable);
    //Joue les sons
    _display->display();
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
    (void)displayLibPath;
}

void Core::Core::closeGameLib()
{
}

void Core::Core::closeDisplayLib()
{
}
