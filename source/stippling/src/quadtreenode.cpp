/**
 * @file QuadTreeNode.cpp
 * @brief QuadTreeNode class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 19/March/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "quadtreenode.h"

bool QuadTreeNode::isBetween(int value, int extreme1, int extreme2)
{
    return ((value >= extreme1) && (value <= extreme2)) || ((value >= extreme2) && (value <= extreme1));
}

bool QuadTreeNode::doesPositionBelongTo(glm::vec2 position, glm::vec4 area)
{
    return isBetween(position.x, area.x, area.z) && isBetween(position.y, area.y, area.w);
}

glm::vec4 QuadTreeNode::intersection(glm::vec4 area1, glm::vec4 area2)
{
    glm::vec4 intersection = glm::vec4(0,0,0,0);

    glm::vec4 areaRight;
    glm::vec4 areaLeft;

    if(area1.x <= area2.x)
    {
        areaRight = area1;
        areaLeft = area2;
    }
    else
    {
        areaRight = area2;
        areaLeft = area1;
    }

    int newX = qMax(areaRight.x, areaLeft.x);
    int newY = qMax(areaRight.y, areaLeft.y);
    int newZ = qMin(areaRight.z, areaLeft.z);
    int newW = qMin(areaRight.w, areaLeft.w);

	
    if((newX == newZ) && (newY == newW))
    {
        intersection = glm::vec4(0,0,0,0);
    }
	else if(!isBetween(newX, areaRight.x, areaRight.z)
		|| !isBetween(newX, areaLeft.x, areaLeft.z)
		|| !isBetween(newZ, areaRight.x, areaRight.z)
		|| !isBetween(newZ, areaLeft.x, areaLeft.z)
		|| !isBetween(newY, areaRight.y, areaRight.w)
		|| !isBetween(newY, areaLeft.y, areaLeft.w)
		|| !isBetween(newW, areaRight.y, areaRight.w)
		|| !isBetween(newW, areaLeft.y, areaLeft.w))
	{
		intersection = glm::vec4(0,0,0,0);
	}
    else
    {
        intersection = glm::vec4(newX,newY,newZ,newW);
    }

    return intersection;
}

QVector<QVector<StippleDot *> *> * QuadTreeNode::getChildrensDotVectors()
{
    if(this->hasChildren())
    {
        foreach(QuadTreeNode * node, _children)
        {
            foreach(QVector<StippleDot *> * dotVector, *(node->getChildrensDotVectors()))
            {
                _dotVectors.append(dotVector);
            }
        }
    }
    else
    {
        _dotVectors.append(&_dots);
    }

    return &_dotVectors;
}

QuadTreeNode::QuadTreeNode(glm::vec4 area)
{
    _area = glm::vec4(int(area.x), int(area.y), int(area.z), int(area.w));

    _children.clear();
    _children.reserve(4);
}

QuadTreeNode::~QuadTreeNode()
{
    foreach(QuadTreeNode * child, _children)
    {
        delete child;
    }
    _children.clear();

    _dots.clear();
}

bool QuadTreeNode::hasChildren() const
{
    return _children.size() != 0;
}

int QuadTreeNode::numberOfDescendants() const
{
    int descendants = 1;
    if(hasChildren())
    {
        foreach(QuadTreeNode * child, _children)
        {
            descendants += child->numberOfDescendants();
        }
    }
    return descendants;
}

int QuadTreeNode::numberOfLeafDescendants() const
{
    int leaves = 0;
    if(hasChildren())
    {
        foreach(QuadTreeNode * child, _children)
        {
            leaves += child->numberOfLeafDescendants();
        }
    }
    else
    {
        leaves = 1;
    }
    return leaves;
}

QuadTreeNode QuadTreeNode::get(QVector<bool> quadcode)
{
    if(quadcode.size() == 0)
    {
        return QuadTreeNode(_area);
    }
    else
    {
        glm::vec4 area = glm::vec4(_area);

        for(unsigned int i=0; i<(unsigned int)(quadcode.size()); i+=2)
        {
            int bit0 = quadcode.at(2*i)?1:0;
            int bit1 = quadcode.at((2*i)+1)?1:0;
            int choice = 2*(bit1) + bit0;

            int xLow = area.x;
            int yLow = area.y;
            int xMid = area.x + ((area.z - area.x)/2);
            int yMid = area.y + ((area.w - area.y)/2);
            int xTop = area.z;
            int yTop = area.w;

            switch(choice)
            {
            case 0:
                area = glm::vec4(xLow,yLow,xMid,yMid);
                break;
            case 1:
                area = glm::vec4(xMid,yLow,xTop,yMid);
                break;
            case 2:
                area = glm::vec4(xLow,yMid,xMid,yTop);
                break;
            case 3:
                area = glm::vec4(xMid,yMid,xTop,yTop);
                break;
            }

            if((abs(area.x - area.z) == 1) && (abs(area.y - area.w) == 1))
            {
                // If the area designates a single pixel, then the rest of the bits arent' representative.
                break;
            }
        }

        return QuadTreeNode(area);
    }
}

QVector<bool> QuadTreeNode::get(glm::vec2 position)
{
    QVector<bool> quadcode;

    glm::vec4 area = glm::vec4(_area);

    bool fullResolution = false;
    while(!fullResolution)
    {
        int xLow = area.x;
        int yLow = area.y;
        int xMid = area.x + ((area.z - area.x)/2);
        int yMid = area.y + ((area.w - area.y)/2);
        int xTop = area.z;
        int yTop = area.w;

        int bit0;
        int bit1;

        if(isBetween(position.x, xLow, xMid))
        {
            bit0 = 0;
        }
        else if(isBetween(position.x, xMid, xTop))
        {
            bit0 = 1;
        }
        if(isBetween(position.y, yLow, yMid))
        {
            bit1 = 0;
        }
        else if(isBetween(position.y, yMid, yTop))
        {
            bit1 = 1;
        }

        int choice = 2*(bit1) + bit0;
        switch(choice)
        {
        case 0:
            area = glm::vec4(xLow,yLow,xMid,yMid);
            break;
        case 1:
            area = glm::vec4(xMid,yLow,xTop,yMid);
            break;
        case 2:
            area = glm::vec4(xLow,yMid,xMid,yTop);
            break;
        case 3:
            area = glm::vec4(xMid,yMid,xTop,yTop);
            break;
        }

        quadcode.push_back(bit0);
        quadcode.push_back(bit1);

        if((abs(area.x - area.z) == 1) && (abs(area.y - area.w) == 1))
        {
            // If the area designates a single pixel, then the rest of the bits arent' representative.
            fullResolution = true;
        }
    }

    return quadcode;
}

int QuadTreeNode::getQuadrant(glm::vec2 position)
{
    int quadrant;

    glm::vec4 quadrant0 = getSubArea(0);
    glm::vec4 quadrant1 = getSubArea(1);
    glm::vec4 quadrant2 = getSubArea(2);
    glm::vec4 quadrant3 = getSubArea(3);

    if(doesPositionBelongTo(position, quadrant0))
    {
        quadrant = 0;
    }
    else if(doesPositionBelongTo(position, quadrant1))
    {
        quadrant = 1;
    }
    else if(doesPositionBelongTo(position, quadrant2))
    {
        quadrant = 2;
    }
    else if(doesPositionBelongTo(position, quadrant3))
    {
        quadrant = 3;
    }
    else
    {
        quadrant = -1;
    }

    return quadrant;
}

glm::vec4 QuadTreeNode::getSubArea(int quadrant)
{
    glm::vec4 area = _area;

    int xLow = area.x;
    int yLow = area.y;
    int xMid = area.x + ((area.z - area.x)/2);
    int yMid = area.y + ((area.w - area.y)/2);
    int xTop = area.z;
    int yTop = area.w;

    switch(quadrant)
    {
    case 0:
        area = glm::vec4(xLow,yLow,xMid,yMid);
        break;
    case 1:
        area = glm::vec4(xMid,yLow,xTop,yMid);
        break;
    case 2:
        area = glm::vec4(xLow,yMid,xMid,yTop);
        break;
    case 3:
        area = glm::vec4(xMid,yMid,xTop,yTop);
        break;
    }

    return area;
}

glm::vec4 QuadTreeNode::getSubArea(glm::vec2 position)
{
    return getSubArea(getQuadrant(position));
}

QVector<QVector<StippleDot *> *> QuadTreeNode::getDots()
{
    return _dotVectors;
}

QVector<QVector<StippleDot *> *> QuadTreeNode::getDots(QVector<bool> quadcode)
{
    QVector<QVector<StippleDot *> *> dotVectors;

    int index_quadcode = 0;

    bool reachedMaxDepth = false;
    while(!reachedMaxDepth)
    {
        if(this->hasChildren())
        {
            int bit0 = quadcode.at(index_quadcode)?1:0;
            ++index_quadcode;
            int bit1 = quadcode.at(index_quadcode)?1:0;
            ++index_quadcode;
            int choice = 2*(bit1) + bit0;

            foreach(QVector<StippleDot *> * dotVector, _children.at(choice)->getDots())
            {
                dotVectors.append(dotVector);
            }
        }
        else
        {
            dotVectors.append(&_dots);
        }
    }

    return dotVectors;
}

QVector<QVector<StippleDot *> *> QuadTreeNode::getDotsInArea(glm::vec4 area)
{
    QVector<QVector<StippleDot *> *> dotVectors;

    glm::vec4 applicableArea = intersection(_area, area);

	/*
    out << "Node area:" << endl;
    Util::printVector(_area);
    out << "Given area:" << endl;
    Util::printVector(area);
    out << "Intersection area:" << endl;
    Util::printVector(applicableArea);
    out << endl;
	*/

    if(applicableArea != glm::vec4(0,0,0,0))
    {
		if(applicableArea == _area)
		{
			return _dotVectors;
		}
        if(hasChildren())
        {
            foreach(QuadTreeNode * node, _children)
            {
                foreach(QVector<StippleDot *> * dotVector, node->getDotsInArea(applicableArea))
                {
                    dotVectors.append(dotVector);
                }
            }
        }
        else
        {
            dotVectors.append(&_dots);
        }
    }

    return dotVectors;
}

void QuadTreeNode::add(StippleDot * dot)
{
    /*
    out << "Node area:" << endl;
    Util::printVector(_area);
    out << "Node descendants: " << numberOfDescendants() << endl;
    */

    int quadrant = getQuadrant(dot->finalPostion());

    if(quadrant != -1)
    {
        if(hasChildren())
        {
            _children.at(quadrant)->add(dot);
        }
        else
        {
            _dots.append(dot);
        }
    }
}

