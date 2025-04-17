#include "engine/font_pool/font_pool.hpp"
#include <functional>

namespace sm
{
    font_pool::font_pool() {}

    void font_pool::add_font(sm::shader* shader, std::string path, float scale, std::string identifier)
    {
        if (m_pool.find(identifier) != m_pool.end()) return;

        m_pool.insert(std::pair<std::string, font>(identifier, font(shader, path, scale)));
    }

    font* font_pool::retrieve_font(std::string identifier)
    {  
        auto it = m_pool.find(identifier);

        if (it != m_pool.end()) return &it->second;

        return nullptr;
    }

    std::map<std::string, font>* font_pool::get_pool()
    { return &m_pool; }
}

