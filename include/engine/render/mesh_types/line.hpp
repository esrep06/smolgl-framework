#pragma once 

#include "../mesh/mesh.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include <format>

namespace sm 
{
    namespace render    
    {
        class line_mesh : public mesh
        {
            public:
                void init();
                void render();
                void create(utilz::vector2f start, utilz::vector2f end, utilz::rgba_color color);
            private:
                uint8_t m_was_initialized = 0;
        };
    }
}

