#include "player.hpp"

player::player(sm::scene* scene, std::string name, utilz::vector2f position)
{
    m_entity = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), m_entity);
    sm::textured_sprite* spr = (sm::textured_sprite*)ECS_GET_TEXTURED_SPRITE(scene->get_ecs(), m_entity);

    t->position = position;
    t->scale = utilz::vector2f(m_starting_size * 2, m_starting_size * 2);

    spr->config = SPRITE_CONFIG_CENTERED;
    scene->get_texture_pool()->add_texture("assets/textures/circle.png", "circle");

    spr->texture = scene->get_texture_pool()->retrieve_texture("circle");

    m_name = name;
    m_size = m_starting_size;
}

void player::update(sm::scene* scene)
{
    sm::transform* t = (sm::transform*)ECS_GET_TRANSFORM(scene->get_ecs(), m_entity);

    sm::font* font = scene->get_font_pool()->retrieve_font("atari");

    float text_scale = 0.8f;

    float text_width = font->get_text_size(m_name, text_scale).x; 
    float text_y_distance = m_size * 2.0f;

    font->render_text(m_name, t->position.x - (text_width * 0.5f), t->position.y - text_y_distance, text_scale, utilz::rgba_color(0.0f, 0.0f, 0.0f, 255.0f));
}

