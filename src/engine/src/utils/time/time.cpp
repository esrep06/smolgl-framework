#include "engine/utils/time/time.hpp"

namespace sm
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

    uint8_t time::timer::update()
    {
        if (timer < target)
            timer += delta_time() * 1000.0f;

        return timer >= target;
    }
   
    void time::timer::reset()
    { timer = 0.0f; }
}

