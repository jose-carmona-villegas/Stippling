/**
 * @file stippledot.h
 * @brief StippleDot class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 23/March/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef STIPPLEDOT_H
#define STIPPLEDOT_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>


#include <QTextStream>


#include "util.h"
#include "glentity.h"
#include "idmanager.h"


/**
 * @brief StippleDot class.
 * Represents a Stipple Dot.
 */
class StippleDot
{
protected:
    glm::vec2 _position; /**<  */
    glm::vec2 _offset; /**<  */
    glm::vec2 _semiSize; /**< */

    int _chosenDot;

    bool _canHaveOffsetApplied;

public:
    StippleDot();

    /**
     * @brief Constructor.
     */
    StippleDot(glm::vec2 position, glm::vec2 size, int chosenDot);

    int chosenDot() const;
    glm::mat4 model() const;
    glm::vec2 finalPostion() const;

    void setOffset(glm::vec2 offset);
    void setCanHaveOffsetApplied(bool canHaveOffsetApplied);

    QString toString();

};

#endif // STIPPLEDOT_H
