#pragma once 

#include "../../../../include/cpp-utilz/math/vector2.hpp"
#include "../../../../include/cpp-utilz/math/vector3.hpp"

#include "../../gl/gl_layer.hpp"

#define TRANSFORM 1
#define SPRITE 2 

namespace sm 
{
    typedef uint8_t component; 

    struct transform 
    {
        utilz::vector2f position;
        utilz::vector2f scale;
    };
    
    struct sprite_vertex
    {
        utilz::vector3f position;
    };

    struct sprite
    {
        gl_layer::vao vao;
        gl_layer::ebo ebo;
        gl_layer::vbo vbo;

        sprite_vertex vertices[4];

        uint32_t indices[6];
        uint8_t was_initialized;
    };
}

