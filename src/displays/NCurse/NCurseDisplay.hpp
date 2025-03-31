/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurseDisplay
*/

#pragma once

#include "IDisplayModule.hpp"
#include "NCurseWrapper.hpp"

class NCurseDisplay : public IDisplayModule {
public:
    NCurseDisplay(int height, int width);
    ~NCurseDisplay();

    void draw(IDrawable drawable) override;
    void display() override;
    void clear() override;
    Event getEvent() override;
    void handleSound(Sound sound) override;

private:
    NCurseWrapper *wrapper;
}
