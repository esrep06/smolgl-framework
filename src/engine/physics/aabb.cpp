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

        uint8_t aabb::check_aabb_overlap(aabb t1, aabb t2) 
        {
            // If there is overlap along both axes, then there is a collision

            // Check for overlap along the x-axis
            bool overlapX = (t1.position.x + t1.scale.x >= t2.position.x) && 
                (t2.position.x + t2.scale.x >= t1.position.x);

            // Check for overlap along the y-axis
            bool overlapY = (t1.position.y + t1.scale.y >= t2.position.y) &&
                (t2.position.y + t2.scale.y >= t1.position.y);

            return overlapX && overlapY; 
        }

        uint8_t aabb::check_aabb_vs_point(aabb r, utilz::vector2f p)
        {
            return (p.x >= r.position.x && 
                    p.x <= r.position.x + r.scale.x && 
                    p.y >= r.position.y && 
                    p.y <= r.position.y + r.scale.y);
        }

        sm::physics::dynamic_collision aabb::dynamic_dynamic_collision_aabb(aabb dynamic_target) 
        {
            sm::physics::dynamic_collision response;
            response.hit = check_aabb_overlap(*this, dynamic_target);

            return response;
        }

        sm::physics::collision aabb::dynamic_static_collision_aabb(aabb static_target)
        {
            sm::physics::collision response;

            response.hit = check_aabb_overlap(*this, static_target);
            
            if (!response.hit) {
                response.side = COLLISION_NONE;
                response.collision_depth = 0;
                return response;
            }

            response.collision_depth.x = std::min(scale.x, static_target.scale.x) - 
                fabs(position.x - static_target.position.x);
            response.collision_depth.y = std::min(scale.y, static_target.scale.y) - 
                fabs(position.y - static_target.position.y);

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
    }
}

