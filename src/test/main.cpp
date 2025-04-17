#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 640

void create_grid(sm::scene* scene)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), e);

    t->scale = utilz::vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);

    spr->shader = scene->get_shader_pool()->retrieve_shader("grid_shader");
    spr->color = utilz::rgba_color(RGBA_BLACK);
    spr->config = SPRITE_CONFIG_TOP_LEFT;
}

void create_water(sm::scene* scene)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), e);

    t->scale = utilz::vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);

    spr->shader = scene->get_shader_pool()->retrieve_shader("water_shader");
    spr->color = utilz::rgba_color(RGBA_BLUE);
    spr->config = SPRITE_CONFIG_TOP_LEFT;
}

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("Test", SCREEN_WIDTH, SCREEN_HEIGHT,
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/grid_fragment.glsl", "grid_shader");
    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/water_fragment.glsl", "water_shader");

    sm::shader* grid_shader = scene.get_shader_pool()->retrieve_shader("grid_shader");
    sm::shader* water_shader = scene.get_shader_pool()->retrieve_shader("water_shader");

    create_grid(&scene);
    create_water(&scene);

    sm::entity e = scene.get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene.get_ecs(), e);
    sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene.get_ecs(), e);

    t->scale = utilz::vector2f(32.0f, 32.0f);

    spr->color = utilz::rgba_color(RGBA_GREEN);
    spr->config = SPRITE_CONFIG_TOP_LEFT;


    while (!glfwWindowShouldClose(win.get_context()))
    {
        float val = (sin(sm::time::get_time()) * 50) + (SCREEN_HEIGHT * 0.5f);

        scene.get_camera()->send_matrices(grid_shader, "projection", "view");
        scene.get_camera()->send_matrices(water_shader, "projection", "view");
        water_shader->send_float(sm::time::get_time(), "time");

        scene.new_frame();
        scene.new_physics_frame();

        t->position.x = sm::input::get_mouse_position().x;
        t->position.y = val;

        scene.render();
        scene.end_frame();
    }

    return 0;
}

