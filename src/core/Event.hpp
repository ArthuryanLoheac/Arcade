#pragma once
#include <any>
#include <string>

#include "arcadeCore/Keys.hpp"


class Event {
 public:
    enum KeyStatus {
        KEY_PRESSED,
        KEY_RELEASED
    };

    struct MousePos {
        int x;
        int y;
    };

    struct MouseStatusClick {
        MousePos pos;
        KeyStatus status;
    };

    struct MouseStatusScroll {
        MousePos pos;
        float value;
    };

    Event(
        enum Key::KeyCode key,
        std::any value): key(key), value(value) {}

    enum Key::KeyCode key;
    std::any value;
};
