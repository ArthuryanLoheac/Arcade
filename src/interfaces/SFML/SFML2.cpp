/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SFML2
*/

#include "SFML2.hpp"

extern "C" {
    int SFML2::SFML2_Init(void)
    {
        return 0;
    }

    void SFML2::SFML2_Quit(void)
    {
    }

    std::shared_ptr<sf::RenderWindow> SFML2::SFML2_CreateWindow(const char *title, int w, int h)
    {
        sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(w, h), title);
        if (!window)
            return nullptr;
        window->setFramerateLimit(60);
        return std::shared_ptr<sf::RenderWindow>(window, [](sf::RenderWindow *w) { w->close(); delete w; });
    }

    std::shared_ptr<sf::Image> SFML2::SFML2_LoadImage(const char *path)
    {
        sf::Image *image = new sf::Image();
        if (!image->loadFromFile(path)) {
            delete image;
            return nullptr;
        }
        return std::shared_ptr<sf::Image>(image, [](sf::Image *i) { delete i; });
    }

    void SFML2::SFML2_SetIcon(sf::RenderWindow *window, sf::Image *icon)
    {
        window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
    }

    void SFML2::SFML2_Display(sf::RenderWindow *window)
    {
        window->display();
    }

    void SFML2::SFML2_Clear(sf::RenderWindow *window, sf::Color color)
    {
        window->clear(color);
    }

    bool SFML2::SFML2_PollEvent(sf::RenderWindow *window, sf::Event *event)
    {
        return window->pollEvent(*event);
    }

    std::shared_ptr<sf::Texture> SFML2::SFML2_LoadTexture(const char *file)
    {
        sf::Texture *texture = new sf::Texture();
        if (!texture->loadFromFile(file)) {
            delete texture;
            return nullptr;
        }
        return std::shared_ptr<sf::Texture>(texture, [](sf::Texture *t) { delete t; });
    }

    void SFML2::SFML2_DrawSprite(sf::RenderWindow *window, sf::Sprite *sprite)
    {
        window->draw(*sprite);
    }

    std::shared_ptr<sf::Font> SFML2::SFML2_LoadFont(const char *file)
    {
        sf::Font *font = new sf::Font();
        if (!font->loadFromFile(file)) {
            delete font;
            return nullptr;
        }
        return std::shared_ptr<sf::Font>(font, [](sf::Font *f) { delete f; });
    }

    void SFML2::SFML2_DrawText(sf::RenderWindow *window, sf::Text *text)
    {
        window->draw(*text);
    }

    std::shared_ptr<sf::SoundBuffer> SFML2::SFML2_LoadSoundBuffer(const char *file)
    {
        sf::SoundBuffer *buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile(file)) {
            delete buffer;
            return nullptr;
        }
        return std::shared_ptr<sf::SoundBuffer>(buffer, [](sf::SoundBuffer *b) { delete b; });
    }
}