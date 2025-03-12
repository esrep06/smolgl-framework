#include "engine/render/mesh_types/simple_batch.hpp"
#include "engine/gl/gl_layer.hpp"
#include "engine/render/mesh/mesh.hpp"

namespace sm 
{
    namespace render 
    {
        void simple_batch::init()
        {
            if (m_was_initialized) return;

            get_vao()->generate();
            get_vbo()->generate();
            get_ebo()->generate();

            get_vao()->bind();
            get_vbo()->bind();
            get_ebo()->bind();

            get_vbo()->send_data(sizeof(struct vertex) * m_batch_mesh_size * 4, get_vertices(), GL_STATIC_DRAW);
            get_ebo()->send_data(sizeof(uint32_t) * m_batch_mesh_size * 6, get_indices(), GL_STATIC_DRAW);

            sm::gl_layer::enable_attrib_ptr(0, 2, GL_FLOAT, sizeof(struct vertex), (void*)0); 
            sm::gl_layer::enable_attrib_array(0);
            sm::gl_layer::enable_attrib_ptr(1, 4, GL_UNSIGNED_BYTE, sizeof(struct vertex), (void*)(sizeof(float) * 2));  
            sm::gl_layer::enable_attrib_array(1);

            get_vao()->detach();
            get_vbo()->detach();
            get_ebo()->detach();

            m_was_initialized = 1;
        }


        void simple_batch::render()
        {
            if (!m_was_initialized) return;

            get_vao()->bind();
            get_vbo()->bind();
            get_ebo()->bind();

            gl_layer::enable_attrib_array(0);
            gl_layer::enable_attrib_array(1);

            gl_layer::draw_indexed(GL_TRIANGLES, 6 * m_batch_size, GL_UNSIGNED_INT, 0);

            get_vao()->detach();
            get_vbo()->detach();
            get_ebo()->detach();

            gl_layer::disable_attrib_array(0);
            gl_layer::disable_attrib_array(1);
        }

        void simple_batch::free()
        {
            delete get_vertices();
            delete m_indices;

            get_vao()->free();
            get_vbo()->free();
            get_ebo()->free();
        }


        void simple_batch::add_mesh(struct vertex v1, struct vertex v2, struct vertex v3, struct vertex v4)
        {
            if (m_batch_size >= m_batch_mesh_size) return;

            get_vertices()[m_batch_size * 4] = v1;
            get_vertices()[(m_batch_size * 4) + 1] = v2;
            get_vertices()[(m_batch_size * 4) + 2] = v3;
            get_vertices()[(m_batch_size * 4) + 3] = v4;

            get_indices()[0 + (m_batch_size * 6)] = 0 + (m_batch_size * 4);
            get_indices()[1 + (m_batch_size * 6)] = 1 + (m_batch_size * 4);
            get_indices()[2 + (m_batch_size * 6)] = 2 + (m_batch_size * 4);
            get_indices()[3 + (m_batch_size * 6)] = 2 + (m_batch_size * 4);
            get_indices()[4 + (m_batch_size * 6)] = 3 + (m_batch_size * 4); 
            get_indices()[5 + (m_batch_size * 6)] = 1 + (m_batch_size * 4);

            m_batch_size++;
        }


        void simple_batch::allocate_size(size_t size)
        {
            allocate_vertices(size * 4);
            m_indices = new uint32_t[size * 6];
            m_batch_mesh_size = size;
        }

        void simple_batch::update_mesh() 
        {
            // Updates vbo 
            update_mesh();

            // Updates ebo
            get_ebo()->bind();
            get_vbo()->update_data(0, sizeof(struct vertex) * 4 * m_batch_mesh_size, get_vertices());
            get_ebo()->detach();
        }

        sm::gl_layer::ebo* simple_batch::get_ebo() { return &m_ebo; } 

        uint32_t* simple_batch::get_indices() { return m_indices; }
    }
}

