#pragma once 

#define GLFW_INCLUDE_NONE
#include "../../../../include/glfw/include/GLFW/glfw3.h"

namespace kt
{
    class time 
    {
        public:
            double delta_time();
            double get_time();
            void calculate_update();
        private:
            double m_start_time, m_delta_time;
    };
}

