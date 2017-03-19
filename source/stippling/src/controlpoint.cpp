/**
 * @file controlpoint.cpp
 * @brief ControlPoint class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 4/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "controlpoint.h"

void ControlPoint::initialize(int id, glm::vec3 selectionColor, unsigned int constraints, glm::vec3 localPos, float radius,
                GLuint renderShader, GLuint selectionShader)
{
    triangles = 0;

    _constraints = constraints;
    _localPos = localPos;
    _radius = radius;

    generateIcoSphere(radius, 0);

    GLfloat * vertexColor = new GLfloat[numberOfValues];

    int index = 0;
    for(index=0; index<numberOfValues; index+=3)
    {
        vertexColor[index] = 1.0f;
        vertexColor[index+1] = 0.0f;
        vertexColor[index+2] = 0.0f;
    }

    const GLfloat * vertexNormal = 0;

    const GLfloat * vertexUV = 0;

    _entity = GLEntity(id, selectionColor);
    _entity.initialize(triangles, vertexColor, vertexNormal, vertexUV, numberOfValues, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                       renderShader, selectionShader);

    if(triangles != 0)
    {
        delete[] triangles;
        triangles = 0;
    }
    if(vertexColor != 0)
    {
        delete vertexColor;
        vertexColor = 0;
    }

    _entity.translate(_localPos);
}

ControlPoint::ControlPoint()
{

}

ControlPoint::ControlPoint(int id, glm::vec3 selectionColor, unsigned int constraints, glm::vec3 localPos, float radius,
                           GLuint renderShader, GLuint selectionShader)
{
    initialize(id, selectionColor, constraints, localPos, radius, renderShader, selectionShader);
}

ControlPoint::ControlPoint(int id, glm::vec3 selectionColor, QDomNode node, float radius,
             GLuint renderShader, GLuint selectionShader)
{
    float x = node.firstChildElement("localPos").firstChildElement("x").text().toFloat();
    float y = node.firstChildElement("localPos").firstChildElement("y").text().toFloat();
    float z = node.firstChildElement("localPos").firstChildElement("z").text().toFloat();
    glm::vec3 localPos = glm::vec3(x,y,z);
    unsigned int constraints = node.firstChildElement("constraints").text().toUInt();

    /*
    out << "Constructing Control Point." << endl;
    out << "localPos = " << endl;
    Util::printVector(localPos);
    out << "constraints = " << constraints << endl;
    */

    initialize(id, selectionColor, constraints, localPos, radius, renderShader, selectionShader);
}

int ControlPoint::id() const
{
    return _entity.id();
}

unsigned int ControlPoint::constraints() const
{
    return _constraints;
}

bool ControlPoint::isConstraintedAs(const unsigned int constraint) const
{
    return (_constraints & constraint) == constraint;
}

bool ControlPoint::isConstraintedAsAny(const unsigned int constraint) const
{
    return (_constraints & constraint) != 0;
}

float ControlPoint::radius() const
{
    return _radius;
}

glm::vec3 ControlPoint::localPos() const
{
    return _localPos;
}

void ControlPoint::setRadius(float radius)
{
    _radius = radius;

    generateIcoSphere(radius, 0);

    _entity.setVertexData(0,numberOfValues, triangles);
}

void ControlPoint::setColor(glm::vec3 color)
{
    _entity.setColor(color);
}

void ControlPoint::setColorByConstraint(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale)
{
    if(isConstraintedAs(CONSTRAINT_TRANSLATION_X)
            || isConstraintedAs(CONSTRAINT_TRANSLATION_Y)
            || isConstraintedAs(CONSTRAINT_TRANSLATION_Z))
    {
        setColor(position);
    }
    else if(isConstraintedAs(CONSTRAINT_RESIZE_X)
                             || isConstraintedAs(CONSTRAINT_RESIZE_Y)
                             || isConstraintedAs(CONSTRAINT_RESIZE_Z))
    {
        setColor(resize);
    }
    else if(isConstraintedAs(CONSTRAINT_ROTATION_X)
            || isConstraintedAs(CONSTRAINT_ROTATION_Y)
            || isConstraintedAs(CONSTRAINT_ROTATION_Z))
    {
        setColor(rotate);
    }
    else if(isConstraintedAs(CONSTRAINT_SCALE_X)
            || isConstraintedAs(CONSTRAINT_SCALE_Y)
            || isConstraintedAs(CONSTRAINT_SCALE_Z))
    {
        setColor(scale);
    }
    else
    {
        setColor(glm::vec3(0.0f,0.0f,0.0f));
    }
}

