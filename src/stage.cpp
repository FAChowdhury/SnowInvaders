#include "stage.h"

extern App app;
extern World stage;

// alien 
int alienStartXPos[NUM_ALIENS_PER_ROW];
int alienStartYPos[NUM_ALIENS_PER_COL];
int alienJumpCooldown;
int alienDirection;
float leftMostAlienX;
float rightMostAlienX;

// barriers
bool barrier[NUM_BRICKS_PER_COL][NUM_BRICKS_PER_ROW] {
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}
};

RGB brickColours[NUM_BRICK_COLOURS] = {
    {178, 255, 102},
    {153, 255, 51},
    {128, 255, 0},
    {102, 204, 0}
};

int barrierStartX[3] = {WIDTH_PADDING + 24, (SCREEN_WIDTH / 2) - ((NUM_BRICKS_PER_ROW * BRICK_WIDTH) / 2), SCREEN_WIDTH - WIDTH_PADDING - NUM_BRICKS_PER_ROW * BRICK_WIDTH - 24};
int barrierStartY = 520;



namespace StageTextures {
    static SDL_Texture *playerTexture;
    static SDL_Texture *playerBulletTexture;
    static SDL_Texture *alien02BulletTexture;
    static SDL_Texture *alien03BulletTexture;
    static SDL_Texture *alien01Texture;
    static SDL_Texture *alien02Texture;
    static SDL_Texture *alien03Texture;
}

namespace StageUtil {
    static double calculateAngle(float fromX, float fromY, float toX, float toY) {
        double angleRadians = atan(-(fromX - toX)/(fromY - toY));
        double angleDegrees = angleRadians * (180 / M_PI);
        return angleDegrees;
    }

    static float calculateSlopeX(float fromX, float fromY, float toX, float toY) {
        return (fromX - toX)/(fromY - toY);
    }

    static void fireAlienBulletAtPlayer(Entity *e) {
        // printf("FIRE!\n");
        // create bullet
        Entity *bullet = (Entity *)malloc(sizeof(Entity));
        memset(bullet, 0, sizeof(Entity));
        // assign bullet values
        bullet->texture = StageTextures::alien03BulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
        bullet->x = e->x + (1.0f/2.0f) * (e->w - bullet->w);
        bullet->y = e->y - bullet->y + 8;
        bullet->dy = ALIEN_BULLET_SPEED;
        bullet->dx = calculateSlopeX(e->x + (e->w / 2), e->y + (e->h / 2), stage.player->x + (stage.player->w / 2), stage.player->y + (stage.player->h / 2)) * ALIEN_BULLET_SPEED;
        bullet->health = 1;
        bullet->side = SIDE::ALIEN;
        // compute bullet angle
        bullet->angle = calculateAngle(e->x + (e->w / 2), e->y + (e->h / 2), stage.player->x + (stage.player->w / 2), stage.player->y + (stage.player->h / 2));
        // reset reload cooldown

        // add it to bullet linked list of stage
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;


        e->reload = (4 + (rand() % 6)) * FPS;
    }

    static void fireAlienBulletDown(Entity *e) {
        // create bullet
        Entity *bullet = (Entity *)malloc(sizeof(Entity));
        memset(bullet, 0, sizeof(Entity));

        // assign values to bullet entity
        // bullet->texture = StageTextures::alien02BulletTexture;
        bullet->texture = StageTextures::alien02BulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
        bullet->x = e->x + (1.0f/2.0f) * (e->w - bullet->w);
        bullet->y = e->y - bullet->y + 8;
        bullet->dy = ALIEN_BULLET_SPEED;
        bullet->health = 1;
        bullet->side = SIDE::ALIEN;

        // add it to bullet linked list of stage
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        // reset reload cooldown
        e->reload = (2 + (rand() % 6)) * FPS;
    }

