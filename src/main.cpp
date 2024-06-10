#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/scene/scene.hpp"
#include "engine/physics/rect.hpp"

#include <random>

#define ASTEROID_TEXTURE_MAX 4

std::random_device dev;
std::mt19937 rng(dev());

struct player 
{
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    utilz::vector2f direction;

    float speed = 250.0f; 
    float rotation_speed = 150.0f;
};

struct asteroid
{   
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    float speed = 0.2f;
    float rotation_speed = 100.0f;
    int8_t rot_speed_offset;

    utilz::vector2f direction;

    float hp;
};

struct bullet
{
    sm::entity id;
    sm::scene* scene;

    float speed = 700.0f;
    utilz::vector2f direction;
};

struct world
{
    struct player player;

    sm::scene* scene;

    std::vector<struct asteroid> asteroids;
};

void bullet_update(sm::behavior* b)
{  
    struct bullet* bullet = (struct bullet*)b->data;

    sm::transform* t = (sm::transform*)bullet->scene->get_ecs()->get_component(bullet->id, TRANSFORM);

    t->position.x += bullet->direction.x * sm::time::delta_time() * bullet->speed;
    t->position.y += bullet->direction.y * sm::time::delta_time() * bullet->speed;

    if (t->position.x > bullet->scene->get_window()->get_resolution().x || t->position.x < 0 
            || t->position.y > bullet->scene->get_window()->get_resolution().y || t->position.y < 0)
    {
        bullet->scene->get_ecs()->remove_entity(bullet->id);
        free(b->data);
    }
}

struct bullet* bullet_create(sm::scene* scene, utilz::vector2f dir, utilz::vector2f pos)
{
    struct bullet* p = new struct bullet;

    p->id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE | BEHAVIOR);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p->id, TRANSFORM);
    sm::textured_sprite* spr = (sm::textured_sprite*)scene->get_ecs()->get_component(p->id, TEXTURED_SPRITE);
    sm::behavior* b = (sm::behavior*)scene->get_ecs()->get_component(p->id, BEHAVIOR);

    t->position = pos;
    t->scale = utilz::vector2f(16.0f);

    spr->config = SPRITE_CONFIG_CENTERED;
    spr->texture = scene->get_texture_pool()->retrieve_texture("asteroids_bullet");

    b->update = bullet_update;

    p->direction = dir;
    p->direction.normalize();
    p->scene = scene;

    return p;
}

struct player player_create(sm::scene* scene)
{
    struct player p;

    p.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::textured_sprite* spr = (sm::textured_sprite*)scene->get_ecs()->get_component(p.id, TEXTURED_SPRITE);

    t->position = scene->get_window()->get_center();
    t->scale = utilz::vector2f(32.0f);

    spr->config = SPRITE_CONFIG_CENTERED;
    spr->texture = scene->get_texture_pool()->retrieve_texture("asteroids_ship");

    p.scene = scene;

    return p;
}

struct asteroid asteroid_create(utilz::vector2f pos, sm::scene* scene)
{
    struct asteroid p; 

    p.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::textured_sprite* ts = (sm::textured_sprite*)scene->get_ecs()->get_component(p.id, TEXTURED_SPRITE);

    t->position = pos;
    t->scale = utilz::vector2f(32.0f);

    ts->config = SPRITE_CONFIG_CENTERED;

    std::uniform_int_distribution<std::mt19937::result_type> rand(1, ASTEROID_TEXTURE_MAX);
    std::uniform_int_distribution<std::mt19937::result_type> rand_angle(0, 360);

    int32_t texture_index = rand(rng); 

    ts->texture = scene->get_texture_pool()->retrieve_texture("asteroids_asteroid_" + std::to_string(texture_index));

    t->angle = rand_angle(rng); 

    return p;
}

void world_populate(struct world* world, sm::scene* scene)
{   
    world->player = player_create(scene);
    world->scene = scene;

    for (int32_t i = 0; i < 10; i++)
    {
        std::uniform_int_distribution<std::mt19937::result_type> x_pos(32, scene->get_window()->get_resolution().x - 32);
        std::uniform_int_distribution<std::mt19937::result_type> y_pos(32, scene->get_window()->get_resolution().y - 32);

        std::uniform_int_distribution<std::mt19937::result_type> dir_x_pos(0, scene->get_window()->get_resolution().x);
        std::uniform_int_distribution<std::mt19937::result_type> dir_y_pos(0, scene->get_window()->get_resolution().y);

        world->asteroids.push_back(asteroid_create(utilz::vector2f(x_pos(rng), y_pos(rng)), scene));

        world->asteroids[i].direction = utilz::vector2f(dir_x_pos(rng), dir_y_pos(rng));

        std::uniform_int_distribution<std::mt19937::result_type> rot_offset(-20, 20);

        world->asteroids[i].rot_speed_offset = rot_offset(rng);
    }
}

