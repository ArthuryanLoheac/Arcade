/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** PacManGame
*/

#include "PacManGame.hpp"
#include "interfaces/Sprite.hpp"
#include "interfaces/Text.hpp"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>

extern "C" std::unique_ptr<IGameModule> getGameModule()
{
    return std::make_unique<PacManGame>();
}

PacManGame::PacManGame()
    : window({15, 15}, "PacMan", "assets/PacMan/Pacman.png"),
        player(1, 1)
{
    InitScore();
    player.timeLeftToMove = timeToMove;
}

void PacManGame::InitScore()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

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

bool PacManGame::update(float deltaTime)
{
    sounds.clear();
    drawables.clear();
    updateWalls();
    if (!gameOver) {
        updateGhost(deltaTime);
        updatePosPlayer(deltaTime);
        updateCollisions();
    }
    updateText();
    AddDrawable(player.x, player.y, "assets/PacMan/Pacman.png", "C ", .25f, player.dir * 90.f);
    AddDrawable(f1.x, f1.y, "assets/PacMan/Orange.png", "F ", .25f, 0.f);
    return false;
}

void PacManGame::updateCollisions(void)
{
    if (player.x == f1.x && player.y == f1.y) {
        gameOver = true;
        playerWon = false;
    }
}

void PacManGame::updateGhost(float deltaTime)
{
    f1.timeLeftToMove -= deltaTime;
    if (f1.timeLeftToMove <= 0.f) {
        f1.timeLeftToMove = timeToMove;
        int PrevX = f1.x;
        int PrevY = f1.y;
        moveGhost(PrevX, PrevY);
    }
}

void PacManGame::moveGhost(int PrevX, int PrevY)
{
    int direction = std::rand() % 4;
    if (direction == 0)
        f1.x += 1;
    else if (direction == 1)
        f1.x -= 1;
    else if (direction == 2)
        f1.y -= 1;
    else if (direction == 3)
        f1.y += 1;
    // Min Max
    if (f1.x < 0)
        f1.x = 0;
    if (f1.y < 0)
        f1.y = 0;
    if (f1.x >= window.size.first)
        f1.x = window.size.first - 1;
    if (f1.y >= window.size.second)
        f1.y = window.size.second - 1;
    // Check if the player is on a wall
    if (map[f1.y][f1.x] == 1) {
        f1.x = PrevX;
        f1.y = PrevY;
        moveGhost(PrevX, PrevY);
    }
}

void PacManGame::updateText(void)
{
    auto statusText = std::make_unique<Text>();
    statusText->setPosition({0, 0});
    statusText->setScale({30, 30});
    statusText->setGUI_Color({255, 255, 255, 255});
    statusText->setFontPath("assets/fonts/NotoSans.ttf");
    std::stringstream ss;
    ss << "Score: " << score;
    if (gameOver) {
        if (playerWon)
            ss << " You win!";
        else
            ss << " You lose!";
    }
    statusText->setStr(ss.str());
    drawables.push_back(std::move(statusText));
}

void PacManGame::updateWalls(void)
{
    bool isEndPacGome = true;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 1)
                AddDrawable(j, i, "assets/PacMan/Wall.jpg", "##", .165f, 0.f);
            else if (map[i][j] == 0){
                AddDrawable(j, i, "assets/PacMan/LittlePacGome.png", ".", .25f, 0.f);
                isEndPacGome = false;
            }
        }
    }
    if (isEndPacGome) {
        gameOver = true;
        playerWon = true;
    }
}


void PacManGame::updatePosPlayer(float deltaTime)
{
    player.timeLeftToMove -= deltaTime;
    if (player.timeLeftToMove <= 0.f) {
        player.timeLeftToMove = timeToMove;
        int PrevX = player.x;
        int PrevY = player.y;
        if (player.dir == PacMan::RIGHT)
            player.x += 1;
        else if (player.dir == PacMan::LEFT)
            player.x -= 1;
        else if (player.dir == PacMan::UP)
            player.y -= 1;
        else if (player.dir == PacMan::DOWN)
            player.y += 1;

        // Min Max
        if (player.x < 0)
            player.x = 0;
        if (player.y < 0)
            player.y = 0;
        if (player.x >= window.size.first)
            player.x = window.size.first - 1;
        if (player.y >= window.size.second)
            player.y = window.size.second - 1;
        
        // Check if the player is on a wall
        if (map[player.y][player.x] == 1) {
            player.x = PrevX;
            player.y = PrevY;
        }
        if (map[player.y][player.x] == 0) {
            map[player.y][player.x] = 2;
            score += 10;
        }
    }
}

const Window &PacManGame::getWindow(void)
{
    return window;
}

const std::vector<std::unique_ptr<IDrawable>> &PacManGame::getDrawables(void)
{
    return drawables;
}

const std::vector<Sound> &PacManGame::getSound(void)
{
    return sounds;
}

bool PacManGame::event(const Event &evt)
{
    if (evt.key == Key::KeyCode::RIGHT || evt.key == Key::KeyCode::KEY_D) {
        player.dir = PacMan::RIGHT;
    } else if (evt.key == Key::KeyCode::LEFT || evt.key == Key::KeyCode::KEY_Q) {
        player.dir = PacMan::LEFT;
    } else if (evt.key == Key::KeyCode::UP || evt.key == Key::KeyCode::KEY_Z) {
        player.dir = PacMan::UP;
    } else if (evt.key == Key::KeyCode::DOWN || evt.key == Key::KeyCode::KEY_D) {
        player.dir = PacMan::DOWN;
    }
    return false;
}

std::vector<std::pair<std::string, int>> PacManGame::getScores(void)
{
    return scoreHistory;
}


void PacManGame::AddDrawable(int x, int y, std::string texturePath,
    std::string cliTexture, float scale, float rotation, std::tuple<int, int, int, int> GUI_Color)
{
    auto sprite = std::make_unique<Sprite>();
    sprite->setPosition({x, y + 2});
    sprite->setScale({scale, scale });
    sprite->setGUI_Color({255, 255, 255, 255});
    sprite->setGUI_Textures({texturePath});
    sprite->setCLI_Textures({cliTexture});
    sprite->setRotation(rotation);
    drawables.push_back(std::move(sprite));
}
