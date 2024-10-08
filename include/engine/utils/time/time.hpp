#pragma once 

#define GLFW_INCLUDE_NONE
#include <glfw/include/GLFW/glfw3.h>

namespace sm
{
    class time 
    {
        public:
            static double delta_time();
            static double get_time();
            static void calculate_update();

            struct timer;
        private:
            inline static double m_start_time, m_delta_time;
    };

    struct time::timer
    {
        double target;
        double timer;

        uint8_t update();
        void reset();
    };
}

