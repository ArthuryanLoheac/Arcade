/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#include "SDLDisplay.hpp"

std::unique_ptr<IDisplayModule> getDisplayModule()
{
    return std::make_unique<SDLDisplay>();
}