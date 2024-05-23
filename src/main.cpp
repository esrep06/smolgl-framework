#include "engine/scene/scene.hpp"

int main(void)
{
    sm::window win = sm::window("SM Framework", 800, 600, 
            utilz::rgba_color(100, 130, 180, 255));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    scene.init();

    while (!glfwWindowShouldClose(win.get_context()))
    {
        sm::time::calculate_update();

        glfwPollEvents();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) 
        {
            utilz::logger::log("Closing...\n");
            break; 
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        scene.render();

        // End frame funcs
        sm::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    return 0;
}

