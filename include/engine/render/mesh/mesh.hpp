#pragma once 

#include "../../gl/gl_layer.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"

namespace sm
{
    namespace render 
    {
        struct vertex
        {
            utilz::vector2f position;
            utilz::rgba_color color;
        };

        class mesh
        {
            public:
                void init();
                void render();
                void free();

                sm::gl_layer::vao* get_vao();
                sm::gl_layer::vbo* get_vbo();

                vertex* get_vertices();
                size_t get_vertex_num();

                void allocate_vertices(size_t vertex_num);
                void update_mesh();
            private: 
                sm::gl_layer::vao m_vao;
                sm::gl_layer::vbo m_vbo;

                vertex* m_vertices;

                size_t m_vertex_num = 0; 
        };
    }
}

