#include "glcamera.h"

glm::vec3 GLCamera::rotateVector(glm::vec3 vector, glm::mat4 rotation)
{
    glm::vec4 v = glm::vec4(vector, 0.0f);
    glm::vec4 r = rotation * v;
    return glm::vec3(r);
}

void GLCamera::update()
{
    // Rotate the vectors.
    _forward = rotateVector(_forward, _rotation);
    //_up = rotateVector(_up, _rotation);
    _right = rotateVector(_right, _rotation);

    // Calculate the lookAt vector (point where the eye looks at).
    _lookAt = _position + _forward;
    _up = glm::cross(_forward, _right);

    // Generate the view matrix using the eye position, the lookAt point, and the up vector).
    _view = glm::lookAt(_position, _lookAt, _up);
}

GLCamera::GLCamera()
{
    reset();
}

GLCamera::~GLCamera()
{

}

void GLCamera::reset()
{
    _pitch = 0.0f;
    _yaw = 0.0f;
    _roll = 0.0f;

    _forward = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _right = glm::vec3(1.0f, 0.0f, 0.0f);

    _lookAt = glm::vec3(0.0f, 0.0f, -1.0f);

    _position = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotation = glm::mat4(1.0f);

    update();
}

void GLCamera::setPosition(const glm::vec3 position)
{
    _position = position;

    update();
}

void GLCamera::setHeight(const float height)
{
    _position.y = height;

    update();
}

void GLCamera::setDistance(const float distance)
{
    _position.z = distance;

    update();
}

void GLCamera::setPitch(const float pitch)
{
    _pitch = pitch;

    _forward = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _right = glm::vec3(1.0f, 0.0f, 0.0f);
    _rotation = glm::mat4(1.0f);

    _rotation = glm::rotate(_rotation, _pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _roll, glm::vec3(0.0f, 0.0f, 1.0f));

    update();
}

void GLCamera::setYaw(const float yaw)
{
    _yaw = yaw;

    _forward = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _right = glm::vec3(1.0f, 0.0f, 0.0f);
    _rotation = glm::mat4(1.0f);

    _rotation = glm::rotate(_rotation, _pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _roll, glm::vec3(0.0f, 0.0f, 1.0f));

    update();
}

void GLCamera::setRoll(const float roll)
{
    _roll = roll;

    _forward = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _right = glm::vec3(1.0f, 0.0f, 0.0f);
    _rotation = glm::mat4(1.0f);

    _rotation = glm::rotate(_rotation, _pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    _rotation = glm::rotate(_rotation, _roll, glm::vec3(0.0f, 0.0f, 1.0f));

    update();
}

void GLCamera::translate(const glm::vec3 offset)
{
    _position += offset;

    update();
}

void GLCamera::rotate(const float angle, const glm::vec3 axis)
{
    _rotation = glm::rotate(_rotation, angle, axis);

    update();
}

const glm::mat4 GLCamera::view()
{
    return _view;
}
