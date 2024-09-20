#pragma once 

#include "collision.hpp"

namespace sm 
{
    namespace physics {
        struct aabb
        {
            utilz::vector2f position;
            utilz::vector2f scale;

            aabb(utilz::vector2f position, utilz::vector2f scale);
            aabb();

            sm::physics::collision dynamic_static_collision_aabb(aabb static_target);
            static sm::physics::collision collision_aabb(aabb r1, aabb r2);
            /* uint8_t collision_aabb(rect target); */
        };
    }
}

