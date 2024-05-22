#include "texture_pool.hpp"

namespace sm 
{
    texture_pool::texture_pool() {}

    void texture_pool::add_texture(std::string path, std::string identifier)
    {
        if (m_pool.find(identifier) != m_pool.end()) return;

        m_pool.insert(std::pair<std::string, texture>(identifier, texture(path)));

        m_pool.at(identifier).load();    
    }

    void texture_pool::initialize_textures()
    {
        for (auto t = m_pool.begin(); t !=  m_pool.end(); t++)
        { t->second.load(); }
    }

    texture* texture_pool::retrieve_texture(std::string identifier)
    {
        auto it = m_pool.find(identifier);

        if (it != m_pool.end()) return &it->second;

        return nullptr;
    }

    void texture_pool::delete_texture(std::string identifier)
    {
        if (m_pool.find(identifier) == m_pool.end()) return;
        m_pool.erase(identifier);
    }

    std::map<std::string, texture>* texture_pool::get_pool()
    { return &m_pool; }
}

