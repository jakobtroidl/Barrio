#version 430
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

// World transformation
uniform mat4    mMatrix;

// View Transformation
uniform mat4    vMatrix;

// Projection transformation
uniform mat4    pMatrix;
//uniform mat4    rMatrix;

out vec3 geom_color;

void main()
{
    mat4 pvmMatrix = pMatrix * vMatrix * mMatrix;
    gl_Position = pvmMatrix * vec4(pos, 1.0);
    
    geom_color = color;
}