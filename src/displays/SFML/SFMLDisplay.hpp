#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <string>
#include <utility>

#include "interfaces/IDisplayModule.hpp"
#include "core/Window.hpp"
#include "interfaces/Text.hpp"
#include "interfaces/Sprite.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class SFMLDisplay : public IDisplayModule {
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
    void drawSquare(const Sprite &sprite);

    std::shared_ptr<sf::RenderWindow> _window;
    std::map<std::string, std::pair<std::shared_ptr<sf::Sound>,
        std::shared_ptr<sf::SoundBuffer>>> _musics;
    std::map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::map<std::string, std::shared_ptr<sf::Font>> _fonts;

    int _lastMouseX = 0;
    int _lastMouseY = 0;
    int UNIT_TO_PIXEL = 20;

    static const std::unordered_map<sf::Keyboard::Key, Key::KeyCode>
        keyboardMap;
    static const std::unordered_map<sf::Mouse::Button, Key::KeyCode>
        mouseMap;
};
