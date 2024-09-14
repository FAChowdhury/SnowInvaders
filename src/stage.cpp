#include "stage.h"

extern App app;
extern World stage;

namespace StageTextures {
    static SDL_Texture *playerTexture;
    static SDL_Texture *playerBulletTexture;
}

namespace StageUtil {
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

    static void updatePlayer() {
        if (stage.player != NULL) {
            // player movement
            stage.player->dx = 0;

            if (app.keyboard[SDL_SCANCODE_LEFT] && (stage.player->x - PLAYER_SPEED > 0))
                {stage.player->dx = -PLAYER_SPEED;}

            if (app.keyboard[SDL_SCANCODE_RIGHT] && (stage.player->x + stage.player->w + PLAYER_SPEED < SCREEN_WIDTH))
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

            // if bullet is outside of window, free it
            if (curr->x > SCREEN_WIDTH || curr->y > SCREEN_HEIGHT || curr->x < -curr->w || curr->y < -curr->h) {
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
            Draw::drawToWindow(curr->texture, curr->x, curr->y);
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

        // initialise stage objects
        StageUtil::initPlayer();
        StageUtil::initBullets();
    }

    void updateStage() {
        StageUtil::updatePlayer();
        StageUtil::updateBullets();
    }

    void drawStage() {
        StageUtil::drawPlayer();
        StageUtil::drawBullets();
    }
}
