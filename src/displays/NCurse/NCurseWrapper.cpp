/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NCurseWrapper
*/

#include "NCurseWrapper.hpp"

NCurseWrapper::NCurseWrapper(int height, int width)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    window = newwin(height, width, 0, 0);
    refresh();
}

NCurseWrapper::~NCurseWrapper()
{
    delwin(window);
    endwin();
}

void NCurseWrapper::clear()
{
    ::clear();
}

void NCurseWrapper::refresh()
{
    ::refresh();
}

void NCurseWrapper::print(int x, int y, const std::string &str)
{
    mvprintw(y, x, "%s", str.c_str());
    attroff(COLOR_PAIR(1));
}

void NCurseWrapper::color(std::pair<int, int> color)
{
    init_pair(1, color.first, color.second);
    attron(COLOR_PAIR(1));
}
