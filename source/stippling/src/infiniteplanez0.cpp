#include "infiniteplanez0.h"

InfinitePlaneZ0::InfinitePlaneZ0() : InfinitePlane()
{

}

InfinitePlaneZ0::InfinitePlaneZ0(int id, glm::vec3 selectionColor, GLuint renderShader, GLuint pickingShader) :
       InfinitePlane()
{
    _model = glm::mat4(1.0f);
    _entity = GLEntity(id, selectionColor);

    float maxSide = PLANE_SIZE;
    float max = maxSide;
    float min = -maxSide;

    // The plane Z=0
    const GLfloat vertexBufferData[] =
    {
        min, min, 0.0f,
        max, min, 0.0f,
        min, max, 0.0f,


        min, max, 0.0f,
        max, min, 0.0f,
        max, max, 0.0f,
    };

    // One color for each vertex.
    const GLfloat colorBufferData[] =
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    // One uv coord for each vertex.
    const GLfloat * uvBufferData = 0;

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferData)/sizeof(GLfloat);

    _entity.initialize(vertexBufferData, colorBufferData, normalBufferData, uvBufferData,
                             numberOfValuesInStaticBuffers, GL_TRIANGLES, 3, GL_STATIC_DRAW,
                             renderShader, pickingShader);
}