void ControlPoint::setLocalPos(glm::vec3 localPos)
{
    _localPos = localPos;
    _entity.resetModel();
    _entity.translate(_localPos);
}

void ControlPoint::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection, const glm::mat4 * extModel)
{
    _entity.paint(selectionMode, view, projection, extModel, 0, 0, 0, 0, 0, 0, 0, false);
}

QDomElement ControlPoint::toXML(QDomDocument * doc) const
{
    QDomElement node = doc->createElement("controlPoint");

    QDomElement localPosElem = doc->createElement("localPos");
    node.appendChild(localPosElem);

    QDomElement xElem = doc->createElement("x");
    localPosElem.appendChild(xElem);

    QDomText xTxt = doc->createTextNode(QString::number(_localPos.x));
    xElem.appendChild(xTxt);

    QDomElement yElem = doc->createElement("y");
    localPosElem.appendChild(yElem);

    QDomText yTxt = doc->createTextNode(QString::number(_localPos.y));
    yElem.appendChild(yTxt);

    QDomElement zElem = doc->createElement("z");
    localPosElem.appendChild(zElem);

    QDomText zTxt = doc->createTextNode(QString::number(_localPos.z));
    zElem.appendChild(zTxt);


    QDomElement constraintsElem = doc->createElement("constraints");
    node.appendChild(constraintsElem);

    QDomText constraintsTxt = doc->createTextNode(QString::number(int(_constraints)));
    constraintsElem.appendChild(constraintsTxt);

    return node;
}

GLfloat * ControlPoint::generateCircle(const float radius, const int numberOfTriangles, const int valuesPerVertex)
{
    int numberOfValues = numberOfTriangles * 3 * valuesPerVertex;

    GLfloat * triangles = new GLfloat[numberOfValues];

    glm::vec3 centre = glm::vec3(0.0f, 0.0f, 0.0f);
    int index = 0;
    for(int i=0; i<numberOfTriangles; ++i)
    {
        triangles[index] = centre.x;
        ++index;
        triangles[index] = centre.y;
        ++index;
        if(valuesPerVertex == 3)
        {
            triangles[index] = centre.z;
            ++index;
        }

        triangles[index] = radius * cos((2*PI / numberOfTriangles) * i);
        ++index;
        triangles[index] = radius * sin((2*PI / numberOfTriangles) * i);
        ++index;
        if(valuesPerVertex == 3)
        {
            triangles[index] = centre.z;
            ++index;
        }

        triangles[index] = radius * cos((2*PI / numberOfTriangles) * Util::mod(i+1,numberOfTriangles));
        ++index;
        triangles[index] = radius * sin((2*PI / numberOfTriangles) * Util::mod(i+1,numberOfTriangles));
        ++index;
        if(valuesPerVertex == 3)
        {
            triangles[index] = centre.z;
            ++index;
        }
    }

    return triangles;
}

