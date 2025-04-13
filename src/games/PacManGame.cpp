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
    : window({30, 15}, "PacMan", "assets/PacMan/Pacman.png"),
        player(14, 8)
{
    Init();
    InitScore();
}

void PacManGame::Init()
{
    map.clear();
    player.x = 14;
    player.y = 8;
    map = Savemap;
    gameOver = false;
    playerWon = false;
    score = 0;
    invisbleTime = 0;
    scoreCombo = 200;
    ghosts.clear();
    ghosts.push_back(Fantome(14, 5));
    ghosts.push_back(Fantome(13, 5));
    ghosts.push_back(Fantome(15, 5));
    ghosts.push_back(Fantome(15, 6));
    player.timeLeftToMove = timeToMove;
    player.timeAnimLeft = timeAnim;
    player.isClose = false;
    map[player.y][player.x] = 2;
    map[4][13] = 1;
    map[4][14] = 1;
    map[4][15] = 1;
}

void PacManGame::InitScore()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        std::ifstream scoreFile("pacman_scores.txt");
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
    timeUntilOpen -= deltaTime;
    if (timeUntilOpen <= 0.f) {
        map[4][13] = 2;
        map[4][14] = 2;
        map[4][15] = 2;
    }
    updateWalls();
    if (!gameOver) {
        for (int i = 0; i < ghosts.size(); i++) {
            if (ghosts[i].isDead)
                continue;
            updateGhost(deltaTime, i);
        }
        updateCollisions();
        if (!gameOver)
            updatePosPlayer(deltaTime);
        if (invisbleTime > 0) {
            invisbleTime -= deltaTime;
            if (invisbleTime <= 0){
                invisbleTime = 0;
                respawnDeadGhost();
            }
        }
        updateCollisions();
    }
    updateText();
    AddDrawable(player.x, player.y,
        player.isClose ? "assets/PacMan/Pacman.png" : "assets/PacMan/PacManClose.png",
        player.isClose ? "C " : "O ", .25f/4, player.dir * 90.f);
    for (int i = 0; i < ghosts.size(); i++){
        if (ghosts[i].isDead)
            continue;
        if (invisbleTime > 0)
            AddDrawable(ghosts[i].x, ghosts[i].y, "assets/PacMan/Eatable.png", "E ", .25f/4, 0.f);
        else
            AddDrawable(ghosts[i].x, ghosts[i].y, ghostTexture[i], ghostTextureCLI[i], .25f/4, 0.f);
    }
    return false;
}

void PacManGame::updateCollisions(void)
{
    for (auto& ghost : ghosts) {
        if (player.x == ghost.x && player.y == ghost.y) {
            if (invisbleTime > 0) {
                ghost.isDead = true;
                ghost.x = -1;
                ghost.y = -1;
                score += scoreCombo;
                scoreCombo *= 2;
            } else {
                gameOver = true;
                playerWon = false;
            }
        }
    }
}

void PacManGame::updateGhost(float deltaTime, int i)
{
    ghosts[i].timeLeftToMove -= deltaTime;
    if (ghosts[i].timeLeftToMove <= 0.f) {
        if (invisbleTime > 0)
            ghosts[i].timeLeftToMove = timeToMoveEatable;
        else
            ghosts[i].timeLeftToMove = timeToMoveGhost;
        int PrevX = ghosts[i].x;
        int PrevY = ghosts[i].y;
        moveGhost(PrevX, PrevY, i);
    }
}

void PacManGame::moveGhost(int PrevX, int PrevY, int i)
{
    int direction = std::rand() % 4;
    if (direction == 0)
        ghosts[i].x += 1;
    else if (direction == 1)
        ghosts[i].x -= 1;
    else if (direction == 2)
        ghosts[i].y -= 1;
    else if (direction == 3)
        ghosts[i].y += 1;
    // Min Max
    if (ghosts[i].x < 0)
        ghosts[i].x = 29;
    if (ghosts[i].y < 0)
        ghosts[i].y = 14;
    if (ghosts[i].x >= window.size.first)
        ghosts[i].x = 0;
    if (ghosts[i].y >= window.size.second)
        ghosts[i].y = 0;
    // Check if the player is on a wall
    if (map[ghosts[i].y][ghosts[i].x] == 1) {
        ghosts[i].x = PrevX;
        ghosts[i].y = PrevY;
        moveGhost(PrevX, PrevY, i);
    }
}

void PacManGame::updateText(void)
{
    auto statusText = std::make_unique<Text>();
    statusText->setPosition({0, 0});
    statusText->setScale({10, 10});
    statusText->setGUI_Color({255, 255, 255, 255});
    statusText->setFontPath("assets/PacMan/NotoSans.ttf");
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
                AddDrawable(j, i, "assets/PacMan/Wall.jpg", "##", .165f/4, 0.f);
            else if (map[i][j] == 0){
                AddDrawable(j, i, "assets/PacMan/LittlePacGome.png", ". ", .25f/4, 0.f);
                isEndPacGome = false;
            } else if (map[i][j] == 3){
                AddDrawable(j, i, "assets/PacMan/PacGome.png", "o ", .25f/4, 0.f);
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
    player.timeAnimLeft -= deltaTime;
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
            player.x = 29;
        if (player.y < 0)
            player.y = 14;
        if (player.x >= window.size.first)
            player.x = 0;
        if (player.y >= window.size.second)
            player.y = 0;
        
        // Check if the player is on a wall
        if (map[player.y][player.x] == 1) {
            player.x = PrevX;
            player.y = PrevY;
        }
        if (map[player.y][player.x] == 0) {
            map[player.y][player.x] = 2;
            score += 10;
        }
        if (map[player.y][player.x] == 3) {
            map[player.y][player.x] = 2;
            score += 100;
            invisbleTime = 10;
            scoreCombo = 200;
        }
    }
    if (player.timeAnimLeft <= 0.f) {
        player.timeAnimLeft = timeAnim;
        player.isClose = !player.isClose;
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
    if (evt.key == Key::KeyCode::KEY_R) {
        Init();
        return false;
    }
    if (gameOver)
        return false;
    if (evt.key == Key::KeyCode::RIGHT || evt.key == Key::KeyCode::KEY_D) {
        player.dir = PacMan::RIGHT;
    } else if (evt.key == Key::KeyCode::LEFT || evt.key == Key::KeyCode::KEY_Q) {
        player.dir = PacMan::LEFT;
    } else if (evt.key == Key::KeyCode::UP || evt.key == Key::KeyCode::KEY_Z) {
        player.dir = PacMan::UP;
    } else if (evt.key == Key::KeyCode::DOWN || evt.key == Key::KeyCode::KEY_S) {
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

void PacManGame::respawnDeadGhost()
{
    for (auto& ghost : ghosts) {
        if (ghost.isDead){
            ghost.x = 14;   
            ghost.y = 5;
            ghost.isDead = false;
        }
    }
}
