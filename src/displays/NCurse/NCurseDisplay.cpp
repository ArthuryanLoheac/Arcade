/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurseDisplay
*/

#include "NCurseDisplay.hpp"

extern "C" std::unique_ptr<IDisplayModule> getDisplayModule(void)
{
    return std::make_unique<NCurseDisplay>();
}

NCurseDisplay::NCurseDisplay()
{
    _window = nullptr;
}

NCurseDisplay::~NCurseDisplay()
{
    if (_window)
        _window->~NCurseWrapper();
}

void NCurseDisplay::createWindow(const Window &window)
{
    _window = std::make_unique<NCurseWrapper>(window.size.second, window.size.first);
}

void NCurseDisplay::draw(const IDrawable &to_draw)
{
    Sprite sprite;
    Text text;

    try {
        sprite = dynamic_cast<const Sprite &>(to_draw);
        _window->print(sprite.getPosition().first, sprite.getPosition().second, sprite.getCLI_Textures()[0]);
    } catch (std::bad_cast &e) {
        text = dynamic_cast<const Text &>(to_draw);
        _window->print(text.getPosition().first, text.getPosition().second, text.getStr());
    }
}

void NCurseDisplay::display(void)
{
    _window->refresh();
}

void NCurseDisplay::clear(void)
{
    _window->clear();
}

Event NCurseDisplay::getEvent(void)
{
    int ch = -1;
    int x = 0, y = 0;
    Key::KeyCode key = Key::NONE;

    ch = getch();

    key = getKeyBoardCode(ch);
    if (key != Key::NONE)
        return Event(key, Key::KeyStatus::KEY_PRESSED);
    key = getMouseCode(x, y);
    if (key != Key::NONE)
        return Event(key, Event::MousePos{x, y});
    return Event(Key::NONE, std::any());
}

Key::KeyCode NCurseDisplay::getMouseCode(int &x, int &y)
{
    MEVENT event;

    if (getmouse(&event) == OK) {
        x = event.x;
        y = event.y;
        if (event.bstate & BUTTON1_PRESSED) return Key::MOUSE_LEFT;
        if (event.bstate & BUTTON2_PRESSED) return Key::MOUSE_MIDDLE;
        if (event.bstate & BUTTON3_PRESSED) return Key::MOUSE_RIGHT;
        if (event.bstate & BUTTON4_PRESSED) return Key::MOUSE_BUTTON_4;
        if (event.bstate & BUTTON5_PRESSED) return Key::MOUSE_BUTTON_5;
        if (event.bstate & REPORT_MOUSE_POSITION) return Key::MOUSE_MOVE;
    }
    return Key::NONE;
}

Key::KeyCode NCurseDisplay::getKeyBoardCode(int ch)
{
    auto it = _keyMap.find(ch);
    return (it != _keyMap.end()) ? it->second : Key::NONE;
}

void NCurseDisplay::handleSound(const Sound &sound)
{
    (void)sound;
}
