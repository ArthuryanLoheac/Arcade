/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#include "Core.hpp"
#include "CoreMenu.hpp"

static bool verifyDisplayLib(const std::string &path)
{
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
        return false;
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
    if (!handle)
        return false;
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
    _gameLibs.push_back("Menu");

    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                std::string filename = entry->d_name;
                if (verifyDisplayLib("./lib/" + filename))
                    _displayLibs.push_back("./lib/" + filename);
                else if (verifyGameLib("./lib/" + filename))
                    _gameLibs.push_back("./lib/" + filename);
                else
                    std::cerr << filename << ": Not a valid library." << std::endl;
            }
        }
        closedir(dir);
    }
    _game = std::make_unique<CoreMenu>(*this);
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
    try {
        if (std::any_cast<Event::KeyStatus>(event.value) != Event::KeyStatus::KEY_RELEASED)
            return false;
    } catch (const std::bad_any_cast& e) {}
    switch (event.key) {
        case Key::KeyCode::KEY_P:
            if (_gameLibs.size() == 0)
                break;
            _gameIndex = (_gameIndex + 1) % _gameLibs.size();
            if (_gameLibs[_gameIndex] == "Menu")
                _game = std::make_unique<CoreMenu>(*this);
            else
                openGame(_gameLibs[_gameIndex]);
            break;
        case Key::KeyCode::KEY_O:
            if (_gameLibs.size() == 0)
                break;
            _gameIndex =(_gameIndex - 1) % _gameLibs.size();
            if (_gameLibs[_gameIndex] == "Menu")
                _game = std::make_unique<CoreMenu>(*this);
            else
                openGame(_gameLibs[_gameIndex]);
            break;
        case Key::KeyCode::KEY_I:
            if (_displayLibs.size() == 0)
                break;
            openDisplay(_displayLibs[++_displayIndex % _displayLibs.size()]);
            break;
        case Key::KeyCode::KEY_U:
            if (_displayLibs.size() == 0)
                break;
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
        handleEventLibs(event);
        if (_game->event(event))
            return StateCore::EXIT_TO_MENU;
        event = _display->getEvent();
    }
    return StateCore::NONE;
}

void Core::Core::draw()
{
    _display->clear();
    for (const std::unique_ptr<IDrawable>& drawable : _game->getDrawables())
        _display->draw(*drawable.get());
    for (auto sound : _game->getSound())
        _display->handleSound(sound);
    _display->display();
}

void Core::Core::openGame(const std::string &gameLibPath)
{
    if (_gameHandle)
        closeGame();
    void *handle = dlopen(gameLibPath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        exit(84);
    }
    auto createModule = reinterpret_cast<std::unique_ptr<IGameModule> (*)()>(dlsym(handle, "getGameModule"));
    if (!createModule) {
        std::cerr << "Error loading symbol: " << dlerror() << std::endl;
        dlclose(handle);
        exit(84);
    }
    auto module = createModule();
    _game = std::move(module);
}

void Core::Core::openDisplay(const std::string &displayLibPath)
{
    if (_displayHandle)
        closeDisplay();
    void *handle = dlopen(displayLibPath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        exit(84);
    }
    auto createModule = reinterpret_cast<std::unique_ptr<IDisplayModule> (*)()>(dlsym(handle, "getDisplayModule"));
    if (!createModule) {
        std::cerr << "Error loading symbol: " << dlerror() << std::endl;
        dlclose(handle);
        exit(84);
    }
    auto module = createModule();
    _display = std::move(module);
    _game->getWindow();
    _display->createWindow(_game->getWindow());
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
