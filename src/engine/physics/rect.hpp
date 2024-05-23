#pragma once 

#include "../ecs/components/components.hpp"

namespace sm 
{
    struct rect
    {
        utilz::vector2f position;
        utilz::vector2f scale;

        rect(utilz::vector2f position, utilz::vector2f scale);
        rect(sm::transform t);
        rect();
        uint8_t collision_aabb(rect target);
    };
}

