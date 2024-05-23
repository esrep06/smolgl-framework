#include "player.hpp"

void snake::create(sm::scene* scene)
{
    this->scene = scene;

    scene->get_texture_pool()->add_texture("assets/textures/snake_player.png", "snake_segment");

    head.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(1, TRANSFORM);
    sm::textured_sprite* sp = (sm::textured_sprite*)scene->get_ecs()->get_component(1, TEXTURED_SPRITE);
    
    t->position = utilz::vector2f(scene->get_window()->get_resolution().x * 0.5f, 
            scene->get_window()->get_resolution().y * 0.5f);
    t->scale = utilz::vector2f(SNAKE_SEGMENT_SCALE, SNAKE_SEGMENT_SCALE); 

    sp->texture = scene->get_texture_pool()->retrieve_texture("snake_segment");
    sp->config |= SPRITE_CONFIG_TOP_LEFT;

    segments.push_back(head);
    current_food.create(scene);
}

void snake::update()
{
    if (move_timer >= SNAKE_STRIDE_SPEED_MS)
    {
        move_timer = 0.0f;
        move();
    }
    else 
    {
        move_timer += sm::time::delta_time() * 1000;
    }

    if (sm::input::get_key_down(GLFW_KEY_A) && direction != RIGHT)
        direction = LEFT;
    else if (sm::input::get_key_down(GLFW_KEY_D) && direction != LEFT)
        direction = RIGHT;
    else if (sm::input::get_key_down(GLFW_KEY_W) && direction != DOWN)
        direction = UP;
    else if (sm::input::get_key_down(GLFW_KEY_S) && direction != UP)
        direction = DOWN;

    eat();
}

void snake::move()
{
    for (int i = segments.size(); i --> 1; )
    {
        sm::transform* t_1 = (sm::transform*)scene->get_ecs()->get_component(segments[i].id, TRANSFORM);
        sm::transform* t_2 = (sm::transform*)scene->get_ecs()->get_component(segments[i - 1].id, TRANSFORM);

        if (t_1 != nullptr && t_2 != nullptr)
            t_1->position = t_2->position;
    }

    sm::transform* head_t = (sm::transform*)scene->get_ecs()->get_component(1, TRANSFORM);

    switch (direction)
    {
        case UP:
            head_t->position.y -= SNAKE_SEGMENT_SCALE;
            break;
        case DOWN:
            head_t->position.y += SNAKE_SEGMENT_SCALE;
            break;
        case LEFT:
            head_t->position.x -= SNAKE_SEGMENT_SCALE;
            break;
        case RIGHT:
            head_t->position.x += SNAKE_SEGMENT_SCALE;
            break;
    }
}

void snake::eat()
{
    sm::transform* head_t = (sm::transform*)scene->get_ecs()->get_component(1, TRANSFORM); 
    sm::transform* food_t = (sm::transform*)scene->get_ecs()->get_component(current_food.id, TRANSFORM);

    if (head_t == nullptr || food_t == nullptr) return;

    sm::rect head_r = sm::rect(*head_t);
    sm::rect food_r = sm::rect(*food_t);

    if (head_r.collision_aabb(food_r))
    {
        grow();

        std::random_device dev;
        std::mt19937 rng(dev());

        std::uniform_int_distribution<std::mt19937::result_type> rand_x(0,scene->get_window()->get_resolution().x - FOOD_SCALE);
        std::uniform_int_distribution<std::mt19937::result_type> rand_y(0, scene->get_window()->get_resolution().y - FOOD_SCALE);
        food_t->position = utilz::vector2f(rand_x(rng), rand_y(rng));
        /* scene->get_ecs()->remove_entity(current_food.id); */
        /* current_food.create(scene); */
    }
}

void snake::grow()
{
    if (segments.size() >= SNAKE_MAX_SIZE) return; 

    segment s;
    s.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);

    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(s.id, TRANSFORM);
    
    sm::transform* head_t = (sm::transform*)scene->get_ecs()->get_component(1, TRANSFORM);

    sm::textured_sprite* sp = (sm::textured_sprite*)scene->get_ecs()->get_component(s.id, TEXTURED_SPRITE);

    t->position = utilz::vector2f(head_t->position.x, head_t->position.y);
    t->scale = utilz::vector2f(SNAKE_SEGMENT_SCALE, SNAKE_SEGMENT_SCALE); 

    sp->texture = scene->get_texture_pool()->retrieve_texture("snake_segment");
    sp->config |= SPRITE_CONFIG_TOP_LEFT;

    segments.push_back(s);
}

void snake::die()
{}

