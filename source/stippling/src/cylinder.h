#ifndef CYLINDER_H
#define CYLINDER_H

#include <entity3d.h>

/**
 * @brief Cylinder class
 * Represents a Cilynder, it can be rendered, repositioned, resized and rotated using its CPs.
 */
class Cylinder : public Entity3D
{
protected:
    static const int _numberOfSidesPerCircle = 36; /**< _numberOfSidesPerCircle The number of sides per base of the cylinder. It defines how well the cylinder is approximated. */

    ControlPoint _resizeRadiusBottom; /**< Control Point used to resize the radius of the cylinder's bottom face. */
    ControlPoint _resizeRadiusTop; /**< Control Point used to resize the radius of the cylinder's top face. */
    ControlPoint _resizeHeight; /**< Control Point used to resize the cylinder's height. */
    ControlPoint _rotateY; /**< Control Point used to rotate the cylinder along its height axis. */

    /**
     * @brief Recalculates the cylinder's geometry (meant to be used after a control point interaction).
     */
    void recalculateGeometry();

    /**
     * @brief Specific interactions. Called after the generic (Entity3D) interactions are processed.
     * Makes sure the height of the top radius resize CP is the same as the height resize CP.
     * @param Control Point being interacted with.
     */
    void interactSpecifically(ControlPoint * cp);

public:
    /**
     * @brief Constructor
     * @param idManager Reference to the ID Manager used to generate all the necessary IDs.
     * @param size Initial size of the cylinder (maximum cylinder that can be fitted in a cube of "size" size).
     * @param entityRenderShader Shader used to render the entity (both the wireframe and solid representations).
     * @param controlPointsRenderShader Shader used to render the control points.
     * @param selectionShader Shader used to render in the selection process.
     * @param color Cylinder's color (both the wireframe and solid representation).
     */
    Cylinder(IDManager * idManager, const float size, const GLuint entityRenderShader,
          const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief XML Constructor
     */
    Cylinder(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
          const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief Destructor.
     */
    ~Cylinder();

    ControlPoint * getControlPointRotateY();
    ControlPoint * getControlPointResizeRadiusBottom();
    ControlPoint * getControlPointResizeRadiusTop();
    ControlPoint * getControlPointResizeHeight();
};

#endif // CYLINDER_H
