#pragma once 

#include "collision.hpp"
#include "cpp-utilz/math/vector2.hpp"

#define PHYSICS_LAYER_STATIC 1
#define PHYSICS_LAYER_DYNAMIC 2

namespace sm 
{
    namespace physics {
        struct aabb
        {
            utilz::vector2f position;
            utilz::vector2f scale;

            aabb(utilz::vector2f position, utilz::vector2f scale);
            aabb();

            static uint8_t check_aabb_overlap(aabb t1, aabb t2);
            static uint8_t check_aabb_vs_point(aabb r, utilz::vector2f p);

            sm::physics::collision dynamic_static_collision_aabb(aabb static_target);
            sm::physics::dynamic_collision dynamic_dynamic_collision_aabb(aabb dynamic_target);

            static sm::physics::collision collision_aabb(aabb r1, aabb r2);
        };
    }
}

