/**
 * @file util.cpp
 * @brief Util class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 13/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "util.h"

int Util::mod(int i, int m)
{
    int mod;
    if(i < 0)
    {
        mod = m+(i%m);
    }
    else
    {
        mod = i%m;
    }
    if(mod >= m)
    {
        mod -= m;
    }

    return mod;
}

void Util::printMatrix(glm::mat4 matrix)
{
    // Printing the matrix row major to better understand it.
    // Since OpenGL uses them column major, it must be transposed.
    glm::mat4 transposed = glm::transpose(matrix);
    for(int i=0; i<int(transposed.row_size()); ++i)
    {
        for(int j=0; j<int(transposed.col_size()); ++j)
        {
            out << transposed[i][j] << " ";
        }
        out << endl;
    }
}

void Util::printVector(glm::vec2 vector)
{
    out << "( " << vector.x << ", " << vector.y << " )" << endl;
}

void Util::printVector(glm::vec3 vector)
{
    out << "( " << vector.x << ", " << vector.y << ", " << vector.z << " )" << endl;
}

void Util::printVector(glm::vec4 vector)
{
    out << "( " << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << " )" << endl;
}

glm::vec3 Util::toGlmVec3(QColor color)
{
    return glm::vec3(color.red()/255.0f, color.green()/255.0f, color.blue()/255.0f);
}

QColor Util::toQColor(glm::vec3 color)
{
    return QColor(255.0f*color.r, 255.0f*color.g, 255.0f*color.b);
}


