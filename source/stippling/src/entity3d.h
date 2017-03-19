/**
 * @file entity3d.h
 * @brief Entity3D header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 4/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef ENTITY3D_H
#define ENTITY3D_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <QVector>
#include <QSet>

#include "idmanager.h"
#include "controlpoint.h"
#include "enums.h"
#include "util.h"
#include "boolmesh.h"

class Entity3D
{
public:
    Entity3D * _parent;

protected:
    EntityType _type;

    QString _name;
    bool _visible;
    PaintMode _paintMode;

    glm::vec3 _color;

    // Control points.
    QVector<ControlPoint*> _controlPoints;

    IDManager * _idManager;

    // Entities to be controlled by the control points.
    GLEntity _wireframe;
    GLEntity _solid;

    glm::mat4 _model;

    ControlPoint _position;

    int lines_NumberOfLines;
    int lines_NumberOfVertices;
    int lines_NumberOfValues;
    GLfloat * lines;

    int triangles_NumberOfTriangles;
    int triangles_NumberOfVertices;
    int triangles_NumberOfValues;
    GLfloat * triangles;

    GLfloat * normals;

    BoolMesh _mesh;
    // TODO Refactor so that the _type is used instead of this member, then get rid of it.
    bool _isOperation;
    unsigned int _operation;

    virtual void recalculateGeometry() = 0;
    virtual void interactSpecifically(ControlPoint * cp) = 0;

    void generateLines(int nVertices, float * vertices, int nFaces, std::vector<unsigned int> * nVerticesPerFace,
                       unsigned int ** faces);

    void deleteLines();

    void generateTriangles(int nVertices, float * vertices, int nFaces, std::vector<unsigned int> * nVerticesPerFace,
                       unsigned int ** faces);

    void deleteTriangles();

    void generateNormals(int nVertices, float * vertices, int nFaces, std::vector<unsigned int> * nVerticesPerFace,
                         unsigned int ** faces);

    void deleteNormals();

    void add(ControlPoint* cp);

public:
    Entity3D();

    Entity3D(IDManager * idManager);

    virtual ~Entity3D();

    void interact(int id, glm::vec3 worldCoords);

    int id() const;

    glm::vec3 position();

    EntityType type() const;
    QString name() const;
    bool visible() const;
    PaintMode paintMode() const;
    bool isOperation() const;
    unsigned int operation() const;
    BoolMesh mesh();

    glm::mat4 model() const;

    void setPosition(const glm::vec3 position);
    void setName(const QString name);
    void setVisible(const bool visible);
    void setPaintMode(const PaintMode paintMode);
    void setOperation(const unsigned int operation);

    void setColor(glm::vec3 color);
    void setColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale);

    ControlPoint * getControlPoint(int id);
    ControlPoint * getControlPointPosition();

    bool ownsControlPoint(int id);

    // Functions provided for hardcoded or batch transformations.
    void translate(const glm::vec3 offset);
    void rotate(const GLfloat angle, const glm::vec3 axis);
    void scale(const glm::vec3 scale);
    void resetModel();

    void paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
               const Entity3D * selectedEntity, const EntitiesRenderMode renderMode) const;

    void paintOnDifferentGLContext(const glm::mat4 * view, const glm::mat4 * projection) const;

    virtual void updateGeometry();

    void requestParentUpdate();

    void printToConsole();
};

#endif // ENTITY3D_H
