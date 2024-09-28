#pragma once 

#include "../mesh/mesh.hpp"
#include "cpp-utilz/math/vector2.hpp"

namespace sm 
{
    namespace render    
    {
        class line_mesh : public mesh
        {
            public: 
                void create(utilz::vector2f start, utilz::vector2f end, utilz::rgba_color color);
                void update();
            private:
        };
    }
}

