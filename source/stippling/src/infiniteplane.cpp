#include "infiniteplane.h"

InfinitePlane::InfinitePlane()
{

}

glm::mat4 InfinitePlane::model()
{
    return _model;
}

void InfinitePlane::translate(const glm::vec3 offset)
{
    _model = glm::translate(_model, offset);
}

void InfinitePlane::rotate(const GLfloat angle, const glm::vec3 axis)
{
    _model = glm::rotate(_model, angle, axis);
}

void InfinitePlane::scale(const glm::vec3 scale)
{
    _model = glm::scale(_model, scale);
}

void InfinitePlane::resetModel()
{
    _model = glm::mat4(1.0f);
}

void InfinitePlane::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
                          GLfloat xLeft, GLfloat xRight, GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar)
{
    _entity.paint(selectionMode, view, projection, &_model, 0, xLeft, xRight, yBottom, yTop, zNear, zFar, false);
}
