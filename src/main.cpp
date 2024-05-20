#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/ecs/systems/transform_system/transform_system.hpp"
#include "engine/window/window.hpp"
#include "engine/utils/time/time.hpp"
#include "engine/scene/scene.hpp"

sm::entity create_water(sm::scene* scene)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::sprite* spr = (sm::sprite*)scene->get_ecs()->get_component(e, SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(e, TRANSFORM);
    spr->color = utilz::rgba_color(0, 0, 255, 100);
    t->position = utilz::vector2f(0.0f, 0.0f);
    t->scale = utilz::vector2f(800.0f, 600.0f);
    spr->shader = scene->get_shader_pool()->retrieve_shader("water_shader");

    return e;
}

struct player 
{
    sm::entity id;
};

player create_player(sm::scene* scene)
{
    player p;
    p.id = scene->get_ecs()->create_entity(SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::sprite* spr = (sm::sprite*)scene->get_ecs()->get_component(p.id, SPRITE);
    t->position.x = scene->get_window()->get_resolution().x * 0.5f;
    t->position.y = scene->get_window()->get_resolution().y * 0.5f;
    t->scale.x = 50.0f;
    t->scale.y = 80.0f;
    spr->color = utilz::rgba_color(RGBA_BLACK);
    spr->shader = scene->get_shader_pool()->retrieve_shader("default_shader");

    return p;
}

int main(void)
{
    sm::window win = sm::window("Hello OpenGL", 800, 600, 
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), utilz::vector2(800, 600)));
    scene.init();

    scene.get_shader_pool()->add_shader(sm::shader("assets/triangle_vertex.glsl", "assets/triangle_fragment.glsl"), "water_shader");

    sm::entity water = create_water(&scene);
    player p = create_player(&scene);

    while (!glfwWindowShouldClose(win.get_context()))
    {
        sm::time::calculate_update();

        glfwPollEvents();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) { break; }

        sm::shader* water_shader = scene.get_shader_pool()->retrieve_shader("water_shader");

        water_shader->send_mat4(scene.get_camera()->get_view(), "view");
        water_shader->send_mat4(scene.get_camera()->get_proj(), "projection");
        water_shader->send_float(win.get_resolution().x, "width");
        water_shader->send_float(win.get_resolution().y, "height");
        water_shader->send_float((float)sm::time::get_time(), "time");
 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        scene.render();

        // End frame funcs
        sm::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    return 0;
}

