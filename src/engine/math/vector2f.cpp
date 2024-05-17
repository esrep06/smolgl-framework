#include "vector2f.hpp"
  
Vector2f::Vector2f(float x, float y)
    : x(x), y(y)
{}

Vector2f::Vector2f(float val)
    : x(val), y(val)
{}

Vector2f::Vector2f()
    : x(0.0f), y(0.0f)
{}

Vector2f Vector2f::Zero()
{ return Vector2f(); }
  
void Vector2f::Normalize()
{

}

