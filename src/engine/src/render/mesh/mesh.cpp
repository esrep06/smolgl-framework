#include "engine/render/mesh/mesh.hpp"
#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include <format>

namespace sm
{
    namespace render
    {
        void mesh::init() {} 

        void mesh::render() {}

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
    }
}

