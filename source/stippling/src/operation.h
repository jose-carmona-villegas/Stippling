#ifndef OPERATION_H
#define OPERATION_H

#include <entity3d.h>

/**
 * @brief Operation class
 * Represents an Operation, it can be rendered, repositioned and have its operation type and operators reassigned.
 */
class Operation : public Entity3D
{
protected:
    GLuint _entityRenderShader; /**< Entity render shader (needed to reinitialize when the geometry changes). */
    GLuint _controlPointsRenderShader; /**< Control points render shader (needed to reinitialize when the geometry changes). */
    GLuint _selectionShader; /**< Selection shader (needed to reinitialize when the geometry changes). */

    ControlPoint _rotateY; /**< Control Point used to rotate the operation along its y axis. */

    Entity3D * _leftOperator; /**< Left operator. Its position will always be the local center of coordinates. */
    Entity3D * _rightOperator; /**< Right operator. Position subject to the left operator. */

    /**
     * @brief Recalculates the operation's geometry (meant to be used after a control point interaction).
     */
    void recalculateGeometry();

    /**
     * @brief Specific interactions. Called after the generic (Entity3D) interactions are processed.
     * It doesn't have any specific interactions.
     * @param Control Point being interacted with.
     */
    void interactSpecifically(ControlPoint * cp);

public:
    /**
     * @brief Constructor
     * @param idManager Reference to the ID Manager used to generate all the necessary IDs.
     * @param entityRenderShader Shader used to render the entity (both the wireframe and solid representations).
     * @param controlPointsRenderShader Shader used to render the control points.
     * @param selectionShader Shader used to render in the selection process.
     * @param color Operation's color (both the wireframe and solid representation).
     */
    Operation(IDManager * idManager, const GLuint entityRenderShader,
              const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief XML Constructor
     */
    Operation(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
              const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief Destructor.
     */
    ~Operation();

    // Getters
    Entity3D * leftOperator() const;
    Entity3D * rightOperator() const;

    ControlPoint * getControlPointRotateY();

    // Setters
    void setLeftOperator(Entity3D * leftOperator);
    void setRightOperator(Entity3D * rightOperator);

    /**
     * @brief Recalculates its geometry by refreshing the operation calculation. Also reinitializes its GLEntities so they hold relevant values.
     */
    void updateGeometry();
};

#endif // OPERATION_H
