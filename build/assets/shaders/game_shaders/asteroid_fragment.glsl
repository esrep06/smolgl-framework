#version 330 core

out vec4 FragColor;
in vec4 ourColor;

uniform float player_x;
uniform float player_y;
uniform float time;

void main()
{
    vec2 pixel_pos = gl_FragCoord.xy;

    if (distance(pixel_pos, vec2(player_x, 600 - player_y)) < 2.5f)
    {
        if ((int(pixel_pos.x) % 2) == 0 || (int(pixel_pos.y) % 2) == 0)
            FragColor = vec4(1.0f * sin(time * 6), 0.4f, 0.3f * sin(time * 3), 1.0f * sin(time * 10));
    }
    else if (int(pixel_pos.y) == int(abs(sin(time) * 600.0f)) || int(pixel_pos.y) == (int(abs(sin(time * 30) * 600.0f))))
        FragColor = vec4(1.0f, 1.0f, 1.0f, 0.2f);
    else FragColor = ourColor / 255;
}

