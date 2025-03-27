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
}