#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform int grid_size;        // 32 in your case
vec2 square_size = vec2(512, 480);     // (512, 480)
vec2 screen_size = vec2(1024, 960);     // (1024, 960) - twice your square size
uniform vec4 grid_color = vec4(0.2, 0.2, 0.2, 0.8);
uniform vec4 bg_color = vec4(0.0, 0.0, 0.0, 0.0);

void main()
{
    vec2 pixel = gl_FragCoord.xy;
    vec2 square_pos = (screen_size - square_size) * 0.5; // Center position
    
    // Convert to square-local coordinates
    vec2 local_pixel = pixel - square_pos;
    
    // Discard fragments outside the square
    if (local_pixel.x < 0.0 || local_pixel.y < 0.0 || 
        local_pixel.x >= square_size.x || local_pixel.y >= square_size.y) {
        FragColor = bg_color;
        return;
    }
    
    float line_width = 1.0;
    
    // Check grid lines (including borders)
    bool is_grid = 
        mod(local_pixel.x, grid_size) < line_width ||
        mod(local_pixel.y, grid_size) < line_width ||
        local_pixel.x >= square_size.x - line_width ||  // Right border
        local_pixel.y >= square_size.y - line_width;    // Top border
    
    FragColor = is_grid ? grid_color : bg_color;
}
