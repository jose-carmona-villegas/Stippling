/**
 * @file quadtreenode.h
 * @brief QuadTreeNode class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 21/March/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>


#include <cmath>
#include <QVector>


#include "util.h"
#include "stippledot.h"
#include "quadtree.h"

/**
 * @brief QuadTreeNode class.
 * Represents a Quad-tree node.
 *
 * The area and its quadrants:
 *  ____
 * |0|1|
 * |---|
 * |2|3|
 * -----
 *
 */
class QuadTreeNode
{
    friend class QuadTree;
protected:
    QVector<QuadTreeNode *> _children;
    QVector<StippleDot *> _dots;
	QVector<QVector<StippleDot *> *> _dotVectors;


    glm::vec4 _area; /**<  */

    bool isBetween(int value, int extreme1, int extreme2);
    bool doesPositionBelongTo(glm::vec2 position, glm::vec4 area);
    glm::vec4 intersection(glm::vec4 area1, glm::vec4 area2);

	QVector<QVector<StippleDot *> *> * getChildrensDotVectors();

public:
    /**
     * @brief Constructor.
     * Initializes the QuadTreeNode using the specified area of the provided image.
     */
    QuadTreeNode(glm::vec4 area);

    ~QuadTreeNode();

    bool hasChildren() const;
    int numberOfDescendants() const;
    int numberOfLeafDescendants() const;

    /**
     * @brief get
     * @param quadcode
     * @return
     */
    QuadTreeNode get(QVector<bool> quadcode);

    QVector<bool> get(glm::vec2 position);

    int getQuadrant(glm::vec2 position);
    glm::vec4 getSubArea(int quadrant);
    glm::vec4 getSubArea(glm::vec2 position);


    QVector<QVector<StippleDot *> *> getDots();
    QVector<QVector<StippleDot *> *> getDots(QVector<bool> quadcode);
    QVector<QVector<StippleDot *> *> getDotsInArea(glm::vec4 area);

    void add(StippleDot * dot);
};

#endif // QUADTREENODE_H
