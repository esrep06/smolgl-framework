#pragma once 

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H 

#include "cpp-utilz/math/vector2.hpp"

class font_render
{
    struct Character;

};

struct font_render::Character
{
    unsigned int TextureID;  // ID handle of the glyph texture
    utilz::vector2f   Size;       // Size of glyph
    utilz::vector2f   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

