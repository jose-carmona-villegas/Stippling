/**
 * @file prism.h
 * @brief Prism header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 11/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef PRISM_H
#define PRISM_H

#include <entity3d.h>

/**
 * @brief Prism class
 * Represents a Prism, it can be rendered, repositioned, resized and rotated using its CPs.
 */
class Prism : public Entity3D
{   
protected:
    ControlPoint _resizeHeight; /**< Control Point used to resize the prism's height. */
    ControlPoint _resizeX; /**< Control Point used to resize the prism along its x axis. */
    ControlPoint _resizeZ; /**< Control Point used to res the prism along its z axis. */
    ControlPoint _rotateY; /**< Control Point used to rotate the prism along its height axis. */

    /**
     * @brief Recalculates the prism's geometry (meant to be used after a control point interaction).
     */
    void recalculateGeometry();

    /**
     * @brief Specific interactions. Called after the generic (Entity3D) interactions are processed.
     * Makes sure the prism always has a volume greater than zero so CGAL doesn't throw an exception.
     * @param Control Point being interacted with.
     */
    void interactSpecifically(ControlPoint * cp);

public:
    /**
     * @brief Constructor
     * @param idManager Reference to the ID Manager used to generate all the necessary IDs.
     * @param size Initial size of the prism (cube of "size" size).
     * @param entityRenderShader Shader used to render the entity (both the wireframe and solid representations).
     * @param controlPointsRenderShader Shader used to render the control points.
     * @param selectionShader Shader used to render in the selection process.
     * @param color Prism's color (both the wireframe and solid representation).
     */
    Prism(IDManager * idManager, const float size, const GLuint entityRenderShader,
          const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief XML Constructor
     */
    Prism(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
          const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color);

    /**
     * @brief Destructor.
     */
    ~Prism();

    ControlPoint * getControlPointRotateY();
    ControlPoint * getControlPointResizeX();
    ControlPoint * getControlPointResizeHeight();
    ControlPoint * getControlPointResizeZ();
};

#endif // PRISM_H
