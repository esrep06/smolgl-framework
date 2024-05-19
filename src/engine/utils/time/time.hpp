#pragma once 

#define GLFW_INCLUDE_NONE
#include "../../../../include/glfw/include/GLFW/glfw3.h"

namespace sm
{
    class time 
    {
        public:
            static double delta_time();
            static double get_time();
            static void calculate_update();
        private:
            inline static double m_start_time, m_delta_time;
    };
}

