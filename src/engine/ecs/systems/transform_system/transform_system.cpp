#include "transform_system.hpp"
#include "cpp-utilz/logger/logger.hpp"

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
        
        m_entities.insert(std::pair<uint16_t, transform>(e, transform()));
    }

    void transform_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' does not exist in transform system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void transform_system::update_transform(shader* shader)
    {
        for (auto it = m_entities.begin(); it != m_entities.end(); it++)
        {
            glm::mat4 trans = glm::mat4(1.0f);

            trans = glm::translate(trans, glm::vec3(it->second.position.x, it->second.position.y, 0.0f));
            trans = glm::scale(trans, glm::vec3(it->second.scale.x, it->second.scale.y, 1.0f));

            shader->use();
            shader->send_mat4(trans, "transform");
        }
    }

    uint8_t transform_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, transform>* transform_system::get_entities()
    { return &m_entities; }
}

