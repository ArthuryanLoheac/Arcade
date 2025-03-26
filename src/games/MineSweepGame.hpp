/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MineSweepGame - Implémentation du jeu classique du Démineur
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
    MineSweepGame(
        int boardWidth = 10,
        int boardHeight = 10,
        int numMines = 15,
        int cellSize = 32,
        float timeLimit = 300.0f
    );

    void update(float deltaTime) override;
    const Window &getWindow(void) override;
    const std::vector<IDrawable> &getDrawables(void) override;
    const std::vector<Sound> &getSound(void) override;
    void event(const Event &events) override;

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

    std::vector<IDrawable> drawables;
    std::vector<Sound> sounds;

    Window window;
};
