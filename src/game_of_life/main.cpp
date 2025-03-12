#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "world/world.hpp"

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("Game of Life", SCREEN_WIDTH, SCREEN_HEIGHT,
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/line_vertex.glsl", "assets/shaders/line_fragment.glsl", "line_shader");
    sm::shader* line_shader = scene.get_shader_pool()->retrieve_shader("line_shader");

    game::world world(1000, &scene);
    
    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();
        scene.new_physics_frame();

        world.update(&scene);

        scene.render();
        scene.end_frame();
    }

    return 0;
}

