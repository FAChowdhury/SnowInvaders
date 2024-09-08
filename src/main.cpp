#include "main.h"

App app;

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));
    Init::initSDL();
    atexit(Init::cleanup);

    while (1) {
        Draw::prepareScene();
        Input::doInput();
        Draw::presentScene();

        SDL_Delay(16);
    }

    return 0;
}