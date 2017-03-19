/**
 * @file QuadTree.cpp
 * @brief QuadTree class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 23/April/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "quadtree.h"

QuadTree::QuadTree()
{
    //out << "QuadTree constructed by default" << endl;
}

QuadTree::QuadTree(glm::vec4 area, int depth)
{
	//out << "Constructing QuadTree with depth of " << depth << "." << endl;

    int numNodes = 0;

    _area = glm::vec4(int(area.x), int(area.y), int(area.z), int(area.w));

    _root = new QuadTreeNode(_area);
    ++numNodes;

    QVector<QuadTreeNode *> currentLevel;
    QVector<QuadTreeNode *> nextLevel;

    if(depth > 0)
    {
        currentLevel.push_back(_root);
    }

    for(int d=1; d<=depth; ++d)
    {
        foreach(QuadTreeNode * node, currentLevel)
        {
            QuadTreeNode * child;

            child = new QuadTreeNode(node->getSubArea(0));
            node->_children.push_back(child);
            nextLevel.append(child);
            ++numNodes;

            child = new QuadTreeNode(node->getSubArea(1));
            node->_children.push_back(child);
            nextLevel.append(child);
            ++numNodes;

            child = new QuadTreeNode(node->getSubArea(2));
            node->_children.push_back(child);
            nextLevel.append(child);
            ++numNodes;

            child = new QuadTreeNode(node->getSubArea(3));
            node->_children.push_back(child);
            nextLevel.append(child);
            ++numNodes;
        }
        currentLevel.clear();
        currentLevel = QVector<QuadTreeNode *>::fromList(nextLevel.toList());
        nextLevel.clear();

		//out << "Depth level " << d << " completed." << endl;
    }

	// This protected method will optimize access to the tree by making 
	// each node get references to the dot vectors of its descendants.
	_root->getChildrensDotVectors();

    /*
    out << "QuadTree initialized" << endl;
    out << "Initialized as a perfect k-ary tree with height = " << depth << endl;
    out << "There should be (4^{h+1} - 1)/(4-1) nodes, which equals " << long((pow(double(4), depth+1)-1)/(4-1)) << " nodes." << endl;
    out << numNodes << " nodes were created for the QuadTree." << endl;
    */
}

QuadTree::~QuadTree()
{
    delete _root;
    _root = 0;
    //out << "QuadTree destroyed." << endl;
}

int QuadTree::numberOfNodes() const
{
	return _root->numberOfDescendants();
}

int QuadTree::numberOfLeaves() const
{
    return _root->numberOfLeafDescendants();
}

glm::vec4 QuadTree::getRootArea() const
{
    return glm::vec4(_root->_area);
}

QVector<QVector<StippleDot *> *> QuadTree::getDotsInFullArea()
{
    return _root->getDots();
}

QVector<QVector<StippleDot *> *> QuadTree::getDotsInArea(glm::vec4 area)
{
    return _root->getDotsInArea(area);
}

QVector<QVector<StippleDot *> *> QuadTree::getDotsInPaddedArea(glm::vec4 area, int padding)
{
    //out << endl << endl << endl << "QuadTree#getDotsInPaddedArea()" << endl;

    glm::vec4 paddedArea = area;

    paddedArea.x -= padding;
    paddedArea.y -= padding;
    paddedArea.z += padding;
    paddedArea.w += padding;

    glm::vec4 applicableArea = _root->intersection(_root->_area, paddedArea);

	/*
    out << "Root area:" << endl;
    Util::printVector(_root->_area);
    out << "Padded area:" << endl;
    Util::printVector(paddedArea);
    out << "Intersection area:" << endl;
    Util::printVector(applicableArea);
    out << endl;
	*/

    return _root->getDotsInArea(paddedArea);
}

void QuadTree::add(StippleDot * dot)
{
    _root->add(dot);
}
