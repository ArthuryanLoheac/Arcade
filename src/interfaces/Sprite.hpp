/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Sprite
*/

#pragma once
#include "IDrawable.hpp"
#include <vector>
#include <string>

class Sprite : public IDrawable {
public:
    std::vector<std::string> getGUI_Textures(void) const { return GUI_Textures; };
    const std::vector<std::string> &getCLI_Textures() const { return CLI_Textures; }
    float getAnimationTime(void) const { return animationTime; };
    void setGUI_Textures(std::vector<std::string> GUI_Textures) { this->GUI_Textures = GUI_Textures; };
    void setCLI_Textures(std::vector<std::string> CLI_Textures) { this->CLI_Textures = CLI_Textures; }
    void setAnimationTime(float animationTime) { this->animationTime = animationTime; };
    
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
    std::vector<std::string> GUI_Textures;
	std::vector<std::string> CLI_Textures;
	float animationTime;
    std::pair<float, float> scale;
	float rotation;
	std::pair<CLI_Color, CLI_Color> CLI_color;
	std::tuple<int, int, int, int> GUI_color;
	std::pair<int, int> position;
};