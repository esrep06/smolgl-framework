#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

#include "game/game.hpp"

// #include "board/board.hpp"

int main(void)
{
    sm::file_util::set_working_dir("../../../");

    sm::window win = sm::window("Battleship", SCREEN_WIDTH, SCREEN_HEIGHT,
            utilz::rgba_color(RGBA_BLACK));

    sm::scene scene = sm::scene(&win, sm::camera(utilz::vector2f(0.0f), 
                utilz::vector2(win.get_resolution().x, win.get_resolution().y)));

    utilz::m_utilz math_utils = utilz::m_utilz();
    
    game main_game = game(1);
    main_game.add_player(&scene);

    while (!glfwWindowShouldClose(win.get_context()))
    {
        scene.new_frame();
        scene.new_physics_frame();

        scene.render();

        main_game.update(&scene);

        scene.get_font_pool()->retrieve_font("atari")->render_text(std::format("FPS: {}", (int)scene.get_fps()), 0.0f, 0.0f, 0.8f, utilz::rgba_color(0, 200, 0, 255));
        scene.end_frame();
    }

    return 0;
}

