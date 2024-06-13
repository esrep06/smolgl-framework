#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/scene/scene.hpp"
#include "engine/physics/rect.hpp"

#include <random>

#define ASTEROID_TEXTURE_MAX 4
#define ASTEROID_MAX_NUM 1
#define PLAYER_I_FRAMES 450

#define PI 3.14159265358979323846

// TODO: Boss death functionality

std::random_device dev;
std::mt19937 rng(dev());

struct player 
{
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    utilz::vector2f direction;

    struct world* world;

    float speed = 250.0f; 
    float rotation_speed = 200.0f;
    int32_t invinsible_frames = PLAYER_I_FRAMES;
    uint8_t alive = 1;

    float damage = 50.0f;

    float health = 100.0f;
};

struct asteroid
{   
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    float speed = 0.2f;
    float rotation_speed = 100.0f;
    float damage_mult = 1.5f;
    int8_t rot_speed_offset;

    utilz::vector2f direction;

    float hp;
};

struct bullet
{
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    float speed = 700.0f;
    utilz::vector2f direction;
};

struct boss 
{
    sm::entity id;
    sm::scene* scene;
    sm::rect rect;

    utilz::vector2f direction;
    sm::time::timer move_timer;

    struct world* world;

    float speed = 2.5f; 
    uint8_t alive = 0;
    float damage = 50.0f;
    float health = 500.0f;
};

struct world
{
    struct player player;
    struct boss boss;

    sm::scene* scene;

    sm::time::timer boss_timer;

    uint8_t in_boss = 0;
    uint8_t asteroids_defeated = 0;
    std::vector<struct asteroid> asteroids;
};

struct bullet_data 
{
    struct bullet* b;
    struct world* w;
};

void bullet_update(sm::behavior* b);
struct bullet* bullet_create(sm::scene* scene, utilz::vector2f dir, utilz::vector2f pos);
void boss_bullet_update(sm::behavior* b);

struct boss boss_create(sm::scene* scene, struct world* world)
{
    struct boss p;

    p.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::textured_sprite* spr = (sm::textured_sprite*)scene->get_ecs()->get_component(p.id, TEXTURED_SPRITE);

    t->position = scene->get_window()->get_center();
    t->scale = utilz::vector2f(32.0f);

    spr->config = SPRITE_CONFIG_CENTERED;
    spr->texture = world->scene->get_texture_pool()->retrieve_texture("asteroids_boss");
    
    p.move_timer.target = 1300.0f;

    p.scene = scene;
    p.world = world;
    p.alive = 1;

    sm::transform* player_t = (sm::transform*)scene->get_ecs()->get_component(world->player.id, TRANSFORM);

    p.direction = player_t->position;
    return p;
}

void boss_update(struct world* world, struct boss* boss)
{
    sm::transform* t = (sm::transform*)world->scene->get_ecs()->get_component(boss->id, TRANSFORM);

    boss->rect = sm::rect(*t);


    if (t->position.eu_dist(boss->direction) < 8.0f)
    {
        if (boss->move_timer.update())
        {
            sm::transform* player_t = (sm::transform*)world->scene->get_ecs()->get_component(world->player.id, TRANSFORM);
            boss->direction = player_t->position;
            boss->move_timer.reset();

            for (int32_t i = 0; i < 360; i++)
            {
                if (i % 30 == 0)
                {
                    double angle_rads = i * (PI/180); 

                    utilz::vector2f b_dir = utilz::vector2f(cos(angle_rads), sin(angle_rads));
                    b_dir.normalize();

                    struct bullet* b = bullet_create(world->scene, b_dir, t->position); 
                    sm::behavior* beh = (sm::behavior*)world->scene->get_ecs()->get_component(b->id, BEHAVIOR);
                    struct bullet_data* data = new struct bullet_data;
                    data->b = b;
                    data->w = world;
                    beh->data = data;
                    beh->update = boss_bullet_update;
                }
            }
        }

        t->position.x += cos( sm::time::get_time() * boss->move_timer.timer * 3);
    }
    else 
        t->position = utilz::vector2f::lerp(boss->direction, t->position, sm::time::delta_time() * boss->speed);

    if (boss->rect.collision_aabb(world->player.rect) && world->player.invinsible_frames <= 0)
    {
        world->player.health -= boss->damage;
        world->player.invinsible_frames = PLAYER_I_FRAMES;
    }  

    if (boss->health <= 0)
    {
        world->scene->get_ecs()->remove_entity(boss->id);
        boss->alive = 0;
        world->in_boss = 0;
    }
}

