/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#pragma once
#include "IDisplayModule.hpp"
#include "Window.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <map>

class SDLDisplay : public IDisplayModule
{
public:
    void createWindow(const Window &window) override;
    void clear(void) override;
    void draw(const IDrawable &to_draw) override;
    void display(void) override;
    Event getEvent(void) override;
    void handleSound(const Sound &sound) override;
    ~SDLDisplay();
private:
    Event getEventKeyBoard(SDL_Event &e, Event::KeyStatus isDown);
    Event getEventMouse(SDL_Event &e, Event::KeyStatus isDown);

    void drawText(const Text &txt);
    void drawSprite(const Sprite &sprite);
    
    typedef struct {
        std::shared_ptr<SDL_Renderer> renderer;
        std::shared_ptr<SDL_Window> window;
    } App;

    std::map<std::string, std::pair<std::shared_ptr<Mix_Chunk>, int>> musics;
    std::map<std::string, std::shared_ptr<TTF_Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL_Texture>> textTextures;

    int LastMouseX, LastMouseY;
    App app;
};
