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
        player(0, 0)
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
    player.timeLeftToMove -= deltaTime;
    if (player.timeLeftToMove <= 0.f) {
        player.timeLeftToMove = timeToMove;
        if (player.dir == PacMan::RIGHT) {
            player.x += 1;
        } else if (player.dir == PacMan::LEFT) {
            player.x -= 1;
        } else if (player.dir == PacMan::UP) {
            player.y -= 1;
        } else if (player.dir == PacMan::DOWN) {
            player.y += 1;
        }
        // Min Max
        if (player.x < 0)
            player.x = 0;
        else if (player.x >= window.size.first)
            player.x = window.size.first - 1;
        if (player.y < 0)
            player.y = 0;
        else if (player.y >= window.size.second)
            player.y = window.size.second - 1;
    }
    AddDrawable(player.x, player.y, "assets/PacMan/Pacman.png", "C ", .25f, player.dir * 90.f);
    return false;
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
    if (gameOver)
        return false;
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
    sprite->setPosition({x, y});
    sprite->setScale({scale, scale});
    sprite->setGUI_Color({255, 255, 255, 255});
    sprite->setGUI_Textures({texturePath});
    sprite->setCLI_Textures({cliTexture});
    sprite->setRotation(rotation);
    drawables.push_back(std::move(sprite));
}
