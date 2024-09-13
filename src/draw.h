#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

namespace Draw {
    void prepareScene();
    void presentScene();
    SDL_Texture *loadTexture(const char *filename);
    void drawToWindow(SDL_Texture *texture, int x, int y);
};