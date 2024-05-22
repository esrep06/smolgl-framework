#version 450 core

out vec4 FragColor;
in vec4 ourColor;

uniform float width;
uniform float height;
uniform float time;

#define TAU 6.28318530718
#define MAX_ITER 5

void main()
{
    vec2 res = vec2(width * 0.5f, height * 0.5f);

    float t_time = time * .5+23.0;
    // uv should be the 0-1 uv of texture...
	vec2 uv = gl_FragCoord.xy / res.xy;
    
    vec2 p = mod(uv*TAU, TAU)-250.0;

	vec2 i = vec2(p);
	float c = 1.0;
	float inten = .005;

	for (int n = 0; n < MAX_ITER; n++) 
	{
		float t = t_time * (1.0 - (3.5 / float(n+1)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
	}
	c /= float(MAX_ITER);
	c = 1.17-pow(c, 1.4);
	vec3 colour = vec3(pow(abs(c), 8.0));
    colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);

    FragColor = vec4(colour, 1.0);

    //FragColor = vec4((ourColor.r / 255.0f), (ourColor.g / 255.0f), (ourColor.b / 250.0f), (ourColor.a / 255.0f));
} 

