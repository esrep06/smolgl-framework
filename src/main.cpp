#include "engine/window/window.hpp"
#include "engine/utils/time/time.hpp"

int main(void)
{
    kt::window win = kt::window("Hello OpenGL", 1920, 1080, utilz::rgba_color(RGBA_WHITE));
    kt::time t;

    while (!glfwWindowShouldClose(win.get_context()))
    {
        t.calculate_update();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        if (kt::input::get_key_down(GLFW_KEY_ESCAPE)) { break; }

        // End frame funcs
        kt::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    return 0;
}

