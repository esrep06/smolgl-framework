#include "world.hpp"
#include "cpp-utilz/color/color.hpp"
#include "cpp-utilz/logger/logger.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include "engine/ecs/components/components.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/input/input.hpp"
#include "glfw/include/GLFW/glfw3.h"

namespace game 
{
    world::world(double tick_delay_ms, sm::scene* scene)
    {
        tick_timer.target = tick_delay_ms;

        for (int32_t r = 0; r < SCREEN_WIDTH / CELL_WIDTH; r++)
        {
            for (int32_t w = 0; w < SCREEN_HEIGHT / CELL_HEIGHT; w++)
            {
                cell c;
                c.alive = 0;
                c.x = r;
                c.y = w;

                sm::ecs* ecs = scene->get_ecs();
                c.id = ecs->create_entity(TRANSFORM | SPRITE);
                sm::transform* t = ECS_GET_TRANSFORM(ecs, c.id);
                sm::sprite* spr = ECS_GET_SPRITE(ecs, c.id);

                t->position = utilz::vector2f(r * CELL_WIDTH, w * CELL_HEIGHT);
                t->scale = utilz::vector2f(CELL_WIDTH, CELL_HEIGHT);
                spr->color = utilz::rgba_color(255, 255, 255, 0);
                spr->config = SPRITE_CONFIG_TOP_LEFT;

                cells.push_back(c);
            }
        }
    }

    void world::update(sm::scene* scene) 
    {
        if (start)
        {
            if (tick_timer.update())
            {
                utilz::logger::log("On Tick!\n");
                tick(scene->get_ecs());
                tick_timer.reset();
            }
        } else 
            if (sm::input::get_key_down(GLFW_KEY_ENTER)) start = 1;

        utilz::vector2f mouse_pos_raw = sm::input::get_mouse_position();
        utilz::vector2 mouse_pos_cell = 
            utilz::vector2(mouse_pos_raw.x / CELL_WIDTH, mouse_pos_raw.y / CELL_HEIGHT);

        if (sm::input::get_button_down(GLFW_MOUSE_BUTTON_1))
            add_cell(mouse_pos_cell.x, mouse_pos_cell.y, scene->get_ecs());
        else if (sm::input::get_button_down(GLFW_MOUSE_BUTTON_2))
            kill_cell(mouse_pos_cell.x, mouse_pos_cell.y, scene->get_ecs());
    }

    void world::tick(sm::ecs* ecs)
    {
        for (int32_t i = 0; i < cells.size(); i++)
        {   
            cells[i].neighbors = 0;

            cell* t = get_cell(cells[i].x, cells[i].y - 1);
            cell* b = get_cell(cells[i].x, cells[i].y + 1);
            cell* r = get_cell(cells[i].x + 1, cells[i].y);
            cell* l = get_cell(cells[i].x - 1, cells[i].y);

            cell* tl = get_cell(cells[i].x - 1, cells[i].y - 1);
            cell* tr = get_cell(cells[i].x + 1, cells[i].y - 1 );
            cell* bl = get_cell(cells[i].x - 1, cells[i].y + 1);
            cell* br = get_cell(cells[i].x + 1, cells[i].y + 1);

            if (t && t->alive) cells[i].neighbors++;
            if (b && b->alive) cells[i].neighbors++;
            if (r && r->alive) cells[i].neighbors++;
            if (l && l->alive) cells[i].neighbors++;

            if (tl && tl->alive) cells[i].neighbors++;
            if (tr && tr->alive) cells[i].neighbors++;
            if (bl && bl->alive) cells[i].neighbors++;
            if (br && br->alive) cells[i].neighbors++;
        }


        for (int32_t i = 0; i < cells.size(); i++)
        {
            if (cells[i].alive) 
            {
                if (cells[i].neighbors < 2) 
                    kill_cell(cells[i].x, cells[i].y, ecs);
                else if (cells[i].neighbors > 3)
                    kill_cell(cells[i].x, cells[i].y, ecs);
            }
            else 
                if (cells[i].neighbors == 3) 
                    add_cell(cells[i].x, cells[i].y, ecs);

        }
    }

    void world::end()
    {
    }

    void world::add_cell(int32_t x, int32_t y, sm::ecs* ecs)
    {
        cell* c = get_cell(x, y);
        if (c->alive) return;

        c->alive = 1;
        
        sm::sprite* spr = ECS_GET_SPRITE(ecs, c->id);

        spr->color = utilz::rgba_color(255, 255, 255, 255);
        ecs->get_sprite_system()->update_color(spr);
    }

    void world::kill_cell(int32_t x, int32_t y, sm::ecs* ecs)
    {
        cell* c = get_cell(x, y);
        if (!c->alive) return;

        c->alive = 0;
        
        sm::transform* t = ECS_GET_TRANSFORM(ecs, c->id);
        sm::sprite* spr = ECS_GET_SPRITE(ecs, c->id);

        spr->color = utilz::rgba_color(255, 255, 255, 0);
        ecs->get_sprite_system()->update_color(spr);
    }

    cell* world::get_cell(int32_t x, int32_t y)
    {
        for (int32_t i = 0; i < cells.size(); i++)
            if (cells[i].x == x && cells[i].y == y)
                return &cells[i];
        
        return NULL;
    }
}
