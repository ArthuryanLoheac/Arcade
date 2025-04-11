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
        {'a', Key::KeyCode::KEY_A}, {'b', Key::KeyCode::KEY_B}, {'c', Key::KeyCode::KEY_C},
        {'d', Key::KeyCode::KEY_D}, {'e', Key::KeyCode::KEY_E}, {'f', Key::KeyCode::KEY_F},
        {'g', Key::KeyCode::KEY_G}, {'h', Key::KeyCode::KEY_H}, {'i', Key::KeyCode::KEY_I},
        {'j', Key::KeyCode::KEY_J}, {'k', Key::KeyCode::KEY_K}, {'l', Key::KeyCode::KEY_L},
        {'m', Key::KeyCode::KEY_M}, {'n', Key::KeyCode::KEY_N}, {'o', Key::KeyCode::KEY_O},
        {'p', Key::KeyCode::KEY_P}, {'q', Key::KeyCode::KEY_Q}, {'r', Key::KeyCode::KEY_R},
        {'s', Key::KeyCode::KEY_S}, {'t', Key::KeyCode::KEY_T}, {'u', Key::KeyCode::KEY_U},
        {'v', Key::KeyCode::KEY_V}, {'w', Key::KeyCode::KEY_W}, {'x', Key::KeyCode::KEY_X},
        {'y', Key::KeyCode::KEY_Y}, {'z', Key::KeyCode::KEY_Z},

        {KEY_F(1), Key::KeyCode::FUNCTION_1}, {KEY_F(2), Key::KeyCode::FUNCTION_2}, {KEY_F(3), Key::KeyCode::FUNCTION_3},
        {KEY_F(4), Key::KeyCode::FUNCTION_4}, {KEY_F(5), Key::KeyCode::FUNCTION_5}, {KEY_F(6), Key::KeyCode::FUNCTION_6},
        {KEY_F(7), Key::KeyCode::FUNCTION_7}, {KEY_F(8), Key::KeyCode::FUNCTION_8}, {KEY_F(9), Key::KeyCode::FUNCTION_9},
        {KEY_F(10), Key::KeyCode::FUNCTION_10}, {KEY_F(11), Key::KeyCode::FUNCTION_11}, {KEY_F(12), Key::KeyCode::FUNCTION_12},

        {KEY_LEFT, Key::KeyCode::LEFT}, {KEY_UP, Key::KeyCode::UP}, {KEY_RIGHT, Key::KeyCode::RIGHT}, {KEY_DOWN, Key::KeyCode::DOWN},

        {' ', Key::KeyCode::SPACE}, {'\n', Key::KeyCode::ENTER}, {'\t', Key::KeyCode::TAB},
        {27, Key::KeyCode::ECHAP}, {KEY_DC, Key::KeyCode::SUPPR},
    };
};
