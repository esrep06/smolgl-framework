#include "sprite_system.hpp"
#include "cpp-utilz/math/vector3.hpp"

namespace sm 
{
    sprite_system::sprite_system() 
    {}

    void sprite_system::add_entity(uint16_t e)
    {

        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in transform system\n", e), utilz::logger::WARNING);
            return;
        }

        m_entities.insert(std::pair<uint16_t, sprite>(e, sprite()));
        /* initialize(e); */
    }

    void sprite_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' does not exist in transform system\n", e), utilz::logger::ERROR);
            return;
        }

        sprite* spr = get_component(e);

        spr->ebo.free();
        spr->vao.free();
        spr->vbo.free();

        m_entities.erase(e);
    }

    void sprite_system::initialize(uint16_t e)
    {
        sprite* spr = &m_entities.at(e);
        
        if (spr->was_initialized) return;
        
        spr->was_initialized = 1;

        // Top Left Origin

        if (spr->config == 0 || spr->config & SPRITE_CONFIG_TOP_LEFT)
        {
            spr->vertices[0].position = utilz::vector3f(0.0f, 0.0f, 1.0f);
            spr->vertices[1].position = utilz::vector3f(1.0f, 0.0f, 1.0f);
            spr->vertices[2].position = utilz::vector3f(0.0f, 1.0f, 1.0f);
            spr->vertices[3].position = utilz::vector3f(1.0f, 1.0f, 1.0f);
        }

        // Centered Origin

        if (spr->config & SPRITE_CONFIG_CENTERED)
        {
            spr->vertices[0].position = utilz::vector3f(-0.5f, -0.5f, 1.0f);
            spr->vertices[1].position = utilz::vector3f(0.5f, -0.5f, 1.0f);
            spr->vertices[2].position = utilz::vector3f(-0.5f, 0.5f, 1.0f);
            spr->vertices[3].position = utilz::vector3f(0.5f, 0.5f, 1.0f);
        }

        spr->vertices[0].color = spr->color;
        spr->vertices[1].color = spr->color;
        spr->vertices[2].color = spr->color;
        spr->vertices[3].color = spr->color;

        spr->indices[0] = 0;
        spr->indices[1] = 1;
        spr->indices[2] = 2;

        spr->indices[3] = 2;
        spr->indices[4] = 3;
        spr->indices[5] = 1;

        spr->vao.generate();
        spr->vbo.generate();
        spr->ebo.generate();

        spr->vao.bind();
        spr->vbo.bind();
        spr->ebo.bind();

        spr->vbo.send_data(sizeof(struct sprite_vertex) * 4, spr->vertices, GL_STATIC_DRAW);
        spr->ebo.send_data(sizeof(uint32_t) * 6, spr->indices, GL_STATIC_DRAW);

        sm::gl_layer::enable_attrib_ptr(0, 3, GL_FLOAT, sizeof(struct sprite_vertex), (void*)0);
        gl_layer::enable_attrib_array(0);

        sm::gl_layer::enable_attrib_ptr(1, 4, GL_UNSIGNED_BYTE, sizeof(struct sprite_vertex), (void*)(3 * sizeof(float)));
        gl_layer::enable_attrib_array(1);

        spr->vbo.detach();
        spr->vao.detach();
        spr->ebo.detach();
    }

    void sprite_system::initialize_all()
    {
        for (auto it = m_entities.begin(); it != m_entities.end(); it++)
            initialize(it->first);
    }

    void sprite_system::draw(uint16_t e)
    {
        sprite* spr = get_component(e);

        if (spr == nullptr || spr->shader == nullptr) 
        {
            utilz::logger::log("Sprite: No shader set, setting default shader...\n", utilz::logger::ERROR);
            return;
        }

        spr->shader->use();

        spr->vbo.bind();
        spr->vao.bind();
        spr->ebo.bind();
  
        gl_layer::enable_attrib_array(0);
        gl_layer::enable_attrib_array(1);
  
        gl_layer::draw_indexed(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  
        gl_layer::disable_attrib_array(0);
        gl_layer::disable_attrib_array(1);
  
        spr->vbo.detach();
        spr->vao.detach();
        spr->ebo.detach();

        spr->shader->detach();
    }

    uint8_t sprite_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    sprite*  sprite_system::get_component(uint16_t e)
    {
        if (has_entity(e))
            return &m_entities.at(e); 
        else
            utilz::logger::log(std::format("Entity '{}' does not have a sprite component!\n", e), utilz::logger::ERROR); 

        return nullptr;
    }

    std::map<uint16_t, sprite>* sprite_system::get_entities()
    { return &m_entities; }
}

