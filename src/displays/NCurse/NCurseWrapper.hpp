/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcurseWrapper
*/

#pragma once
#include <ncurses.h>
#include <memory>
#include <string>

class NCurseWrapper {
 public:
    NCurseWrapper(int height, int width);
    ~NCurseWrapper();

    void clear();
    void refresh();
    void print(int x, int y, const std::string &str);
    void color(std::pair<int, int> color);

 private:
    WINDOW *window;
};
