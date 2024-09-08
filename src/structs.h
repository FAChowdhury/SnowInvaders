#pragma once

#include <SDL2/SDL.h>

typedef struct App {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;