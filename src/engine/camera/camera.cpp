#include "camera.hpp"

namespace sm
{
    camera::camera(utilz::vector2f pos, utilz::vector2 bounds)
        : pos(pos), bounds(bounds)
    { }

    void camera::update()
    {
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 pos = glm::vec3(this->pos.x, this->pos.y, 1.0f);

        m_proj = glm::mat4(1.0f);
        m_proj = glm::ortho(0.0f, (float)bounds.x, (float)bounds.y, 0.0f, 0.0f, 100.0f);

        m_view = glm::mat4(1.0f);
        m_view = glm::lookAt(pos, pos + front, up); 
    }

    void camera::send_matrices(shader* shader, std::string p_name, std::string v_name)
    {
        shader->send_mat4(get_view(), v_name.c_str());
        shader->send_mat4(get_proj(), p_name.c_str());
    }

    glm::mat4 camera::get_view()
    { return m_view; }

    glm::mat4 camera::get_proj()
    { return m_proj; }
}

