/**
 * @file stippledot.cpp
 * @brief StippleDot class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 23/March/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "stippledot.h"

StippleDot::StippleDot()
{
    _position = glm::vec2(0.0f, 0.0f);
    _semiSize = glm::vec2(0.0f, 0.0f);
    _offset = glm::vec2(0.0f, 0.0f);

    _canHaveOffsetApplied = true;
}

StippleDot::StippleDot(glm::vec2 position, glm::vec2 size, int chosenDot)
{
    _position = position;
    _semiSize = size / 2.0f;
    _offset = glm::vec2(0.0f, 0.0f);

    _chosenDot = chosenDot;

    _canHaveOffsetApplied = true;

    /*
    out << "=== New StippleDot ===" << endl;
    out << "Position:" << endl;
    Util::printVector(_position);
    out << "Offset:" << endl;
    Util::printVector(_offset);
    out << "Semisize:" << endl;
    Util::printVector(_semiSize);
    out << "UVx limits: " << minX << ", " << maxX << endl;
    out << "UVy limits: " << minY << ", " << maxY << endl;
    out << "Sprite (" << spriteRow << ", " << spriteColumn << ")" << endl;
    out << "out of a sprite matrix of size (" << spriteRows << ", " << spriteColumns << ")" << endl;
    out << "======================" << endl;
    */
}

int StippleDot::chosenDot() const
{
    return _chosenDot;
}

glm::mat4 StippleDot::model() const
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 temp = _position + _offset;
    glm::vec3 finalPosition = glm::vec3(temp.x, temp.y, 0.0f);
    model = glm::translate(model, finalPosition);
    return model;
}

glm::vec2 StippleDot::finalPostion() const
{
    glm::vec2 finalPosition = _position;
    if(_canHaveOffsetApplied)
    {
        finalPosition += _offset;
    }
    return finalPosition;
}

void StippleDot::setOffset(glm::vec2 offset)
{
    _offset = offset;
}

void StippleDot::setCanHaveOffsetApplied(bool canHaveOffsetApplied)
{
    _canHaveOffsetApplied = canHaveOffsetApplied;
}

QString StippleDot::toString()
{
    QString result = "";
    QTextStream s(&result);

    s << "===== StippleDot =====" << endl;
    s << "Position:" << endl;
    s << "( " << _position.x << ", " << _position.y << " )" << endl;
    s << "Offset:" << endl;
    s << "( " << _offset.x << ", " << _offset.y << " )" << endl;
    s << "Semisize:" << endl;
    s << "( " << _semiSize.x << ", " << _semiSize.y << " )" << endl;
    s << "chosenDot: " << _chosenDot << endl;
    s << "Offset applicable: " << _canHaveOffsetApplied << endl;
    s << "======================" << endl;

    return result;
}

