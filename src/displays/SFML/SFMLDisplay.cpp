/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SFMLDisplay
*/

#include "SFMLDisplay.hpp"
#include "Window.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <unordered_map>
#include "Text.hpp"
#include "Sprite.hpp"
#include <iostream>

const std::unordered_map<sf::Keyboard::Key, Key::KeyCode> SFMLDisplay::keyboardMap = {
    {sf::Keyboard::A, Key::KeyCode::KEY_A},
    {sf::Keyboard::B, Key::KeyCode::KEY_B},
    {sf::Keyboard::C, Key::KeyCode::KEY_C},
    {sf::Keyboard::D, Key::KeyCode::KEY_D},
    {sf::Keyboard::E, Key::KeyCode::KEY_E},
    {sf::Keyboard::F, Key::KeyCode::KEY_F},
    {sf::Keyboard::G, Key::KeyCode::KEY_G},
    {sf::Keyboard::H, Key::KeyCode::KEY_H},
    {sf::Keyboard::I, Key::KeyCode::KEY_I},
    {sf::Keyboard::J, Key::KeyCode::KEY_J},
    {sf::Keyboard::K, Key::KeyCode::KEY_K},
    {sf::Keyboard::L, Key::KeyCode::KEY_L},
    {sf::Keyboard::M, Key::KeyCode::KEY_M},
    {sf::Keyboard::N, Key::KeyCode::KEY_N},
    {sf::Keyboard::O, Key::KeyCode::KEY_O},
    {sf::Keyboard::P, Key::KeyCode::KEY_P},
    {sf::Keyboard::Q, Key::KeyCode::KEY_Q},
    {sf::Keyboard::R, Key::KeyCode::KEY_R},
    {sf::Keyboard::S, Key::KeyCode::KEY_S},
    {sf::Keyboard::T, Key::KeyCode::KEY_T},
    {sf::Keyboard::U, Key::KeyCode::KEY_U},
    {sf::Keyboard::V, Key::KeyCode::KEY_V},
    {sf::Keyboard::W, Key::KeyCode::KEY_W},
    {sf::Keyboard::X, Key::KeyCode::KEY_X},
    {sf::Keyboard::Y, Key::KeyCode::KEY_Y},
    {sf::Keyboard::Z, Key::KeyCode::KEY_Z},
    {sf::Keyboard::Num0, Key::KeyCode::KEY_0},
    {sf::Keyboard::Num1, Key::KeyCode::KEY_1},
    {sf::Keyboard::Num2, Key::KeyCode::KEY_2},
    {sf::Keyboard::Num3, Key::KeyCode::KEY_3},
    {sf::Keyboard::Num4, Key::KeyCode::KEY_4},
    {sf::Keyboard::Num5, Key::KeyCode::KEY_5},
    {sf::Keyboard::Num6, Key::KeyCode::KEY_6},
    {sf::Keyboard::Num7, Key::KeyCode::KEY_7},
    {sf::Keyboard::Num8, Key::KeyCode::KEY_8},
    {sf::Keyboard::Num9, Key::KeyCode::KEY_9},
    {sf::Keyboard::Space, Key::KeyCode::SPACE},
    {sf::Keyboard::Return, Key::KeyCode::ENTER},
    {sf::Keyboard::Tab, Key::KeyCode::TAB},
    {sf::Keyboard::Escape, Key::KeyCode::ECHAP},
    {sf::Keyboard::Delete, Key::KeyCode::SUPPR},
    {sf::Keyboard::F1, Key::KeyCode::FUNCTION_1},
    {sf::Keyboard::F2, Key::KeyCode::FUNCTION_2},
    {sf::Keyboard::F3, Key::KeyCode::FUNCTION_3},
    {sf::Keyboard::F4, Key::KeyCode::FUNCTION_4},
    {sf::Keyboard::F5, Key::KeyCode::FUNCTION_5},
    {sf::Keyboard::F6, Key::KeyCode::FUNCTION_6},
    {sf::Keyboard::F7, Key::KeyCode::FUNCTION_7},
    {sf::Keyboard::F8, Key::KeyCode::FUNCTION_8},
    {sf::Keyboard::F9, Key::KeyCode::FUNCTION_9},
    {sf::Keyboard::F10, Key::KeyCode::FUNCTION_10},
    {sf::Keyboard::F11, Key::KeyCode::FUNCTION_11},
    {sf::Keyboard::F12, Key::KeyCode::FUNCTION_12},
    {sf::Keyboard::Left, Key::KeyCode::LEFT},
    {sf::Keyboard::Up, Key::KeyCode::UP},
    {sf::Keyboard::Right, Key::KeyCode::RIGHT},
    {sf::Keyboard::Down, Key::KeyCode::DOWN},
    {sf::Keyboard::LShift, Key::KeyCode::L_SHIFT},
    {sf::Keyboard::RShift, Key::KeyCode::R_SHIFT},
    {sf::Keyboard::LControl, Key::KeyCode::L_CONTROL},
    {sf::Keyboard::RControl, Key::KeyCode::R_CONTROL},
    {sf::Keyboard::LAlt, Key::KeyCode::ALT},
    {sf::Keyboard::RAlt, Key::KeyCode::ALTGR}
};

