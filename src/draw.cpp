#include "draw.h"

extern App app;

namespace Draw {
    void prepareScene() {
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);
    }

    void presentScene() {
        SDL_RenderPresent(app.renderer);
    }

    SDL_Texture *loadTexture(const char *filename) {
        SDL_Texture *texture = NULL;
    
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, 
                        SDL_LOG_PRIORITY_INFO, 
                        "Loading %s", 
                        filename);

        texture = IMG_LoadTexture(app.renderer, filename);

        return texture;
    }

    void drawToWindow(SDL_Texture *texture, int x, int y) {
        SDL_Rect dest = {.x = x, .y = y};
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(app.renderer, texture, NULL, &dest);
        // SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, 45, NULL, SDL_FLIP_NONE);
    }

    void drawToWindowEx(SDL_Texture *texture, int x, int y, double angle) {
        SDL_Rect dest = {.x = x, .y = y};
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
    }
};