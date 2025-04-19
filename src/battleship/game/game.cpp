#include "game.hpp"

game::game(uint8_t game_id)
    : m_game_id(game_id)
{
}

void game::add_player(sm::scene* scene)
{
    if (m_player_num >= 1) return;
    m_players[m_player_num] = player(scene, m_game_id, m_player_num + 1);
    m_player_num++;
}

// We will only deal with a single player for now 

void game::update(sm::scene* scene) 
{
    m_players[0].update(scene);
}

player* game::get_player(uint8_t id)
{ 
    if (id < 0 || id > 1) return NULL;

    return &m_players[id]; 
}

