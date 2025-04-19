#include "board.hpp"

board::board()
{}

board::board(sm::scene* scene)
{
    sm::shader* grid_shader = scene->get_shader_pool()->add_shader(
            "assets/shaders/default_vertex.glsl", "assets/shaders/grid_fragment.glsl", "grid_shader");
    sm::shader* water_shader = scene->get_shader_pool()->add_shader(
            "assets/shaders/default_vertex.glsl", "assets/shaders/water_fragment.glsl", "water_shader");

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

    t->position = utilz::vector2f(GRID_X_POS, GRID_Y_POS);
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

boat::boat(sm::scene* scene, int32_t segments, utilz::vector2 origin, ORIENTATION orientation, uint8_t player, uint8_t game)
    : m_player(player), m_game_id(game), m_orientation(orientation), m_origin(origin), m_segments(segments)
{
    int x_dir = 0;
    int y_dir = 0;

    switch (orientation)
    {
        case LEFT: 
            x_dir = -1;
            break;
        case RIGHT:
            x_dir = 1;
            break;
        case UP: 
            y_dir = -1;
            break;
        case DOWN:
            y_dir = 1;
            break;
    }

    for (int i = 0; i < segments; i++)
    {
        float x_pos = (GRID_X_POS + ((origin.x + (i * x_dir)) * GRID_SIZE)) + (GRID_SIZE * 0.25f);
        float y_pos = (GRID_Y_POS + ((origin.y + (i * y_dir)) * GRID_SIZE)) + (GRID_SIZE * 0.25f); 

        if (x_pos < GRID_X_POS || x_pos > GRID_X_POS + (SCREEN_WIDTH * 0.5f) || 
            y_pos < GRID_Y_POS || y_pos > GRID_Y_POS + (SCREEN_HEIGHT * 0.5f))
                continue;

        segment s;
        s.id = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
        sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), s.id);
        sm::sprite* spr = (sm::sprite*)ECS_GET_SPRITE(scene->get_ecs(), s.id);

        t->position.x = x_pos;
        t->position.y = y_pos;

        t->scale = utilz::vector2f(GRID_SIZE * 0.5f, GRID_SIZE * 0.5f);

        spr->color = utilz::rgba_color(RGBA_BLACK);
        spr->config = SPRITE_CONFIG_TOP_LEFT;

        m_segments.push_back(s);
    }
}

