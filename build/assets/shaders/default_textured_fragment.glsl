#version 330 core

out vec4 FragColor;

uniform sampler2D ourTexture; // texture
in vec2 TexCoord; // uvs

void main()
{
    //FragColor = vec4(1,0, 0, 1);
    FragColor = texture(ourTexture, TexCoord);
}  

