/**
 * @file entity3d.cpp
 * @brief Entity3D class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 4/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "entity3d.h"

void Entity3D::generateLines(int nVertices, float * vertices, int nFaces,
                             std::vector<unsigned int> * nVerticesPerFace, unsigned int ** faces)
{
    deleteLines();

    int sumLines = 0;
    for(int f=0; f<nFaces; ++f)
    {
        sumLines += nVerticesPerFace->at(f);
    }

    lines_NumberOfLines = sumLines;
    lines_NumberOfVertices = 2 * lines_NumberOfLines;
    lines_NumberOfValues = 3 * lines_NumberOfVertices;

    lines = new GLfloat[lines_NumberOfValues];

    int index = 0;

    for(int f=0; f<nFaces; ++f)
    {
        for(int vf=0; vf< int(nVerticesPerFace->at(f)); ++vf)
        {
            int origin = faces[f][vf];
            int destiny = faces[f][Util::mod(vf+1,nVerticesPerFace->at(f))];

            lines[index] = vertices[(3*origin)+0];
            ++index;
            lines[index] = vertices[(3*origin)+1];
            ++index;
            lines[index] = vertices[(3*origin)+2];
            ++index;

            lines[index] = vertices[(3*destiny)+0];
            ++index;
            lines[index] = vertices[(3*destiny)+1];
            ++index;
            lines[index] = vertices[(3*destiny)+2];
            ++index;
        }
    }
}

void Entity3D::deleteLines()
{
    if(lines != 0)
    {
        delete[] lines;
        lines = 0;
    }
}

void Entity3D::generateTriangles(int nVertices, float * vertices, int nFaces,
                             std::vector<unsigned int> * nVerticesPerFace, unsigned int ** faces)
{
    deleteTriangles();

    // Check that all faces are triangles
    // TODO Make sure that CGAL always returns triangles and remove this check
    for(int f=0; f<nFaces; ++f)
    {
        if(nVerticesPerFace->at(f) != 3)
        {
            out << "Error on " << _name << " - Cannot generate triangles because a face is not triangular." << endl;

            triangles_NumberOfValues = 0;
            return;
        }
    }

    triangles_NumberOfTriangles = nFaces;
    triangles_NumberOfVertices = 3 * triangles_NumberOfTriangles;
    triangles_NumberOfValues = 3 * triangles_NumberOfVertices;

    triangles = new GLfloat[triangles_NumberOfValues];

    int index = 0;

    for(int f=0; f<nFaces; ++f)
    {
        for(int vf=0; vf< int(nVerticesPerFace->at(f)); ++vf)
        {
            int vertex = faces[f][vf];

            triangles[index] = vertices[(3*vertex)+0];
            ++index;
            triangles[index] = vertices[(3*vertex)+1];
            ++index;
            triangles[index] = vertices[(3*vertex)+2];
            ++index;
        }
    }
}

void Entity3D::deleteTriangles()
{
    if(triangles != 0)
    {
        delete[] triangles;
        triangles = 0;
    }
}

void Entity3D::generateNormals(int nVertices, float * vertices, int nFaces, std::vector<unsigned int> * nVerticesPerFace,
                     unsigned int ** faces)
{
    deleteNormals();

    // Check that all faces are triangles
    // TODO Make sure that CGAL always returns triangles and remove this check
    for(int f=0; f<nFaces; ++f)
    {
        if(nVerticesPerFace->at(f) != 3)
        {
            out << "Error on " << _name << " - Cannot generate normals because a face is not triangular." << endl;

            triangles_NumberOfValues = 0;
            return;
        }
    }

    triangles_NumberOfTriangles = nFaces;
    triangles_NumberOfVertices = 3 * triangles_NumberOfTriangles;
    triangles_NumberOfValues = 3 * triangles_NumberOfVertices;

    normals = new GLfloat[triangles_NumberOfValues];

    int index = 0;

    for(int f=0; f<nFaces; ++f)
    {
        // Calculate the face normal
        int vertex = faces[f][0];
        glm::vec3 vertex1 = glm::vec3(vertices[(3*vertex)+0], vertices[(3*vertex)+1], vertices[(3*vertex)+2]);
        vertex = faces[f][1];
        glm::vec3 vertex2 = glm::vec3(vertices[(3*vertex)+0], vertices[(3*vertex)+1], vertices[(3*vertex)+2]);
        vertex = faces[f][2];
        glm::vec3 vertex3 = glm::vec3(vertices[(3*vertex)+0], vertices[(3*vertex)+1], vertices[(3*vertex)+2]);

        glm::vec3 vector1 = glm::normalize(vertex2 - vertex1);
        glm::vec3 vector2 = glm::normalize(vertex3 - vertex1);

        glm::vec3 normal = glm::normalize(glm::cross(vector1, vector2));

        // Correct the normal if the winding is not CCW.
        // Thanks to Dirk Bartz (University of Thuebingen) for the method.
        // (http://www.graphicsgroups.com/6-opengl/f0097cc7d778fbc2.htm)
        glm::mat3x3 vertexMatrix;
        vertexMatrix[0] = vertex1;
        vertexMatrix[1] = vertex2;
        vertexMatrix[2] = vertex3;
        float det = glm::determinant(vertexMatrix);


        if(det < 0)
        {
            // Clockwise winding
            normal = -normal;
        }
        else
        {
            // Counter-clockwise winding
        }

        for(int vf=0; vf< int(nVerticesPerFace->at(f)); ++vf)
        {
            // Assign the calculated face normal to each face vertex
            normals[index] = normal.x;
            ++index;
            normals[index] = normal.y;
            ++index;
            normals[index] = normal.z;
            ++index;
        }
    }
}

void Entity3D::deleteNormals()
{
    if(normals != 0)
    {
        delete[] normals;
        normals = 0;
    }
}

void Entity3D::add(ControlPoint* cp)
{
    _controlPoints.append(cp);
}

Entity3D::Entity3D()
{
    _type = ABSTRACT;

    _parent = 0;

    _visible = true;
    _paintMode = ANY_CP;
    _model = glm::mat4(1.0f);

	_isOperation = false;
	_operation = 0;

    lines = 0;
    triangles = 0;
    normals = 0;
}

Entity3D::~Entity3D()
{
    deleteLines();
    deleteTriangles();
    deleteNormals();
}

Entity3D::Entity3D(IDManager * idManager)
{
    _color = glm::vec3(0.0f, 0.0f, 0.0f);
    _idManager = idManager;
    _visible = true;
    _paintMode = ANY_CP;
    _model = glm::mat4(1.0f);
    lines = 0;
    triangles = 0;
    normals = 0;
}

int Entity3D::id() const
{
    return _solid.id();
}

glm::vec3 Entity3D::position()
{
    glm::vec4 modelPos = glm::vec4(_position.localPos(), 1.0f);
    glm::vec4 worldPos = _model * modelPos;

    return glm::vec3(worldPos);
}

EntityType Entity3D::type() const
{
    return _type;
}

QString Entity3D::name() const
{
    return _name;
}

bool Entity3D::visible() const
{
    return _visible;
}

PaintMode Entity3D::paintMode() const
{
    return _paintMode;
}

bool Entity3D::isOperation() const
{
    return _isOperation;
}

unsigned int Entity3D::operation() const
{
    return _operation;
}

BoolMesh Entity3D::mesh()
{
    BoolMesh copy = _mesh;
    return copy;
}

glm::mat4 Entity3D::model() const
{
    return _model;
}

void Entity3D::setPosition(const glm::vec3 position)
{
    glm::vec3 oldPos(_model[3]);

    // Take the entity back to origin of coordinates...
    translate(-oldPos);

    translate(position);
}

void Entity3D::setName(const QString name)
{
    _name = QString(name);
}

void Entity3D::setVisible(const bool visible)
{
    _visible = visible;
}

void Entity3D::setPaintMode(const PaintMode paintMode)
{
    _paintMode = paintMode;
}

void Entity3D::setOperation(const unsigned int operation)
{
    bool valid = (operation == BoolMesh::OP_UNION) || (operation == BoolMesh::OP_INTERSECTION) || (operation == BoolMesh::OP_DIFFERENCE);
    if(valid)
    {
        _operation = operation;

        updateGeometry();
    }

    requestParentUpdate();
}

void Entity3D::setColor(glm::vec3 color)
{
    _color = color;
    _wireframe.setColor(color);
    _solid.setColor(color);
}

void Entity3D::setColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale)
{
    foreach(ControlPoint * c, _controlPoints)
    {
        c->setColorByConstraint(position, resize, rotate, scale);
    }
}

ControlPoint * Entity3D::getControlPoint(int id)
{
    for(int i=0; i<_controlPoints.size(); ++i)
    {
        if(_controlPoints.at(i)->id() == id)
        {
            return _controlPoints.at(i);
        }
    }
    return 0;
}

ControlPoint * Entity3D::getControlPointPosition()
{
    return &_position;
}

bool Entity3D::ownsControlPoint(int id)
{
    for(int i=0; i<_controlPoints.size(); ++i)
    {
        if(_controlPoints.at(i)->id() == id)
        {
            return true;
        }
    }
    return false;
}

void Entity3D::translate(const glm::vec3 offset)
{
    _model = glm::translate(_model, offset);
}

void Entity3D::rotate(const GLfloat angle, const glm::vec3 axis)
{
    _model = glm::rotate(_model,angle,axis);
}

void Entity3D::scale(const glm::vec3 scale)
{
    _model = glm::scale(_model, scale);
}

void Entity3D::resetModel()
{
    _model = glm::mat4(1.0f);

    requestParentUpdate();
}

void Entity3D::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
           const Entity3D * selectedEntity, const EntitiesRenderMode renderMode) const
{
    // QSets automatically discard duplicated elements.
    QSet<ControlPoint *> cpsToPaint;

    unsigned int testMask = 0;
    bool anyCp = false;

    switch(_paintMode)
    {
    case ENTITY:
        testMask = 0;
        break;

    case TRANSLATION:
        testMask = CONSTRAINT_TRANSLATION_X | CONSTRAINT_TRANSLATION_Y | CONSTRAINT_TRANSLATION_Z;
        break;

    case RESIZE:
        testMask = CONSTRAINT_RESIZE_X | CONSTRAINT_RESIZE_Y | CONSTRAINT_RESIZE_Z;
        break;

    case ROTATION:
        testMask = CONSTRAINT_ROTATION_X | CONSTRAINT_ROTATION_Y | CONSTRAINT_ROTATION_Z;
        break;

    case SCALE:
        testMask = CONSTRAINT_SCALE_X | CONSTRAINT_SCALE_Y | CONSTRAINT_SCALE_Y;
        break;

    case ANY_CP:
        anyCp = true;
        break;

    default:
        // Unrecognized paint mode
        anyCp = true;
        out << "Entity3D#paint() - Unrecognized paint mode" << endl;
        break;
    }

    if(anyCp)
    {
        foreach(ControlPoint * cp, _controlPoints)
        {
            cpsToPaint.insert(cp);
        }
    }
    else if(testMask != 0)
    {
        foreach(ControlPoint * cp, _controlPoints)
        {
            if(cp->isConstraintedAsAny(testMask))
            {
                cpsToPaint.insert(cp);
            }
        }
    }

    bool drawStippled = (selectedEntity != 0) && (selectedEntity->id() == id());
    const GLEntity * entity;
    if(renderMode == WIREFRAME)
    {
        entity = &_wireframe;
    }
    else
    {
        entity = &_solid;
    }

    bool illuminated = (renderMode == ILLUMINATED_SOLID);

    if(selectionMode == OFF)
    {
        entity->paint(selectionMode, view, projection, &_model, 0, 0, 0, 0, 0, 0, 0, drawStippled, illuminated);

        if(drawStippled)
        {
            foreach(ControlPoint * cp, cpsToPaint)
            {
                cp->paint(selectionMode, view, projection, &_model);
            }
        }
    }
    else
    {
        if(renderMode != CPS_ONLY)
        {
            entity->paint(selectionMode, view, projection, &_model, 0, 0, 0, 0, 0, 0, 0, drawStippled);
        }
        else
        {
            if(drawStippled)
            {
                foreach(ControlPoint * cp, cpsToPaint)
                {
                    cp->paint(selectionMode, view, projection, &_model);
                }
            }
        }
    }
}

void Entity3D::paintOnDifferentGLContext(const glm::mat4 * view, const glm::mat4 * projection) const
{
    _wireframe.paint(OFF, view, projection, &_model, 0, 0, 0, 0, 0, 0, 0, false, true);
}

void Entity3D::interact(int id, glm::vec3 worldCoords)
{
    bool mustRecalculate = false;
    ControlPoint * cp = getControlPoint(id);

    if(cp->isConstraintedAs(CONSTRAINT_TRANSLATION_X))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float deltaX = mouseLocalCoords.x - _position.localPos().x;

        translate(glm::vec3(deltaX,0.0f,0.0f));
    }
    if(cp->isConstraintedAs(CONSTRAINT_TRANSLATION_Y))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float deltaY = mouseLocalCoords.y - _position.localPos().y;

        translate(glm::vec3(0.0f,deltaY,0.0f));
    }
    if(cp->isConstraintedAs(CONSTRAINT_TRANSLATION_Z))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float deltaZ = mouseLocalCoords.z - _position.localPos().z;

        translate(glm::vec3(0.0f,0.0f,deltaZ));
    }
    if(cp->isConstraintedAs(CONSTRAINT_RESIZE_Y))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float newY = mouseLocalCoords.y - _position.localPos().y;

        if(newY <= _position.localPos().y)
        {
            newY = 1.0f;
        }

        cp->setLocalPos(glm::vec3(0.0f,newY,0.0f));

        mustRecalculate = true;
    }
    if(cp->isConstraintedAs(CONSTRAINT_RESIZE_X))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float newX = mouseLocalCoords.x - _position.localPos().x;

        if(newX >= _position.localPos().x)
        {
            newX = -1.0f;
        }

        glm::vec3 newWorldCoords = cp->localPos();
        newWorldCoords.x = newX;

        cp->setLocalPos(newWorldCoords);

        mustRecalculate = true;
    }
    if(cp->isConstraintedAs(CONSTRAINT_RESIZE_Z))
    {
        glm::vec4 mouseWorldCoords = glm::vec4(worldCoords, 1.0f);
        glm::vec4 temp = glm::inverse(_model) * mouseWorldCoords;
        glm::vec3 mouseLocalCoords = glm::vec3(temp);

        float newZ = mouseLocalCoords.z - _position.localPos().z;

        if(newZ >= _position.localPos().z)
        {
            newZ = -1.0f;
        }

        glm::vec3 newWorldCoords = cp->localPos();
        newWorldCoords.z = newZ;

        cp->setLocalPos(newWorldCoords);

        mustRecalculate = true;
    }
    if(cp->isConstraintedAs(CONSTRAINT_ROTATION_Y))
    {
        glm::vec4 cpWorldCoords;
        glm::vec4 temp;

        glm::vec3 p;
        glm::vec3 r1;
        glm::vec3 r2;

        cpWorldCoords = glm::vec4(_position.localPos(), 1.0f);
        temp = _model * cpWorldCoords;
        p = glm::vec3(temp);

        cpWorldCoords = glm::vec4(cp->localPos(), 1.0f);
        temp = _model * cpWorldCoords;
        r1 = glm::vec3(temp);

        r2 = worldCoords;

        float alpha1;
        if(r1.x == p.x)
        {
            if(r1.z >= p.z)
            {
                alpha1 = 90.0f;
            }
            else
            {
                alpha1 = -90.0f;
            }
        }
        else
        {
            alpha1 = atan2((r1.z - p.z), (r1.x - p.x)) * 180.0f / PI;
        }

        float alpha2;
        if(r2.x == p.x)
        {
            if(r2.z >= p.z)
            {
                alpha2 = 90.0f;
            }
            else
            {
                alpha2 = -90.0f;
            }
        }
        else
        {
            alpha2 = atan2((r2.z - p.z), (r2.x - p.x)) * 180.0f / PI;
        }

        float alpha = alpha2 - alpha1;

        rotate(-alpha,glm::vec3(0.0f,1.0f,0.0f));
    }

    interactSpecifically(cp);

    if(mustRecalculate)
    {
        recalculateGeometry();
        _wireframe.setVertexData(0, lines_NumberOfValues, lines);
        _solid.setVertexData(0, triangles_NumberOfValues, triangles);
    }
}

void Entity3D::updateGeometry()
{

}

void Entity3D::requestParentUpdate()
{
    if(_parent != 0 && _parent->isOperation())
    {
        _parent->updateGeometry();
    }
}

void Entity3D::printToConsole()
{
    out << "Entity3D" << endl;
    out << "name: " << name() << endl;
    out << "id: " << id() << endl;
    out << "visible: " << visible() << endl;
    out << "position:" << endl;
    Util::printVector(position());
    out << "model:" << endl;
    Util::printMatrix(_model);
    out << endl;
}