void ControlPoint::generateIcoSphere(const float radius, const int resolution)
{
    const float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    const float alpha = atan(1.0f / phi);

    float u = radius * sin(alpha);
    float t = sqrt( (radius*radius) - (u*u) );

    QVector<glm::vec3> icosahedronVertex;

    icosahedronVertex.append(glm::vec3( -u,  t,  0));
    icosahedronVertex.append(glm::vec3(  u,  t,  0));
    icosahedronVertex.append(glm::vec3( -u, -t,  0));
    icosahedronVertex.append(glm::vec3(  u, -t,  0));

    icosahedronVertex.append(glm::vec3(  0, -u,  t));
    icosahedronVertex.append(glm::vec3(  0,  u,  t));
    icosahedronVertex.append(glm::vec3(  0, -u, -t));
    icosahedronVertex.append(glm::vec3(  0,  u, -t));

    icosahedronVertex.append(glm::vec3(  t,  0, -u));
    icosahedronVertex.append(glm::vec3(  t,  0,  u));
    icosahedronVertex.append(glm::vec3( -t,  0, -u));
    icosahedronVertex.append(glm::vec3( -t,  0,  u));

    QList<Triangle> trianglesList;

    // Initially, load the icosahedron faces.

    // 5 faces around point 0
    trianglesList.append(Triangle(icosahedronVertex.at(0), icosahedronVertex.at(11), icosahedronVertex.at(5)));
    trianglesList.append(Triangle(icosahedronVertex.at(0), icosahedronVertex.at(5), icosahedronVertex.at(1)));
    trianglesList.append(Triangle(icosahedronVertex.at(0), icosahedronVertex.at(1), icosahedronVertex.at(7)));
    trianglesList.append(Triangle(icosahedronVertex.at(0), icosahedronVertex.at(7), icosahedronVertex.at(10)));
    trianglesList.append(Triangle(icosahedronVertex.at(0), icosahedronVertex.at(10), icosahedronVertex.at(11)));

    // 5 adjacent faces
    trianglesList.append(Triangle(icosahedronVertex.at(1), icosahedronVertex.at(5), icosahedronVertex.at(9)));
    trianglesList.append(Triangle(icosahedronVertex.at(5), icosahedronVertex.at(11), icosahedronVertex.at(4)));
    trianglesList.append(Triangle(icosahedronVertex.at(11), icosahedronVertex.at(10), icosahedronVertex.at(2)));
    trianglesList.append(Triangle(icosahedronVertex.at(10), icosahedronVertex.at(7), icosahedronVertex.at(6)));
    trianglesList.append(Triangle(icosahedronVertex.at(7), icosahedronVertex.at(1), icosahedronVertex.at(8)));

    // 5 faces around point 3
    trianglesList.append(Triangle(icosahedronVertex.at(3), icosahedronVertex.at(9), icosahedronVertex.at(4)));
    trianglesList.append(Triangle(icosahedronVertex.at(3), icosahedronVertex.at(4), icosahedronVertex.at(2)));
    trianglesList.append(Triangle(icosahedronVertex.at(3), icosahedronVertex.at(2), icosahedronVertex.at(6)));
    trianglesList.append(Triangle(icosahedronVertex.at(3), icosahedronVertex.at(6), icosahedronVertex.at(8)));
    trianglesList.append(Triangle(icosahedronVertex.at(3), icosahedronVertex.at(8), icosahedronVertex.at(9)));

    // 5 adjacent faces
    trianglesList.append(Triangle(icosahedronVertex.at(4), icosahedronVertex.at(9), icosahedronVertex.at(5)));
    trianglesList.append(Triangle(icosahedronVertex.at(2), icosahedronVertex.at(4), icosahedronVertex.at(11)));
    trianglesList.append(Triangle(icosahedronVertex.at(6), icosahedronVertex.at(2), icosahedronVertex.at(10)));
    trianglesList.append(Triangle(icosahedronVertex.at(8), icosahedronVertex.at(6), icosahedronVertex.at(7)));
    trianglesList.append(Triangle(icosahedronVertex.at(9), icosahedronVertex.at(8), icosahedronVertex.at(1)));

    for(int r=0; r<resolution; ++r)
    {
        // Subdivide each triangle in 4 triangles (the new vertices will be the middle points of each of the sides of the triangle)
        // Same pattern as the triforce from the Zelda games.

        QList<Triangle> trianglesTemp;
        foreach(Triangle t, trianglesList)
        {
            glm::vec3 v1 = t.v1;
            glm::vec3 v2 = t.v2;
            glm::vec3 v3 = t.v3;
            glm::vec3 v4 = radius * glm::normalize(t.getMiddlePoint(1,2));
            glm::vec3 v5 = radius * glm::normalize(t.getMiddlePoint(2,3));
            glm::vec3 v6 = radius * glm::normalize(t.getMiddlePoint(1,3));

            trianglesTemp.append(Triangle(v1,v4,v6));
            trianglesTemp.append(Triangle(v4,v2,v5));
            trianglesTemp.append(Triangle(v4,v5,v6));
            trianglesTemp.append(Triangle(v6,v5,v3));
        }

        trianglesList = trianglesTemp;
    }

    // Finally, for each triangle, add its vertices to the array that will be copied in the GPU's memory.
    numberOfTriangles = trianglesList.size();
    numberOfVertices = 3 * numberOfTriangles;
    numberOfValues = 3 * numberOfVertices;

    if(triangles != 0)
    {
        delete[] triangles;
        triangles = 0;
    }

    triangles = new GLfloat[numberOfValues];

    int index = 0;
    foreach(Triangle t, trianglesList)
    {
        triangles[index] = t.v1.x;
        ++index;
        triangles[index] = t.v1.y;
        ++index;
        triangles[index] = t.v1.z;
        ++index;

        triangles[index] = t.v2.x;
        ++index;
        triangles[index] = t.v2.y;
        ++index;
        triangles[index] = t.v2.z;
        ++index;

        triangles[index] = t.v3.x;
        ++index;
        triangles[index] = t.v3.y;
        ++index;
        triangles[index] = t.v3.z;
        ++index;
    }
}

