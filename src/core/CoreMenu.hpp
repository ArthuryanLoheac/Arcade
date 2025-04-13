#pragma once
#include <vector>
#include <string>
#include <tuple>

#include "interfaces/IGameModule.hpp"
#include "core/Core.hpp"


namespace Core {
class CoreMenu : public IGameModule {
 public:
    explicit CoreMenu(Core &core);
    ~CoreMenu() override;
    bool update(float deltaTime) override;
    const Window &getWindow(void) override;
    const std::vector<std::unique_ptr<IDrawable>> &getDrawables(void)
        override;
    const std::vector<Sound> &getSound(void) override;
    bool event(const Event &events) override;
    std::vector<std::pair<std::string, int>> getScores(void) override;

 private:
    std::vector<std::unique_ptr<IDrawable>> _drawables;
    Window _window;
    std::vector<Sound> _sounds;
    Core *core;

    void AddText(const std::string &str, int x, int y, int fontSize,
        std::tuple <int, int, int, int> colorGUI = std::make_tuple(
            255, 255, 255, 255),
        enum CLI_Color colorCLI = CLI_Color::CLI_WHITE);
};
}  // namespace Core
