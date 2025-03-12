#include "engine/ecs/systems/physics_system/physics_system.hpp"
#include "engine/ecs/components/components.hpp"

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
            .aabb = physics::aabb(utilz::vector2f(0.0f), utilz::vector2f(0.0f)),
            .dynamic_collision = physics::dynamic_collision(),
            .custom = 0,
            .grounded = 0,
            .collision_layer = 0,
            .collision_mask = 0,
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

        sm::physics::aabb horizontal_aabb;
        sm::physics::aabb vertical_aabb;
        sm::physics::aabb ground_check;
        sm::physics::collision vertical_collision;
        sm::physics::collision horizontal_collision;

        body->aabb.position.x = t->position.x;
        body->aabb.position.y = t->position.y; 

        // To set custom scales on the hitboxes 
        if (!body->custom)
        {
            body->aabb.scale = t->scale;
            horizontal_aabb.scale = t->scale;
            vertical_aabb.scale = t->scale;
        } else 
        {
            horizontal_aabb.scale = body->aabb.scale;
            vertical_aabb.scale = body->aabb.scale;
        }

        ground_check.position = utilz::vector2f(body->aabb.position.x, body->aabb.position.y + body->aabb.scale.y);
        ground_check.scale = utilz::vector2f(body->aabb.scale.x, body->aabb.scale.y * 0.5f);

        body->dynamic_collision.hit = 0;
        body->grounded = 0;

        if (body->body_type == BODY_DYNAMIC)
            body->velocity.y += WORLD_GRAVITY * sm::time::delta_time() * sm::time::delta_time();

        for (auto it = get_entities()->begin(); it != get_entities()->end(); it++) 
        {
            // Prevents collision within the same entity and collision checks with non dynamic entities 
            if (it->first == e) continue;

            physics_body* target_body = get_component(it->first);

            if (!(body->collision_mask & target_body->collision_layer)) continue;

            if (target_body->body_type == BODY_STATIC) 
            {
                horizontal_aabb.position = utilz::vector2f(body->aabb.position.x + body->velocity.x, body->aabb.position.y);

                horizontal_collision = horizontal_aabb.dynamic_static_collision_aabb(target_body->aabb);

                if (horizontal_collision.hit) 
                    body->velocity.x = 0.0f;;

                vertical_aabb.position = utilz::vector2f(body->aabb.position.x, body->aabb.position.y + body->velocity.y);

                vertical_collision = vertical_aabb.dynamic_static_collision_aabb(target_body->aabb);

                sm::physics::collision ground_collision = ground_check.dynamic_static_collision_aabb(target_body->aabb);

                if (ground_collision.hit) 
                    body->grounded = 1;

                if (vertical_collision.hit) 
                    body->velocity.y = 0.0f;
            } 

            // Gets collisions between dynamic objects, these dont stop movement, just register a hit

            if (target_body->body_type == BODY_DYNAMIC && body->body_type == BODY_KINEMATIC || 
                    body->body_type == BODY_DYNAMIC || target_body->body_type == BODY_KINEMATIC)                 
            {
                sm::physics::dynamic_collision collision = body->aabb.dynamic_dynamic_collision_aabb(target_body->aabb);

                if (collision.hit) {
                    body->dynamic_collision = collision; 
                    continue;
                } 

                /* if (target_body->collision_mask & body->collision_layer) */
                /*     target_body->dynamic_collision = target_body->aabb.dynamic_dynamic_collision_aabb(body->aabb); */
            }
        }

        if (body->body_type != BODY_STATIC) {
            t->position.x += body->velocity.x;
            t->position.y += body->velocity.y;
        }

    }

    // TODO: Fix this 
    void physics_system::dynamic_body_move(utilz::vector2f delta, transform* t, physics_body* body)
    {
        body->velocity.x = delta.x;
        body->velocity.y = delta.y;

        t->position.x += body->velocity.x * sm::time::delta_time();
        t->position.y += body->velocity.y * sm::time::delta_time();
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

