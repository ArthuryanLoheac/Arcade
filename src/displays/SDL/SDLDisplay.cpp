/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDLDisplay
*/

#include "SDLDisplay.hpp"
#include "Window.hpp"
#include <map>
#include <Text.hpp>
#include <Sprite.hpp>
#include "SDL2.hpp"
#include <iostream>

extern "C" std::unique_ptr<IDisplayModule> getDisplayModule(void)
{
    return std::make_unique<SDLDisplay>();
}

static void exitError(const std::string &msg, bool ex = true)
{
    std::cerr << msg << std::endl;
    if (ex)
        exit(84);
}

void SDLDisplay::createWindow(const Window &window)
{
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (std::getenv("SDL_AUDIODRIVER") == nullptr) {
        exitError("SDL_AUDIODRIVER not set, sound will be disabled", false);
        setenv("SDL_AUDIODRIVER", "dummy", 1);
    }
    if (SDL2::SDL2_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        exitError("Error initializing SDL");
    if (SDL2::TTF2_Init() < 0)
        exitError("Error initializing SDL_ttf / Text will be disabled", false);
    if (SDL2::Mix2_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        exitError("Error initializing SDL_mixer / Sound will be disabled", false);
    app.window = SDL2::SDL2_CreateWindow(window.title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window.size.first, window.size.second, windowFlags);
    if (!app.window)
        exitError("Error creating SDL window");
    SDL2::SDL2_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL2::SDL2_CreateRenderer(app.window.get(), -1, rendererFlags);
    if (!app.renderer)
        exitError("Error creating SDL renderer");
    std::shared_ptr<SDL_Surface> icon = SDL2::IMG2_Load("assets/arcade.png");
    if (icon.get())
        SDL2::SDL2_SetWindowIcon(app.window.get(), icon.get());
}

void SDLDisplay::draw(const IDrawable &drawable)
{
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

void SDLDisplay::display(void)
{
    SDL2::SDL2_RenderPresent(app.renderer.get());
}

void SDLDisplay::clear(void)
{
    SDL2::SDL2_SetRenderDrawColor(app.renderer.get(), 0, 0, 0, 255);
    SDL2::SDL2_RenderClear(app.renderer.get());
}

Event SDLDisplay::getEvent(void)
{
    SDL_Event event;

    if (!SDL2::SDL2_PollEvent(&event)){
        return Event(Key::KeyCode::NONE, std::any());
    }
    switch (event.type)
    {
        case SDL_QUIT:
            return Event(Key::KeyCode::FUNCTION_4, Event::KeyStatus::KEY_PRESSED);
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

void SDLDisplay::drawText(const Text &txt)
{
    const int fontSize = txt.getScale().first ;
    std::string fontKey = txt.getFontPath();
    SDL_Rect dstRect;
    SDL_Color textColor = { (Uint8)std::get<0>(txt.getGUI_Color()),
                            (Uint8)std::get<1>(txt.getGUI_Color()),
                            (Uint8)std::get<2>(txt.getGUI_Color()),
                            (Uint8)std::get<3>(txt.getGUI_Color())};

    if (fonts.find(fontKey) == fonts.end()) {
        fonts[fontKey] = SDL2::TTF2_OpenFont(txt.getFontPath().c_str(), fontSize);
        if (!fonts[fontKey]){
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    std::string textKey = fontKey + txt.getStr();
    if (textTextures.find(textKey) == textTextures.end()) {
        std::shared_ptr<SDL_Surface> textSurface = SDL2::TTF2_RenderText_Blended(fonts[fontKey].get(), txt.getStr().c_str(), textColor);
        if (!textSurface){
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
        textTextures[textKey] = SDL2::SDL2_CreateTextureFromSurface(app.renderer.get(), textSurface.get());
        if (!textTextures[textKey]){
            std::cerr << "Error: " << TTF_GetError() << std::endl;
            return;
        }
    }

    dstRect = {txt.getPosition().first, txt.getPosition().second, 0, 0};
    SDL2::SDL2_QueryTexture(textTextures[textKey].get(), nullptr, nullptr, &dstRect.w, &dstRect.h);
    SDL2::SDL2_RenderCopy(app.renderer.get(), textTextures[textKey].get(), nullptr, &dstRect);
}

void SDLDisplay::drawSprite(const Sprite &sprite)
{
    SDL_Rect dest;
    std::shared_ptr<SDL_Texture> texture = SDL2::IMG2_LoadTexture(app.renderer.get(), sprite.getGUI_Textures()[0].c_str());

    dest.x = sprite.getPosition().first;
    dest.y = sprite.getPosition().second;
    SDL2::SDL2_QueryTexture(texture.get(), NULL, NULL, &dest.w, &dest.h);

    dest.w = dest.w * sprite.getScale().first;
    dest.h = dest.h * sprite.getScale().second;
    SDL2::SDL2_RenderCopy(app.renderer.get(), texture.get(), NULL, &dest);
}

SDLDisplay::~SDLDisplay()
{
    SDL2::SDL2_DestroyRenderer(app.renderer.get());
    SDL2::SDL2_DestroyWindow(app.window.get());
    SDL2::SDL2_Quit();
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

