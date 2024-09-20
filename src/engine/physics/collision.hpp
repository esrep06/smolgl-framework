#pragma once

#include <inttypes.h>
#include "cpp-utilz/math/vector2.hpp"

namespace sm {
    namespace physics {

        enum COLLISION_SIDE {
            COLLISION_LEFT, 
            COLLISION_RIGHT, 
            COLLISION_TOP,
            COLLISION_BOTTOM,
            COLLISION_NONE
        };

        struct collision {
            uint8_t hit;
            utilz::vector2f collision_depth;
            COLLISION_SIDE side;
        };
    }
}

