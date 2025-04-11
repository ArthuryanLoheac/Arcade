/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL2
*/

#include "SDL2.hpp"
#include <SDL2/SDL_image.h>

extern "C" {
    int SDL2::SDL2_Init(Uint32 flags)
    {
        return SDL_Init(flags);
    }

    void SDL2::SDL2_Quit(void)
    {
        SDL_Quit();
    }

    void SDL2::SDL2_DestroyRenderer(SDL_Renderer *renderer)
    {
        SDL_DestroyRenderer(renderer);
    }

    void SDL2::SDL2_DestroyWindow(SDL_Window *window)
    {
        SDL_DestroyWindow(window);
    }

    void SDL2::SDL2_SetHint(const char *name, const char *value)
    {
        SDL_SetHint(name, value);
    }

    std::shared_ptr<SDL_Window> SDL2::SDL2_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
    {
        SDL_Window *rawWindow = SDL_CreateWindow(title, x, y, w, h, flags);
        if (!rawWindow)
            return nullptr;
        return std::shared_ptr<SDL_Window>(rawWindow, SDL_DestroyWindow);
    }

    std::shared_ptr<SDL_Renderer> SDL2::SDL2_CreateRenderer(SDL_Window *window, int index, Uint32 flags)
    {
        SDL_Renderer *rawRenderer = SDL_CreateRenderer(window, index, flags);
        if (!rawRenderer)
            return nullptr;
        return std::shared_ptr<SDL_Renderer>(rawRenderer, SDL_DestroyRenderer);
    }

    std::shared_ptr<SDL_Texture> SDL2::IMG2_LoadTexture(SDL_Renderer *renderer, const char *file)
    {
        SDL_Texture *rawTexture = IMG_LoadTexture(renderer, file);
        if (!rawTexture)
            return nullptr;
        return std::shared_ptr<SDL_Texture>(rawTexture, SDL_DestroyTexture);
    }

    void SDL2::SDL2_QueryTexture(SDL_Texture *texture, Uint32 *format, int *access, int *w, int *h)
    {
        SDL_QueryTexture(texture, format, access, w, h);
    }

    void SDL2::SDL2_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
    {
        SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    }

    void SDL2::SDL2_RenderPresent(SDL_Renderer *renderer)
    {
        SDL_RenderPresent(renderer);
    }

    int SDL2::SDL2_PollEvent(SDL_Event *event)
    {
        return SDL_PollEvent(event);
    }

    void SDL2::SDL2_RenderClear(SDL_Renderer *renderer)
    {
        SDL_RenderClear(renderer);
    }

    std::shared_ptr<SDL_Surface> SDL2::IMG2_Load(const char *file)
    {
        SDL_Surface *rawSurface = IMG_Load(file);
        if (!rawSurface)
            return nullptr;
        return std::shared_ptr<SDL_Surface>(rawSurface, SDL_FreeSurface);
    }

    void SDL2::SDL2_SetWindowIcon(SDL_Window *window, SDL_Surface *icon)
    {
        SDL_SetWindowIcon(window, icon);
    }

    void SDL2::SDL2_SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    std::shared_ptr<TTF_Font> SDL2::TTF2_OpenFont(const char *file, int ptsize)
    {
        TTF_Font *rawFont = TTF_OpenFont(file, ptsize);
        if (!rawFont)
            return nullptr;
        return std::shared_ptr<TTF_Font>(rawFont, TTF_CloseFont);
    }

    void SDL2::SDL2_RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect)
    {
        SDL_RenderFillRect(renderer, rect);
    }

    std::shared_ptr<SDL_Surface> SDL2::TTF2_RenderText_Blended(TTF_Font *font, const char *text, SDL_Color fg)
    {
        SDL_Surface *rawSurface = TTF_RenderText_Blended(font, text, fg);
        if (!rawSurface)
            return nullptr;
        return std::shared_ptr<SDL_Surface>(rawSurface, SDL_FreeSurface);
    }

    std::shared_ptr<SDL_Texture> SDL2::SDL2_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface)
    {
        SDL_Texture *rawTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!rawTexture)
            return nullptr;
        return std::shared_ptr<SDL_Texture>(rawTexture, SDL_DestroyTexture);
    }

    void SDL2::TTF2_Quit(void)
    {
        TTF_Quit();
    }

    int SDL2::TTF2_Init(void)
    {
        return TTF_Init();
    }

    void SDL2::Mix2_CloseAudio(void)
    {
        Mix_CloseAudio();
    }

    int SDL2::Mix2_OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
    {
        return Mix_OpenAudio(frequency, format, channels, chunksize);
    }

    std::shared_ptr<Mix_Chunk> SDL2::Mix2_LoadWAV(const char *file)
    {
        Mix_Chunk *rawChunk = Mix_LoadWAV(file);
        if (!rawChunk)
            return nullptr;
        return std::shared_ptr<Mix_Chunk>(rawChunk, Mix_FreeChunk);
    }

    void SDL2::Mix2_PlayChannel(int channel, Mix_Chunk *chunk, int loops)
    {
        Mix_PlayChannel(channel, chunk, loops);
    }
}