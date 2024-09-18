#pragma once

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WIDTH_PADDING 260

#define PLAYER_SPEED 5
#define PLAYER_FIRE_RATE 16
#define PLAYER_BULLET_SPEED 10

#define NUM_BARRIERS 3

#define NUM_BRICKS_PER_COL 7
#define NUM_BRICKS_PER_ROW 14
#define NUM_BRICK_COLOURS 4

#define BRICK_WIDTH 8
#define BRICK_HEIGHT 8

#define NUM_ALIENS_PER_ROW 7
#define NUM_ALIENS_PER_COL 5
#define SPACE_BETWEEN_ALIENS_X 60
#define SPACE_BETWEEN_ALIENS_Y 50
#define MAX_ALIEN_Y_POS 20

#define ALIEN_JUMP_DISTANCE_X (SPACE_BETWEEN_ALIENS_X / 2)
#define ALIEN_JUMP_DISTANCE_Y SPACE_BETWEEN_ALIENS_Y
#define ALIEN_JUMP_COOLDOWN 60

#define MAX_KEYBOARD_KEYS 350

enum class SIDE {
    NONE,
    PLAYER,
    ALIEN,
};