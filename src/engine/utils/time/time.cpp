#include "time.hpp"

namespace kt
{
    double time::delta_time()
    { return m_delta_time; }

    double time::get_time()
    { return glfwGetTime(); }

    void time::calculate_update()
    {
        m_delta_time = get_time() - m_start_time;
        m_start_time = get_time();
    }
}

