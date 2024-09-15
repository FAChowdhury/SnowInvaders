#pragma once

#include <SDL2/SDL.h>
#include "defs.h"
#include "structs.h"

namespace Util {
    bool collision(Rect *r1, Rect *r2);
}