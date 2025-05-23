/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <map>
#include <string>
#include <utility>

#include "interfaces/IDisplayModule.hpp"
#include "core/Window.hpp"
#include "interfaces/Text.hpp"
#include "interfaces/Sprite.hpp"


class SDLDisplay : public IDisplayModule {
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
    void drawSquare(const Sprite &sprite);

    typedef struct {
        std::shared_ptr<SDL_Renderer> renderer;
        std::shared_ptr<SDL_Window> window;
    } App;

    std::map<std::string, std::pair<std::shared_ptr<Mix_Chunk>, int>> musics;
    std::map<std::string, std::shared_ptr<TTF_Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL_Texture>> textTextures;
    std::map<std::string, std::shared_ptr<SDL_Texture>> textures;

    int LastMouseX, LastMouseY;
    App app;
    int UNIT_TO_PIXEL = 20;
};
