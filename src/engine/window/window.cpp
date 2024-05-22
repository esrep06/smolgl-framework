#include "window.hpp"
#include "cpp-utilz/math/vector2.hpp"

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
#endif 

        m_context = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);

        if (!m_context)
        {
            utilz::logger::log("Window creation failed\n", utilz::logger::ERROR);
            return;
        }

        glfwMakeContextCurrent(m_context);

        GLenum err = glewInit();

        if (err != GLEW_OK)
        {
            utilz::logger::log("GLEW init failed\n", utilz::logger::ERROR);
            return;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSwapInterval(1);

        // Set callbacks
        glfwSetKeyCallback(m_context, input::key_callback);
        glfwSetCharCallback(m_context, input::text_stream_callback);

        input::switch_cursor_state(input::CURSOR_ENABLED, m_context);

        utilz::logger::log("Window created successfully!\n", utilz::logger::SUCCESS);

        const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

        utilz::logger::log("Renderer Info:\n    Vendor: " + std::string(reinterpret_cast<const char*>(vendor)) + "\n    Renderer: " + std::string(reinterpret_cast<const char*>(renderer)) + "\n");
    }

    window::window()
    { }

    window::~window()
    {
        utilz::logger::log("Closing window\n");

        glfwDestroyWindow(m_context);
        glfwSetWindowShouldClose(m_context, GLFW_TRUE);
        glfwTerminate();
    }

    GLFWwindow* window::get_context()
    { return m_context; } 

    utilz::rgba_color window::get_color()
    { return m_color; }

    utilz::vector2 window::get_resolution()
    { return utilz::vector2(m_width, m_height); } 
}
