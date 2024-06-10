#include "input.hpp"
#include "glfw/include/GLFW/glfw3.h"

namespace sm
{
    void input::end_frame()
    {
        for (int i = 0; i < KEY_NUMBER; i++) m_keys_pressed[i] = 0;

        for (int i = 0; i < MOUSE_BUTTON_NUMBER; i++) 
        {
            m_buttons_pressed[i] = 0;
            m_buttons_released[i] = 0;
        }

        m_input_stream = 0;
    }

    // Keyboard functions

    void input::key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        switch (action)
        {   
            case GLFW_PRESS:
                m_keys_pressed[key] = 1;
                m_keys_held[key] = 1;

                if (key == GLFW_KEY_ESCAPE) 
                {
                    utilz::logger::log("Closing...\n");
                    glfwSetWindowShouldClose(window, true);
                }

                break;
            case GLFW_RELEASE:
                m_keys_pressed[key] = 0;
                m_keys_held[key] = 0;
        }
    }

    void input::text_stream_callback(GLFWwindow* window, uint32_t character)
    { 
        m_input_stream = character; 
    }

    uint8_t input::get_key_down(uint32_t key)
    { return input::m_keys_pressed[key]; }

    uint8_t input::get_key(uint32_t key)
    { return input::m_keys_held[key]; }

    uint8_t input::get_input_stream()
    { return input::m_input_stream; }


    // Mouse Functions

    void input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        switch(action)
        {
            case GLFW_PRESS:
                m_buttons_held[button] = 1;
                m_buttons_pressed[button] = 1;
                break;
            case GLFW_RELEASE:
                m_buttons_held[button] = 0;
                m_buttons_released[button] = 1;
                break;
        }
    }

    void input::mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
    { 
        m_mouse_position = utilz::vector2f(xpos, ypos); 
    } 

    uint8_t input::get_button_down(uint32_t button)
    { return m_buttons_pressed[button]; }

    uint8_t input::get_button(uint32_t button)
    { return m_buttons_held[button]; }

    uint8_t input::get_button_up(uint32_t button)
    { return m_buttons_released[button]; }

    void input::switch_cursor_state(CURSOR_STATE state, GLFWwindow* window)
    {
        switch (state) 
        {
            case CURSOR_HIDDEN:
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                m_cursor_state = CURSOR_HIDDEN;
                break;
            case CURSOR_ENABLED:
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                m_cursor_state = CURSOR_ENABLED;
                break;
        };
    }

    input::CURSOR_STATE input::get_cursor_state()
    { return m_cursor_state; }

    utilz::vector2f input::get_mouse_position() 
    { return m_mouse_position; }

    uint8_t input::m_keys_pressed[KEY_NUMBER];
    uint8_t input::m_keys_held[KEY_NUMBER];
    uint8_t input::m_buttons_pressed[MOUSE_BUTTON_NUMBER];
    uint8_t input::m_buttons_held[MOUSE_BUTTON_NUMBER];
    uint8_t input::m_buttons_released[MOUSE_BUTTON_NUMBER];
    uint8_t input::m_input_stream;
    utilz::vector2f input::m_mouse_position;

    input::CURSOR_STATE input::m_cursor_state;
}
