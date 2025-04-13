#pragma once
#include <vector>
#include <memory>
#include <string>

#include "core/Window.hpp"
#include "interfaces/IDrawable.hpp"
#include "core/Sound.hpp"
#include "core/Event.hpp"

class IGameModule {
 public:
    virtual ~IGameModule() = default;
    virtual bool update(float deltaTime) = 0;
    virtual const Window &getWindow(void) = 0;
    virtual const std::vector<std::unique_ptr<IDrawable>>
        &getDrawables(void) = 0;
    virtual const std::vector<Sound> &getSound(void) = 0;
    virtual bool event(const Event &events) = 0;
    virtual std::vector<std::pair<std::string, int>> getScores(void) = 0;
};

// extern "C" std::unique_ptr<IGameModule> getGameModule();
