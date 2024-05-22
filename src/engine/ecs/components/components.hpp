#pragma once 

#include "../../../../include/cpp-utilz/math/vector2.hpp"
#include "../../../../include/cpp-utilz/math/vector3.hpp"

#include "../../gl/gl_layer.hpp"
#include "cpp-utilz/color/color.hpp"
#include "../../shader/shader.hpp"
#include "../../texture/texture.hpp"

#define TRANSFORM 1
#define SPRITE 2
#define TEXTURED_SPRITE 4

#define SPRITE_CONFIG_CENTERED 1
#define SPRITE_CONFIG_TOP_LEFT 2

namespace sm 
{
    typedef uint8_t component; 

    struct transform 
    {
        utilz::vector2f position;
        utilz::vector2f scale;
        float angle = 0.0f;
    };
    
    struct sprite_vertex
    {
        utilz::vector3f position;
        utilz::rgba_color color;
    };
    
    struct textured_sprite_vertex
    {
        utilz::vector3f position;
        utilz::vector2f uv;
    };

    struct sprite
    {
        gl_layer::vao vao;
        gl_layer::ebo ebo;
        gl_layer::vbo vbo;

        sm::shader* shader;

        utilz::rgba_color color;

        sprite_vertex vertices[4];

        uint32_t indices[6];
        uint8_t was_initialized;
        uint8_t config;
    };
    
    struct textured_sprite
    {
        gl_layer::vao vao;
        gl_layer::ebo ebo;
        gl_layer::vbo vbo;

        sm::shader* shader;
        sm::texture* texture;

        textured_sprite_vertex vertices[4];
        uint32_t indices[6];
        uint8_t was_initialized;
        uint8_t config;
    };
}

