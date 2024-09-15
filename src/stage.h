#pragma once

#include <SDL2/SDL.h>
#include "structs.h"
#include "draw.h"
#include "defs.h"
#include "util.h"

namespace Stage {
    void initStage();
    void updateStage();
    void drawStage();
}