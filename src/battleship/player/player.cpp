#include "player.hpp"

player::player()
{} 

player::player(sm::scene* scene, uint8_t game_id, uint8_t id)
    : m_board(scene), m_game_id(game_id), m_id(id)
{


}

void player::update(sm::scene* scene)
{
    std::string player_text = std::format("Player: {}", m_id);
    float text_width = scene->get_font_pool()->retrieve_font("atari")->get_text_size(player_text, 0.8f).x;
    scene->get_font_pool()->retrieve_font("atari")->render_text(player_text, (SCREEN_WIDTH - text_width) * 0.5f, 16.0f, 0.8f, utilz::rgba_color(0, 0, 0, 255));

    float mouse_x = sm::input::get_mouse_position().x;
    float mouse_y = sm::input::get_mouse_position().y;

    // Mouse position relative to the grid 
    int grid_mouse_x = (utilz::m_utilz::clamp(0, (SCREEN_WIDTH * 0.5f), mouse_x - GRID_X_POS) / GRID_SIZE);
    int grid_mouse_y = (utilz::m_utilz::clamp(0, (SCREEN_HEIGHT * 0.5f), mouse_y - GRID_Y_POS) / GRID_SIZE);

    // Mouse inside grid
    if (mouse_x > GRID_X_POS && mouse_x < GRID_X_POS + (SCREEN_WIDTH * 0.5f) && 
        mouse_y > GRID_Y_POS && mouse_y < GRID_Y_POS + (SCREEN_HEIGHT * 0.5f))
    {
        scene->get_font_pool()->retrieve_font("atari")->render_text(
                std::format("x: {}, y: {}", grid_mouse_x, grid_mouse_y), 
                0.0f, SCREEN_HEIGHT - 16.0f, 0.8f, utilz::rgba_color(0, 0, 0, 255));

        if (sm::input::get_button_down(GLFW_MOUSE_BUTTON_1))
        {
            boat b = boat(scene, 4, utilz::vector2(grid_mouse_x, grid_mouse_y), boat::ORIENTATION::LEFT, m_id, m_game_id);  
            place_boat(b);
        }
    }

    m_board.update(scene);
}

void player::place_boat(boat b)
{

}

