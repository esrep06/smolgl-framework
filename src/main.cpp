#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/scene/scene.hpp"
#include "glfw/include/GLFW/glfw3.h"

#define GRAVITY 600

void create_static_body(utilz::vector2f pos, utilz::vector2f scale, sm::scene* scene) 
{
    sm::entity stat = scene->get_ecs()->create_entity(TRANSFORM | SPRITE | PHYSICS_BODY);

    struct sm::transform* ts = ECS_GET_TRANSFORM(scene->get_ecs(), stat); 
    struct sm::sprite* sps = ECS_GET_SPRITE(scene->get_ecs(), stat);
    struct sm::physics_body* pbs = ECS_GET_PHYSICS_BODY(scene->get_ecs(), stat);

    ts->position = pos;
    ts->scale = scale;

    sps->color = utilz::rgba_color(230, 150, 150, 255);

    pbs->body_type = sm::BODY_STATIC;
}

int main(void)
{
    sm::window win = sm::window("Engine Window", 1200, 800, 
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    sm::entity dyn = scene.get_ecs()->create_entity(TRANSFORM | SPRITE | PHYSICS_BODY);

    struct sm::transform* t = ECS_GET_TRANSFORM(scene.get_ecs(), dyn); 
    struct sm::sprite* sp = ECS_GET_SPRITE(scene.get_ecs(), dyn);
    struct sm::physics_body* pb = ECS_GET_PHYSICS_BODY(scene.get_ecs(), dyn);

    struct sm::physics::aabb ground_check;

    t->position = utilz::vector2f(scene.get_window()->get_center().x, 200);
    t->scale = utilz::vector2f(30, 30);

    sp->color = utilz::rgba_color(150, 150, 150, 255);

    pb->body_type = sm::BODY_DYNAMIC;

    utilz::vector2f movement;

    create_static_body(utilz::vector2f(100, 700 ), utilz::vector2f(1000.0f, 30.0f), &scene);

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();

        if (sm::input::get_button_down(GLFW_MOUSE_BUTTON_1))
            create_static_body(sm::input::get_mouse_position(), utilz::vector2f(30, 30.0f), &scene);

        scene.new_physics_frame();

        movement.x = sm::input::get_key(GLFW_KEY_A) ? -1.0f : sm::input::get_key(GLFW_KEY_D) ? 1.0f : 0.0f;
        movement.y = sm::input::get_key(GLFW_KEY_W) ? -1.0f : sm::input::get_key(GLFW_KEY_S) ? 1.0f : 0.0f;
        movement.normalize();

        float delta_x = movement.x * sm::time::delta_time() * 600.0f;
        float delta_y = GRAVITY * sm::time::delta_time();

        if (pb->grounded)
        {
            if (sm::input::get_key_down(GLFW_KEY_SPACE)) 
                delta_y = -350.0f;
        }

        pb->velocity.x = delta_x;
        pb->velocity.y += delta_y * sm::time::delta_time();

        scene.render();

        scene.end_frame();
    }

    return 0;
}

