/**
 * @file quadtree.h
 * @brief QuadTree class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 23/April/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef QUADTREE_H
#define QUADTREE_H

#include "quadtreenode.h"

class QuadTreeNode;

class QuadTree
{
protected:
    glm::vec4 _area; /**<  */

    QuadTreeNode * _root;

public:
    QuadTree();
    QuadTree(glm::vec4 area, int depth);
    ~QuadTree();

	int numberOfNodes() const;
    int numberOfLeaves() const;
    glm::vec4 getRootArea() const;

    QVector<QVector<StippleDot *> *> getDotsInFullArea();
    QVector<QVector<StippleDot *> *> getDotsInArea(glm::vec4 area);
    QVector<QVector<StippleDot *> *> getDotsInPaddedArea(glm::vec4 area, int padding);

    void add(StippleDot * dot);
};

#endif // QUADTREE_H
