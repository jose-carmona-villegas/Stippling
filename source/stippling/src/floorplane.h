#ifndef FLOORPLANE_H
#define FLOORPLANE_H

#include <glentity.h>
#include <enums.h>
#include <controlpoint.h>
#include <idmanager.h>

#include <glm/gtc/matrix_inverse.hpp>

#include <qmath.h>

class FloorPlane
{
private:
    GLEntity _entity;
    GLEntity _grid;

    glm::mat4 _model;

public:
    // Always initializes as the semiplane Y=0,Z>=0
    // Apply transformations as needed.
    FloorPlane();
    FloorPlane(IDManager * idManager, GLuint planeRenderShader, GLuint gridRenderShader, GLuint pickingShader,
               GLuint controlPointRenderShader, glm::vec3 gridColor);

    glm::mat4 model();

    bool contains(glm::vec3 point);

    float getY();

    void setGridColor(glm::vec3 color);

    void translate(const glm::vec3 offset);
    void rotate(const GLfloat angle, const glm::vec3 axis);
    void scale(const glm::vec3 scale);
    void resetModel();

    void paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
               GLfloat xLeft, GLfloat xRight, GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar,
               FloorRenderMode renderMode);
};

#endif // FLOORPLANE_H
