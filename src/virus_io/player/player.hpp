#pragma once 

#include "cpp-utilz/math/vector2.hpp"
#include "engine/scene/scene.hpp"
#include "engine/utils/file.hpp"
#include "cpp-utilz/math/m_utilz.hpp"
#include "engine/font/font.hpp"

class player 
{
    public: 
        player(sm::scene* scene, std::string name, utilz::vector2f position);
        void update(sm::scene* scene);
    private:
        const float m_starting_size = 16.0f;
        float m_size;

        sm::entity m_entity;
        std::string m_name;
};

