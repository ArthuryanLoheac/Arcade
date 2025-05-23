/*
** EPITECH PROJECT, 2025
** Arcade-Headers
** File description:
** IDrawable
*/

#pragma once
#include <tuple>
#include <utility>

#include "arcadeCore/CLI_Colors.hpp"

class IDrawable {
 public:
    virtual std::pair<float, float> getScale(void) const = 0;
    virtual float getRotation(void) const = 0;
    virtual std::pair<CLI_Color, CLI_Color> getCLI_Color(void) const = 0;
    virtual std::tuple<int, int, int, int> getGUI_Color(void) const = 0;
    virtual std::pair<int, int> getPosition(void) const = 0;
    virtual void setScale(std::pair<float, float> scale) = 0;
    virtual void setRotation(float rotation) = 0;
    virtual void setCLI_Color(std::pair<CLI_Color, CLI_Color> CLI_Color) = 0;
    virtual void setGUI_Color(std::tuple<int, int, int, int> GUI_Color) = 0;
    virtual void setPosition(std::pair<int, int> position) = 0;
};
