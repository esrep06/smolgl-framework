#include "ecs.hpp"
#include "components/components.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "systems/transform_system/transform_system.hpp"

namespace sm
{
    
    ecs::ecs()
        : m_entity_num(0)
    {}

    entity ecs::create_entity(component components)
    {
        // Check if we havent reached the max number of entities
        if (get_entity_num() > ECS_MAX_ENTITIES) 
        {
            utilz::logger::log("Entity max number reached!!\n", utilz::logger::ERROR);
            return ECS_MAX_ENTITIES;
        };

        // Find an empty slot
        auto it = std::find(m_entities.begin(), m_entities.end(), 0);

        uint32_t index = it - m_entities.begin();

        // Entity id's start at 1 
        entity e = index + 1;

        // Push entity to stack
        m_entities.push_back(e);

        add_components(e, components);

        // Return 
        return e;
    }

    void ecs::remove_entity(entity e)
    {
        // Find if entity exists
        auto it = std::find(m_entities.begin(), m_entities.end(), e);

        if (it == m_entities.end()) 
        {
            utilz::logger::log("Trying to remove non-existent entity\n", utilz::logger::ERROR);
            return;
        }

        if (m_transform_system.has_entity(e))
            m_transform_system.remove_entity(e);

        // Erase if it does 
        m_entities.erase(it);
    }

    void* ecs::get_component(entity e, component c)
    {
        if (c & TRANSFORM)
            return m_transform_system.get_component(e);

        if (c & SPRITE)
            return m_sprite_system.get_component(e);
        
        utilz::logger::log(std::format("Error retrieving component: entity '{}'\n", e), utilz::logger::ERROR);

        return nullptr;
    }

    void ecs::add_components(entity e, component components)
    {
        if (components & TRANSFORM)
            m_transform_system.add_entity(e);

        if (components & SPRITE)
            m_sprite_system.add_entity(e);
    }

    uint32_t ecs::get_entity_num()
    { return m_entities.size(); }

    std::vector<entity>* ecs::get_entities()
    { return &m_entities; }

    transform_system* ecs::get_transform_system()
    { return &m_transform_system; }

    sprite_system* ecs::get_sprite_system()
    { return &m_sprite_system; }
}

