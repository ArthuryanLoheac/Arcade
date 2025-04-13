/*
** EPITECH PROJECT, 2025
** Arcade-Headers
** File description:
** IDisplayModule
*/

#pragma once
#include <memory>
#include <string>

#include "interfaces/IDrawable.hpp"
#include "core/Event.hpp"
#include "core/Sound.hpp"
#include "core/Window.hpp"

class IDisplayModule {
 public:
    virtual ~IDisplayModule() = default;
    virtual void createWindow(const Window &window) = 0;
    virtual void draw(const IDrawable &to_draw) = 0;
    virtual void display(void) = 0;
    virtual void clear(void) = 0;
    virtual Event getEvent(void) = 0;
    virtual void handleSound(const Sound &sound) = 0;
};

// extern "C" std::unique_ptr<IDisplayModule> getDisplayModule();
