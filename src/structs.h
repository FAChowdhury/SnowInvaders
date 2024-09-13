#pragma once

#include <SDL2/SDL.h>
#include "defs.h"

typedef struct App {
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity {
    SDL_Texture *texture;
    Entity *next;
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    SIDE side;
} Entity;

typedef struct World {
    Entity *player;
} World;