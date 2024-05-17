#pragma once 

#include <inttypes.h>
#include <stdexcept>

#define INCLUDE_GLFW_NONE
#include "../../../include/glfw/include/GLFW/glfw3.h"

namespace kt
{
    class input
    {
        public:
            static const uint16_t KEY_NUMBER = 350;
            static const uint16_t MOUSE_BUTTON_NUMBER = 5;

            enum CURSOR_STATE
            {
                CURSOR_HIDDEN,
                CURSOR_ENABLED,
            };

            static void key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
            static void text_stream_callback(GLFWwindow* window, uint32_t character);
            static uint8_t get_key_down(uint32_t key);
            static uint8_t get_key(uint32_t key);
            static uint8_t get_input_stream();
            static void switch_cursor_state(CURSOR_STATE state, GLFWwindow* window);
            static void end_frame();
            static CURSOR_STATE get_cursor_state();
        private:
            static uint8_t m_keys_pressed[KEY_NUMBER];
            static uint8_t m_keys_held[KEY_NUMBER];
            static uint8_t m_input_stream;

            static uint8_t m_buttons_pressed[MOUSE_BUTTON_NUMBER];
            static uint8_t m_buttons_held[MOUSE_BUTTON_NUMBER];
            static CURSOR_STATE m_cursor_state;
    };
}

