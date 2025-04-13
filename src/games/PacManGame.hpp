/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** PacManGame
*/

#pragma once

#include <vector>
#include <random>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "core/Window.hpp"
#include "interfaces/IGameModule.hpp"
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
        float timeAnimLeft = 0.14f;
        bool isClose = false;
        PacMan(int x, int y) : x(x), y(y) { dir = RIGHT;}
    };

    class Fantome {
     public:
        int x, y;
        float timeLeftToMove = 0.25f;
        bool isDead = false;
        Fantome(int x, int y) : x(x), y(y) {}
    };

    PacManGame();
    void InitScore();
    void Init();

    bool update(float deltaTime) override;
    const Window &getWindow(void) override;
    const std::vector<std::unique_ptr<IDrawable>> &getDrawables(void) override;
    const std::vector<Sound> &getSound(void) override;
    bool event(const Event &evt) override;
    std::vector<std::pair<std::string, int>> getScores(void) override;
    void updateWalls(void);
    void updateText(void);
    void updatePosPlayer(float deltaTime);
    void updateGhost(float deltaTime, int i);
    void updateCollisions(void);
    void moveGhost(int PrevX, int PrevY, int i);
    void AddDrawable(int x, int y, std::string texturePath,
        std::string cliTexture, float scale = 1.f, float rotation = 0,
        std::tuple<int, int, int, int> GUI_Color = {255, 255, 255, 255});
    void respawnDeadGhost();
    void initAll();

 private:
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> Savemap = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1,
             1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
             1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 2, 2, 2, 1, 1,
             1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 2, 2, 2, 1, 0,
             0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0,
             1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1,
             0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1,
             0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
        {1, 3, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    std::vector<std::string> ghostTexture = {
        "assets/PacMan/Orange.png",
        "assets/PacMan/Pink.png",
        "assets/PacMan/Yellow.png",
        "assets/PacMan/Blue.png"
    };
    std::vector<std::string> ghostTextureCLI = {
        "O ",
        "P ",
        "Y ",
        "B "
    };

    bool gameOver = false;
    bool playerWon;
    float gameTime;
    int score = 0;
    PacMan player;
    float timeUntilOpen = 10.f;
    std::vector<Fantome> ghosts;
    float timeToMove = 0.25f;
    float timeToMoveGhost = 0.23f;
    float timeAnim = 0.2f;
    float timeToMoveEatable = 0.30f;
    float invisbleTime;
    int scoreCombo = 200;

    std::vector<std::unique_ptr<IDrawable>> drawables;
    std::vector<Sound> sounds;

    std::string playerName;
    std::vector<std::pair<std::string, int>> scoreHistory;

    Window window;
};
