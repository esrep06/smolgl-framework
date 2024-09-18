#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/ecs/systems/animator_system/animator_system.hpp"
#include "engine/ecs/systems/transform_system/transform_system.hpp"
#include "engine/input/input.hpp"
#include "engine/scene/scene.hpp"
#include "glfw/include/GLFW/glfw3.h"

sm::entity create_line_segment(sm::scene* scene, utilz::vector2f pos, float line_width)
{
    sm::entity e = scene->get_ecs()->create_entity(TRANSFORM | TEXTURED_SPRITE | ANIMATOR);
    sm::textured_sprite* sp = ECS_GET_TEXTURED_SPRITE(scene->get_ecs(), e);
    sm::transform* t = ECS_GET_TRANSFORM(scene->get_ecs(), e);
    sm::animator* a = (sm::animator*)scene->get_ecs()->get_component(e, ANIMATOR);
    sp->texture = scene->get_texture_pool()->retrieve_texture("coin_spin");
    t->position = pos;
    t->scale = utilz::vector2f(line_width);

    sm::animation anim;

    anim.name = "coin_spin";
    anim.switch_timer.target = 90.0f;
    anim.texture = sp->texture;
    anim.x_bound = 6;
    anim.x = 0;
    anim.w = 16;

    sm::animator_system::add_animation(a, anim);
    sm::animator_system::enable_animation(a, "coin_spin");

    return e;
}

int main(void)
{
    sm::window win = sm::window("Asteroids", 800, 600, 
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    utilz::vector2f ray_dir = utilz::vector2f(500.0f, 320.0f);
    utilz::vector2f ray_origin = win.get_center();

    scene.get_texture_pool()->add_texture("assets/textures/coin_spin_2.png", "coin_spin");


    std::vector<sm::entity> line_entities;

    const float LINE_WIDTH = 16.0f;

    for (int32_t i = 0; i < 5; i++)
    {
        utilz::vector2f p;
        p.x = ray_origin.x + (ray_dir.x * i * LINE_WIDTH);
        p.y = ray_origin.y + (ray_dir.y * i * LINE_WIDTH);

        line_entities.push_back(create_line_segment(&scene, p, LINE_WIDTH));
    }

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();

        scene.render();

        // Line function should ask for, target, source, size 
        // target: target point in world coordinates 
        // source: 2d point in world coordinates 
        // size: segment size of our line 

        // Update position and size of our line based on an arbitrary target point, a direction calculated from that point, and a source  
        for (size_t i = 0; i < line_entities.size(); i++)
        {
            utilz::vector2f p;  
            utilz::vector2f target_point = sm::input::get_mouse_position();

            // Calculate direction vector from target point 
            utilz::vector2f dir = utilz::vector2f(target_point.x - ray_origin.x, target_point.y - ray_origin.y);

            dir.normalize();

            // Point p along our line, p(n) = origin + (dir * n * WIDTH)
            p.x = ray_origin.x + (dir.x * i * LINE_WIDTH);
            p.y = ray_origin.y + (dir.y * i * LINE_WIDTH);

            // Our last point in our line is calculated with the same function p(n), n being the size of the segments currently in the line
            utilz::vector2f last_point = utilz::vector2f((ray_origin.x + (dir.x * (line_entities.size() - 1) * LINE_WIDTH)) + LINE_WIDTH, (ray_origin.y + (dir.y * (line_entities.size() - 1) * LINE_WIDTH)) + LINE_WIDTH);

            // Calculate the target vector, if our point is greater than the last segment, our point becomes our target, otherwise the last segment does 
            utilz::vector2f target;
            target.x = target_point.x > ray_origin.x ? target_point.x : last_point.x;
            target.y = target_point.y > ray_origin.y ? target_point.y : last_point.y;

            // Calculate the source vector, if our point is less than the last segment, our point becomes our source, otherwise the last segment does 
            utilz::vector2f source;
            source.x = target_point.x < ray_origin.x ? target_point.x : last_point.x;
            source.y = target_point.y < ray_origin.y ? target_point.y : last_point.y;

            // We substract the source from the target to obtain the rough distance between the target and our last segment
            utilz::vector2f size_sub = utilz::vector2f(target.x - source.x, target.y - source.y);

            // We can calculate the rough number of squares by adding both x and y from our sub vector and dividing it by our line width
            int32_t squares = (size_sub.x + size_sub.y) / LINE_WIDTH;

            /* std::cout << "Mouse: " + target_point.to_string() << std::endl; */
            /* std::cout << "Last point: " + last_point.to_string() << std::endl; */
            /* std::cout << "Size sub: " + size_sub.to_string() << std::endl; */
            /* std::cout << "Squares: " + std::to_string(squares) << std::endl; */

            sm::entity e = line_entities[i];
            sm::transform* t = ECS_GET_TRANSFORM(scene.get_ecs(), e);
            t->position = p;

            // Add or remove squares depending on the number of squares that make up the distance from target to source

            if (squares < 0 && line_entities.size() > 2)
            {
                for (int32_t r = squares; r < 0; r++)
                {
                    // We simply pop the last segment from our list, we always want to leave at least 1 segment just in case our mouse was out of our window, otherwise the line is removed and we are locked out of the loop 

                    sm::entity back = line_entities.back();

                    scene.get_ecs()->remove_entity(back);
                    line_entities.pop_back();

                }
            } 
            else if (squares > 0)
            {
                for (int32_t a = squares; a > 0; a--)
                {

                    // We can calculate our new point by using our p(n) function, n being the size of line segment + a, a being the number of squares to add 
                    utilz::vector2f new_point = utilz::vector2f((ray_origin.x + (dir.x * ((line_entities.size() - 1) + a) * LINE_WIDTH)) + LINE_WIDTH, (ray_origin.y + (dir.y * ((line_entities.size() - 1) + a) * LINE_WIDTH)) + LINE_WIDTH);

                    line_entities.push_back(create_line_segment(&scene, new_point, LINE_WIDTH));
                }
            }
        }

        scene.end_frame();
    }

    return 0;
}