const std::unordered_map<sf::Mouse::Button, Key::KeyCode> SFMLDisplay::mouseMap = {
    {sf::Mouse::Left, Key::KeyCode::MOUSE_LEFT},
    {sf::Mouse::Middle, Key::KeyCode::MOUSE_MIDDLE},
    {sf::Mouse::Right, Key::KeyCode::MOUSE_RIGHT},
    {sf::Mouse::XButton1, Key::KeyCode::MOUSE_BUTTON_4},
    {sf::Mouse::XButton2, Key::KeyCode::MOUSE_BUTTON_5}
};

extern "C" std::unique_ptr<IDisplayModule> getDisplayModule()
{
    return std::make_unique<SFMLDisplay>();
}

void SFMLDisplay::createWindow(const Window &window)
{
    int windowWidth = window.size.first * UNIT_TO_PIXEL;
    int windowHeight = window.size.second * UNIT_TO_PIXEL;

    _window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(windowWidth, windowHeight),
        window.title);

    if (!_window)
        throw std::runtime_error("Failed to create SFML window");

    _window->setFramerateLimit(60);
    if (!window.iconPath.empty()) {
        auto icon = std::make_shared<sf::Image>();
        if (icon->loadFromFile(window.iconPath))
            _window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
    }
}

void SFMLDisplay::draw(const IDrawable &drawable)
{
    Sprite sprite;
    Text text;

    try {
        sprite = dynamic_cast<const Sprite &>(drawable);
        drawSprite(sprite);
    } catch (std::bad_cast &e) {
        text = dynamic_cast<const Text &>(drawable);
        drawText(text);
    }
}

void SFMLDisplay::display(void)
{
    _window->display();
}

void SFMLDisplay::clear(void)
{
    _window->clear(sf::Color::Black);
}

Event SFMLDisplay::getEvent(void)
{
    sf::Event event;

    if (!_window->pollEvent(event))
        return Event(Key::KeyCode::NONE, std::any());

    switch (event.type)
    {
        case sf::Event::Closed:
            return Event(Key::KeyCode::FUNCTION_4, Event::KeyStatus::KEY_PRESSED);
        case sf::Event::KeyPressed:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_PRESSED);
        case sf::Event::KeyReleased:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_RELEASED);
        case sf::Event::MouseMoved:
            _lastMouseX = event.mouseMove.x / UNIT_TO_PIXEL;
            _lastMouseY = event.mouseMove.y / UNIT_TO_PIXEL;
            return Event(Key::KeyCode::MOUSE_MOVE, Event::MousePos{event.mouseMove.x  / UNIT_TO_PIXEL, event.mouseMove.y / UNIT_TO_PIXEL});
        case sf::Event::MouseButtonPressed:
            return getEventMouse(event, Event::KeyStatus::KEY_PRESSED);
        case sf::Event::MouseButtonReleased:
            return getEventMouse(event, Event::KeyStatus::KEY_RELEASED);
        case sf::Event::MouseWheelScrolled:
            return Event(Key::KeyCode::MOUSE_SCROLL, Event::MouseStatusScroll{Event::MousePos{_lastMouseX, _lastMouseY}, event.mouseWheelScroll.delta});
        default:
            return getEvent();
    }
}

Event SFMLDisplay::getEventKeyBoard(sf::Event &e, Event::KeyStatus isDown)
{
    auto it = keyboardMap.find(e.key.code);
    if (it != keyboardMap.end()) {
        return Event(it->second, std::any(isDown));
    }
    return getEvent();
}

