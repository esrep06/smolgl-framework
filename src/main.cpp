#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/window/window.hpp"
#include "engine/utils/time/time.hpp"
#include "engine/scene/scene.hpp"

// Fix component retrieving

int main(void)
{
    sm::window win = sm::window("Hello OpenGL", 1920, 1080, 
            utilz::rgba_color(RGBA_WHITE));
    sm::time t;
    sm::scene scene = sm::scene(sm::camera(utilz::vector2f(0.0f, 0.0f), utilz::vector2(1920, 1080)));

    sm::entity e = scene.get_ecs()->create_entity(TRANSFORM | SPRITE);
    sm::transform* tr = (sm::transform*)scene.get_ecs()->get_component(e, TRANSFORM);
    tr->position = utilz::vector2f(100.0, 100.0f);
    tr->scale = utilz::vector2f(50.0f, 50.0f);

    sm::shader* s = scene.get_shader_pool()->retrieve_shader("triangle_shader");

    s->use();

    while (!glfwWindowShouldClose(win.get_context()))
    {
        t.calculate_update();
        glfwPollEvents();

        if (sm::input::get_key_down(GLFW_KEY_ESCAPE)) { break; }

        s->send_mat4(scene.get_camera()->get_view(), "view");
        s->send_mat4(scene.get_camera()->get_proj(), "projection");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(win.get_color()));

        scene.render();

        // End frame funcs
        sm::input::end_frame();
        glfwSwapBuffers(win.get_context());
    }

    s->free();

    return 0;
}

