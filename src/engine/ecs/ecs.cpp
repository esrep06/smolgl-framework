#include "ecs.hpp"
#include "components/components.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "systems/transform_system/transform_system.hpp"
#include <cstddef>

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

        utilz::logger::log("Created entity with: " + std::bitset<8>(components).to_string() + '\n');

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

        // Send to our queue if it does  
        m_remove_queue.push_back(e);

        /* m_entities.erase(it); */
    }

    void ecs::clear_remove_queue()
    {
        for (entity e : m_remove_queue)
        {
            utilz::logger::log(std::format("Removed entity '{}'\n", e));

            if (m_transform_system.has_entity(e))
            {
                utilz::logger::log("    Removing transform...\n");
                m_transform_system.remove_entity(e);
            }

            if (m_sprite_system.has_entity(e))
            {
                m_sprite_system.remove_entity(e);
                utilz::logger::log("    Removing sprite...\n");
            }

            if (m_textured_sprite_system.has_entity(e))
            {       
                m_textured_sprite_system.remove_entity(e);
                utilz::logger::log("    Removing textured sprite...\n");
            }

            auto it = std::find(m_entities.begin(), m_entities.end(), e);
            m_entities.erase(it);
        }

        m_remove_queue.clear();
    }

    void* ecs::get_component(entity e, component c)
    {
        if (c & TRANSFORM)
            return m_transform_system.get_component(e);

        if (c & SPRITE)
            return m_sprite_system.get_component(e);
        
        if (c & TEXTURED_SPRITE)
            return m_textured_sprite_system.get_component(e);
        
        utilz::logger::log(std::format("Error retrieving component: entity '{}'\n", e), utilz::logger::ERROR);

        return nullptr;
    }

    void ecs::add_components(entity e, component components)
    {
        if (components & TRANSFORM)
            m_transform_system.add_entity(e);

        if (components & SPRITE)
        {
            // We need a transform component if we are going to draw so we force adding it in case the user didnt add it 
            m_sprite_system.add_entity(e);

            if (!m_transform_system.has_entity(e))
                m_transform_system.add_entity(e);
        }
        
        if (components & TEXTURED_SPRITE)
        {
            m_textured_sprite_system.add_entity(e);

            if (!m_transform_system.has_entity(e))
                m_transform_system.add_entity(e);
        }
    }

    void ecs::free()
    {
        utilz::logger::log("Freeing all entity memory!\n");

        for (uint16_t i = 0; i < m_entities.size(); i++)
            remove_entity(i);

        clear_remove_queue();
    }

    uint32_t ecs::get_entity_num()
    { return m_entities.size(); }

    std::vector<entity>* ecs::get_entities()
    { return &m_entities; }

    transform_system* ecs::get_transform_system()
    { return &m_transform_system; }

    sprite_system* ecs::get_sprite_system()
    { return &m_sprite_system; }

    textured_sprite_system* ecs::get_textured_sprite_system()
    { return &m_textured_sprite_system; }
}

