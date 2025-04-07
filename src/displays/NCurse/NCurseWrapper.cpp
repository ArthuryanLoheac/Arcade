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
    raw();
    keypad(stdscr, TRUE);
    noecho();
    timeout(0);
    cbreak();
    curs_set(0);
    start_color();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    window = newwin(height, width, 0, 0);
    refresh();
}

NCurseWrapper::~NCurseWrapper()
{
    endwin();
}

void NCurseWrapper::clear()
{
    erase();
}

void NCurseWrapper::refresh()
{
    wrefresh(window);
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
