/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SFML2
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class SFML2
{
public:
    static int SFML2_Init(void);
    static void SFML2_Quit(void);

    static std::shared_ptr<sf::RenderWindow> SFML2_CreateWindow(const char *title, int w, int h);
    static void SFML2_SetIcon(sf::RenderWindow *window, sf::Image *icon);
    static void SFML2_Display(sf::RenderWindow *window);
    static void SFML2_Clear(sf::RenderWindow *window, sf::Color color);
    static bool SFML2_PollEvent(sf::RenderWindow *window, sf::Event *event);

    static std::shared_ptr<sf::Texture> SFML2_LoadTexture(const char *file);
    static void SFML2_DrawSprite(sf::RenderWindow *window, sf::Sprite *sprite);

    static std::shared_ptr<sf::Font> SFML2_LoadFont(const char *file);
    static void SFML2_DrawText(sf::RenderWindow *window, sf::Text *text);

    static std::shared_ptr<sf::SoundBuffer> SFML2_LoadSoundBuffer(const char *file);
    static std::shared_ptr<sf::Image> SFML2_LoadImage(const char *path);
};