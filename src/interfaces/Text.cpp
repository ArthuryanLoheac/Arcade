#include "Text.hpp"

Text::Text(void)
{
}

std::string Text::getFontPath(void) const
{
    return fontPath;
}

std::string Text::getStr(void) const
{
    return str;
}

void Text::setFontPath(std::string fontPath)
{
    this->fontPath = fontPath;
}

void Text::setStr(std::string str)
{
    this->str = str;
}

std::pair<float, float> Text::getScale(void) const
{
    return scale;
}

float Text::getRotation(void) const
{
    return rotation;
}

std::pair<CLI_Color, CLI_Color> Text::getCLI_Color(void) const
{
    return CLI_color;
}

std::tuple<int, int, int, int> Text::getGUI_Color(void) const
{
    return GUI_color;
}

std::pair<int, int> Text::getPosition(void) const
{
    return position;
}

void Text::setScale(std::pair<float, float> scale)
{
    this->scale = scale;
}

void Text::setRotation(float rotation)
{
    this->rotation = rotation;
}

void Text::setCLI_Color(std::pair<CLI_Color, CLI_Color> CLI_Color)
{
    this->CLI_color = CLI_Color;
}

void Text::setGUI_Color(std::tuple<int, int, int, int> GUI_Color)
{
    this->GUI_color = GUI_Color;
}

void Text::setPosition(std::pair<int, int> position)
{
    this->position = position;
}
