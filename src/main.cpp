#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/window/window.hpp"
#include "engine/utils/time/time.hpp"
#include "engine/scene/scene.hpp"
#include <cinttypes>

// Fix component retrieving

int main(void)
{
    sm::window win = sm::window("Hello OpenGL", 1920, 1080, 
            utilz::rgba_color(RGBA_WHITE));

    sm::scene scene = sm::scene(sm::camera(utilz::vector2f(0.0f, 0.0f), utilz::vector2(1920, 1080)));

    scene.init();

    while (!glfwWindowShouldClose(win.get_context()))
    {
        sm::time::calculate_update();

        glfwPollEvents();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) { break; }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        scene.render();

        // End frame funcs
        sm::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    return 0;
}

