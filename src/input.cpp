#include "input.h"

extern App app;

namespace InputUtil {
    static void doKeyDown(SDL_KeyboardEvent *event) {
        if (!event->repeat && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
            app.keyboard[event->keysym.scancode] = true;
        }
    }

    static void doKeyUp(SDL_KeyboardEvent *event) {
        if (!event->repeat && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
            app.keyboard[event->keysym.scancode] = false;
        }
    }
}

namespace Input {
    void doInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    exit(0);
                } break;
                case SDL_KEYUP: {
                    InputUtil::doKeyUp(&event.key);
                } break;
                case SDL_KEYDOWN: {
                    InputUtil::doKeyDown(&event.key);
                } break;
                default: 
                    break;
            }
        }
    }
};