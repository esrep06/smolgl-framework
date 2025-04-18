#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

#include "../player/player.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 960
#define GRID_SIZE 32

class board 
{
    public: 
        board(sm::scene* scene);
        void add_player(player p);
        void update(sm::scene* scene);
    private: 
        std::vector<player> players;
        int32_t m_grid_size = GRID_SIZE;
};

