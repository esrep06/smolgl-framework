#include "engine/ecs/systems/transform_system/transform_system.hpp"

namespace sm
{
    transform_system::transform_system()
    {}

    void transform_system::add_entity(uint16_t e)
    {
        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in transform system\n", e), utilz::logger::WARNING);
            return;
        }

        transform default_transform = {
            .position = utilz::vector2f(0.0f, 0.0f),
            .scale = utilz::vector2f(32.0f)
        };
        
        m_entities.insert(std::pair<uint16_t, transform>(e, default_transform));
    }

    void transform_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Cant remove entity '{}' -> does not exist in transform system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void transform_system::update_transform(shader* shader, uint16_t e)
    {
        glm::mat4 trans;
        transform* entity_t = get_component(e);
        utilz::vector2f pos = entity_t->position;
        utilz::vector2f scale = entity_t->scale;
        glm::vec3 rotation_vec = glm::vec3(0.0f, 0.0f, entity_t->angle > 0 ? 1 : entity_t->angle < 0 ? -1 : 1);

        trans = glm::mat4(1.0f);

        trans = glm::translate(trans, glm::vec3(pos.x, pos.y, 0.0f));
        trans = glm::rotate(trans, glm::radians(entity_t->angle), rotation_vec);
        trans = glm::scale(trans, glm::vec3(scale.x, scale.y, 1.0f));

        shader->send_mat4(trans, "transform");
    }

    transform* transform_system::get_component(uint16_t e)
    {
        if (has_entity(e)) 
            return &m_entities.at(e); 
        else 
            utilz::logger::log(std::format("Cant get component of entity '{}' -> does not have a transform component!\n", e), utilz::logger::ERROR); 

        return nullptr;
    }

    uint8_t transform_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, transform>* transform_system::get_entities()
    { return &m_entities; }
}