Event SFMLDisplay::getEventMouse(sf::Event &e, Event::KeyStatus isDown)
{
    int unitX = e.mouseButton.x / UNIT_TO_PIXEL;
    int unitY = e.mouseButton.y / UNIT_TO_PIXEL;

    auto it = mouseMap.find(e.mouseButton.button);
    if (it != mouseMap.end()) {
        return Event(it->second, std::any(Event::MouseStatusClick{Event::MousePos{unitX, unitY}, isDown}));
    }
    return getEvent();
}

void SFMLDisplay::handleSound(const Sound &sound)
{
    if (sound.state == Sound::STOP) {
        try {
            _musics.at(sound.id).first->stop();
        } catch (const std::exception& e) {}
        return;
    }

    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromFile(sound.filePath))
        return;

    auto sfSound = std::make_shared<sf::Sound>();
    sfSound->setBuffer(*buffer);

    if (sound.state == Sound::LOOP)
        sfSound->setLoop(true);
    sfSound->play();
    _musics[sound.id] = std::make_pair(sfSound, buffer);
}

void SFMLDisplay::drawText(const Text &txt)
{
    std::shared_ptr<sf::Font> font;

    if (_fonts.find(txt.getFontPath()) == _fonts.end()) {
        font = std::make_shared<sf::Font>();
        if (!font->loadFromFile(txt.getFontPath()))
            return;
        _fonts[txt.getFontPath()] = font;
    } else {
        font = _fonts[txt.getFontPath()];
    }
    sf::Text text;
    text.setFont(*font);
    text.setString(txt.getStr());
    text.setCharacterSize(txt.getScale().first);
    text.setFillColor(sf::Color(
        std::get<0>(txt.getGUI_Color()),
        std::get<1>(txt.getGUI_Color()),
        std::get<2>(txt.getGUI_Color()),
        std::get<3>(txt.getGUI_Color())
    ));
    text.setPosition(txt.getPosition().first * UNIT_TO_PIXEL, txt.getPosition().second * UNIT_TO_PIXEL);
    text.setRotation(txt.getRotation());
    _window->draw(text);
}

void SFMLDisplay::drawSquare(const Sprite &sprite)
{
    sf::RectangleShape square(sf::Vector2f(sprite.getScale().first * UNIT_TO_PIXEL, sprite.getScale().second * UNIT_TO_PIXEL));
    square.setPosition(
        (sprite.getPosition().first)  * UNIT_TO_PIXEL,
        (sprite.getPosition().second)  * UNIT_TO_PIXEL
    );
    square.setRotation(sprite.getRotation());
    square.setFillColor(sf::Color(
        std::get<0>(sprite.getGUI_Color()),
        std::get<1>(sprite.getGUI_Color()),
        std::get<2>(sprite.getGUI_Color()),
        std::get<3>(sprite.getGUI_Color())
    ));
    _window->draw(square);
}

void SFMLDisplay::drawSprite(const Sprite &sprite)
{
    if (sprite.getGUI_Textures().empty()){
        drawSquare(sprite);
        return;
    }

    std::string texturePath = sprite.getGUI_Textures()[0];
    std::shared_ptr<sf::Texture> texture;

    if (_textures.find(texturePath) == _textures.end()) {
        texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(texturePath))
            return;
        _textures[texturePath] = texture;
    } else {
        texture = _textures[texturePath];
    }
    sf::Sprite sfSprite;
    sfSprite.setTexture(*texture);
    sfSprite.setPosition(
        (sprite.getPosition().first + 0.5f)  * UNIT_TO_PIXEL,
        (sprite.getPosition().second + 0.5f)  * UNIT_TO_PIXEL
    );
    sfSprite.setScale(sprite.getScale().first, sprite.getScale().second);
    sfSprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
    sfSprite.setRotation(sprite.getRotation());
    sfSprite.setColor(sf::Color(
        std::get<0>(sprite.getGUI_Color()),
        std::get<1>(sprite.getGUI_Color()),
        std::get<2>(sprite.getGUI_Color()),
        std::get<3>(sprite.getGUI_Color())
    ));

    _window->draw(sfSprite);
}

SFMLDisplay::~SFMLDisplay()
{
    _musics.clear();
    _textures.clear();
    _fonts.clear();
}