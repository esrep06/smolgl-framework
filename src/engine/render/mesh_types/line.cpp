#include "line.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "glm/glm/vector_relational.hpp"

namespace sm 
{
    namespace render 
    {
        void line_mesh::create(utilz::vector2f start, utilz::vector2f end, utilz::rgba_color color) 
        {
            if (was_initialized()) return;

            allocate_vertices(2);

            get_vertices()[0].position = start;
            get_vertices()[0].color = color;
            get_vertices()[1].position = end;
            get_vertices()[1].color = color;

            init(GL_DYNAMIC_DRAW);
        }
        
        void line_mesh::update() 
        {
            render(GL_LINES, 0, get_vertex_num());
        }
    }
}

