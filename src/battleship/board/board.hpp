#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 960
#define GRID_X_POS 256
#define GRID_Y_POS 240
#define GRID_SIZE 32

class boat 
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

        boat(sm::scene* scene, int32_t segments, 
                utilz::vector2 origin, ORIENTATION orientation, uint8_t player, uint8_t game);

        void destroy_segment(int32_t location);
        void kill();
    private: 
        std::vector<segment> m_segments;
        ORIENTATION m_orientation;
        utilz::vector2 m_origin;
        uint8_t m_player;
        uint8_t m_game_id;
};

class board 
{
    public:
        board();
        board(sm::scene* scene);
        void update(sm::scene* scene);
    private: 
        int32_t m_grid_size = GRID_SIZE;
};

