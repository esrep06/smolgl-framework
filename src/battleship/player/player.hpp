#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

#include <vector>

#include "../board/board.hpp"

class player 
{
    public:
        player();
        player(sm::scene* scene, uint8_t game_id, uint8_t id);
        void update(sm::scene* scene);
        void place_boat(boat b);
    private: 
        board m_board;
        std::vector<boat> m_boats;
        uint8_t m_game_id;
        uint8_t m_id;
};

