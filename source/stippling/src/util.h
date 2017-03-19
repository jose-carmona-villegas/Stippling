/**
 * @file util.h
 * @brief Util header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 13/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef UTIL_H
#define UTIL_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <QTextStream>
#include <QColor>

static const double PI = 3.14159265358979323846264338327950288; /**< Pi constant. */
static QTextStream out(stdout); /**< Qt standard output (console). */

/**
 * @brief Util class
 * Non instantiable class that provides general utility static methods.
 */
class Util
{
public:
    /**
     * @brief Returns the value i evaluated in the Ring of Integers with modulus m (namely the ring Zm).
     * @param i An integer value.
     * @param m A positive integer value.
     * @return value i evaluated in the Ring of Integers with modulus m.
     */
    static int mod(int i, int m);

    /**
     * @brief Prints a matrix via console.
     * The matrix is transposed before being printed, since OpenGL stores its matrices Column major, but they are better understood when Row major for debugging purposes.
     * @param matrix The matrix to be printed.
     */
    static void printMatrix(glm::mat4 matrix);

    /**
     * @brief Prints a vector via console.
     * @param vector The vector to be printed.
     */
    static void printVector(glm::vec2 vector);

    /**
     * @brief Prints a vector via console.
     * @param vector The vector to be printed.
     */
    static void printVector(glm::vec3 vector);

    /**
     * @brief Prints a vector via console.
     * @param vector The vector to be printed.
     */
    static void printVector(glm::vec4 vector);

    /**
     * @brief Converts a color from QColor format to glm::vec3.
     * @param The Color to be converted.
     * @return The converted color.
     */
    static glm::vec3 toGlmVec3(QColor color);

    /**
     * @brief Converts a color from glm::vec3 format to QColor.
     * @param The Color to be converted.
     * @return The converted color.
     */
    static QColor toQColor(glm::vec3 color);
};

#endif // UTIL_H
