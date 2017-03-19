#include "floorplane.h"

FloorPlane::FloorPlane()
{

}

FloorPlane::FloorPlane(IDManager * idManager, GLuint planeRenderShader, GLuint gridRenderShader, GLuint pickingShader,
                       GLuint controlPointRenderShader, glm::vec3 gridColor)
{
    _model = glm::mat4(1.0f);

    int id = idManager->getNewID();
    _entity = GLEntity(id, idManager->encodeID(id));

    float maxSide = 10000;
    float max = maxSide;
    float min = -maxSide;

    // The semiplane Y=0,Z>=0
    const GLfloat vertexBufferData[] =
    {
        min, 0.0f, 0.0f,
        min, 0.0f, max,
        max, 0.0f, 0.0f,

        max, 0.0f, 0.0f,
        min, 0.0f, max,
        max, 0.0f, max,
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
                             planeRenderShader, pickingShader);

    id = idManager->getNewID();
    _grid = GLEntity(id, idManager->encodeID(id));

    const int lowX = -5000;
    const int highX = 5000;
    const int lowZ = 0;
    const int highZ = 30000;
    const int step = 200;

    const int numberOfLinesX = (highX - lowX) / step;
    const int numberOfLinesZ = (highZ - lowZ) / step;

    const int numberOfLines = numberOfLinesX + numberOfLinesZ;
    const int numberOfVertices = 2 * numberOfLines;
    const int numberOfValues = 3 * numberOfVertices;

    GLfloat * lines = new GLfloat[numberOfValues];

    int index = 0;

    for(int i=0; i<numberOfLinesX; ++i)
    {
        lines[index] = lowX + (i*step);
        ++index;
        lines[index] = 0.1f;
        ++index;
        lines[index] = lowZ;
        ++index;

        lines[index] = lowX + (i*step);
        ++index;
        lines[index] = 0.1f;
        ++index;
        lines[index] = highZ;
        ++index;
    }

    for(int i=0; i<numberOfLinesZ; ++i)
    {
        lines[index] = lowX;
        ++index;
        lines[index] = 0.1f;
        ++index;
        lines[index] = lowZ + (i*step);
        ++index;

        lines[index] = highX;
        ++index;
        lines[index] = 0.1f;
        ++index;
        lines[index] = lowZ + (i*step);
        ++index;
    }

    GLfloat * colors = new GLfloat[numberOfValues];

    index = 0;
    for(int i=0; i<numberOfVertices; ++i)
    {
        colors[index] = gridColor.r;
        ++index;
        colors[index] = gridColor.g;
        ++index;
        colors[index] = gridColor.b;
        ++index;
    }

    normalBufferData = 0;
    uvBufferData = 0;

    _grid.initialize(lines, colors, normalBufferData, uvBufferData,
                             numberOfValues, GL_LINES, 3, GL_DYNAMIC_DRAW,
                             gridRenderShader, pickingShader);

    if(lines != 0)
    {
        delete[] lines;
        lines = 0;
    }
    if(colors != 0)
    {
        delete[] colors;
        colors = 0;
    }
}

glm::mat4 FloorPlane::model()
{
    return _model;
}

bool FloorPlane::contains(glm::vec3 point)
{
    glm::mat4 inverseModel = glm::inverse(_model);

    glm::vec4 point4 = glm::vec4(point.x, point.y, point.z, 1.0f);

    glm::vec4 transformedPoint = inverseModel * point4;

    return ( (transformedPoint.y == 0) && (transformedPoint.z > 0) );
}

float FloorPlane::getY()
{
    // Point that belongs to the plane (without transformations).
    glm::vec4 point = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // Apply the transformations.
    glm::vec4 transformedPoint = _model * point;
    // Return its y coordinate.
    return transformedPoint.y;
}

void FloorPlane::setGridColor(glm::vec3 color)
{
    _grid.setColor(color);
}

void FloorPlane::translate(const glm::vec3 offset)
{
    _model = glm::translate(_model, offset);
}

void FloorPlane::rotate(const GLfloat angle, const glm::vec3 axis)
{
    _model = glm::rotate(_model, angle, axis);
}

void FloorPlane::scale(const glm::vec3 scale)
{
    _model = glm::scale(_model, scale);
}

void FloorPlane::resetModel()
{
    _model = glm::mat4(1.0f);
}

void FloorPlane::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
                       GLfloat xLeft, GLfloat xRight, GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar,
                       FloorRenderMode renderMode)
{
    if(renderMode == SOLID || renderMode == GRID_SOLID)
    {
        _entity.paint(selectionMode, view, projection, &_model, 0, xLeft, xRight, yBottom, yTop, zNear, zFar, false);
    }
    if(renderMode == GRID || renderMode == GRID_SOLID)
    {
        _grid.paint(selectionMode, view, projection, &_model, 0, xLeft, xRight, yBottom, yTop, zNear, zFar, true);
    }
}