struct asteroid asteroid_create(utilz::vector2f pos, float scale, sm::scene* scene)
{
    struct asteroid p; 

    p.id = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::textured_sprite* ts = (sm::textured_sprite*)scene->get_ecs()->get_component(p.id, TEXTURED_SPRITE);

    t->position = pos;
    t->scale = utilz::vector2f(scale);

    ts->config = SPRITE_CONFIG_CENTERED;

    std::uniform_int_distribution<std::mt19937::result_type> rand(1, ASTEROID_TEXTURE_MAX);
    std::uniform_int_distribution<std::mt19937::result_type> rand_angle(0, 360);

    int32_t texture_index = rand(rng); 

    ts->texture = scene->get_texture_pool()->retrieve_texture("asteroids_asteroid_" + std::to_string(texture_index));

    t->angle = rand_angle(rng); 

    std::uniform_int_distribution<std::mt19937::result_type> dir_x_pos(0, scene->get_window()->get_resolution().x);
    std::uniform_int_distribution<std::mt19937::result_type> dir_y_pos(0, scene->get_window()->get_resolution().y);

    p.direction = utilz::vector2f(dir_x_pos(rng), dir_y_pos(rng));

    return p;
}


void bullet_update(sm::behavior* b)
{  
    struct bullet_data* bullet = (struct bullet_data*)b->data;

    sm::transform* t = (sm::transform*)bullet->b->scene->get_ecs()->get_component(bullet->b->id, TRANSFORM);

    t->position.x += bullet->b->direction.x * sm::time::delta_time() * bullet->b->speed;
    t->position.y += bullet->b->direction.y * sm::time::delta_time() * bullet->b->speed;

    bullet->b->rect = sm::rect(*t);

    for (size_t i = 0; i < bullet->w->asteroids.size(); i++)
    {
        struct asteroid a = bullet->w->asteroids[i];

        if (a.rect.collision_aabb(bullet->b->rect)) 
        {
            sm::transform* a_t = (sm::transform*)bullet->b->scene->get_ecs()->get_component(a.id, TRANSFORM);
            a_t->scale.x *= 0.5f;
            a_t->scale.y *= 0.5f;

            // Has been shot twice, meaning that its size is less than 8^2
            if (a_t->scale.mag() < 12)
            {
                bullet->w->scene->get_ecs()->remove_entity(a.id);
                // Move to back and remove
                bullet->w->asteroids[i] = bullet->w->asteroids.back();
                bullet->w->asteroids.pop_back();
            }
            else 
                bullet->w->asteroids.push_back(asteroid_create(a_t->position, a_t->scale.x, bullet->w->scene));

            bullet->b->scene->get_ecs()->remove_entity(bullet->b->id);
            free(bullet->b);
            free(b->data);

            return;
        }
    }

    if (bullet->w->in_boss)
    {
        if (bullet->b->rect.collision_aabb(bullet->w->boss.rect))
        {
            bullet->w->boss.health -= 70.0f;

            bullet->b->scene->get_ecs()->remove_entity(bullet->b->id);
            free(bullet->b);
            free(b->data);
            
            return;
        }
    }

    if (t->position.x > bullet->b->scene->get_window()->get_resolution().x || t->position.x < 0 
            || t->position.y > bullet->b->scene->get_window()->get_resolution().y || t->position.y < 0)
    {
        bullet->b->scene->get_ecs()->remove_entity(bullet->b->id);
        free(bullet->b);
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

void boss_bullet_update(sm::behavior* b)
{
    struct bullet_data* bullet = (struct bullet_data*)b->data;

    sm::transform* t = (sm::transform*)bullet->b->scene->get_ecs()->get_component(bullet->b->id, TRANSFORM);

    t->position.x += bullet->b->direction.x * sm::time::delta_time() * bullet->b->speed;
    t->position.y += bullet->b->direction.y * sm::time::delta_time() * bullet->b->speed;

    bullet->b->rect = sm::rect(*t);

    if (bullet->w->player.rect.collision_aabb(bullet->b->rect) && bullet->w->player.invinsible_frames <= 0)
    {
        bullet->w->player.health -= 35.0f;
        bullet->w->player.invinsible_frames = PLAYER_I_FRAMES;

        bullet->b->scene->get_ecs()->remove_entity(bullet->b->id);
        free(bullet->b);
        free(b->data);

        return;
    }

    if (t->position.x > bullet->b->scene->get_window()->get_resolution().x || t->position.x < 0 
            || t->position.y > bullet->b->scene->get_window()->get_resolution().y || t->position.y < 0)
    {
        bullet->b->scene->get_ecs()->remove_entity(bullet->b->id);
        free(bullet->b);
        free(b->data);
    }

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

void world_populate(struct world* world, sm::scene* scene)
{   
    world->player = player_create(scene);
    world->player.world = world;
    world->scene = scene;
    world->boss_timer.target = 5000.0f;

    for (int32_t i = 0; i < ASTEROID_MAX_NUM; i++)
    {
        std::uniform_int_distribution<std::mt19937::result_type> x_pos(32, scene->get_window()->get_resolution().x - 32);
        std::uniform_int_distribution<std::mt19937::result_type> y_pos(32, scene->get_window()->get_resolution().y - 32);

        world->asteroids.push_back(asteroid_create(utilz::vector2f(x_pos(rng), y_pos(rng)), 32.0f, scene));

        std::uniform_int_distribution<std::mt19937::result_type> rot_offset(-20, 20);

        world->asteroids[i].rot_speed_offset = rot_offset(rng);
    }
}

void player_update(struct player* player)
{
    if (!player->alive) return;

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
        struct bullet_data* data = new struct bullet_data;

        data->b = bullet_create(player->scene, player->direction, t->position);
        data->w = player->world;

        sm::behavior* beh = (sm::behavior*)player->scene->get_ecs()->get_component(data->b->id, BEHAVIOR);
        beh->data = data;
    }

    for (struct asteroid a : player->world->asteroids)
    {
        if (player->rect.collision_aabb(a.rect) && player->invinsible_frames <= 0)
        {
            sm::transform* a_t = (sm::transform*)player->scene->get_ecs()->get_component(a.id, TRANSFORM);
            player->health -= a.damage_mult * a_t->scale.x; 
            player->invinsible_frames = PLAYER_I_FRAMES;
        }
    }

    if (player->invinsible_frames > 0)
        player->invinsible_frames--;

    player->scene->get_shader_pool()->retrieve_shader("asteroid_shader")->send_int(player->invinsible_frames, "player_i_frames");

    if (player->health <= 0)
    {
        player->scene->get_ecs()->remove_entity(player->id);
        player->alive = 0;
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
   

    world->asteroids_defeated = world->asteroids.size() == 0 && !world->in_boss;

    if (world->asteroids_defeated && !world->in_boss)
    {
        if (world->boss_timer.update())
        {
            world->boss = boss_create(world->scene, world); 
            world->in_boss = 1;
        }
    }

    world->scene->get_shader_pool()->retrieve_shader("asteroid_shader")->send_int(world->asteroids_defeated, "asteroids_defeated");

    if (world->in_boss)
    {
        boss_update(world, &world->boss);
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

    scene.get_texture_pool()->add_texture("assets/textures/boss.png", "asteroids_boss");
    
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


        sp->shader->use();
        scene.get_camera()->send_matrices(sp->shader, "projection", "view");
        sp->shader->send_float(sm::time::get_time(), "time");


        if (world.player.alive)
        {
            player_update(&world.player);
            world_move_asteroids(&world);

            sm::transform* player_t = (sm::transform*)scene.get_ecs()->get_component(world.player.id, TRANSFORM);
            sp->shader->send_float(player_t->position.x, "player_x");
            sp->shader->send_float(player_t->position.y, "player_y");
            sp->shader->send_float(sm::time::get_time(), "time");
            sp->shader->detach();
        }

        scene.render();

        scene.end_frame();
    }

    return 0;
}

