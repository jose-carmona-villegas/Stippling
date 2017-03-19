#ifndef HEIGHTINDICATOR_H
#define HEIGHTINDICATOR_H

#include "glentity.h"
#include "enums.h"
#include "util.h"

/**
 * @brief HeightIndicator class.
 * Represents the height indicator that will be rendered when an entity is not at the same height as the floor.
 */
class HeightIndicator
{
private:
    GLEntity _line; /**< Graphic entity that represents the vertical line that goes from the bottom of the entity to the floor. */
    GLEntity _shadow; /**< Graphic entity that represents a symbolic shadow of the entity on the floor. */

    glm::mat4 _model; /**< Model matrix. */

public:
    /**
     * @brief Constructor.
     * @param renderShader Shader used to render the graphic entities.
     * @param pickingShader Shader used to render during selection mode.
     * @param color Color used to render the enter outside selection mode.
     * @param floorHeight Coordinate Y of the reference floor.
     * @param indicatedHeight Coordinate Y of the entity whose height over the floor will be indicated.
     * @param l
     */
    HeightIndicator(GLuint renderShader, GLuint pickingShader, glm::vec3 color,
                    float floorHeight, float indicatedHeight, float l);

    // Getters
    glm::mat4 model();

    // Setters
    // Model transformations
    void translate(const glm::vec3 offset);
    void rotate(const GLfloat angle, const glm::vec3 axis);
    void scale(const glm::vec3 scale);
    void resetModel();

    /**
     * @brief Renders the both the height line and the symbolic shadow.
     * @param selectionMode Flag that indicates if the selection mode is on or off.
     * @param view View matrix.
     * @param projection Projection matrix.
     */
    void paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection);
};

#endif // HEIGHTINDICATOR_H
