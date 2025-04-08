/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MineSweepGame
*/

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

    bool update(float deltaTime) override;
    const Window &getWindow(void) override;
    const std::vector<std::unique_ptr<IDrawable>> &getDrawables(void) override;
    const std::vector<Sound> &getSound(void) override;
    bool event(const Event &evt) override;
    std::vector<std::pair<std::string, int>> getScores(void) override;

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
        bool exploded = false;
    };

    void initializeBoard();
    void placeMines();
    void calculateAdjacentMines();
    void revealCell(int x, int y);
    void toggleFlag(int x, int y);
    bool checkWin();
    void revealAllMines();

    void ensureFirstCellIsSafe(int x, int y);
    void updateTimer(float deltaTime);
    void updateScore(int newlyRevealedCells);
    void displayMinesCounter();

    int boardWidth;
    int boardHeight;
    int numMines;
    int cellSize;
    float timeLimit;

    bool gameOver;
    bool playerWon;
    std::vector<std::vector<Cell>> board;
    int revealedCount;
    int flaggedCount;
    float gameTime;
    int score;
    bool firstMove;

    std::vector<std::unique_ptr<IDrawable>> drawables;
    std::vector<Sound> sounds;

    std::string playerName;
    std::vector<std::pair<std::string, int>> scoreHistory;

    Window window;

    static const int CELL_UNIT_SIZE = 1;
    static const int BOARD_MARGIN_X = 1;
    static const int BOARD_MARGIN_Y = 2;
};
