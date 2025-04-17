#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform float circle_pos_x;
uniform float circle_pos_y;
uniform float radius;

void main()
{
    vec2 pixel = vec2(gl_FragCoord.x, 800 - gl_FragCoord.y);

    if (distance(pixel, vec2(circle_pos_x, circle_pos_y)) < radius)  
        FragColor = ourColor / 255.0f;
    else 
        FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
