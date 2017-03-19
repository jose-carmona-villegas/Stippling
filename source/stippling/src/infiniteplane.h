#ifndef INFINITEPLANE_H
#define INFINITEPLANE_H

#include <glentity.h>
#include <enums.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <qmath.h>

/**
 * @brief The InfinitePlane class
 * Can't instantiate an useful object. It's used as a common base for other implementations (infiniteplaney0 or infiniteplanez0).
 * TODO Make this class virtual as soon as possible.
 */
class InfinitePlane
{
protected:
    static const int PLANE_SIZE = 20000; /**< Constant used to define a big enough size for the plane. */
    glm::mat4 _model; /**< Model matrix. */
    GLEntity _entity; /**< Graphical entity that represents the plane. */

public:
    /**
     * @brief Default constructor.
     */
    InfinitePlane();

    // Getters
    virtual glm::mat4 model();

    // Setters
    // Model transformations.
    /**
     * @brief Moves the object (using the amounts specified). It accumulates with former transformations.
     * @param offset Amounts of pixels (in x,y,z coords.) that will be added to the object's current position.
     */
    virtual void translate(const glm::vec3 offset);
    /**
     * @brief Rotates the object (using both the angle and axis specified). It accumulates with former transformations.
     * @param angle Angle (in degrees) to be rotated.
     * @param axis Three dimensional vector that specifies the rotation axis.
     */
    virtual void rotate(const GLfloat angle, const glm::vec3 axis);
    /**
     * @brief Scales the object (using the amounts specified). It accumulates with former transformations.
     * @param scale Three dimensional vector that specifies the scale factors on (x,y,z).
     */
    virtual void scale(const glm::vec3 scale);

    /**
     * @brief Resets the model matrix to an identity matrix (position on (0,0,0), no rotations nor scaling).
     */
    virtual void resetModel();

    /**
     * @brief Renders the plane.
     * @param selectionMode Flag that indicates if the selection mode is on or off.
     * @param view View matrix.
     * @param projection Projection matrix.
     * @param xLeft
     * @param xRight
     * @param yBottom
     * @param yTop
     * @param zNear
     * @param zFar
     */
    virtual void paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
                       GLfloat xLeft, GLfloat xRight, GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar);
};

#endif // INFINITEPLANE_H
