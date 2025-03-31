/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#include "Core.hpp"

static bool verifyDisplayLib(const std::string &path)
{
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << path << ": Couldn't open dynamic lib." << std::endl;
        return false;
    }
    void* symbolPtr = dlsym(handle, "getDisplayModule");
    if (!symbolPtr) {
        std::cerr << path << ": Couldn't load entrypoint." << std::endl;
        dlclose(handle);
        return false;
    }
    try {
        auto test = reinterpret_cast<getDisplay *>(symbolPtr);
        (void)test;
    } catch(const std::exception& e) {
        std::cerr << path << ": Bad format for entry point." << std::endl;
        return false;
    }
    dlclose(handle);
    return true;
}

static bool verifyGameLib(const std::string &path)
{
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << path << ": Couldn't open dynamic lib." << std::endl;
        return false;
    }
    void* symbolPtr = dlsym(handle, "getGameModule");
    if (!symbolPtr) {
        std::cerr << path << ": Couldn't load entrypoint." << std::endl;
        dlclose(handle);
        return false;
    }
    try {
        auto test = reinterpret_cast<getGame *>(symbolPtr);
        (void)test;
    } catch(const std::exception& e) {
        std::cerr << path << ": Bad format for entry point." << std::endl;
        return false;
    }
    dlclose(handle);
    return true;
}

Core::Core::Core()
{
    _display = nullptr;
    _game = nullptr;
    _displayHandle = nullptr;
    _gameHandle = nullptr;

    DIR *dir = opendir("./lib");
    struct dirent *entry;

    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                std::string filename = entry->d_name;
                if (verifyDisplayLib("./lib/" + filename))
                    _displayLibs.push_back("./lib/" + filename);
                if (verifyGameLib("./lib/" + filename))
                    _gameLibs.push_back("./lib/" + filename);
            }
        }
        closedir(dir);
    }
}

Core::StateCore Core::Core::update()
{
    float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastFrameTime).count();

    if (_game->update(deltaTime))
        return StateCore::EXIT_TO_MENU;
    lastFrameTime = std::chrono::steady_clock::now();
    return StateCore::NONE;
}

bool Core::Core::handleEventLibs(const Event &event)
{
    switch (event.key)
    {
        case Key::KeyCode::KEY_P:
            openGame(_gameLibs[++_gameIndex % _gameLibs.size()]);
            break;
        case Key::KeyCode::KEY_O:
            if (_gameIndex == 0)
                openGame(_gameLibs[_gameLibs.size() - 1]);
            else
                openGame(_gameLibs[--_gameIndex % _gameLibs.size()]);
            break;
        case Key::KeyCode::KEY_I:
            openDisplay(_displayLibs[++_displayIndex % _displayLibs.size()]);
            break;
        case Key::KeyCode::KEY_U:
            if (_displayIndex == 0)
                openDisplay(_displayLibs[_displayLibs.size() - 1]);
            else
                openDisplay(_displayLibs[--_displayIndex % _displayLibs.size()]);
            break;
        default:
            return false;
    }
    return true;
}

Core::StateCore Core::Core::events()
{
    Event event = _display->getEvent();

    while (event.key != Key::KeyCode::NONE) {
        if (event.key == Key::KeyCode::ECHAP)
            return StateCore::EXIT_TO_MENU;
        if (event.key == Key::KeyCode::FUNCTION_4)
            return StateCore::EXIT;
        if (handleEventLibs(event)) {
            if (_game->event(event))
                return StateCore::EXIT_TO_MENU;
        }
        Event event = _display->getEvent();
    }
    return StateCore::NONE;
}

void Core::Core::draw()
{
    _display->clear();
    for (auto drawable : _game->getDrawables())
        _display->draw(drawable);
    for (auto sound : _game->getSound())
        _display->handleSound(sound);
    _display->display();
}

void Core::Core::openGame(const std::string &gameLibPath)
{
    if (_gameHandle)
        closeGame();
    _gameHandle = dlopen(gameLibPath.c_str(), RTLD_LAZY);
    if (!_gameHandle)
        throw Error(dlerror(), "openGameLib");
    getGame *getGameModule = (getGame *)dlsym(_gameHandle, "getGameModule");
    if (!getGameModule)
        throw Error(dlerror(), "openGameLib");
    _game = (*getGameModule)();
}

void Core::Core::openDisplay(const std::string &displayLibPath)
{
    if (_displayHandle)
        closeDisplay();
    _displayHandle = dlopen(displayLibPath.c_str(), RTLD_LAZY);
    if (!_displayHandle)
        throw Error(dlerror(), "openDisplayLib");
    getDisplay *getDisplayModule = (getDisplay *)dlsym(_displayHandle, "getDisplayModule");
    if (!getDisplayModule)
        throw Error(dlerror(), "openDisplayLib");
    _display = (*getDisplayModule)();
}

void Core::Core::closeGame()
{
    if (_game) {
        _game->~IGameModule();
        _game = nullptr;
    }
    if (_gameHandle) {
        dlclose(_gameHandle);
        _gameHandle = nullptr;
    }
}

void Core::Core::closeDisplay()
{
    if (_display) {
        _display->~IDisplayModule();
        _display = nullptr;
    }
    if (_displayHandle) {
        dlclose(_displayHandle);
        _displayHandle = nullptr;
    }
}
