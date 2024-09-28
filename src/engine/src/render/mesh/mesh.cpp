#include "engine/render/mesh/mesh.hpp"
#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include <format>

namespace sm
{
    namespace render
    {
        void mesh::init(GLenum mode) 
        {
            if (m_was_initialized) return;

            m_vao.generate();
            m_vbo.generate();

            m_vao.bind();
            m_vbo.bind();

            m_vbo.send_data(sizeof(struct vertex) * m_vertex_num, m_vertices, mode);

            sm::gl_layer::enable_attrib_ptr(0, 2, GL_FLOAT, sizeof(struct vertex), (void*)0); 
            sm::gl_layer::enable_attrib_array(0);
            sm::gl_layer::enable_attrib_ptr(1, 4, GL_UNSIGNED_BYTE, sizeof(struct vertex), (void*)(sizeof(float) * 2));  
            sm::gl_layer::enable_attrib_array(1);

            m_vao.detach();
            m_vbo.detach();
            
            m_was_initialized = 1;

            utilz::logger::log(std::format("Created mesh with {} vertices\n", m_vertex_num));
        } 

        void mesh::render(GLenum mode, GLint first, GLsizei count) 
        {
            m_vao.bind();
            m_vbo.bind();

            sm::gl_layer::enable_attrib_array(0);
            sm::gl_layer::enable_attrib_array(1);

            sm::gl_layer::draw_simple(mode, first, count);

            m_vao.detach();
            m_vbo.detach();

            sm::gl_layer::disable_attrib_array(0);
            sm::gl_layer::disable_attrib_array(1);
        }

        void mesh::free() 
        {
            utilz::logger::log(std::format("Freeing mesh data, {} vertices deleted\n", m_vertex_num));

            delete m_vertices;
            m_vao.free();
            m_vbo.free();
        } 

        sm::gl_layer::vao* mesh::get_vao() { return &m_vao; }
        sm::gl_layer::vbo* mesh::get_vbo() { return &m_vbo; }

        vertex* mesh::get_vertices() { return m_vertices; }
        size_t mesh::get_vertex_num() { return m_vertex_num; } 

        void mesh::allocate_vertices(size_t vertex_num) 
        {
            m_vertices = new vertex[vertex_num];
            m_vertex_num = vertex_num;
        }

        void mesh::update_mesh() 
        {

            m_vbo.bind();
            m_vbo.update_data(0, sizeof(struct vertex) * m_vertex_num, get_vertices());
            m_vbo.detach();
        }

        uint8_t mesh::was_initialized() { return m_was_initialized; }
    }
}

