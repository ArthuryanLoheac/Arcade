/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#include "SDLDisplay.hpp"
#include "Window.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <Text.hpp>
#include <Sprite.hpp>

std::unique_ptr<IDisplayModule> getDisplayModule()
{
    return std::make_unique<SDLDisplay>();
}

void SDLDisplay::createWindow(const Window &window)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(1);
    app.window = SDL_CreateWindow(window.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window.size.first, window.size.second, windowFlags);
    if (!app.window)
        exit(1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);
    if (!app.renderer)
        exit(1);
}

void SDLDisplay::draw(const IDrawable &drawable)
{
    SDL_Rect dest;
    if (dynamic_cast<const Text *>(&drawable))
        return;
    Sprite sprite = dynamic_cast<const Sprite &>(drawable);
    SDL_Texture *texture = IMG_LoadTexture(app.renderer, sprite.getGUI_Textures()[0].c_str());

    dest.x = drawable.getPosition().first;
    dest.y = drawable.getPosition().second;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void SDLDisplay::display(void)
{
    SDL_RenderPresent(app.renderer);
}

void SDLDisplay::clear(void)
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
}

Event SDLDisplay::getEvent(void)
{
    SDL_Event event;

    if (!SDL_PollEvent(&event))
        return Event(Key::KeyCode::NONE, std::any());
    switch (event.type)
    {
        case SDL_KEYDOWN:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_PRESSED);
        case SDL_KEYUP:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_RELEASED);
        case SDL_MOUSEMOTION:
            LastMouseX = event.motion.x;
            LastMouseY = event.motion.y;
            return Event(Key::KeyCode::MOUSE_MOVE, Event::MousePos{event.motion.x, event.motion.y});
        case SDL_MOUSEBUTTONDOWN:
            return getEventMouse(event, Event::KeyStatus::KEY_PRESSED);
        case SDL_MOUSEBUTTONUP:
            return getEventMouse(event, Event::KeyStatus::KEY_RELEASED);
        case SDL_MOUSEWHEEL:
            return Event(Key::KeyCode::MOUSE_SCROLL, Event::MouseStatusScroll{Event::MousePos{LastMouseX, LastMouseY}, (float)event.wheel.y});
        default:
            return getEvent();
            break;
    }
}

void SDLDisplay::handleSound(const Sound &sound)
{
    (void)sound;
}

