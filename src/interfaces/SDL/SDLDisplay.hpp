/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#pragma once
#include "IDisplayModule.hpp"
#include <memory>

std::unique_ptr<IDisplayModule> getDisplayModule();

class SDLDisplay : public IDisplayModule
{
public:
    void draw(IDrawable) override;
    void display(void) override;
    void clear(void) override;
    Event getEvent(void) override;
    void handleSound(Sound) override;
};

