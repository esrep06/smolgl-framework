#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "../scene/scene.hpp"

#define RAY_ADVANCE 3.0f

namespace sm
{
    namespace physics 
    {
        struct ray_hit 
        {
            utilz::vector2f point;
            float dist;
            sm::entity entity;
        };

        struct ray 
        {
            utilz::vector2f origin;
            utilz::vector2f direction;

            float distance;
        };

        void shoot_ray(struct ray ray, sm::scene* scene, ray_hit& hit, uint8_t mask);
    }
}

