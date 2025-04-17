#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"

#include "engine/font/font.hpp"

#include "player/player.hpp"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800

void create_grid(sm::scene* scene)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), e);

    t->scale = utilz::vector2f(3200, 1600);

    spr->shader = scene->get_shader_pool()->retrieve_shader("grid_shader");
    spr->color = utilz::rgba_color(RGBA_BLACK);
    spr->config = SPRITE_CONFIG_TOP_LEFT;
}

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("VirusIO", SCREEN_WIDTH, SCREEN_HEIGHT,
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/grid_fragment.glsl", "grid_shader");
    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/circle_fragment.glsl", "circle_shader");

    sm::shader* grid_shader = scene.get_shader_pool()->retrieve_shader("grid_shader");

    create_grid(&scene);

    player p1 = player(&scene, "P1", utilz::vector2f(100.0f, SCREEN_HEIGHT * 0.5f)); 
    player p2 = player(&scene, "P2", utilz::vector2f(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT * 0.5f));
   
    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.get_camera()->send_matrices(grid_shader, "projection", "view");

        scene.new_frame();
        scene.new_physics_frame();

        scene.get_font_pool()->retrieve_font("atari")->render_text(std::to_string((int)scene.get_fps()), 0.0f, 0.0f, 1.0f, utilz::rgba_color(13, 184, 49, 255));
        for (int i = 0; i < 10; i++)
        {
            scene.get_font_pool()->retrieve_font("atari")->render_text("minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit", 0.0f, 640 + (i * 16), 1.0f, utilz::rgba_color(0.0f, 0.0f, 0.0f, 255.0f));
        }


        p1.update(&scene);
        p2.update(&scene);

        scene.render();
        scene.end_frame();
    }

    return 0;
}

