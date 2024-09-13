#pragma once

#include <SDL2/SDL.h>

namespace Helper {
    void capFrameRate(long *then, float *remainder);
}