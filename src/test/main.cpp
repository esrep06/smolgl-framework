#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"

#include <unistd.h>

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("Engine Window", 1600, 1200,
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();

    scene.get_shader_pool()->add_shader("assets/shaders/line_vertex.glsl", "assets/shaders/line_fragment.glsl", "line_shader");

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();
        scene.new_physics_frame();

        scene.render();
        scene.end_frame();
    }

    return 0;
}

