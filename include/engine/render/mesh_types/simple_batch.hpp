#pragma once 

#include "engine/gl/gl_layer.hpp"
#include "engine/render/mesh/mesh.hpp"

namespace sm 
{
    namespace render 
    {
        class simple_batch : public mesh 
        {
            public:
                void init();
                void render();
                void free();

                void add_mesh(struct vertex v1, struct vertex v2, struct vertex v3, struct vertex v4);
                void allocate_size(size_t size);

                void update_mesh();

                uint32_t* get_indices();
                sm::gl_layer::ebo* get_ebo();

            private:
                sm::gl_layer::ebo m_ebo;

                uint32_t* m_indices;

                uint8_t m_was_initialized = 0;

                // Actual size of the batch 
                size_t m_batch_size = 0;

                // Maximum size allocated for meshes 
                size_t m_batch_mesh_size = 0;

                /* eg:
                     We allocate memory for 10 meshes with m_batch_mesh_size
                     but we only have data for 5 meshes in m_batch_size
                */
        };
    }
}



