#include "input.hpp"

namespace kt
{
    void input::key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        switch (action)
        {   
            case GLFW_PRESS:
                m_keys_pressed[key] = 1;
                m_keys_held[key] = 1;
                break;
            case GLFW_RELEASE:
                m_keys_pressed[key] = 0;
                m_keys_held[key] = 0;
        }
    }

    void input::text_stream_callback(GLFWwindow* window, uint32_t character)
    { m_input_stream = character; }

    uint8_t input::get_key_down(uint32_t key)
    { return input::m_keys_pressed[key]; }

    uint8_t input::get_key(uint32_t key)
    { return input::m_keys_held[key]; }

    uint8_t input::get_input_stream()
    { return input::m_input_stream; }

    void input::end_frame()
    {
        for (int i = 0; i < KEY_NUMBER; i++) m_keys_pressed[i] = 0;
        m_input_stream = 0;
    }

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

    uint8_t input::m_keys_pressed[KEY_NUMBER];
    uint8_t input::m_keys_held[KEY_NUMBER];
    uint8_t input::m_buttons_pressed[MOUSE_BUTTON_NUMBER];
    uint8_t input::m_buttons_held[MOUSE_BUTTON_NUMBER];
    uint8_t input::m_input_stream;

    input::CURSOR_STATE input::m_cursor_state;
}
