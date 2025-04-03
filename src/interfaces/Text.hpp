/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Text
*/

#pragma once
#include "IDrawable.hpp"
#include <vector>
#include <string>
#include <tuple>

class Text : public IDrawable
{
public:
    Text() = default;
    std::string getFontPath(void) const { return fontPath; };
    std::string getStr(void) const { return str; };
    void setFontPath(std::string fontPath) { this->fontPath = fontPath; };
    void setStr(std::string str) { this->str = str; };
    
    std::pair<float, float> getScale(void) const override { return scale; };
    float getRotation(void) const override { return rotation; };
    std::pair<CLI_Color, CLI_Color> getCLI_Color(void) const override { return CLI_color; };
    std::tuple<int, int, int, int> getGUI_Color(void) const override { return GUI_color; };
    std::pair<int, int> getPosition(void) const override { return position; };
    void setScale(std::pair<float, float> scale) override { this->scale = scale; };
    void setRotation(float rotation) override { this->rotation = rotation; };
    void setCLI_Color(std::pair<CLI_Color, CLI_Color> CLI_Color) override { this->CLI_color = CLI_Color; };
    void setGUI_Color(std::tuple<int, int, int, int> GUI_Color) override { this->GUI_color = GUI_Color; };
    void setPosition(std::pair<int, int> position) override { this->position = position; };
private:
    std::string fontPath;
	std::string str;

    std::pair<float, float> scale;
	float rotation;
	std::pair<CLI_Color, CLI_Color> CLI_color;
	std::tuple<int, int, int, int> GUI_color;
	std::pair<int, int> position;
};
