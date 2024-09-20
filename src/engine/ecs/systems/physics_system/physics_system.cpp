#include "physics_system.hpp"

#include "physics_system.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "cpp-utilz/math/vector2.hpp"

#include "../../../../../include/cpp-utilz/math/vector2.hpp"
#include "../../../../../include/cpp-utilz/logger/logger.hpp"

#include <format>

namespace sm
{
    physics_system::physics_system()
    {}

    void physics_system::add_entity(uint16_t e)
    {
        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in physics system\n", e), utilz::logger::WARNING);
            return;
        }

        physics_body default_physics = {
            .velocity = utilz::vector2f(0.0f),
            .horizontal_aabb = physics::aabb(utilz::vector2f(0.0f), utilz::vector2f(0.0f)),
            .vertical_aabb = physics::aabb(utilz::vector2f(0.0f), utilz::vector2f(0.0f)),
            .aabb = physics::aabb(utilz::vector2f(0.0f), utilz::vector2f(0.0f)),
            .body_type = BODY_DYNAMIC,
        };
        
        m_entities.insert(std::pair<uint16_t, physics_body>(e, default_physics));
    }

    void physics_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Cant remove entity '{}' -> does not exist in physics system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void physics_system::update_physics(sm::transform* t, uint16_t e)
    {
        physics_body* body = get_component(e);

        body->horizontal_aabb.scale = t->scale;
        body->vertical_aabb.scale = t->scale;
        body->aabb.scale = t->scale;
        body->aabb.position.x = t->position.x;
        body->aabb.position.y = t->position.y; 
        body->grounded = 0;
        body->ground_check.position = utilz::vector2f(t->position.x, t->position.y + t->scale.y);
        body->ground_check.scale = utilz::vector2f(t->scale.x, t->scale.y * 0.5f);

        for (auto it = get_entities()->begin(); it != get_entities()->end(); it++) 
        {
            // Prevents collision within the same entity and collision checks with non dynamic entities 
            if (it->first == e || body->body_type != BODY_DYNAMIC) continue;

            physics_body* target_body = get_component(it->first);

            body->horizontal_aabb.position = utilz::vector2f(t->position.x + body->velocity.x, t->position.y);
            
            body->horizontal_collision = body->horizontal_aabb.dynamic_static_collision_aabb(target_body->aabb);

            if (body->horizontal_collision.hit) 
                body->velocity.x = 0.0f;;

            body->vertical_aabb.position = utilz::vector2f(t->position.x, t->position.y + body->velocity.y);

            body->vertical_collision = body->vertical_aabb.dynamic_static_collision_aabb(target_body->aabb);

            sm::physics::collision ground_collision = body->ground_check.dynamic_static_collision_aabb(target_body->aabb);

            if (ground_collision.hit) 
                body->grounded = 1;

            if (body->vertical_collision.hit) { 
                body->velocity.y = 0.0f;
            } 
        }

        t->position.x += body->velocity.x;
        t->position.y += body->velocity.y;
    }

    physics_body* physics_system::get_component(uint16_t e)
    {
        if (has_entity(e)) 
            return &m_entities.at(e); 
        else 
            utilz::logger::log(std::format("Cant get component of entity '{}' -> does not have a physics component!\n", e), utilz::logger::ERROR); 

        return nullptr;
    }

    uint8_t physics_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, physics_body>* physics_system::get_entities()
    { return &m_entities; }
}

