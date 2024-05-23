#pragma once 

#include "../../../engine/scene/scene.hpp"
#include <random>

#define FOOD_SCALE 8

struct food 
{
    sm::entity id;

    void create(sm::scene* scene);
};

