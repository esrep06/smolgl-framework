#pragma once 

struct Vector2f
{
    float x, y;
    
    Vector2f(float x, float y);
    Vector2f(float val);
    Vector2f();
    
    void Normalize();
    static Vector2f Zero();
};

