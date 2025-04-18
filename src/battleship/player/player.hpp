#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

#include <vector>

class player 
{
    public:
        enum ORIENTATION
        {
            LEFT, 
            RIGHT, 
            UP, 
            DOWN,
        };

        struct segment
        {
            sm::entity id;
        };

        player(sm::scene* scene, int32_t segments, utilz::vector2 origin, ORIENTATION orientation);

        void destroy_segment(int32_t location);
        void kill();
    private: 
        std::vector<segment> m_segments;
        ORIENTATION m_orientation;
        utilz::vector2 m_origin;
};

