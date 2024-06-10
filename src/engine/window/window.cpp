#include "window.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "glfw/include/GLFW/glfw3.h"

namespace sm
{
    window::window(std::string title, uint16_t width, uint16_t height, utilz::rgba_color color)
        : m_width(width), m_height(height), m_title(title), m_color(color)
    {
        if (!glfwInit())
        {
            utilz::logger::log("GLFW init failed\n", utilz::logger::ERROR);
            return;
        }

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        const char* glsl_version = "#version 150";
#else
        const char* glsl_version = "#version 130";
#endif

        m_context = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

        if (!m_context)
        {
            utilz::logger::log("Window creation failed\n", utilz::logger::ERROR);
            return;
        }

        glfwMakeContextCurrent(m_context);

        glfwSwapInterval(1);

        GLenum err = glewInit();

        if (err != GLEW_OK)
        {
            utilz::logger::log("GLEW init failed\n", utilz::logger::ERROR);
            return;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set callbacks

        glfwSetKeyCallback(m_context, input::key_callback);
        glfwSetCharCallback(m_context, input::text_stream_callback);
        glfwSetCursorPosCallback(m_context, input::mouse_position_callback);
        glfwSetMouseButtonCallback(m_context, input::mouse_button_callback);

        glfwSetWindowUserPointer(m_context, this);

        glfwSetWindowSizeCallback(m_context, window_size_callback);

        input::switch_cursor_state(input::CURSOR_ENABLED, m_context);

        utilz::logger::log("Window created successfully!\n", utilz::logger::SUCCESS);

        const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

        utilz::logger::log("Renderer Info:\n    Vendor: " + std::string(reinterpret_cast<const char*>(vendor)) + "\n    Renderer: " + std::string(reinterpret_cast<const char*>(renderer)) + "\n");
    }

    window::window()
    { }

    void window::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(RGBA_NORMALIZED_FLOAT(m_color));
    }

    void window::swap_buffers()
    {
        glfwSwapBuffers(m_context);
    }

    void window::window_size_callback(GLFWwindow* window, int width, int height)
    {
        sm::window* w = (sm::window*)glfwGetWindowUserPointer(window);
        w->set_resolution(width, height);

        glViewport(0.0f, width, 0.0f, -height);
    }

    window::~window()
    {
        utilz::logger::log("Closing window\n");

        glfwDestroyWindow(m_context);
        glfwSetWindowShouldClose(m_context, GLFW_TRUE);
        glfwTerminate();
    }

    void window::set_resolution(int32_t width, int32_t height)
    { 
        m_width = width;
        m_height = height;
    }

    GLFWwindow* window::get_context()
    { return m_context; } 

    utilz::rgba_color window::get_color()
    { return m_color; }

    utilz::vector2 window::get_resolution()
    { return utilz::vector2(m_width, m_height); }

    utilz::vector2f window::get_center()
    { return utilz::vector2f(m_width * 0.5f, m_height * 0.5f); }
}
