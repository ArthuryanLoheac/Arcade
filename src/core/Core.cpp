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

    DIR *dir = opendir("./dynlib");
    struct dirent *entry;

    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                std::string filename = entry->d_name;
                if (verifyDisplayLib("./dynlib/" + filename))
                    _displayLibs.push_back("./dynlib/" + filename);
                if (verifyGameLib("./dynlib/" + filename))
                    _gameLibs.push_back("./dynlib/" + filename);
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
            openGameLib(_gameLibs[++_gameIndex % _gameLibs.size()]);
            break;
        case Key::KeyCode::KEY_O:
            if (_gameIndex == 0)
                openGameLib(_gameLibs[_gameLibs.size() - 1]);
            else
                openGameLib(_gameLibs[--_gameIndex % _gameLibs.size()]);
            break;
        case Key::KeyCode::KEY_I:
            openDisplayLib(_displayLibs[++_displayIndex % _displayLibs.size()]);
            break;
        case Key::KeyCode::KEY_U:
            if (_displayIndex == 0)
                openDisplayLib(_displayLibs[_displayLibs.size() - 1]);
            else
                openDisplayLib(_displayLibs[--_displayIndex % _displayLibs.size()]);
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
