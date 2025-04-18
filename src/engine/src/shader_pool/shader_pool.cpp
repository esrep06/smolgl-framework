#include "engine/shader_pool/shader_pool.hpp"
#include <functional>

namespace sm
{
    shader_pool::shader_pool() {}

    sm::shader* shader_pool::add_shader(std::string vertex, std::string fragment, std::string identifier)
    {
        if (m_pool.find(identifier) != m_pool.end()) return NULL;

        m_pool.insert(std::pair<std::string, shader>(identifier, shader(vertex, fragment)));

        m_pool.at(identifier).load_and_compile();

        return retrieve_shader(identifier);
    }

    shader* shader_pool::retrieve_shader(std::string identifier)
    {  
        auto it = m_pool.find(identifier);

        if (it != m_pool.end()) return &it->second;

        return nullptr;
    }

    void shader_pool::initialize_shaders()
    {
        for (auto t = m_pool.begin(); t !=  m_pool.end(); t++)
        { t->second.load_and_compile(); }
    }

    void shader_pool::delete_shader(std::string identifier)
    {
        if (m_pool.find(identifier) == m_pool.end()) return;
        m_pool.erase(identifier);
    }

    std::map<std::string, shader>* shader_pool::get_pool()
    { return &m_pool; }
}

