#include "stage.h"

extern App app;
extern World stage;

namespace StageTextures {
    static SDL_Texture *playerTexture;
}

namespace StageUtil {
    static void initPlayer() {
        stage.player = (Entity *)malloc(sizeof(Entity));
        memset(stage.player, 0, sizeof(Entity));

        stage.player->texture = StageTextures::playerTexture;
        SDL_QueryTexture(stage.player->texture, NULL, NULL, &stage.player->w, &stage.player->h);
        stage.player->x = (SCREEN_WIDTH / 2) - (stage.player->w / 2);
        stage.player->y = SCREEN_HEIGHT - (stage.player->h) * (5.0f/2.0f);
        stage.player->health = 1;
        stage.player->side = SIDE::PLAYER;
    }

    static void updatePlayer() {
        if (stage.player != NULL) {
            stage.player->dx = 0;

            if (app.keyboard[SDL_SCANCODE_LEFT] && (stage.player->x - PLAYER_SPEED > 0))
                {stage.player->dx = -PLAYER_SPEED;}

            if (app.keyboard[SDL_SCANCODE_RIGHT] && (stage.player->x + stage.player->w + PLAYER_SPEED < SCREEN_WIDTH))
                {stage.player->dx = PLAYER_SPEED;}

            stage.player->x += stage.player->dx;
        }
    }

    static void drawPlayer() {
        // render player to back buffer
        Draw::drawToWindow(stage.player->texture, stage.player->x, stage.player->y);
    }
}

namespace Stage {
    void initStage() {
        // zero the stage
        memset(&stage, 0, sizeof(World));

        // load all textures
        StageTextures::playerTexture = Draw::loadTexture("../gfx/Player.png");

        // initialise stage objects
        StageUtil::initPlayer();
    }

    void updateStage() {
        StageUtil::updatePlayer();
    }

    void drawStage() {
        StageUtil::drawPlayer();
    }
}
