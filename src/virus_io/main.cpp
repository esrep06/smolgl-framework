#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"

#include "engine/font/font.hpp"

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

typedef struct 
{
    sm::entity e;
    float radius;
} circle;


circle create_circle(sm::scene* scene, utilz::vector2f pos, utilz::rgba_color color, float radius)
{
    circle c;

    c.e = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), c.e);
    sm::textured_sprite* spr = (sm::textured_sprite*)ECS_GET_TEXTURED_SPRITE(scene->get_ecs(), c.e);

    t->position = pos;
    t->scale = utilz::vector2f(radius * 2, radius * 2);

    spr->config = SPRITE_CONFIG_TOP_LEFT;
    scene->get_texture_pool()->add_texture("assets/textures/circle.png", "circle");

    spr->texture = scene->get_texture_pool()->retrieve_texture("circle");
    c.radius = radius;

    return c;
}

void update_circle(circle* c, sm::scene* scene)
{
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), c->e);
}

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("VirusIO", SCREEN_WIDTH, SCREEN_HEIGHT,
            utilz::rgba_color(120, 145, 135, 255));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/grid_fragment.glsl", "grid_shader");
    scene.get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/circle_fragment.glsl", "circle_shader");

    sm::shader* grid_shader = scene.get_shader_pool()->retrieve_shader("grid_shader");

    create_grid(&scene);
    circle player = create_circle(&scene, utilz::vector2f(0.0f, 0.0f), utilz::rgba_color(255, 0, 0, 255), 32.0f);
    circle enemy = create_circle(&scene, utilz::vector2f(500.0f, 400.0f), utilz::rgba_color(255, 255, 0, 255), 16.0f);

    sm::font f = sm::font("assets/fonts/RobotoMono-Regular.ttf", 48);
    f.load_characters();

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.get_camera()->send_matrices(grid_shader, "projection", "view");

        scene.new_frame();
        scene.new_physics_frame();

        f.render_text(scene.get_shader_pool()->retrieve_shader("font_shader"), scene.get_camera()->get_proj(), "FREETYPE TEST", 0.0f, SCREEN_HEIGHT - 55.0f, 1.0f, utilz::rgba_color(0, 0, 0, 255));

        update_circle(&player, &scene);
        update_circle(&enemy, &scene);

        scene.render();
        scene.end_frame();
    }

    return 0;
}

