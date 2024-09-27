#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/input/input.hpp"
#include "engine/physics/ray.hpp"
#include "engine/scene/scene.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/render/mesh_types/line.hpp"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm/ext/matrix_clip_space.hpp"
#include <cmath>
#include <vector>

void create_static(sm::scene* scene, utilz::vector2f pos) 
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | SPRITE | PHYSICS_BODY);

    sm::transform* t = ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::sprite* sp = ECS_GET_SPRITE(scene->get_ecs(), e);
    sm::physics_body* ph = ECS_GET_PHYSICS_BODY(scene->get_ecs(), e);
    t->position = pos;
    t->scale = utilz::vector2f(32.0f);
    sp->color = utilz::rgba_color(255, 0, 255, 255);

    ph->body_type = sm::BODY_STATIC;
}

void generate_scene(sm::scene* scene, int obj_count) 
{
    for (int i = 0; i < obj_count; i++)
        create_static(scene, utilz::vector2f(utilz::m_utilz::rand_i(0, scene->get_window()->get_resolution().x), utilz::m_utilz::rand_i(0, scene->get_window()->get_resolution().y)));
}

struct debug_ray 
{
    sm::physics::ray ray;
    sm::physics::ray_hit hit;
    sm::render::line_mesh mesh;
    sm::entity hit_entity;
};

void generate_rays(sm::scene* scene, int ray_count, utilz::vector2f origin, std::vector<debug_ray>* rays)
{
    float angle = 0.0f;
    float angle_increase = 360.0f / ray_count;

    for (int i = 0; i < ray_count; i++)
    {
        rays->push_back(debug_ray());
        debug_ray* ray = &rays->at(i);

        ray->hit_entity = scene->get_ecs()->create_entity(TRANSFORM | SPRITE);

        sm::transform* t = ECS_GET_TRANSFORM(scene->get_ecs(), ray->hit_entity);
        sm::sprite* sp = ECS_GET_SPRITE(scene->get_ecs(), ray->hit_entity);

        t->scale = utilz::vector2f(10.0f);
        sp->color = utilz::rgba_color(RGBA_RED);
        sp->config = SPRITE_CONFIG_CENTERED;

        double rad = utilz::m_utilz::deg_2_rad(angle);

        sm::render::line_mesh* line = &rays->at(i).mesh;
        utilz::vector2f dir = utilz::vector2f(cos(rad), sin(rad));
        dir.normalize();

        line->create(origin, utilz::vector2f(origin.x + dir.x * 2000.0f, origin.y + dir.y * 2000.0f), utilz::rgba_color(RGBA_BLUE));
    
        angle += angle_increase;
    }
}

void render_rays(sm::scene* scene, sm::shader* line_shader, std::vector<debug_ray>* rays)
{
    scene->get_camera()->send_matrices(line_shader, "projection", "view");

    line_shader->use();

    for (debug_ray ray : *rays)
    {
        ray.mesh.update();
    }

    line_shader->detach();
}

void update_rays_mesh(sm::scene* scene, std::vector<debug_ray>* rays, utilz::vector2f pos)
{
    float angle = 0.0f;
    float angle_increase = 360.0f / rays->size();

    for (debug_ray ray : *rays)
    {
        double rad = utilz::m_utilz::deg_2_rad(angle);

        utilz::vector2f dir = utilz::vector2f(cos(rad), sin(rad));
        dir.normalize();

        ray.mesh.get_vertices()[0].position = pos;

        ray.mesh.get_vertices()[1].position = utilz::vector2f(pos.x + dir.x * 2000.0f, pos.y + dir.y * 2000.0f);
        ray.mesh.update_mesh();

        ray.ray.origin = ray.mesh.get_vertices()[0].position;
        ray.ray.direction = dir;
        
        sm::physics::shoot_ray(ray.ray, scene, ray.hit);

        sm::transform* t = ECS_GET_TRANSFORM(scene->get_ecs(), ray.hit_entity);
        t->position = ray.hit.point;

        angle += angle_increase;
    }
}

void delete_lines(std::vector<debug_ray>* rays)
{
    for (debug_ray ray : *rays)
        ray.mesh.free();
}

int main(void)
{
    sm::window win = sm::window("Engine Window", 1600, 1200,
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/line_vertex.glsl", "assets/shaders/line_fragment.glsl", "line_shader");

    sm::shader* line_shader = scene.get_shader_pool()->retrieve_shader("line_shader");

    std::vector<debug_ray> rays;

    generate_scene(&scene, 80);
    generate_rays(&scene, 50, scene.get_window()->get_center(), &rays);

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();
        scene.new_physics_frame();

        update_rays_mesh(&scene, &rays, sm::input::get_mouse_position());
        render_rays(&scene, line_shader, &rays);

        scene.render();
        scene.end_frame();
    }

    delete_lines(&rays);
    
    return 0;
}

