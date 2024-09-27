#include "ray.hpp"
#include <cmath>
#include <limits>
#include <utility>

namespace sm 
{
    namespace physics
    {
        void shoot_ray(struct ray ray, sm::scene* scene, ray_hit& hit)
        {
            sm::physics_system* system = scene->get_ecs()->get_physics_system();

            float tMin = 0.0f;
            float tMax = std::numeric_limits<float>::infinity();
            float closestT = std::numeric_limits<float>::infinity();
            uint8_t hit_found = 0;

            for (auto it = system->get_entities()->begin(); it != system->get_entities()->end(); it++)
            {   
                sm::physics::aabb aabb = it->second.aabb;

                float xMin = aabb.position.x;
                float xMax = aabb.position.x + aabb.scale.x;
                float yMin = aabb.position.y;
                float yMax = aabb.position.y + aabb.scale.y;
              
                float txMin, txMax;

                // X axis
                
                if (ray.direction.x != 0.0f) 
                {
                    txMin = (xMin - ray.origin.x) / ray.direction.x;
                    txMax = (xMax - ray.origin.x) / ray.direction.x;

                    if (txMin > txMax) std::swap(txMin, txMax);
                } 
                else 
                {
                    txMin = -std::numeric_limits<float>::infinity();
                    txMax = std::numeric_limits<float>::infinity();
                }

                // Y axis

                // For y-axis, check ray direction
                float tyMin, tyMax;
                if (ray.direction.y != 0.0f) 
                {
                    tyMin = (yMin - ray.origin.y) / ray.direction.y;
                    tyMax = (yMax - ray.origin.y) / ray.direction.y;

                    if (tyMin > tyMax) std::swap(tyMin, tyMax);  // Ensure tMin <= tMax
                } 
                else 
                {
                    // Ray is parallel to y-axis
                    tyMin = -std::numeric_limits<float>::infinity();
                    tyMax = std::numeric_limits<float>::infinity();
                }

                tMin = std::fmax(txMin, tyMin);
                tMax = std::fmin(txMax, tyMax);

                if (tMax < 0 || tMin > tMax) 
                    continue;

                if (tMin < closestT)
                {
                    closestT = tMin;

                    hit.point.x = ray.origin.x + tMin * ray.direction.x;
                    hit.point.y = ray.origin.y + tMin * ray.direction.y;
                    hit.dist = hit.point.eu_dist(ray.origin);
                    hit.e = it->first;

                    hit_found = 1;
                }
            }
            
            if (!hit_found)
            {
                hit.point.x = std::numeric_limits<float>::infinity();
                hit.point.y = std::numeric_limits<float>::infinity();
                hit.dist = 0.0f;
                hit.e = 0;
            }

            return;
        }
    }
}

