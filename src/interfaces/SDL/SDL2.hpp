/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SDL2
*/

#pragma once
#include <SDL2/SDL.h>
#include <memory>

class SDL2
{
    public:
        static int SDL2_Init(Uint32 flags);
        static void SDL2_Quit(void);
        static void SDL2_DestroyRenderer(SDL_Renderer *renderer);
        static void SDL2_DestroyWindow(SDL_Window *window);
        static void SDL2_SetHint(const char *name, const char *value);
        static std::shared_ptr<SDL_Window> SDL2_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
        static std::shared_ptr<SDL_Renderer> SDL2_CreateRenderer(SDL_Window *window, int index, Uint32 flags);
        static std::shared_ptr<SDL_Texture> IMG2_LoadTexture(SDL_Renderer *renderer, const char *file);
        static void SDL2_QueryTexture(SDL_Texture *texture, Uint32 *format, int *access, int *w, int *h);
        static void SDL2_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect);
        static void SDL2_RenderPresent(SDL_Renderer *renderer);
        static int SDL2_PollEvent(SDL_Event *event);
        static void SDL2_SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        static void SDL2_RenderClear(SDL_Renderer *renderer);
};
