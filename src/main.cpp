#include "engine/scene/scene.hpp"

int main(void)
{
    sm::window win = sm::window("SM Framework", 1920, 1080, 
            utilz::rgba_color(100, 130, 180, 255));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    scene.init();

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) 
        {
            utilz::logger::log("Closing...\n");
            break;
        }

        scene.render();

        scene.end_frame();
    }

    return 0;
}

