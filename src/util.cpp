#include "util.h"

namespace Util {
    bool collision(Rect *r1, Rect *r2) {
        return (MAX(r1->x, r2->x) < MIN(r1->x + r1->w, r2->x + r2->w)) && (MAX(r1->y, r2->y) < MIN(r1->y + r1->h, r2->y + r2->h));
    }
}