#version 430
out vec4 FragColor;

in vec2 texCoord;

//uniform sampler3D volume;

void main()
{
    FragColor = vec4(texCoord, 0.0, 1.0); //texture(volume, vec3(TexCoord, 0.0));
}