    static bool isBulletHittingBarrier(Entity *bullet) {
        Rect bulletHitBox = {.x = bullet->x, .y = bullet->y, .w = bullet->w, .h = bullet->h};
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            for (int row = NUM_BRICKS_PER_COL - 1; row >= 0; --row) {
                for (int col = 0; col < NUM_BRICKS_PER_ROW; ++col) {
                    if (stage.barriers[i][row][col]) {
                        Rect brickHitBox = {.x = stage.barriers[i][row][col]->x, .y = stage.barriers[i][row][col]->y, .w = stage.barriers[i][row][col]->w, .h = stage.barriers[i][row][col]->h};
                        if (bullet->side != stage.barriers[i][row][col]->side && Util::collision(&bulletHitBox, &brickHitBox)) {
                            if (bullet->side == SIDE::ALIEN) {
                                for (int j = 0; j < NUM_BRICKS_PER_COL; ++j) {
                                    if (stage.barriers[i][j][col] != NULL) {
                                        stage.barriers[i][j][col]->health = 0;
                                        return true;
                                    }
                                }
                            } else if (bullet->side == SIDE::PLAYER) {
                                for (int j = NUM_BRICKS_PER_COL - 1; j >= 0; --j) {
                                    if (stage.barriers[i][j][col] != NULL) {
                                        stage.barriers[i][j][col]->health = 0;
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    static bool isBulletHittingAlien(Entity *bullet) {
        Rect bulletHitBox = {.x = bullet->x, .y = bullet->y, .w = bullet->w, .h = bullet->h};
        // alien01s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien01s[row][col] != NULL) {
                    Rect alienHitBox = {.x = stage.alien01s[row][col]->x, .y = stage.alien01s[row][col]->y, .w = stage.alien01s[row][col]->w, .h = stage.alien01s[row][col]->h};
                    if (bullet->side != stage.alien01s[row][col]->side && Util::collision(&bulletHitBox, &alienHitBox)) {
                        stage.alien01s[row][col]->health = 0;
                        return true;
                    }
                }
            }
        }
        // alien02s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien02s[row][col] != NULL) {
                    Rect alienHitBox = {.x = stage.alien02s[row][col]->x, .y = stage.alien02s[row][col]->y, .w = stage.alien02s[row][col]->w, .h = stage.alien02s[row][col]->h};
                    if (bullet->side != stage.alien02s[row][col]->side && Util::collision(&bulletHitBox, &alienHitBox)) {
                        stage.alien02s[row][col]->health = 0;
                        return true;
                    }
                }
            }
        }
        // alien03s
        for (int i = 0; i < NUM_ALIENS_PER_ROW; ++i) {
            if (stage.alien03s[i] != NULL) {
                Rect alienHitBox = {.x = stage.alien03s[i]->x, .y = stage.alien03s[i]->y, .w = stage.alien03s[i]->w, .h = stage.alien03s[i]->h};
                if (bullet->side != stage.alien03s[i]->side && Util::collision(&bulletHitBox, &alienHitBox)) {
                    stage.alien03s[i]->health = 0;
                    return true;
                }
            }
        }

        return false;
    }

    static bool isBulletHittingPlayer(Entity *bullet) {return false;}

    static void fireBullet() {
        // make the bullet entity
        Entity *bullet = (Entity *)malloc(sizeof(Entity));
        memset(bullet, 0, sizeof(Entity));

        // assign values to bullet entity
        bullet->texture = StageTextures::playerBulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
        bullet->x = stage.player->x + (1.0f/2.0f) * (stage.player->w - bullet->w);
        bullet->y = stage.player->y - bullet->y - 8;
        bullet->dy = -PLAYER_BULLET_SPEED;
        bullet->health = 1;
        bullet->side = SIDE::PLAYER;

        // add it to bullet linked list of stage
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        stage.player->reload = PLAYER_FIRE_RATE;
    }

    static void initPlayer() {
        stage.player = (Entity *)malloc(sizeof(Entity));
        memset(stage.player, 0, sizeof(Entity));

        stage.player->texture = StageTextures::playerTexture;
        SDL_QueryTexture(stage.player->texture, NULL, NULL, &stage.player->w, &stage.player->h);
        stage.player->x = (SCREEN_WIDTH / 2) - (stage.player->w / 2);
        stage.player->y = SCREEN_HEIGHT - (stage.player->h) * (5.0f/2.0f);
        stage.player->health = 1;
        stage.player->side = SIDE::PLAYER;
        stage.player->reload = PLAYER_FIRE_RATE;
    }

    static void initBullets() {stage.bulletTail = &stage.bulletHead;}

    static void initAliens() {
        // set alien jump cooldown
        alienJumpCooldown = ALIEN_JUMP_COOLDOWN;

        // set alien movement direction
        alienDirection = -1; // left

        // set up starting positions
        int startPosX = (SCREEN_WIDTH / 2) - (SPACE_BETWEEN_ALIENS_X * (NUM_ALIENS_PER_ROW / 2));
        for (int i = 0; i < NUM_ALIENS_PER_ROW; ++i) {
            alienStartXPos[i] = startPosX;
            startPosX += SPACE_BETWEEN_ALIENS_X;
        }

        int startPosY = MAX_ALIEN_Y_POS;
        for (int i = 0; i < NUM_ALIENS_PER_COL; ++i) {
            alienStartYPos[i] = startPosY;
            startPosY += SPACE_BETWEEN_ALIENS_Y;
        }
        
        // allocate memory for every alien and initialise values
        // alien01s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                stage.alien01s[row][col] = (Entity *)malloc(sizeof(Entity));
                memset(stage.alien01s[row][col], 0, sizeof(Entity));

                stage.alien01s[row][col]->texture = StageTextures::alien01Texture;
                SDL_QueryTexture(stage.alien01s[row][col]->texture, NULL, NULL, &stage.alien01s[row][col]->w, &stage.alien01s[row][col]->h);
                stage.alien01s[row][col]->x = alienStartXPos[col] - (stage.alien01s[row][col]->w / 2);
                stage.alien01s[row][col]->y = alienStartYPos[row + 3]; // 3 and 4
                stage.alien01s[row][col]->health = 1;
                stage.alien01s[row][col]->side = SIDE::ALIEN;
                // stage.alien01s[row][col]->reload = PLAYER_FIRE_RATE; TODO LATER
            }
        }
        // alien02s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                stage.alien02s[row][col] = (Entity *)malloc(sizeof(Entity));
                memset(stage.alien02s[row][col], 0, sizeof(Entity));

                stage.alien02s[row][col]->texture = StageTextures::alien02Texture;
                SDL_QueryTexture(stage.alien02s[row][col]->texture, NULL, NULL, &stage.alien02s[row][col]->w, &stage.alien02s[row][col]->h);
                stage.alien02s[row][col]->x = alienStartXPos[col] - (stage.alien02s[row][col]->w / 2);
                stage.alien02s[row][col]->y = alienStartYPos[row + 1]; // 1 and 2
                stage.alien02s[row][col]->health = 1;
                stage.alien02s[row][col]->side = SIDE::ALIEN;
                // stage.alien02s[row][col]->reload = PLAYER_FIRE_RATE; TODO LATER

                // alien shooting
                stage.alien02s[row][col]->reload = (1 + (rand() % 8)) * FPS;
            }
        }
        // alien03s
        for (int i = 0; i < NUM_ALIENS_PER_ROW; ++i) {
            stage.alien03s[i] = (Entity *)malloc(sizeof(Entity));
            memset(stage.alien03s[i], 0, sizeof(Entity));

            stage.alien03s[i]->texture = StageTextures::alien03Texture;
            SDL_QueryTexture(stage.alien03s[i]->texture, NULL, NULL, &stage.alien03s[i]->w, &stage.alien03s[i]->h);
            stage.alien03s[i]->x = alienStartXPos[i] - (stage.alien03s[i]->w / 2);
            stage.alien03s[i]->y = alienStartYPos[0]; // 0
            stage.alien03s[i]->health = 1;
            stage.alien03s[i]->side = SIDE::ALIEN;
            // stage.alien03s[i]->reload = PLAYER_FIRE_RATE; TODO LATER

            // alien shooting
            stage.alien03s[i]->reload = (1 + (rand() % 8)) * FPS;
        }
    }

    static void initBarriers() {
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            for (int row = 0; row < NUM_BRICKS_PER_COL; ++row) {
                for (int col = 0; col < NUM_BRICKS_PER_ROW; ++col) {
                    if (barrier[row][col]) {
                        stage.barriers[i][row][col] = (Brick *)malloc(sizeof(Brick));
                        memset(stage.barriers[i][row][col], 0, sizeof(Brick));

                        stage.barriers[i][row][col]->x = barrierStartX[i] + col * BRICK_WIDTH;
                        stage.barriers[i][row][col]->y = barrierStartY + row * BRICK_HEIGHT;
                        stage.barriers[i][row][col]->w = BRICK_WIDTH;
                        stage.barriers[i][row][col]->h = BRICK_HEIGHT;
                        stage.barriers[i][row][col]->health = 1;
                        stage.barriers[i][row][col]->colour = brickColours[rand() % NUM_BRICK_COLOURS];
                        stage.barriers[i][row][col]->side = SIDE::NONE;
                    } else {    
                        stage.barriers[i][row][col] = NULL;
                    }
                }
            }
        }
    }
    
    static void updateBarriers() {
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            for (int row = 0; row < NUM_BRICKS_PER_COL; ++row) {
                for (int col = 0; col < NUM_BRICKS_PER_ROW; ++col) {
                    if (stage.barriers[i][row][col]) {
                        if (stage.barriers[i][row][col]->health == 0) {
                            free(stage.barriers[i][row][col]);
                            stage.barriers[i][row][col] = NULL;
                        }
                    }
                }
            }
        }
    }

    static void drawBarriers() {
        for (int i = 0; i < NUM_BARRIERS; ++i) {
            for (int row = 0; row < NUM_BRICKS_PER_COL; ++row) {
                for (int col = 0; col < NUM_BRICKS_PER_ROW; ++col) {
                    if (stage.barriers[i][row][col]) {
                        SDL_SetRenderDrawColor(app.renderer, stage.barriers[i][row][col]->colour.r, stage.barriers[i][row][col]->colour.g, stage.barriers[i][row][col]->colour.b, 255);
                        SDL_Rect rect = {.x = (int)stage.barriers[i][row][col]->x, .y = (int)stage.barriers[i][row][col]->y, .w = stage.barriers[i][row][col]->w, .h = stage.barriers[i][row][col]->h};
                        SDL_RenderFillRect(app.renderer, &rect);
                    }
                }
            }
        }
    }

    static void updatePlayer() {
        if (stage.player != NULL) {
            // player movement
            stage.player->dx = 0;

            if (app.keyboard[SDL_SCANCODE_LEFT] && (stage.player->x - PLAYER_SPEED > WIDTH_PADDING))
                {stage.player->dx = -PLAYER_SPEED;}

            if (app.keyboard[SDL_SCANCODE_RIGHT] && (stage.player->x + stage.player->w + PLAYER_SPEED < SCREEN_WIDTH - WIDTH_PADDING))
                {stage.player->dx = PLAYER_SPEED;}

            stage.player->x += stage.player->dx;

            // player fire
            if (--(stage.player->reload) < 0 && app.keyboard[SDL_SCANCODE_LCTRL]) {
                fireBullet();
            }
        }
    }

    static void drawPlayer() {
        // render player to back buffer
        Draw::drawToWindow(stage.player->texture, stage.player->x, stage.player->y);
    }

    static void updateBullets() {
        Entity *prev = &stage.bulletHead;
        for (Entity *curr = stage.bulletHead.next; curr != NULL; curr = curr->next) {
            // move the bullet
            curr->x += curr->dx;
            curr->y += curr->dy;

            // if bullet is outside of window or collides with something, free it
            if (isBulletHittingBarrier(curr) || isBulletHittingAlien(curr) || isBulletHittingPlayer(curr) || curr->x > SCREEN_WIDTH || curr->y > SCREEN_HEIGHT || curr->x < -curr->w || curr->y < -curr->h) {
                if (curr == stage.bulletTail) {stage.bulletTail = prev;}

                prev->next = curr->next;
                free(curr);
                curr = prev;
            }

            prev = curr;
        }
    }

    static void drawBullets() {
        for (Entity *curr = stage.bulletHead.next; curr != NULL; curr = curr->next) {
            // Draw::drawToWindow(curr->texture, curr->x, curr->y);
            Draw::drawToWindowEx(curr->texture, curr->x, curr->y, curr->angle);
        }
    }

    static void updateAliens() {
        --alienJumpCooldown;
        // compute left-most and right-most alien columns per frame {range from 0 to and not including NUM_ALIENS_PER_ROW}
        // find left-most alien
        bool found = false;
        for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
            for (int row = 0; row < 2; ++row) {
                if (stage.alien01s[row][col]) {
                    leftMostAlienX = stage.alien01s[row][col]->x;
                    found = true;
                    break;
                }
            }
            if (found) {break;}
            for (int row = 0; row < 2; ++row) {
                if (stage.alien02s[row][col]) {
                    leftMostAlienX = stage.alien02s[row][col]->x;
                    found = true;
                    break;
                }
            }
            if (found) {break;}
            if (stage.alien03s[col]) {
                leftMostAlienX = stage.alien03s[col]->x;
                break;
            }
        }

        // find right-most alien
        found = false;
        for (int col = NUM_ALIENS_PER_ROW - 1; col >= 0; --col) {
            for (int row = 0; row < 2; ++row) {
                if (stage.alien01s[row][col]) {
                    rightMostAlienX = stage.alien01s[row][col]->x + stage.alien01s[row][col]->w;
                    found = true;
                    break;
                }
            }
            if (found) {break;}
            for (int row = 0; row < 2; ++row) {
                if (stage.alien02s[row][col]) {
                    rightMostAlienX = stage.alien02s[row][col]->x + stage.alien02s[row][col]->w;
                    found = true;
                    break;
                }
            }
            if (found) {break;}
            if (stage.alien03s[col]) {
                rightMostAlienX = stage.alien03s[col]->x + stage.alien03s[col]->w;
                break;
            }
        }

        // if alien does not die:
        // if cooldown is 0: move the alien
        // conditions for moving alien:
        // if left-most goes out of bounds after move, jump y direction, set direction to opposite
        // if right-most goes out of bounds after move, jump y direction, set direction to opposite
        bool reverseAlienDirection = false;
        // alien01s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien01s[row][col]) {
                    if (stage.alien01s[row][col]->health == 0) { // dies
                        free(stage.alien01s[row][col]);
                        stage.alien01s[row][col] = NULL;
                    } else { // does not die
                        if (alienJumpCooldown == 0) {
                            if (leftMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection < WIDTH_PADDING || rightMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection > SCREEN_WIDTH - WIDTH_PADDING) {
                                stage.alien01s[row][col]->y += ALIEN_JUMP_DISTANCE_Y;
                                reverseAlienDirection = true;
                            } else {
                                stage.alien01s[row][col]->x += ALIEN_JUMP_DISTANCE_X * alienDirection;
                            }
                        }
                    }
                }
            }
        }
        // alien02s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien02s[row][col] != NULL) {
                    if (stage.alien02s[row][col]->health == 0) {
                        free(stage.alien02s[row][col]);
                        stage.alien02s[row][col] = NULL;
                    } else {
                        if (alienJumpCooldown == 0) {
                            if (leftMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection < WIDTH_PADDING || rightMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection > SCREEN_WIDTH - WIDTH_PADDING) {
                                stage.alien02s[row][col]->y += ALIEN_JUMP_DISTANCE_Y;
                                reverseAlienDirection = true;
                            } else {
                                stage.alien02s[row][col]->x += ALIEN_JUMP_DISTANCE_X * alienDirection;
                            }
                        }
                        if (--stage.alien02s[row][col]->reload < 0) {
                            fireAlienBulletDown(stage.alien02s[row][col]);
                        }
                    }
                }
            }
        }
        // alien03s
        for (int i = 0; i < NUM_ALIENS_PER_ROW; ++i) {
            if (stage.alien03s[i] != NULL) {
                if (stage.alien03s[i]->health == 0) {
                    free(stage.alien03s[i]);
                    stage.alien03s[i] = NULL;
                } else {
                    if (alienJumpCooldown == 0) {
                        if (leftMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection < WIDTH_PADDING || rightMostAlienX + ALIEN_JUMP_DISTANCE_X * alienDirection > SCREEN_WIDTH - WIDTH_PADDING) {
                            stage.alien03s[i]->y += ALIEN_JUMP_DISTANCE_Y;
                            reverseAlienDirection = true;
                        } else {
                            stage.alien03s[i]->x += ALIEN_JUMP_DISTANCE_X * alienDirection;
                        }
                    }
                    if (--stage.alien03s[i]->reload < 0) { 
                        fireAlienBulletAtPlayer(stage.alien03s[i]);
                    }
                }
            }
        }
        if (reverseAlienDirection) {alienDirection *= -1;}
        if (alienJumpCooldown == 0) {alienJumpCooldown = ALIEN_JUMP_COOLDOWN;}
    }

    static void drawAliens() {
        // alien01s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien01s[row][col] != NULL) {Draw::drawToWindow(stage.alien01s[row][col]->texture, stage.alien01s[row][col]->x, stage.alien01s[row][col]->y);}
            }
        }
        // alien02s
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < NUM_ALIENS_PER_ROW; ++col) {
                if (stage.alien02s[row][col] != NULL) {Draw::drawToWindow(stage.alien02s[row][col]->texture, stage.alien02s[row][col]->x, stage.alien02s[row][col]->y);}
            }
        }
        // alien03s
        for (int i = 0; i < NUM_ALIENS_PER_ROW; ++i) {
            if (stage.alien03s[i] != NULL) {Draw::drawToWindow(stage.alien03s[i]->texture, stage.alien03s[i]->x, stage.alien03s[i]->y);}
        }
    }
}

