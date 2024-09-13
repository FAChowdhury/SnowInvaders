#include "main.h"

App app;
World stage;

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));
    Init::initSDL();
    atexit(Init::cleanup);

    Stage::initStage();

    long then = SDL_GetTicks();
    float remainder = 0;
    while (1) {
        Draw::prepareScene();
        Input::doInput();
        Stage::updateStage();
        Stage::drawStage();
        Draw::presentScene();

        Helper::capFrameRate(&then, &remainder);
    }

    return 0;
}