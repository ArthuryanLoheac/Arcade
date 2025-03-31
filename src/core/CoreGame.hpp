/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** CoreGame
*/

#pragma once
#include "IGameModule.hpp"
#include "Core.hpp"

namespace Core {
    class CoreGame : public IGameModule {
        public:
            CoreGame(Core &core);
            ~CoreGame() override;
            bool update(float deltaTime) override;
            const Window &getWindow(void) override;
            const std::vector<std::unique_ptr<IDrawable>> &getDrawables(void) override;
            const std::vector<Sound> &getSound(void) override;
            bool event(const Event &events) override;
            std::vector<std::pair<std::string, int>> getScores(void) override;
        private:
            std::vector<std::unique_ptr<IDrawable>> _drawables;
            Window _window;
            std::vector<Sound> _sounds;
    };
}