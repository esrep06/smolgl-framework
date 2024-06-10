#pragma once 

#include <cstdint>

#define INCLUDE_GLFW_NONE
#include "../../../include/glfw/include/GLFW/glfw3.h"
#include "../../../include/cpp-utilz/math/vector2.hpp"

#include "../include/cpp-utilz/logger/logger.hpp"

#include <iostream>

namespace sm
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

            static void end_frame();

            // Keyboard functions
            static void key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
            static void text_stream_callback(GLFWwindow* window, uint32_t character);

            static uint8_t get_key_down(uint32_t key);
            static uint8_t get_key(uint32_t key);

            static uint8_t get_input_stream();

            // Mouse functions

            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);

            static uint8_t get_button_down(uint32_t button);
            static uint8_t get_button(uint32_t button);
            static uint8_t get_button_up(uint32_t button);
            static utilz::vector2f get_mouse_position();

            static void switch_cursor_state(CURSOR_STATE state, GLFWwindow* window);
            static CURSOR_STATE get_cursor_state();
        private:
            static uint8_t m_keys_pressed[KEY_NUMBER];
            static uint8_t m_keys_held[KEY_NUMBER];
            static uint8_t m_input_stream;

            static uint8_t m_buttons_pressed[MOUSE_BUTTON_NUMBER];
            static uint8_t m_buttons_held[MOUSE_BUTTON_NUMBER];
            static uint8_t m_buttons_released[MOUSE_BUTTON_NUMBER];
            static CURSOR_STATE m_cursor_state;
            static utilz::vector2f m_mouse_position;
    };
}