void player_update(struct player* player)
{
    sm::transform* t = (sm::transform*)player->scene->get_ecs()->get_component(player->id, TRANSFORM);
    player->rect = sm::rect(*t);

    int8_t rotate_input = sm::input::get_key(GLFW_KEY_A) ? -1.0f : sm::input::get_key(GLFW_KEY_D) ? 1.0f : 0;
    uint8_t move_input = sm::input::get_key(GLFW_KEY_W) ? 1.0f : 0.0f;

    if (t->angle < 0) t->angle = 360;
    if (t->angle > 360) t->angle = 0;

    t->angle += rotate_input * sm::time::delta_time() * player->rotation_speed;

    player->direction = utilz::vector2f(sin(glm::radians(t->angle)), -cos(glm::radians(t->angle)));

    t->position.x += player->direction.x * move_input * player->speed * sm::time::delta_time();
    t->position.y += player->direction.y * move_input * player->speed * sm::time::delta_time();

    if (sm::input::get_key_down(GLFW_KEY_SPACE))
    {
        struct bullet* b = bullet_create(player->scene, player->direction, t->position);

        sm::behavior* beh = (sm::behavior*)player->scene->get_ecs()->get_component(b->id, BEHAVIOR);
        beh->data = b;
    }
}

void world_move_asteroids(struct world* world)
{

    for (size_t i = 0; i < world->asteroids.size(); i++)
    {
        struct asteroid* asteroid = &world->asteroids[i];

        sm::transform* t = (sm::transform*)world->scene->get_ecs()->get_component(asteroid->id, TRANSFORM);

        asteroid->rect = sm::rect(*t);

        t->position.x += (asteroid->direction.x - t->position.x) * sm::time::delta_time() * asteroid->speed;
        t->position.y += (asteroid->direction.y - t->position.y) * sm::time::delta_time() * asteroid->speed;

        t->angle += sm::time::delta_time() * (asteroid->rotation_speed + asteroid->rot_speed_offset);

        if (t->position.x > world->scene->get_window()->get_resolution().x || 
                t->position.x < -30.0f || t->position.y > world->scene->get_window()->get_resolution().y || 
                t->position.y < -30.0f || t->position.eu_dist(asteroid->direction) < 40.0f)
        {
            std::uniform_int_distribution<std::mt19937::result_type> x_pos(0, world->scene->get_window()->get_resolution().x);
            std::uniform_int_distribution<std::mt19937::result_type> y_pos(0, world->scene->get_window()->get_resolution().y);

            asteroid->direction = utilz::vector2f(x_pos(rng), y_pos(rng));
        }
    }
}

int main(void)
{
    sm::window win = sm::window("Asteroids", 800, 600, 
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    scene.get_texture_pool()->add_texture("assets/textures/ship.png", "asteroids_ship");
    scene.get_texture_pool()->add_texture("assets/textures/bullet.png", "asteroids_bullet");
    scene.get_shader_pool()->add_shader("assets/shaders/game_shaders/asteroid_vertex.glsl", "assets/shaders/game_shaders/asteroid_fragment.glsl", "asteroid_shader");

    for (int i = 1; i <= ASTEROID_TEXTURE_MAX; i++)
    {
        scene.get_texture_pool()->add_texture("assets/textures/asteroid_" + std::to_string(i) + ".png", "asteroids_asteroid_" + std::to_string(i));
    }
    
    struct world world; 

    sm::entity background = scene.get_ecs()->create_entity(TRANSFORM | SPRITE);
        
    sm::transform* t = (sm::transform*)scene.get_ecs()->get_component(background, TRANSFORM);
    sm::sprite* sp = (sm::sprite*)scene.get_ecs()->get_component(background, SPRITE);

    t->scale = utilz::vector2f(win.get_resolution().x, win.get_resolution().y);

    sp->shader = scene.get_shader_pool()->retrieve_shader("asteroid_shader");
    sp->config = SPRITE_CONFIG_TOP_LEFT;
    sp->color = utilz::rgba_color(0, 0, 0, 255);
    
    world_populate(&world, &scene);

    std::uniform_int_distribution<std::mt19937::result_type> x_pos(32, win.get_resolution().x - 32);
    std::uniform_int_distribution<std::mt19937::result_type> y_pos(32, win.get_resolution().y - 32);

    sp->shader->send_int(x_pos(rng), "rand_x");
    sp->shader->send_float(y_pos(rng), "rand_y");

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();
        
        scene.get_camera()->bounds = win.get_resolution();

        player_update(&world.player);
        world_move_asteroids(&world);

        sp->shader->use();
        scene.get_camera()->send_matrices(sp->shader, "projection", "view");
        sp->shader->send_float(sm::time::get_time(), "time");
        sp->shader->detach();
    
        scene.render();

        scene.end_frame();
    }

    return 0;
}

