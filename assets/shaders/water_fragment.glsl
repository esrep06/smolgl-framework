#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform float time;

const int WATER_START_POS = 440;
const int WAVE_AMPLITUDE = 10;
const float WAVE_FREQ = 0.02;
const float WAVE_SPEED = 1.2f;

void main()
{
    vec2 pixel = gl_FragCoord.xy;
    float wave = sin(pixel.x * WAVE_FREQ + time * WAVE_SPEED) * WAVE_AMPLITUDE;
    float water_level = wave + WATER_START_POS;

    float frag_y = 630 - pixel.y;

    // FragColor = vec4(sin(time), 0.0f, 0.0f, 1.0f);

    if (frag_y < water_level)
        FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    else
        FragColor = ourColor / 255.0f;
}