namespace Stage {
    void initStage() {
        // zero the stage
        memset(&stage, 0, sizeof(World));

        // load all textures
        StageTextures::playerTexture = Draw::loadTexture("../gfx/Player.png");
        StageTextures::playerBulletTexture = Draw::loadTexture("../gfx/PlayerBullet.png");
        StageTextures::alien02BulletTexture = Draw::loadTexture("../gfx/Alien02Bullet.png");
        StageTextures::alien03BulletTexture = Draw::loadTexture("../gfx/Alien03Bullet.png");
        StageTextures::alien01Texture = Draw::loadTexture("../gfx/Alien01.png");
        StageTextures::alien02Texture = Draw::loadTexture("../gfx/Alien02.png");
        StageTextures::alien03Texture = Draw::loadTexture("../gfx/Alien03.png");

        // initialise stage objects
        StageUtil::initPlayer();
        StageUtil::initBullets();
        StageUtil::initAliens();
        StageUtil::initBarriers();
    }

    void updateStage() {
        StageUtil::updatePlayer();
        StageUtil::updateAliens();
        StageUtil::updateBullets();
        StageUtil::updateBarriers();
    }

    void drawStage() {
        StageUtil::drawPlayer();
        StageUtil::drawAliens();
        StageUtil::drawBullets();
        StageUtil::drawBarriers();
    }
}
