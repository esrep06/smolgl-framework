#include "rect.hpp"

namespace sm 
{
    rect::rect(utilz::vector2f position, utilz::vector2f scale)
        : position(position), scale(scale) {}

    rect::rect(sm::transform t)
    {
        position = t.position;
        scale = t.scale;
    }

    rect::rect()
    {}

    uint8_t rect::collision_aabb(rect target)
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
}

