#pragma once  

#include "engine/scene/scene.hpp"
#include "engine/utils/time/time.hpp"

#define CELL_WIDTH 16 
#define CELL_HEIGHT 16
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ROWS SCREEN_WIDTH / CELL_WIDTH
#define COLS SCREEN_HEIGHT / CELL_HEIGHT

namespace game 
{
    struct cell 
    {
        uint8_t alive;
        sm::entity id;
        int32_t x, y;
        int32_t neighbors;
    }; 

    class world
    {
        public:
            world(double tick_delay_ms, sm::scene* scene); 

            void update(sm::scene* scene);
            void end();
        private:
            void tick(sm::ecs* ecs);
            void add_cell(int32_t x, int32_t y, sm::ecs* ecs);
            void kill_cell(int32_t x, int32_t y, sm::ecs* ecs);
            cell* get_cell(int32_t x, int32_t y);

            std::vector<cell> cells;

            uint8_t start = 0;

            sm::time::timer tick_timer;
    };
}

