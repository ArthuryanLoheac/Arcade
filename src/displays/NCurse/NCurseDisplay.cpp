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
    int ch = getch();
    Event event(Key::KEY_A, 0);

    std::cout << "Key pressed: " << ch << std::endl;

    return event;
}

void NCurseDisplay::handleSound(const Sound &sound)
{
    (void)sound;
}
