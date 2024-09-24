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
    double angle;
    SIDE side;
} Entity;

typedef struct RGB {
    int r;
    int g;
    int b;
} RGB;

typedef struct Brick {
    float x;
    float y;
    int w;
    int h;
    int health;
    RGB colour;
    SIDE side;
} Brick;

typedef struct World {
    Entity *player;
    Entity bulletHead, *bulletTail;
    Entity *alien01s[2][NUM_ALIENS_PER_ROW];
    Entity *alien02s[2][NUM_ALIENS_PER_ROW];
    Entity *alien03s[NUM_ALIENS_PER_ROW];
    Brick *barriers[3][NUM_BRICKS_PER_COL][NUM_BRICKS_PER_ROW];
} World;

typedef struct Rect {
    float x;
    float y;
    int w;
    int h;
} Rect;
