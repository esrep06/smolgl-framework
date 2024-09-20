#include "aabb.hpp"
#include "collision.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include <algorithm>

namespace sm 
{
    namespace physics {
        aabb::aabb(utilz::vector2f position, utilz::vector2f scale)
            : position(position), scale(scale) {}

        /*
        aabb::aabb(sm::transform t)
        {
            position = t.position;
            scale = t.scale;
        }
        */

        aabb::aabb()
        {}

        sm::physics::collision aabb::dynamic_static_collision_aabb(aabb static_target)
        {
            // Check for overlap along the x-axis
            bool overlapX = (position.x + scale.x >= static_target.position.x) && 
                (static_target.position.x + static_target.scale.x >= position.x);

            // Check for overlap along the y-axis
            bool overlapY = (position.y + scale.y >= static_target.position.y) &&
                (static_target.position.y + static_target.scale.y >= position.y);

            // If there is overlap along both axes, then there is a collision
            sm::physics::collision response;

            response.hit = overlapX && overlapY;

            if (!response.hit) {
                response.side = COLLISION_NONE;
                response.collision_depth = 0;
                return response;
            }

            response.collision_depth.x = std::min(scale.x, static_target.scale.x) - fabs(position.x - static_target.position.x);
            response.collision_depth.y = std::min(scale.y, static_target.scale.y) - fabs(position.y - static_target.position.y);

            // Collision is to be resolved on the x axis
            if (response.collision_depth.x < response.collision_depth.y) 
            {
                // Our dynamic object is to the left of the object 
                if (position.x < static_target.position.x) 
                    response.side = COLLISION_LEFT;
                else 
                    response.side = COLLISION_RIGHT;
            }
            else // Collision is to be resolved on the y axis 
            {
                // Our dynamic object is above the object
                if (position.y < static_target.position.y)
                    response.side = COLLISION_TOP;
                else 
                    response.side = COLLISION_BOTTOM;
            }

            return response;
        }


        /*
        uint8_t aabb::collision_aabb(aabb target)
        {
            // Check for overlap along the x-axis
            bool overlapX = (position.x + scale.x >= target.position.x) && 
                (target.position.x + target.scale.x >= position.x);

            // Check for overlap along the y-axis
            bool overlapY = (position.y + scale.y >= target.position.y) &&
                (target.position.y + target.scale.y >= position.y);

            // If there is overlap along both axes, then there is a collision
            return overlapX && overlapY;
        }
        */
    }
}

