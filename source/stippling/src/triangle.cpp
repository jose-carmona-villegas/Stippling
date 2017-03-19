#include "triangle.h"

Triangle::Triangle()
{

}

Triangle::Triangle(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3)
{
    v1 = vertex1;
    v2 = vertex2;
    v3 = vertex3;
}

glm::vec3 Triangle::getMiddlePoint(int v1, int v2)
{
    glm::vec3 vertex1;
    glm::vec3 vertex2;

    switch(v1)
    {
    case 1:
       vertex1 = this->v1;
       break;
    case 2:
       vertex1 = this->v2;
       break;
    case 3:
       vertex1 = this->v3;
       break;
    }

    switch(v2)
    {
    case 1:
       vertex2 = this->v1;
       break;
    case 2:
       vertex2 = this->v2;
       break;
    case 3:
       vertex2 = this->v3;
       break;
    }

    return (vertex1 + vertex2) / 2.0f;
}
