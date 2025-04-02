#include "CoreMenu.hpp"
#include "Text.hpp"
#include "Sprite.hpp"

Core::CoreMenu::CoreMenu(Core &core)
    : _window(std::make_pair(800, 800), "Arcade", "assets/arcade.png")
{
    std::vector<std::string> displays = core.getDisplayLibs();
    std::vector<std::string> games = core.getGameLibs();

    Sprite s;
    s.setAnimationTime(0);
    s.setCLI_Textures(std::vector<std::string>{"HI"});
    s.setGUI_Color(std::make_tuple(255, 255, 255, 255));
    s.setGUI_Textures(std::vector<std::string>{"assets/arcade.png"});
    s.setPosition(std::make_pair(0, 0));
    s.setRotation(0);
    s.setScale(std::make_pair(0.5f, 0.5f));
    _drawables.push_back(std::make_unique<Sprite>(s));
}

Core::CoreMenu::~CoreMenu()
{
}

bool Core::CoreMenu::update(float deltaTime)
{
    (void)deltaTime;
    return false;
}

const Window &Core::CoreMenu::getWindow(void)
{
    return _window;
}

const std::vector<std::unique_ptr<IDrawable>> &Core::CoreMenu::getDrawables(void)
{
    return _drawables;
}

const std::vector<Sound> &Core::CoreMenu::getSound(void)
{
    return _sounds;
}

bool Core::CoreMenu::event(const Event &events)
{
    (void)events;
    return false;
}

std::vector<std::pair<std::string, int>> Core::CoreMenu::getScores(void)
{
    return std::vector<std::pair<std::string, int>>();
}

