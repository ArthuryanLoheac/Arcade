#include "CoreMenu.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "CLI_Colors.hpp"


Core::CoreMenu::CoreMenu(Core &core)
    : _window(std::make_pair(800, 800), "Arcade", "assets/arcade.png")
{
    std::vector<std::string> displays = core.getDisplayLibs();
    std::vector<std::string> games = core.getGameLibs();
    this->core = &core;
    int i = 0;

    AddText("Displays:", 50, 50, 30);
    for (auto c:core.getDisplayLibs()) {
        AddText(c, 100, 100 + i * 50, 20);
        i++;
    }
    AddText("Games:", 50, 100 + i * 50, 30);
    i++;
    for (auto c:core.getGameLibs()) {
        AddText(c, 100, 100 + i * 50, 20);
        i++;
    }
}

void Core::CoreMenu::AddText(const std::string &str, int x, int y, int fontSize, std::tuple <int, int, int, int> colorGUI, enum CLI_Color colorCLI)
{
    Text text;
    text.setCLI_Color(std::make_pair(colorCLI, CLI_Color::CLI_BLACK));
    text.setGUI_Color(colorGUI);
    text.setFontPath("assets/fonts/NotoSans.ttf");
    text.setPosition(std::make_pair(x, y));
    text.setScale(std::make_pair(fontSize, fontSize));
    text.setStr(str);
    text.setRotation(0);
    _drawables.push_back(std::make_unique<Text>(text));
}

Core::CoreMenu::~CoreMenu()
{
}

bool Core::CoreMenu::update(float deltaTime)
{
    (void)deltaTime;
    for (auto &d : _drawables) {
        if (dynamic_cast<Text *>(d.get())) {
            Text &txt = dynamic_cast<Text &>(*d);
            if (txt.getStr() == core->getDisplayLibPath() || txt.getStr() == core->getGameLibPath()) {
                txt.setCLI_Color(std::make_pair(CLI_Color::CLI_RED, CLI_Color::CLI_BLACK));
                txt.setGUI_Color(std::make_tuple(255, 0, 0, 255));
            } else {
                txt.setCLI_Color(std::make_pair(CLI_Color::CLI_WHITE, CLI_Color::CLI_BLACK));
                txt.setGUI_Color(std::tuple(255, 255, 255, 255));
            }
        }
    }
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
