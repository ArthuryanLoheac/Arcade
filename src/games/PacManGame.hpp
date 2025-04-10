/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** PacManGame
*/

#pragma once

#include "interfaces/IGameModule.hpp"
#include <vector>
#include <random>
#include <memory>
#include "core/Window.hpp"
#include "interfaces/IDrawable.hpp"
#include "interfaces/Sprite.hpp"
#include "core/Sound.hpp"
#include "core/Event.hpp"

class PacManGame : public IGameModule {
public:
    class PacMan {
        public:
            enum Direction {
                RIGHT,
                DOWN,
                LEFT,
                UP,
            };

            int x, y;
            Direction dir;
            float timeLeftToMove;
            PacMan(int x, int y) : x(x), y(y) { dir = RIGHT;}
    };

    PacManGame();
    void InitScore();

    bool update(float deltaTime) override;
    const Window &getWindow(void) override;
    const std::vector<std::unique_ptr<IDrawable>> &getDrawables(void) override;
    const std::vector<Sound> &getSound(void) override;
    bool event(const Event &evt) override;
    std::vector<std::pair<std::string, int>> getScores(void) override;
    void updateWalls(void);
    void updateText(void);
    void updatePosPlayer(float deltaTime);
    void AddDrawable(int x, int y, std::string texturePath,
        std::string cliTexture, float scale=1.f, float rotation=0, std::tuple<int, int, int, int>
        GUI_Color={255, 255, 255, 255});

private:
    std::vector<std::vector<int>> map = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,0,1,1,0,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,0,0,0,1},
        {1,0,1,0,1,0,0,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,1,1},
        {1,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,0,0,1,0,0,0,1,0,1},
        {1,0,0,0,0,0,1,0,1,0,1,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    bool gameOver = false;
    bool playerWon;
    float gameTime;
    int score;
    PacMan player;
    float timeToMove = 0.25f;

    std::vector<std::unique_ptr<IDrawable>> drawables;
    std::vector<Sound> sounds;

    std::string playerName;
    std::vector<std::pair<std::string, int>> scoreHistory;

    Window window;
};
