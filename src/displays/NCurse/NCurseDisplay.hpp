/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurseDisplay
*/

#pragma once
#include <iostream>
#include <map>

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
    Key::KeyCode getKeyBoardCode(int ch);
    Key::KeyCode getMouseCode(int &x, int &y);

    std::unique_ptr<NCurseWrapper> _window;
    const std::map<int, Key::KeyCode> _keyMap {
        {'a', Key::KEY_A}, {'b', Key::KEY_B}, {'c', Key::KEY_C},
        {'d', Key::KEY_D}, {'e', Key::KEY_E}, {'f', Key::KEY_F},
        {'g', Key::KEY_G}, {'h', Key::KEY_H}, {'i', Key::KEY_I},
        {'j', Key::KEY_J}, {'k', Key::KEY_K}, {'l', Key::KEY_L},
        {'m', Key::KEY_M}, {'n', Key::KEY_N}, {'o', Key::KEY_O},
        {'p', Key::KEY_P}, {'q', Key::KEY_Q}, {'r', Key::KEY_R},
        {'s', Key::KEY_S}, {'t', Key::KEY_T}, {'u', Key::KEY_U},
        {'v', Key::KEY_V}, {'w', Key::KEY_W}, {'x', Key::KEY_X},
        {'y', Key::KEY_Y}, {'z', Key::KEY_Z},

        {KEY_F(1), Key::FUNCTION_1}, {KEY_F(2), Key::FUNCTION_2}, {KEY_F(3), Key::FUNCTION_3},
        {KEY_F(4), Key::FUNCTION_4}, {KEY_F(5), Key::FUNCTION_5}, {KEY_F(6), Key::FUNCTION_6},
        {KEY_F(7), Key::FUNCTION_7}, {KEY_F(8), Key::FUNCTION_8}, {KEY_F(9), Key::FUNCTION_9},
        {KEY_F(10), Key::FUNCTION_10}, {KEY_F(11), Key::FUNCTION_11}, {KEY_F(12), Key::FUNCTION_12},

        {KEY_LEFT, Key::LEFT}, {KEY_UP, Key::UP}, {KEY_RIGHT, Key::RIGHT}, {KEY_DOWN, Key::DOWN},

        {' ', Key::SPACE}, {'\n', Key::ENTER}, {'\t', Key::TAB},
        {27, Key::ECHAP}, {KEY_DC, Key::SUPPR},
    };
};
