#include "animator_system.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include <locale>

namespace sm
{
    animator_system::animator_system()
    {}

    void animator_system::add_entity(uint16_t e)
    {
        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in animator system\n", e), utilz::logger::WARNING);
            return;
        }
        
        m_entities.insert(std::pair<uint16_t, animator>(e, animator()));
    }

    void animator_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Cant remove entity '{}' -> does not exist in animator system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void animator_system::update_animation(uint16_t e, textured_sprite* sprite)
    {
        animator* anim = get_component(e);

        if (anim->current_animation == nullptr || anim->current_animation->texture == nullptr) return;

        sprite->texture = anim->current_animation->texture;

        utilz::vector2f texture_res = utilz::vector2f(sprite->texture->get_width(), sprite->texture->get_height());

        // Switch sprite and reset timer
        if (anim->current_animation->switch_timer.update())
        {
            sprite->vbo.bind();

            sprite->vertices[0].uv = utilz::vector2f((anim->current_animation->w / texture_res.x) * anim->current_animation->x, 1.0f);
            sprite->vertices[1].uv = utilz::vector2f(sprite->vertices[0].uv.x + (anim->current_animation->w / texture_res.x), 1.0f);
            sprite->vertices[2].uv = utilz::vector2f(sprite->vertices[0].uv.x, 0.0f);
            sprite->vertices[3].uv = utilz::vector2f(sprite->vertices[1].uv.x, 0.0f);

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct textured_sprite_vertex) * 4, sprite->vertices);
    
            sprite->vbo.detach();

            anim->current_animation->x += 1;

            if (anim->current_animation->x >= anim->current_animation->x_bound)
                anim->current_animation->x = 0;

            anim->current_animation->switch_timer.reset();
        }
    }

    void animator_system::enable_animation(animator* anim, std::string name)
    {
        for (size_t i = 0; i < anim->animations.size(); i++)
        {
            animation* a = &anim->animations[i];

            if (!a->name.compare(name))
            {
                anim->current_animation = a;
                anim->current_animation->switch_timer.timer = anim->current_animation->switch_timer.target;
                return;
            }
        }

        utilz::logger::log("Animation: '" + name + "' not found!!!\n", utilz::logger::ERROR);
        return;
    }

    int32_t animator_system::add_animation(animator *anim, animation a)
    {
        anim->animations.push_back(a);

        return anim->animations.size() - 1;
    }

    animator* animator_system::get_component(uint16_t e)
    {
        if (has_entity(e)) 
            return &m_entities.at(e); 
        else 
            utilz::logger::log(std::format("Cant get component of entity '{}' -> does not have a animator component!\n", e), utilz::logger::ERROR); 

        return nullptr;
    }

    uint8_t animator_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, animator>* animator_system::get_entities()
    { return &m_entities; }
}

