#include "engine/render/mesh_types/line.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/shader/shader.hpp"
#include "glm/glm/vector_relational.hpp"

namespace sm 
{
    namespace render 
    {
        void line_mesh::init()
        {
            if (m_was_initialized) return;

            get_vao()->generate();
            get_vbo()->generate();

            get_vao()->bind();
            get_vbo()->bind();

            get_vbo()->send_data(sizeof(struct vertex) * get_vertex_num(), get_vertices(), GL_STATIC_DRAW);

            sm::gl_layer::enable_attrib_ptr(0, 2, GL_FLOAT, sizeof(struct vertex), (void*)0); 
            sm::gl_layer::enable_attrib_array(0);
            sm::gl_layer::enable_attrib_ptr(1, 4, GL_UNSIGNED_BYTE, sizeof(struct vertex), (void*)(sizeof(float) * 2));  
            sm::gl_layer::enable_attrib_array(1);

            get_vao()->detach();
            get_vbo()->detach();

            m_was_initialized = 1;
        }

        void line_mesh::render()
        {
            get_vao()->bind();
            get_vbo()->bind();

            sm::gl_layer::enable_attrib_array(0);
            sm::gl_layer::enable_attrib_array(1);

            sm::gl_layer::draw_simple(GL_LINES, 0, 2);

            get_vao()->detach();
            get_vbo()->detach();

            sm::gl_layer::disable_attrib_array(0);
            sm::gl_layer::disable_attrib_array(1);
        }
    

        void line_mesh::create(utilz::vector2f start, utilz::vector2f end, utilz::rgba_color color) 
        {
            if (m_was_initialized) return;

            allocate_vertices(2);

            get_vertices()[0].position = start;
            get_vertices()[0].color = color;
            get_vertices()[1].position = end;
            get_vertices()[1].color = color;

            init();
        }
    }
}

