#include "ecs.hpp"
#include "components/components.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "systems/transform_system/transform_system.hpp"
#include <cstddef>

namespace sm
{
    
    ecs::ecs()
    {
        // Push all entity ids to the available queue
        for (int i = 0; i < ECS_MAX_ENTITIES; i++)
            m_available_entity_queue.push(i);
    }

    entity ecs::create_entity(component components)
    {
        // Check if we havent reached the max number of entities
        if (get_entity_num() > ECS_MAX_ENTITIES) 
        {
            utilz::logger::log("Entity max number reached!!\n", utilz::logger::ERROR);
            return ECS_MAX_ENTITIES;
        };

        // Get first available entity
        entity e = m_available_entity_queue.front();

        // Push entity to stackuu
        m_entities.push_back(e);

        add_components(e, components);

        utilz::logger::log("Created entity '" + std::to_string(e) + "' with: " + std::bitset<8>(components).to_string() + '\n');

        m_available_entity_queue.pop();

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
    }

    void ecs::clear_remove_queue()
    {
        for (int i = m_remove_queue.size() - 1; i >= 0; i--)
        {
            entity e = m_remove_queue[i];

            utilz::logger::log(std::format("Removed entity '{}'\n", e));

            // Get all components owned by the entity and apply the remove entity function

            if (m_transform_system.has_entity(e))
            {
                utilz::logger::log("    Removing transform...\n");
                m_transform_system.remove_entity(e);
            }

            if (m_sprite_system.has_entity(e))
            {
                utilz::logger::log("    Removing sprite...\n");
                m_sprite_system.remove_entity(e);
            }

            if (m_textured_sprite_system.has_entity(e))
            {       
                utilz::logger::log("    Removing textured sprite...\n");
                m_textured_sprite_system.remove_entity(e);
            }

            if (m_behavior_system.has_entity(e))
            {
                utilz::logger::log("    Removing behavior...\n");
                m_behavior_system.remove_entity(e);
            }   

            auto it = std::find(m_entities.begin(), m_entities.end(), e);
          
            // Erase the entity from the entity vector 

            m_entities.erase(it);

            // Add entity id to available queue 
            m_available_entity_queue.push(e);
        }
        
        // Clear queue of entities 
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

        if (c & BEHAVIOR)
            return m_behavior_system.get_component(e);
        
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

        if (components & BEHAVIOR)
            m_behavior_system.add_entity(e); 
    }

    uint8_t ecs::get_entity_components(entity e)
    {
        uint8_t components = 0;

        if (m_transform_system.has_entity(e))
            components |= TRANSFORM;
        if (m_sprite_system.has_entity(e))
            components |= SPRITE;
        if (m_textured_sprite_system.has_entity(e))
            components |= TEXTURED_SPRITE;
        if (m_behavior_system.has_entity(e))
            components |= BEHAVIOR;

        return components;
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

    behavior_system* ecs::get_behavior_system()
    { return &m_behavior_system; }
}

