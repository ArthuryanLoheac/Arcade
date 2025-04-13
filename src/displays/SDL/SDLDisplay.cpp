#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <utility>

#include "displays/SDL/SDLDisplay.hpp"
#include "core/Window.hpp"
#include "interfaces/Text.hpp"
#include "interfaces/Sprite.hpp"
#include "displays/SDL/SDL2.hpp"


extern "C" std::unique_ptr<IDisplayModule> getDisplayModule(void) {
    return std::make_unique<SDLDisplay>();
}

static void exitError(const std::string &msg, bool ex = true) {
    std::cerr << msg << std::endl;
    if (ex)
        exit(84);
}

void SDLDisplay::createWindow(const Window &window) {
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (std::getenv("SDL_AUDIODRIVER") == nullptr) {
        exitError("SDL_AUDIODRIVER not set, Setting it", false);
        setenv("SDL_AUDIODRIVER", "dummy", 1);
    }
    if (SDL2::SDL2_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        exitError("Error initializing SDL");
    if (SDL2::TTF2_Init() < 0)
        exitError("Error initializing SDL_ttf / Text will be disabled", false);
    if (SDL2::Mix2_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        exitError("Error initializing SDL_mixer / Sound will be disabled",
            false);

    int windowWidth = window.size.first * UNIT_TO_PIXEL;
    int windowHeight = window.size.second * UNIT_TO_PIXEL;

    app.window = SDL2::SDL2_CreateWindow(window.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight, windowFlags);
    if (!app.window)
        exitError("Error creating SDL window");
    SDL2::SDL2_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL2::SDL2_CreateRenderer(app.window.get(),
        -1, rendererFlags);
    if (!app.renderer)
        exitError("Error creating SDL renderer");
    std::shared_ptr<SDL_Surface> icon = SDL2::IMG2_Load("assets/arcade.png");
    if (icon.get())
        SDL2::SDL2_SetWindowIcon(app.window.get(), icon.get());
}

void SDLDisplay::draw(const IDrawable &drawable) {
    Sprite sprite;
    Text text;

    try {
        sprite = dynamic_cast<const Sprite &>(drawable);
        drawSprite(sprite);
    } catch (std::bad_cast &e) {
        text = dynamic_cast<const Text &>(drawable);
        drawText(text);
    }
}

void SDLDisplay::display(void) {
    SDL2::SDL2_RenderPresent(app.renderer.get());
}

void SDLDisplay::clear(void) {
    SDL2::SDL2_SetRenderDrawColor(app.renderer.get(), 0, 0, 0, 255);
    SDL2::SDL2_RenderClear(app.renderer.get());
}

Event SDLDisplay::getEvent(void) {
    SDL_Event event;

    if (!SDL2::SDL2_PollEvent(&event))
        return Event(Key::KeyCode::NONE, std::any());
    switch (event.type) {
        case SDL_QUIT:
            return Event(Key::KeyCode::ECHAP,
                Event::KeyStatus::KEY_PRESSED);
        case SDL_KEYDOWN:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_PRESSED);
        case SDL_KEYUP:
            return getEventKeyBoard(event, Event::KeyStatus::KEY_RELEASED);
        case SDL_MOUSEMOTION:
            LastMouseX = event.motion.x / UNIT_TO_PIXEL;
            LastMouseY = event.motion.y / UNIT_TO_PIXEL;
            return Event(Key::KeyCode::MOUSE_MOVE, Event::MousePos
            {event.motion.x / UNIT_TO_PIXEL, event.motion.y / UNIT_TO_PIXEL});
        case SDL_MOUSEBUTTONDOWN:
            return getEventMouse(event, Event::KeyStatus::KEY_PRESSED);
        case SDL_MOUSEBUTTONUP:
            return getEventMouse(event, Event::KeyStatus::KEY_RELEASED);
        case SDL_MOUSEWHEEL:
            return Event(Key::KeyCode::MOUSE_SCROLL,
            Event::MouseStatusScroll{Event::MousePos
            {LastMouseX, LastMouseY}, static_cast<float>(event.wheel.y)});
        default:
            return getEvent();
            break;
    }
}

void SDLDisplay::handleSound(const Sound &sound) {
    std::shared_ptr<Mix_Chunk> music;
    static int channel = 0;

    if (sound.state == Sound::STOP) {
        try {
            Mix_HaltChannel(musics[sound.id].second);
        }
        catch(const std::exception& e) {}
        return;
    }
    music = SDL2::Mix2_LoadWAV(sound.filePath.c_str());
    if (!music)
        return;
    if (sound.state == Sound::PLAY)
        SDL2::Mix2_PlayChannel(channel, music.get(), 0);
    else if (sound.state == Sound::LOOP)
        SDL2::Mix2_PlayChannel(channel, music.get(), -1);
    musics[sound.id] = std::make_pair(music, channel);
    channel++;
}

void SDLDisplay::drawText(const Text &txt) {
    const int fontSize = txt.getScale().first;
    std::string fontKey = txt.getFontPath();
    SDL_Rect dstRect;
    SDL_Color textColor = { (Uint8)std::get<0>(txt.getGUI_Color()),
                            (Uint8)std::get<1>(txt.getGUI_Color()),
                            (Uint8)std::get<2>(txt.getGUI_Color()),
                            (Uint8)std::get<3>(txt.getGUI_Color())};

    if (fonts.find(fontKey) == fonts.end()) {
        fonts[fontKey] = SDL2::TTF2_OpenFont(
            txt.getFontPath().c_str(), fontSize);
        if (!fonts[fontKey]) {
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    std::string textKey = fontKey + txt.getStr();
    if (textTextures.find(textKey) == textTextures.end()) {
        std::shared_ptr<SDL_Surface> textSurface =
            SDL2::TTF2_RenderText_Blended(fonts[fontKey].get(),
            txt.getStr().c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
        textTextures[textKey] = SDL2::SDL2_CreateTextureFromSurface(
            app.renderer.get(), textSurface.get());
        if (!textTextures[textKey]) {
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    dstRect = {txt.getPosition().first * UNIT_TO_PIXEL,
        txt.getPosition().second * UNIT_TO_PIXEL, 0, 0};
    SDL2::SDL2_QueryTexture(textTextures[textKey].get(), nullptr, nullptr,
        &dstRect.w, &dstRect.h);
    SDL2::SDL2_RenderCopy(app.renderer.get(), textTextures[textKey].get(),
        nullptr, &dstRect);
}

void SDLDisplay::drawSquare(const Sprite &sprite) {
    SDL_Rect square;
    square.x = sprite.getPosition().first * UNIT_TO_PIXEL;
    square.y = sprite.getPosition().second * UNIT_TO_PIXEL;
    square.w = sprite.getScale().first * UNIT_TO_PIXEL;
    square.h = sprite.getScale().second * UNIT_TO_PIXEL;

    SDL2::SDL2_SetRenderDrawColor(app.renderer.get(),
        std::get<0>(sprite.getGUI_Color()),
        std::get<1>(sprite.getGUI_Color()),
        std::get<2>(sprite.getGUI_Color()),
        std::get<3>(sprite.getGUI_Color()));
    SDL2::SDL2_RenderFillRect(app.renderer.get(), &square);
}

void SDLDisplay::drawSprite(const Sprite &sprite) {
    if (sprite.getGUI_Textures().empty()) {
        drawSquare(sprite);
        return;
    }

    std::string textKey = sprite.getGUI_Textures()[0];
    std::shared_ptr<SDL_Texture> texture;

    if (textures.find(textKey) == textures.end()) {
        std::shared_ptr<SDL_Texture> textSurface =
            SDL2::IMG2_LoadTexture(app.renderer.get(), textKey.c_str());
        textures[textKey] =
            SDL2::IMG2_LoadTexture(app.renderer.get(), textKey.c_str());
        if (!textures[textKey]) {
            std::cerr << "Error: " << std::endl;
            return;
        }
    }
    texture = textures[textKey];
    SDL_Rect dest;
    dest.x = sprite.getPosition().first * UNIT_TO_PIXEL;
    dest.y = sprite.getPosition().second * UNIT_TO_PIXEL;
    SDL2::SDL2_QueryTexture(texture.get(), NULL, NULL, &dest.w, &dest.h);

    dest.w = dest.w * sprite.getScale().first;
    dest.h = dest.h * sprite.getScale().second;
    SDL2::SDL2_RenderCopy(app.renderer.get(), texture.get(), NULL, &dest);
}

SDLDisplay::~SDLDisplay() {
    SDL2::SDL2_DestroyRenderer(app.renderer.get());
    SDL2::SDL2_DestroyWindow(app.window.get());
    SDL2::SDL2_Quit();
}

static Event returnEvent(Key::KeyCode key, Event::KeyStatus isDown) {
    return Event(key, std::make_any<Event::KeyStatus>(isDown));
}

Event SDLDisplay::getEventKeyBoard(SDL_Event &e, Event::KeyStatus isDown) {
    switch (e.key.keysym.sym) {
        case SDLK_a:
            return returnEvent(Key::KeyCode::KEY_A, isDown);
        case SDLK_b:
            return returnEvent(Key::KeyCode::KEY_B, isDown);
        case SDLK_c:
            return returnEvent(Key::KeyCode::KEY_C, isDown);
        case SDLK_d:
            return returnEvent(Key::KeyCode::KEY_D, isDown);
        case SDLK_e:
            return returnEvent(Key::KeyCode::KEY_E, isDown);
        case SDLK_f:
            return returnEvent(Key::KeyCode::KEY_F, isDown);
        case SDLK_g:
            return returnEvent(Key::KeyCode::KEY_G, isDown);
        case SDLK_h:
            return returnEvent(Key::KeyCode::KEY_H, isDown);
        case SDLK_i:
            return returnEvent(Key::KeyCode::KEY_I, isDown);
        case SDLK_j:
            return returnEvent(Key::KeyCode::KEY_J, isDown);
        case SDLK_k:
            return returnEvent(Key::KeyCode::KEY_K, isDown);
        case SDLK_l:
            return returnEvent(Key::KeyCode::KEY_L, isDown);
        case SDLK_m:
            return returnEvent(Key::KeyCode::KEY_M, isDown);
        case SDLK_n:
            return returnEvent(Key::KeyCode::KEY_N, isDown);
        case SDLK_o:
            return returnEvent(Key::KeyCode::KEY_O, isDown);
        case SDLK_p:
            return returnEvent(Key::KeyCode::KEY_P, isDown);
        case SDLK_q:
            return returnEvent(Key::KeyCode::KEY_Q, isDown);
        case SDLK_r:
            return returnEvent(Key::KeyCode::KEY_R, isDown);
        case SDLK_s:
            return returnEvent(Key::KeyCode::KEY_S, isDown);
        case SDLK_t:
            return returnEvent(Key::KeyCode::KEY_T, isDown);
        case SDLK_u:
            return returnEvent(Key::KeyCode::KEY_U, isDown);
        case SDLK_v:
            return returnEvent(Key::KeyCode::KEY_V, isDown);
        case SDLK_w:
            return returnEvent(Key::KeyCode::KEY_W, isDown);
        case SDLK_x:
            return returnEvent(Key::KeyCode::KEY_X, isDown);
        case SDLK_y:
            return returnEvent(Key::KeyCode::KEY_Y, isDown);
        case SDLK_z:
            return returnEvent(Key::KeyCode::KEY_Z, isDown);
        case SDLK_0:
            return returnEvent(Key::KeyCode::KEY_0, isDown);
        case SDLK_1:
            return returnEvent(Key::KeyCode::KEY_1, isDown);
        case SDLK_2:
            return returnEvent(Key::KeyCode::KEY_2, isDown);
        case SDLK_3:
            return returnEvent(Key::KeyCode::KEY_3, isDown);
        case SDLK_4:
            return returnEvent(Key::KeyCode::KEY_4, isDown);
        case SDLK_5:
            return returnEvent(Key::KeyCode::KEY_5, isDown);
        case SDLK_6:
            return returnEvent(Key::KeyCode::KEY_6, isDown);
        case SDLK_7:
            return returnEvent(Key::KeyCode::KEY_7, isDown);
        case SDLK_8:
            return returnEvent(Key::KeyCode::KEY_8, isDown);
        case SDLK_9:
            return returnEvent(Key::KeyCode::KEY_9, isDown);
        case SDLK_SPACE:
            return returnEvent(Key::KeyCode::SPACE, isDown);
        case SDLK_RETURN:
            return returnEvent(Key::KeyCode::ENTER, isDown);
        case SDLK_TAB:
            return returnEvent(Key::KeyCode::TAB, isDown);
        case SDLK_ESCAPE:
            return returnEvent(Key::KeyCode::ECHAP, isDown);
        case SDLK_DELETE:
            return returnEvent(Key::KeyCode::SUPPR, isDown);
        case SDLK_F1:
            return returnEvent(Key::KeyCode::FUNCTION_1, isDown);
        case SDLK_F2:
            return returnEvent(Key::KeyCode::FUNCTION_2, isDown);
        case SDLK_F3:
            return returnEvent(Key::KeyCode::FUNCTION_3, isDown);
        case SDLK_F4:
            return returnEvent(Key::KeyCode::FUNCTION_4, isDown);
        case SDLK_F5:
            return returnEvent(Key::KeyCode::FUNCTION_5, isDown);
        case SDLK_F6:
            return returnEvent(Key::KeyCode::FUNCTION_6, isDown);
        case SDLK_F7:
            return returnEvent(Key::KeyCode::FUNCTION_7, isDown);
        case SDLK_F8:
            return returnEvent(Key::KeyCode::FUNCTION_8, isDown);
        case SDLK_F9:
            return returnEvent(Key::KeyCode::FUNCTION_9, isDown);
        case SDLK_F10:
            return returnEvent(Key::KeyCode::FUNCTION_10, isDown);
        case SDLK_F11:
            return returnEvent(Key::KeyCode::FUNCTION_11, isDown);
        case SDLK_F12:
            return returnEvent(Key::KeyCode::FUNCTION_12, isDown);
        case SDLK_LEFT:
            return returnEvent(Key::KeyCode::LEFT, isDown);
        case SDLK_UP:
            return returnEvent(Key::KeyCode::UP, isDown);
        case SDLK_RIGHT:
            return returnEvent(Key::KeyCode::RIGHT, isDown);
        case SDLK_DOWN:
            return returnEvent(Key::KeyCode::DOWN, isDown);
        case SDLK_LSHIFT:
            return returnEvent(Key::KeyCode::L_SHIFT, isDown);
        case SDLK_RSHIFT:
            return returnEvent(Key::KeyCode::R_SHIFT, isDown);
        case SDLK_LCTRL:
            return returnEvent(Key::KeyCode::L_CONTROL, isDown);
        case SDLK_RCTRL:
            return returnEvent(Key::KeyCode::R_CONTROL, isDown);
        case SDLK_LALT:
            return returnEvent(Key::KeyCode::ALT, isDown);
        case SDLK_RALT:
            return returnEvent(Key::KeyCode::ALTGR, isDown);
        default:
            return getEvent();
            break;
    }
}

Event SDLDisplay::getEventMouse(SDL_Event &e, Event::KeyStatus isDown) {
    switch (e.button.button) {
        case SDL_BUTTON_LEFT:
            return Event(Key::KeyCode::MOUSE_LEFT, Event::MouseStatusClick
            {Event::MousePos{e.button.x / UNIT_TO_PIXEL,
                e.button.y / UNIT_TO_PIXEL}, isDown});
        case SDL_BUTTON_MIDDLE:
            return Event(Key::KeyCode::MOUSE_MIDDLE, Event::MouseStatusClick
            {Event::MousePos{e.button.x / UNIT_TO_PIXEL,
                e.button.y / UNIT_TO_PIXEL}, isDown});
        case SDL_BUTTON_RIGHT:
            return Event(Key::KeyCode::MOUSE_RIGHT, Event::MouseStatusClick
            {Event::MousePos{e.button.x / UNIT_TO_PIXEL,
                e.button.y / UNIT_TO_PIXEL}, isDown});
        case SDL_BUTTON_X1:
            return Event(Key::KeyCode::MOUSE_BUTTON_4, Event::MouseStatusClick
            {Event::MousePos{e.button.x / UNIT_TO_PIXEL,
                e.button.y / UNIT_TO_PIXEL}, isDown});
        case SDL_BUTTON_X2:
            return Event(Key::KeyCode::MOUSE_BUTTON_5, Event::MouseStatusClick
            {Event::MousePos{e.button.x / UNIT_TO_PIXEL,
                e.button.y / UNIT_TO_PIXEL}, isDown});
        default:
            return getEvent();
            break;
    }
}

