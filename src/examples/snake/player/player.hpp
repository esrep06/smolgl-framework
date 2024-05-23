#pragma once

#include <vector>

#include "../../../engine/scene/scene.hpp"
#include "../../../engine/physics/rect.hpp"

#include "../food/food.hpp"

#define SNAKE_MAX_SIZE 255
#define SNAKE_SEGMENT_SCALE 16
#define SNAKE_STRIDE_SPEED_MS 200

enum SNAKE_DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct segment 
{
    sm::entity id;
};

struct snake 
{      
    std::vector<segment> segments;

    segment head;

    uint8_t size;

    float move_timer;

    SNAKE_DIRECTION direction;

    sm::scene* scene;

    food current_food;

    void create(sm::scene* scene);
    void update();
    void move();
    void die();
    void grow();
    void eat();
};

