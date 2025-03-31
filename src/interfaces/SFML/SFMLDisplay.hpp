/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SFMLDisplay
*/

#pragma once
#include "IDisplayModule.hpp"
#include "Window.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <unordered_map>

std::unique_ptr<IDisplayModule> getDisplayModule();

class SFMLDisplay : public IDisplayModule
{
public:
    void createWindow(const Window &window) override;
    void clear(void) override;
    void draw(const IDrawable &to_draw) override;
    void display(void) override;
    Event getEvent(void) override;
    void handleSound(const Sound &sound) override;
    ~SFMLDisplay();

private:
    Event getEventKeyBoard(sf::Event &e, Event::KeyStatus isDown);
    Event getEventMouse(sf::Event &e, Event::KeyStatus isDown);

    void drawText(const Text &txt);
    void drawSprite(const Sprite &sprite);

    std::shared_ptr<sf::RenderWindow> _window;
    std::map<std::string, std::pair<std::shared_ptr<sf::Sound>, std::shared_ptr<sf::SoundBuffer>>> _musics;
    std::map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::map<std::string, std::shared_ptr<sf::Font>> _fonts;

    int _lastMouseX = 0;
    int _lastMouseY = 0;

    static const std::unordered_map<sf::Keyboard::Key, Key::KeyCode> keyboardMap;
    static const std::unordered_map<sf::Mouse::Button, Key::KeyCode> mouseMap;
};