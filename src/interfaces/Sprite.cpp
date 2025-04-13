#include <string>
#include <vector>
#include <utility>
#include <tuple>

#include "interfaces/Sprite.hpp"

std::vector<std::string> Sprite::getGUI_Textures(void) const {
    return GUI_Textures;
}

const std::vector<std::string> &Sprite::getCLI_Textures() const {
    return CLI_Textures;
}

float Sprite::getAnimationTime(void) const {
    return animationTime;
}

unsigned int Sprite::getCurrentTexture(void) const {
    return currentTexture;
}

void Sprite::setGUI_Textures(std::vector<std::string> GUI_Textures) {
    this->GUI_Textures = GUI_Textures;
}

void Sprite::setCLI_Textures(std::vector<std::string> CLI_Textures) {
    this->CLI_Textures = CLI_Textures;
}

void Sprite::setAnimationTime(float animationTime) {
    this->animationTime = animationTime;
}

void Sprite::setCurrentTexture(unsigned int currentTexture) {
    this->currentTexture = currentTexture;
}

std::pair<float, float> Sprite::getScale(void) const {
    return scale;
}

float Sprite::getRotation(void) const {
    return rotation;
}

std::pair<CLI_Color, CLI_Color> Sprite::getCLI_Color(void) const {
    return CLI_color;
}

std::tuple<int, int, int, int> Sprite::getGUI_Color(void) const {
    return GUI_color;
}

std::pair<int, int> Sprite::getPosition(void) const {
    return position;
}

void Sprite::setScale(std::pair<float, float> scale) {
    this->scale = scale;
}

void Sprite::setCLI_Color(std::pair<CLI_Color, CLI_Color> CLI_Color) {
    this->CLI_color = CLI_Color;
}

void Sprite::setGUI_Color(std::tuple<int, int, int, int> GUI_Color) {
    this->GUI_color = GUI_Color;
}

void Sprite::setPosition(std::pair<int, int> position) {
    this->position = position;
}

void Sprite::setRotation(float rotation) {
    this->rotation = rotation;
}
