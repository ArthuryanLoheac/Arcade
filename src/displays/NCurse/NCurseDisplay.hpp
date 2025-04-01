/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurseDisplay
*/

#pragma once
#include <iostream>

#include "IDisplayModule.hpp"
#include "NCurseWrapper.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

class NCurseDisplay : public IDisplayModule {
public:
    NCurseDisplay();
    ~NCurseDisplay() override;

    void createWindow(const Window &window) override;
    void draw(const IDrawable &to_draw) override;
    void display(void) override;
    void clear(void) override;
    Event getEvent(void) override;
    void handleSound(const Sound &sound) override;

private:
    std::unique_ptr<NCurseWrapper> _window;
};
