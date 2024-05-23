#include "food.hpp"


void food::create(sm::scene* scene)
{
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> rand_x(0,scene->get_window()->get_resolution().x - FOOD_SCALE);
    std::uniform_int_distribution<std::mt19937::result_type> rand_y(0, scene->get_window()->get_resolution().y - FOOD_SCALE);

    id = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);

    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(id, TRANSFORM);
    sm::sprite* spr = (sm::sprite*)scene->get_ecs()->get_component(id, SPRITE);

    t->position = utilz::vector2f(rand_x(rng), rand_y(rng));
    t->scale = utilz::vector2f(FOOD_SCALE, FOOD_SCALE);

    spr->color = utilz::rgba_color(RGBA_RED);
    spr->config = SPRITE_CONFIG_TOP_LEFT;
}

