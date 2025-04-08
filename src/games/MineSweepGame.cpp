/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MineSweepGame
*/

#include "MineSweepGame.hpp"
#include "interfaces/Sprite.hpp"
#include "interfaces/Text.hpp"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>

extern "C" std::unique_ptr<IGameModule> getGameModule()
{
    return std::make_unique<MineSweepGame>();
}

MineSweepGame::MineSweepGame()
    : boardWidth(10), boardHeight(10), numMines(15), cellSize(32),
      timeLimit(300.0f), gameOver(false), playerWon(false), revealedCount(0),
      flaggedCount(0), gameTime(0.0f), score(0), firstMove(true),
      window({BOARD_MARGIN_X * 2 + 10 * CELL_UNIT_SIZE,
              BOARD_MARGIN_Y * 2 + 10 * CELL_UNIT_SIZE + 3},
             "Minesweeper", "")
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    initializeBoard();
    try {
        std::ifstream scoreFile("minesweeper_scores.txt");
        if (scoreFile.is_open()) {
            std::string name;
            int playerScore;
            while (scoreFile >> name >> playerScore) {
                scoreHistory.push_back({name, playerScore});
            }
            scoreFile.close();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading scores: " << e.what() << std::endl;
    }
    std::sort(scoreHistory.begin(), scoreHistory.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    playerName = "Player";
}

bool MineSweepGame::update(float deltaTime)
{
    if (!gameOver)
        updateTimer(deltaTime);
    drawables.clear();
    sounds.clear();
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            const Cell& cell = board[y][x];
            auto sprite = std::make_unique<Sprite>();
            sprite->setPosition({
                BOARD_MARGIN_X + x * CELL_UNIT_SIZE,
                BOARD_MARGIN_Y + y * CELL_UNIT_SIZE
            });
            sprite->setScale({1.0f, 1.0f});
            sprite->setGUI_Color({255, 255, 255, 255});
            std::string texturePath;
            std::string cliTexture = "[]";
            if (cell.state == CellState::HIDDEN) {
                texturePath = "assets/MineSweeper/caché.png";
                cliTexture = "##";
            } else if (cell.state == CellState::FLAGGED) {
                texturePath = "assets/MineSweeper/drapeau.png";
                cliTexture = "!F";
            } else if (cell.hasMine) {
                if (cell.exploded) {
                    texturePath = "assets/MineSweeper/bombe_rouge.png";
                } else {
                    texturePath = "assets/MineSweeper/bombe.png";
                }
                cliTexture = "**";
            } else {
                if (cell.adjacentMines >= 1 && cell.adjacentMines <= 8) {
                    texturePath = "assets/MineSweeper/" + std::to_string(cell.adjacentMines) + ".png";
                } else {
                    texturePath = "assets/MineSweeper/découvert.png";
                }
                cliTexture = (cell.adjacentMines == 0) ? "  " : std::to_string(cell.adjacentMines) + " ";
            }
            sprite->setGUI_Textures({texturePath});
            sprite->setCLI_Textures({cliTexture});
            drawables.push_back(std::move(sprite));
        }
    }
    auto statusText = std::make_unique<Text>();
    statusText->setPosition({BOARD_MARGIN_X, 0});
    statusText->setScale({15, 15});
    statusText->setGUI_Color({255, 255, 255, 255});
    statusText->setFontPath("assets/fonts/NotoSans.ttf");
    std::stringstream ss;
    if (gameOver) {
        if (playerWon) {
            ss << "YOU WIN! Score: " << score;
        } else {
            ss << "GAME OVER! Press R to restart";
        }
    } else {
        int remainingMines = numMines - flaggedCount;
        int remainingTime = static_cast<int>(timeLimit - gameTime);
        ss << "Mines: " << remainingMines << " | Time: " << remainingTime << " | Score: " << score;
    }
    statusText->setStr(ss.str());
    drawables.push_back(std::move(statusText));
    auto helpText = std::make_unique<Text>();
    helpText->setPosition({BOARD_MARGIN_X, BOARD_MARGIN_Y + boardHeight + 1});
    helpText->setScale({12, 12});
    helpText->setGUI_Color({200, 200, 200, 255});
    helpText->setFontPath("assets/fonts/NotoSans.ttf");
    helpText->setStr("Left click: Reveal | Right click: Flag | R: Restart");
    drawables.push_back(std::move(helpText));
    if (gameTime >= timeLimit && !gameOver) {
        gameOver = true;
        playerWon = false;
        revealAllMines();
    }
    return false;
}

const Window &MineSweepGame::getWindow(void)
{
    return window;
}

const std::vector<std::unique_ptr<IDrawable>> &MineSweepGame::getDrawables(void)
{
    return drawables;
}

const std::vector<Sound> &MineSweepGame::getSound(void)
{
    return sounds;
}

