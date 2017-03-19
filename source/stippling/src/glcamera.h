#ifndef GLCAMERA_H
#define GLCAMERA_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
// quaternion
#include <glm/gtx/quaternion.hpp>

class GLCamera
{
private:
    float _pitch;
    float _yaw;
    float _roll;

    glm::vec3 _forward;
    glm::vec3 _up;
    glm::vec3 _right;

    glm::vec3 _lookAt;

    glm::vec3 _position;
    glm::mat4 _rotation;

    glm::mat4 _view;

    glm::vec3 rotateVector(glm::vec3 vector, glm::mat4 rotation);

    void update();
public:
    GLCamera();
    ~GLCamera();

    void reset();

    void setPosition(const glm::vec3 position);

    void setHeight(const float height);

    void setDistance(const float distance);

    void setPitch(const float pitch);
    void setYaw(const float yaw);
    void setRoll(const float roll);

    void translate(const glm::vec3 offset);
    void rotate(const float angle, const glm::vec3 axis);

    const glm::mat4 view();

};

#endif // GLCAMERA_H
