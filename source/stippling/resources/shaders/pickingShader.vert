#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexUV;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

uniform vec3 pickingColor;

uniform float xLeft;
uniform float xRight;

uniform float yBottom;
uniform float yTop;

uniform float zNear;
uniform float zFar;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

void main()
{
    // Output position of the vertex, in clip space : mvp * position
    gl_Position =  mvp * vec4(vertexPosition,1);

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    // All the vertices are expected to share the same selection color
    fragmentColor = pickingColor;
}