bool MineSweepGame::event(const Event &evt)
{
    if (evt.key == Key::KeyCode::KEY_R) {
        try {
            auto status = std::any_cast<Event::KeyStatus>(evt.value);
            if (status == Event::KeyStatus::KEY_PRESSED) {
                gameOver = false;
                playerWon = false;
                revealedCount = 0;
                flaggedCount = 0;
                gameTime = 0.0f;
                score = 0;
                firstMove = true;
                initializeBoard();
                return false;
            }
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Error: Invalid event data" << std::endl;
        }
    }

    if (gameOver)
        return false;
    if (evt.key == Key::KeyCode::MOUSE_LEFT || evt.key == Key::KeyCode::MOUSE_RIGHT) {
        try {
            auto mouseClick = std::any_cast<Event::MouseStatusClick>(evt.value);
            int boardX = (mouseClick.pos.x - BOARD_MARGIN_X) / CELL_UNIT_SIZE;
            int boardY = (mouseClick.pos.y - BOARD_MARGIN_Y) / CELL_UNIT_SIZE;
            if (boardX >= 0 && boardX < boardWidth && boardY >= 0 && boardY < boardHeight) {
                if (mouseClick.status == Event::KeyStatus::KEY_PRESSED) {
                    if (evt.key == Key::KeyCode::MOUSE_LEFT) {
                        revealCell(boardX, boardY);
                    } else if (evt.key == Key::KeyCode::MOUSE_RIGHT) {
                        toggleFlag(boardX, boardY);
                    }
                }
            }
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Error: Invalid event data" << std::endl;
        }
    }
    return false;
}

std::vector<std::pair<std::string, int>> MineSweepGame::getScores(void)
{
    return scoreHistory;
}

void MineSweepGame::initializeBoard()
{
    board.resize(boardHeight);
    for (int y = 0; y < boardHeight; ++y) {
        board[y].resize(boardWidth);
        for (int x = 0; x < boardWidth; ++x) {
            board[y][x] = {false, CellState::HIDDEN, 0};
        }
    }
    placeMines();
    calculateAdjacentMines();
}

void MineSweepGame::placeMines()
{
    int minesPlaced = 0;

    while (minesPlaced < numMines) {
        int x = std::rand() % boardWidth;
        int y = std::rand() % boardHeight;
        if (!board[y][x].hasMine) {
            board[y][x].hasMine = true;
            minesPlaced++;
        }
    }
}

void MineSweepGame::calculateAdjacentMines()
{
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            if (board[y][x].hasMine) {
                continue;
            }
            int mineCount = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < boardWidth && ny >= 0 && ny < boardHeight) {
                        if (board[ny][nx].hasMine) {
                            mineCount++;
                        }
                    }
                }
            }
            board[y][x].adjacentMines = mineCount;
        }
    }
}

void MineSweepGame::revealCell(int x, int y)
{
    if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight ||
        board[y][x].state == CellState::REVEALED ||
        board[y][x].state == CellState::FLAGGED) {
        return;
    }

    if (firstMove) {
        ensureFirstCellIsSafe(x, y);
        firstMove = false;
    }
    if (board[y][x].hasMine) {
        board[y][x].state = CellState::REVEALED;
        board[y][x].exploded = true;
        gameOver = true;
        playerWon = false;
        revealAllMines();
        return;
    }
    board[y][x].state = CellState::REVEALED;
    revealedCount++;
    updateScore(1);

    if (board[y][x].adjacentMines == 0) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                revealCell(x + dx, y + dy);
            }
        }
    }
    if (checkWin()) {
        gameOver = true;
        playerWon = true;
        int timeBonus = static_cast<int>((timeLimit - gameTime) * 10);
        score += timeBonus;
        scoreHistory.push_back({playerName, score});
        std::sort(scoreHistory.begin(), scoreHistory.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        if (scoreHistory.size() > 10) {
            scoreHistory.resize(10);
        }
        try {
            std::ofstream scoreFile("minesweeper_scores.txt");
            for (const auto& [name, playerScore] : scoreHistory) {
                scoreFile << name << " " << playerScore << std::endl;
            }
            scoreFile.close();
        } catch (const std::exception& e) {
            std::cerr << "Error saving scores: " << e.what() << std::endl;
        }
    }
}

void MineSweepGame::toggleFlag(int x, int y)
{
    if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight ||
        board[y][x].state == CellState::REVEALED) {
        return;
    }
    if (board[y][x].state == CellState::HIDDEN) {
        board[y][x].state = CellState::FLAGGED;
        flaggedCount++;
    } else if (board[y][x].state == CellState::FLAGGED) {
        board[y][x].state = CellState::HIDDEN;
        flaggedCount--;
    }
}

bool MineSweepGame::checkWin()
{
    return revealedCount == (boardWidth * boardHeight - numMines);
}

void MineSweepGame::revealAllMines()
{
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            if (board[y][x].hasMine) {
                if (board[y][x].exploded) {
                } else if (board[y][x].state != CellState::FLAGGED) {
                    board[y][x].state = CellState::REVEALED;
                }
            } else if (board[y][x].state == CellState::FLAGGED) {
            }
        }
    }
}

void MineSweepGame::ensureFirstCellIsSafe(int x, int y)
{
    if (board[y][x].hasMine) {
        board[y][x].hasMine = false;

        bool minePlaced = false;
        while (!minePlaced) {
            int nx = std::rand() % boardWidth;
            int ny = std::rand() % boardHeight;
            if ((nx != x || ny != y) && !board[ny][nx].hasMine) {
                board[ny][nx].hasMine = true;
                minePlaced = true;
            }
        }
        calculateAdjacentMines();
    }
}

void MineSweepGame::updateTimer(float deltaTime)
{
    gameTime += deltaTime / 1000.0f;
}

void MineSweepGame::updateScore(int newlyRevealedCells)
{
    score += newlyRevealedCells * 10;
    if (gameTime > 0) {
        float timeBonus = 1.0f + (timeLimit / gameTime) * 0.1f;
        score += static_cast<int>(newlyRevealedCells * timeBonus);
    }
}
