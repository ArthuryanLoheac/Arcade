#include "CoreMenu.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "CLI_Colors.hpp"

Core::CoreMenu::CoreMenu(Core &core)
    : _window(std::make_pair(800, 800), "Arcade", "assets/arcade.png")
{
    std::vector<std::string> displays = core.getDisplayLibs();
    std::vector<std::string> games = core.getGameLibs();
    int i = 0;

    for (auto c:core.getDisplayLibs()) {
        Text text;
        text.setCLI_Color(std::make_pair(CLI_Color::CLI_WHITE, CLI_Color::CLI_BLACK));
        text.setGUI_Color(std::make_tuple(255, 255, 255, 255));
        text.setFontPath("assets/fonts/NotoSans.ttf");
        text.setPosition(std::make_pair(100, 100 + i * 50));
        text.setScale(std::make_pair(20, 20));
        text.setStr(c);
        text.setRotation(0);
        _drawables.push_back(std::make_unique<Text>(text));
        i++;
    }
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

