#include "engine/ecs/systems/behavior_system/behavior_system.hpp"

namespace sm
{
    behavior_system::behavior_system()
    {}

    void behavior_system::add_entity(uint16_t e)
    {
        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in behavior system\n", e), utilz::logger::WARNING);
            return;
        }
        
        m_entities.insert(std::pair<uint16_t, behavior>(e, behavior()));

        get_component(e)->entity = e;
    }

    void behavior_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Cant remove entity '{}' -> does not exist in behavior system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void behavior_system::start(uint16_t e)
    {
        sm::behavior* b = get_component(e);

        if (b->start && !b->was_initialized)
            b->start(b);

        b->was_initialized = 1;
    }

    void behavior_system::update(uint16_t e)
    {
        sm::behavior* b = get_component(e);

        if (b->update)
            b->update(b);
    }
    
    behavior* behavior_system::get_component(uint16_t e)
    {
        if (has_entity(e)) 
            return &m_entities.at(e); 
        else 
            utilz::logger::log(std::format("Cant get component of entity '{}' -> does not have a behavior component!\n", e), utilz::logger::ERROR); 

        return nullptr;
    }

    uint8_t behavior_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, behavior>* behavior_system::get_entities()
    { return &m_entities; }
}

