#include <map>
#include <memory>
#include <string>
#include <utility>

#include "displays/NCurse/NCurseDisplay.hpp"

extern "C" std::unique_ptr<IDisplayModule> getDisplayModule(void) {
    return std::make_unique<NCurseDisplay>();
}

NCurseDisplay::NCurseDisplay() {
    _window = nullptr;
}

NCurseDisplay::~NCurseDisplay() {
    if (_window)
        _window->~NCurseWrapper();
}

void NCurseDisplay::createWindow(const Window &window) {
    _window = std::make_unique<NCurseWrapper>(
        window.size.second, window.size.first);
}

void NCurseDisplay::draw(const IDrawable &to_draw) {
    Sprite sprite;
    Text text;

    try {
        sprite = dynamic_cast<const Sprite &>(to_draw);
        if (sprite.getGUI_Textures().empty()) {
            _window->print(sprite.getPosition().first*2,
                sprite.getPosition().second, "XX");
            return;
        }
        _window->print(sprite.getPosition().first*2,
            sprite.getPosition().second, sprite.getCLI_Textures()[0]);
    } catch (std::bad_cast &e) {
        text = dynamic_cast<const Text &>(to_draw);
        _window->print(text.getPosition().first, text.getPosition().second,
            text.getStr());
    }
}

void NCurseDisplay::display(void) {
    _window->refresh();
}

void NCurseDisplay::clear(void) {
    _window->clear();
}

Event NCurseDisplay::getEvent(void) {
    int ch = -1;
    int x = 0, y = 0;
    Key::KeyCode key = Key::KeyCode::NONE;

    ch = getch();

    key = getKeyBoardCode(ch);
    if (key != Key::KeyCode::NONE)
        return Event(key, Key::KeyStatus::KEY_PRESSED);
    key = getMouseCode(x, y);
    if (key != Key::KeyCode::NONE)
        return Event(key, Event::MouseStatusClick{Event::MousePos{x, y},
            Event::KeyStatus::KEY_PRESSED});
    return Event(Key::KeyCode::NONE, std::any());
}

Key::KeyCode NCurseDisplay::getMouseCode(int &x, int &y) {
    MEVENT event;

    if (getmouse(&event) == OK) {
        x = event.x;
        y = event.y;
        if (event.bstate & BUTTON1_CLICKED) return Key::MOUSE_LEFT;
        if (event.bstate & BUTTON2_CLICKED) return Key::MOUSE_MIDDLE;
        if (event.bstate & BUTTON3_CLICKED) return Key::MOUSE_RIGHT;
        if (event.bstate & BUTTON4_CLICKED) return Key::MOUSE_BUTTON_4;
        if (event.bstate & BUTTON5_CLICKED) return Key::MOUSE_BUTTON_5;
        if (event.bstate & REPORT_MOUSE_POSITION) return Key::MOUSE_MOVE;
    }
    return Key::KeyCode::NONE;
}

Key::KeyCode NCurseDisplay::getKeyBoardCode(int ch) {
    auto it = _keyMap.find(ch);
    return (it != _keyMap.end()) ? it->second : Key::KeyCode::NONE;
}

void NCurseDisplay::handleSound(const Sound &sound) {
    (void)sound;
}
