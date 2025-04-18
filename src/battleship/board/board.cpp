#include "board.hpp"

board::board(sm::scene* scene)
{
    sm::shader* grid_shader = scene->get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/grid_fragment.glsl", "grid_shader");
    sm::shader* water_shader = scene->get_shader_pool()->add_shader("assets/shaders/default_vertex.glsl", "assets/shaders/water_fragment.glsl", "water_shader");

    sm::entity w = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* w_t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), w);
    sm::sprite* w_spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), w);

    w_t->position = utilz::vector2f(0.0f, 0.0f);
    w_t->scale = utilz::vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    w_spr->color = utilz::rgba_color(RGBA_BLACK);
    w_spr->shader = water_shader;

    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), e);

    t->position = utilz::vector2f(256, 240);
    t->scale = utilz::vector2f(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5);

    spr->shader = grid_shader;
    spr->color = utilz::rgba_color(RGBA_BLACK);
    spr->config = SPRITE_CONFIG_TOP_LEFT;
}

void board::update(sm::scene* scene)
{
    sm::shader* grid_shader = scene->get_shader_pool()->retrieve_shader("grid_shader");
    sm::shader* water_shader = scene->get_shader_pool()->retrieve_shader("water_shader");

    scene->get_camera()->send_matrices(grid_shader, "projection", "view");
    scene->get_camera()->send_matrices(water_shader, "projection", "view");

    grid_shader->send_int(m_grid_size, "grid_size");
    water_shader->send_float(sm::time::get_time(), "time");
}

void board::add_player(player p)
{


}

