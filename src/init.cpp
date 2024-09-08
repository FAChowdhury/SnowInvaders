#include "init.h"

extern App app;

namespace Init {
    void initSDL() {
        // initialise SDL Video Subsystem
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("Initialisation failed: %s\n", SDL_GetError());
            exit(1);
        }

        // initialise app with window and renderer
        app.window = SDL_CreateWindow("Snow Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (app.window == NULL) {
            printf("Failed to create window: %s\n", SDL_GetError());
            exit(1);
        }

        app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
        if (app.renderer == NULL) {
            printf("Failed to create renderer: %s\n", SDL_GetError());
            exit(1);
        }
    }

    void cleanup() {
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
    }
};