SDLDisplay::~SDLDisplay()
{
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

Event SDLDisplay::getEventKeyBoard(SDL_Event &e, Event::KeyStatus isDown)
{
    switch (e.key.keysym.sym)
    {
        case SDLK_a:
            return Event(Key::KeyCode::KEY_A, isDown);
        case SDLK_b:
            return Event(Key::KeyCode::KEY_B, isDown);
        case SDLK_c:
            return Event(Key::KeyCode::KEY_C, isDown);
        case SDLK_d:
            return Event(Key::KeyCode::KEY_D, isDown);
        case SDLK_e:
            return Event(Key::KeyCode::KEY_E, isDown);
        case SDLK_f:
            return Event(Key::KeyCode::KEY_F, isDown);
        case SDLK_g:
            return Event(Key::KeyCode::KEY_G, isDown);
        case SDLK_h:
            return Event(Key::KeyCode::KEY_H, isDown);
        case SDLK_i:
            return Event(Key::KeyCode::KEY_I, isDown);
        case SDLK_j:
            return Event(Key::KeyCode::KEY_J, isDown);
        case SDLK_k:
            return Event(Key::KeyCode::KEY_K, isDown);
        case SDLK_l:
            return Event(Key::KeyCode::KEY_L, isDown);
        case SDLK_m:
            return Event(Key::KeyCode::KEY_M, isDown);
        case SDLK_n:
            return Event(Key::KeyCode::KEY_N, isDown);
        case SDLK_o:
            return Event(Key::KeyCode::KEY_O, isDown);
        case SDLK_p:
            return Event(Key::KeyCode::KEY_P, isDown);
        case SDLK_q:
            return Event(Key::KeyCode::KEY_Q, isDown);
        case SDLK_r:
            return Event(Key::KeyCode::KEY_R, isDown);
        case SDLK_s:
            return Event(Key::KeyCode::KEY_S, isDown);
        case SDLK_t:
            return Event(Key::KeyCode::KEY_T, isDown);
        case SDLK_u:
            return Event(Key::KeyCode::KEY_U, isDown);
        case SDLK_v:
            return Event(Key::KeyCode::KEY_V, isDown);
        case SDLK_w:
            return Event(Key::KeyCode::KEY_W, isDown);
        case SDLK_x:
            return Event(Key::KeyCode::KEY_X, isDown);
        case SDLK_y:
            return Event(Key::KeyCode::KEY_Y, isDown);
        case SDLK_z:
            return Event(Key::KeyCode::KEY_Z, isDown);
        case SDLK_0:
            return Event(Key::KeyCode::KEY_0, isDown);
        case SDLK_1:
            return Event(Key::KeyCode::KEY_1, isDown);
        case SDLK_2:
            return Event(Key::KeyCode::KEY_2, isDown);
        case SDLK_3:
            return Event(Key::KeyCode::KEY_3, isDown);
        case SDLK_4:
            return Event(Key::KeyCode::KEY_4, isDown);
        case SDLK_5:
            return Event(Key::KeyCode::KEY_5, isDown);
        case SDLK_6:
            return Event(Key::KeyCode::KEY_6, isDown);
        case SDLK_7:
            return Event(Key::KeyCode::KEY_7, isDown);
        case SDLK_8:
            return Event(Key::KeyCode::KEY_8, isDown);
        case SDLK_9:
            return Event(Key::KeyCode::KEY_9, isDown);
        case SDLK_SPACE:
            return Event(Key::KeyCode::SPACE, isDown);
        case SDLK_RETURN:
            return Event(Key::KeyCode::ENTER, isDown);
        case SDLK_TAB:
            return Event(Key::KeyCode::TAB, isDown);
        case SDLK_ESCAPE:
            return Event(Key::KeyCode::ECHAP, isDown);
        case SDLK_DELETE:
            return Event(Key::KeyCode::SUPPR, isDown);
        case SDLK_F1:
            return Event(Key::KeyCode::FUNCTION_1, isDown);
        case SDLK_F2:
            return Event(Key::KeyCode::FUNCTION_2, isDown);
        case SDLK_F3:
            return Event(Key::KeyCode::FUNCTION_3, isDown);
        case SDLK_F4:
            return Event(Key::KeyCode::FUNCTION_4, isDown);
        case SDLK_F5:
            return Event(Key::KeyCode::FUNCTION_5, isDown);
        case SDLK_F6:
            return Event(Key::KeyCode::FUNCTION_6, isDown);
        case SDLK_F7:
            return Event(Key::KeyCode::FUNCTION_7, isDown);
        case SDLK_F8:
            return Event(Key::KeyCode::FUNCTION_8, isDown);
        case SDLK_F9:
            return Event(Key::KeyCode::FUNCTION_9, isDown);
        case SDLK_F10:
            return Event(Key::KeyCode::FUNCTION_10, isDown);
        case SDLK_F11:
            return Event(Key::KeyCode::FUNCTION_11, isDown);
        case SDLK_F12:
            return Event(Key::KeyCode::FUNCTION_12, isDown);
        case SDLK_LEFT:
            return Event(Key::KeyCode::LEFT, isDown);
        case SDLK_UP:
            return Event(Key::KeyCode::UP, isDown);
        case SDLK_RIGHT:
            return Event(Key::KeyCode::RIGHT, isDown);
        case SDLK_DOWN:
            return Event(Key::KeyCode::DOWN, isDown);
        case SDLK_LSHIFT:
            return Event(Key::KeyCode::L_SHIFT, isDown);
        case SDLK_RSHIFT:
            return Event(Key::KeyCode::R_SHIFT, isDown);
        case SDLK_LCTRL:
            return Event(Key::KeyCode::L_CONTROL, isDown);
        case SDLK_RCTRL:
            return Event(Key::KeyCode::R_CONTROL, isDown);
        case SDLK_LALT:
            return Event(Key::KeyCode::ALT, isDown);
        case SDLK_RALT:
            return Event(Key::KeyCode::ALTGR, isDown);
        default:
            return getEvent();
            break;
    }
}

Event SDLDisplay::getEventMouse(SDL_Event &e, Event::KeyStatus isDown)
{
    switch (e.button.button)
    {
        case SDL_BUTTON_LEFT:
            return Event(Key::KeyCode::MOUSE_LEFT, Event::MouseStatusClick{Event::MousePos{e.button.x, e.button.y}, isDown});
        case SDL_BUTTON_MIDDLE:
            return Event(Key::KeyCode::MOUSE_MIDDLE, Event::MouseStatusClick{Event::MousePos{e.button.x, e.button.y}, isDown});
        case SDL_BUTTON_RIGHT:
            return Event(Key::KeyCode::MOUSE_RIGHT, Event::MouseStatusClick{Event::MousePos{e.button.x, e.button.y}, isDown});
        case SDL_BUTTON_X1:
            return Event(Key::KeyCode::MOUSE_BUTTON_4, Event::MouseStatusClick{Event::MousePos{e.button.x, e.button.y}, isDown});
        case SDL_BUTTON_X2:
            return Event(Key::KeyCode::MOUSE_BUTTON_5, Event::MouseStatusClick{Event::MousePos{e.button.x, e.button.y}, isDown});
        default:
            return getEvent();
            break;
    }
}
