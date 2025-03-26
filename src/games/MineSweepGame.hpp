/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MineSweepGame
*/

#ifndef MINESWEEPGAME_HPP_
    #define MINESWEEPGAME_HPP_

    #pragma once

    #include "interfaces/IGameModule.hpp"
    #include <vector>
    #include <random>
    #include <memory>
    #include "core/Window.hpp"
    #include "interfaces/IDrawable.hpp"
    #include "core/Sound.hpp"
    #include "core/Event.hpp"

class MineSweepGame : public IGameModule {
public:
    MineSweepGame();

    void start(void) override;
    void end(void) override;
    void update(float deltaTime) override;
    Window getWindow(void) override;
    std::vector<IDrawable> getDrawables(void) override;
    std::vector<Sound> getSound(void) override;
    void event(Event evt) override;

private:
    enum class CellState {
        HIDDEN,
        REVEALED,
        FLAGGED
    };

    struct Cell {
        bool hasMine;
        CellState state;
        int adjacentMines;
    };

    void initializeBoard();
    void placeMines();
    void calculateAdjacentMines();
    void revealCell(int x, int y);
    void toggleFlag(int x, int y);
    bool checkWin();
    void revealAllMines();

    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 10;
    static constexpr int NUM_MINES = 15;
    static constexpr int CELL_SIZE = 32;
    bool gameOver;
    bool playerWon;
    std::vector<std::vector<Cell>> board;
    int revealedCount;
    int flaggedCount;

    std::vector<IDrawable> drawables;
    std::vector<Sound> sounds;

    Window window;
};

#endif /* !MINESWEEPGAME_HPP_ */