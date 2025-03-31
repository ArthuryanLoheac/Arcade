#include "CoreGame.hpp"
#include <Text.hpp>

Core::CoreGame::CoreGame(Core &core)
    : _window(std::make_pair(800, 800), "Arcade", "assets/arcade.png")
{
    std::vector<std::string> displays = core.getDisplayLibs();
    std::vector<std::string> games = core.getGameLibs(); 
}

Core::CoreGame::~CoreGame()
{
}

bool Core::CoreGame::update(float deltaTime)
{
    (void)deltaTime;
    return false;
}

const Window &Core::CoreGame::getWindow(void)
{
    return _window;
}

const std::vector<std::unique_ptr<IDrawable>> &Core::CoreGame::getDrawables(void)
{
    return _drawables;
}

const std::vector<Sound> &Core::CoreGame::getSound(void)
{
    return _sounds;
}

bool Core::CoreGame::event(const Event &events)
{
    (void)events;
    return false;
}

std::vector<std::pair<std::string, int>> Core::CoreGame::getScores(void)
{
    return std::vector<std::pair<std::string, int>>();
}

