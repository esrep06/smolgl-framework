#pragma once 

#include "engine/scene/scene.hpp"
#include "../player/player.hpp"

#define MAX_PLAYERS 2 

class game 
{
    public: 
        game(uint8_t game_id);

        void add_player(sm::scene* scene);
        void update(sm::scene* scene);
        player* get_player(uint8_t id);
    private:
        uint8_t m_player_num = 0;
        uint8_t m_game_id;
        player m_players[MAX_PLAYERS];
};

