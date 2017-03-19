#ifndef TRIANGLE_H
#define TRIANGLE_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>


/**
 * @brief Triangle class
 * Represents a Triangle, holds its vertices and can calculate the middle point between two of them.
 */
class Triangle
{
public:
    glm::vec3 v1; /**< v1 Vertex 1. */
    glm::vec3 v2; /**< v2 Vertex 2. */
    glm::vec3 v3; /**< v3 Vertex 3. */

    /**
     * @brief Default constructor
     */
    Triangle();

    /**
     * @brief Constructor
     * @param vertex1 Vertex 1 values.
     * @param vertex2 Vertex 2 values.
     * @param vertex3 Vertex 3 values.
     */
    Triangle(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);

    /**
     * @brief Gets the middle point between the two given vertices.
     * @param v1 Integer that represents the vertex (1, 2 or 3).
     * @param v2 Integer that represents the vertex (1, 2 or 3).
     * @return
     */
    glm::vec3 getMiddlePoint(int v1, int v2);
};

#endif // TRIANGLE_H
