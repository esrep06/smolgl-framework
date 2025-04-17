#version 330 core

out vec4 FragColor;
in vec4 ourColor;

const int GRID_SIZE = 32;

void main()
{
    vec2 pixel = gl_FragCoord.xy;

    if (int(pixel.x) % GRID_SIZE == 0 || int(pixel.y) % GRID_SIZE == 0)
    {
        FragColor = vec4(0.2f, 0.2f, 0.2f, 0.3f);
    }
    else
        FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
