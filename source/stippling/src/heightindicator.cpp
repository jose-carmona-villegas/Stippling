#include "heightindicator.h"

HeightIndicator::HeightIndicator(GLuint renderShader, GLuint pickingShader, glm::vec3 color,
                                 float floorHeight, float indicatedHeight, float l)
{
    _model = glm::mat4(1.0f);

    _line = GLEntity(0, glm::vec3(0.0f, 0.0f, 0.0f));
    _shadow = GLEntity(0, glm::vec3(0.0f, 0.0f, 0.0f));

    float height = indicatedHeight - floorHeight;

    const GLfloat vertexBufferDataLine[] =
    {
        0.0f, -height, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    const GLfloat colorBufferDataLine[] =
    {
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
    };

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    // One uv coord for each vertex.
    const GLfloat * uvBufferData = 0;

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferDataLine)/sizeof(GLfloat);

    _line.initialize(vertexBufferDataLine, colorBufferDataLine, normalBufferData, uvBufferData,
                       numberOfValuesInStaticBuffers, GL_LINES, 3, GL_STATIC_DRAW,
                       renderShader, pickingShader);



    float h = l * cos(30.0f * PI / 180.0f);
    float h2 = l/2.0f * tan(30.0f * PI / 180.0f);
    float h1 = h - h2;

    const GLfloat vertexBufferDataShadow[] =
    {
        0.0f, -height, 0.0f,
        0.0f, -height, -h2,
        0.0f, -height, 0.0f,
        l/2.0f, -height, h1,
        0.0f, -height, 0.0f,
        -l/2.0f, -height, h1,

        0.0f, -height, -h2,
        l/2.0f, -height, h1,
        l/2.0f, -height, h1,
        -l/2.0f, -height, h1,
        -l/2.0f, -height, h1,
        0.0f, -height, -h2,
    };

    const GLfloat colorBufferDataShadow[] =
    {
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,

        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
        color.r,  color.g,  color.b,
    };

    normalBufferData = 0;
    uvBufferData = 0;

    numberOfValuesInStaticBuffers = sizeof(vertexBufferDataShadow)/sizeof(GLfloat);

    _shadow.initialize(vertexBufferDataShadow, colorBufferDataShadow, normalBufferData, uvBufferData,
                       numberOfValuesInStaticBuffers, GL_LINES, 3, GL_STATIC_DRAW,
                       renderShader, pickingShader);
}

glm::mat4 HeightIndicator::model()
{
    return _model;
}

void HeightIndicator::translate(const glm::vec3 offset)
{
    _model = glm::translate(_model, offset);
}

void HeightIndicator::rotate(const GLfloat angle, const glm::vec3 axis)
{
    _model = glm::rotate(_model, angle, axis);
}

void HeightIndicator::scale(const glm::vec3 scale)
{
    _model = glm::scale(_model, scale);
}

void HeightIndicator::resetModel()
{
    _model = glm::mat4(1.0f);
}

void HeightIndicator::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection)
{
    if(selectionMode == OFF)
    {
        _line.paint(selectionMode, view, projection, &_model, 0, 0, 0, 0, 0, 0, 0, true);
        _shadow.paint(selectionMode, view, projection, &_model, 0, 0, 0, 0, 0, 0, 0, true);
    }
}

