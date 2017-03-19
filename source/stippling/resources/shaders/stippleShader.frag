#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

uniform int nStipples;

void main()
{
    vec4 stippledColor;

    int m = int(sqrt((pow(gl_FragCoord.x,2))+(pow(gl_FragCoord.y,2))+(pow(gl_FragCoord.z,2))));

    m *= 11;

    if( (m%nStipples) % 2 == 0 )
    {

        stippledColor = vec4(fragmentColor, 1.0f);
    }
    else
    {
        //stippledColor = vec4(0.0f);
        //discard;
    }

    stippledColor = vec4(fragmentColor, 1.0f);
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = stippledColor;
}


