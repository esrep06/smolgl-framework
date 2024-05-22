#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/input/input.hpp"
#include "engine/window/window.hpp"
#include "engine/utils/time/time.hpp"
#include "engine/scene/scene.hpp"

# define PI 3.1415926535897

sm::entity create_water(sm::scene* scene)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::sprite* spr = (sm::sprite*)scene->get_ecs()->get_component(e, SPRITE);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(e, TRANSFORM);
    spr->color = utilz::rgba_color(0, 0, 255, 100);
    t->position = utilz::vector2f(0.0f, 0.0f);
    t->scale = utilz::vector2f(800.0f, 600.0f);
    spr->shader = scene->get_shader_pool()->retrieve_shader("water_shader");

    return e;
}

struct player 
{
    sm::entity id;
    float speed = 90.0f;
    float rotation_speed = 50.0f;
};

player create_player(sm::scene* scene)
{
    player p;
    p.id = scene->get_ecs()->create_entity(TEXTURED_SPRITE | TRANSFORM);
    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(p.id, TRANSFORM);
    sm::textured_sprite* spr = (sm::textured_sprite*)scene->get_ecs()->get_component(p.id, TEXTURED_SPRITE);
    spr->shader = scene->get_shader_pool()->retrieve_shader("default_textured_shader");
    t->position.x = scene->get_window()->get_resolution().x * 0.5f;
    t->position.y = scene->get_window()->get_resolution().y * 0.5f;
    t->scale.x = 64.0f;
    t->scale.y = 128.0f;
    spr->texture = scene->get_texture_pool()->retrieve_texture("player_boat");
    spr->config |= SPRITE_CONFIG_CENTERED;

    return p;
}

float deg_2_rad(float angle)
{ return angle * PI / 180; }

void move_player(player* player, sm::scene* scene)
{
    int8_t rotation_input = sm::input::get_key(GLFW_KEY_D) ? 1.0f : sm::input::get_key(GLFW_KEY_A) ? -1.0f : 0.0f;
    uint8_t move_input = sm::input::get_key(GLFW_KEY_W) ? 1.0f : 0.0f;

    sm::transform* t = (sm::transform*)scene->get_ecs()->get_component(player->id, TRANSFORM);

    utilz::vector2f dir = utilz::vector2f(sin(deg_2_rad(t->angle)), -cos(deg_2_rad(t->angle)));

    if (t->angle < 0) t->angle = 360;
    if (t->angle > 360) t->angle = 0;

    if (move_input)
        t->angle += rotation_input * sm::time::delta_time() * player->rotation_speed;

    t->position.x += dir.x * move_input * player->speed * sm::time::delta_time();
    t->position.y += dir.y * move_input * player->speed * sm::time::delta_time();
}

int main(void)
{
    sm::window win = sm::window("Hello OpenGL", 800, 600, 
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    scene.get_shader_pool()->add_shader("assets/game_shaders/water_vertex.glsl", "assets/game_shaders/water_fragment.glsl", "water_shader");
    scene.get_texture_pool()->add_texture("assets/textures/player_boat.png", "player_boat");

    scene.init();

    sm::entity water = create_water(&scene);
    player p = create_player(&scene);

    while (!glfwWindowShouldClose(win.get_context()))
    {
        sm::time::calculate_update();

        glfwPollEvents();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) 
        {
            utilz::logger::log("Closing...\n");
            break; 
        }

        move_player(&p, &scene);

        sm::shader* water_shader = scene.get_shader_pool()->retrieve_shader("water_shader");

        water_shader->send_mat4(scene.get_camera()->get_view(), "view");
        water_shader->send_mat4(scene.get_camera()->get_proj(), "projection");
        water_shader->send_float(win.get_resolution().x, "width");
        water_shader->send_float(win.get_resolution().y, "height");
        water_shader->send_float((float)sm::time::get_time(), "time");
        sm::transform* tr = (sm::transform*)scene.get_ecs()->get_component(p.id, TRANSFORM);
        water_shader->send_float(tr->position.x, "pos_x");
        water_shader->send_float(tr->position.y, "pos_y");
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        scene.render();

        // End frame funcs
        sm::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    return 0;
}

