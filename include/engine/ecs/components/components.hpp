#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "cpp-utilz/math/vector3.hpp"
#include "cpp-utilz/color/color.hpp"

#include "../../physics/aabb.hpp"

#include "../../gl/gl_layer.hpp"
#include "../../shader/shader.hpp"
#include "../../texture/texture.hpp"
#include "../../utils/time/time.hpp"

/* ----- COMPONENTS ----- */

#define TRANSFORM 1
#define SPRITE 2
#define TEXTURED_SPRITE 4
#define BEHAVIOR 8
#define ANIMATOR 16
#define PHYSICS_BODY 32

/* ----- SPRUTE CONFIG ----- */ 

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

    struct behavior 
    {
        std::function<void(behavior*)> start = nullptr;
        std::function<void(behavior*)> update = nullptr;

        void* data;

        uint16_t entity;
        int8_t was_initialized;
    };

    struct animation 
    {
        sm::time::timer switch_timer;
        int32_t x;
        int32_t w;
        int32_t x_bound;
        std::string name;
        sm::texture* texture;
    };

    struct animator
    {
        std::vector<animation> animations;

        sm::time::timer animation_timer;

        animation* current_animation = nullptr;
    };

    enum PHYSICS_BODY_TYPE 
    { 
        BODY_STATIC,
        BODY_DYNAMIC,
        BODY_KINEMATIC,
    };

    struct physics_body 
    {
        utilz::vector2f velocity;

        sm::physics::aabb aabb;
        sm::physics::dynamic_collision dynamic_collision;

        uint8_t custom; // To set custom scales on the hitboxes

        uint8_t grounded;

        uint8_t collision_layer;
        uint8_t collision_mask;

        PHYSICS_BODY_TYPE body_type;
    };
}

