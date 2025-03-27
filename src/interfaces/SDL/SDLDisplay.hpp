/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#pragma once
#include "IDisplayModule.hpp"
#include "Window.hpp"
#include <memory>
#include <SDL2/SDL.h>

std::unique_ptr<IDisplayModule> getDisplayModule();

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
    
    typedef struct {
        std::shared_ptr<SDL_Renderer> renderer;
        std::shared_ptr<SDL_Window> window;
    } App;

    int LastMouseX, LastMouseY;
    App app;
};

