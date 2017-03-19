#include "entitytreenode.h"

EntityTreeNode::EntityTreeNode(Entity3D * entity, EntityTreeNode * parent)
{
    this->parent = parent;
    this->entity = entity;

    _configuration = new SpecificEntityConfiguration();
}

EntityTreeNode::~EntityTreeNode()
{
    if(this->parent != 0)
    {
        this->parent->removeChild(this);
    }

    foreach(EntityTreeNode * n, children)
    {
        if(n != 0)
        {
            delete n;
            n = 0;
        }
    }
    children.clear();

    if(entity != 0)
    {
        delete entity;
        entity = 0;
    }

    if(_configuration != 0)
    {
        delete _configuration;
        _configuration = 0;
    }
}

int EntityTreeNode::id() const
{
    return entity->id();
}

EntityType EntityTreeNode::type() const
{
    return entity->type();
}

QString EntityTreeNode::name() const
{
    return entity->name();
}

bool EntityTreeNode::visible() const
{
    return entity->visible();
}

PaintMode EntityTreeNode::paintMode() const
{
    return entity->paintMode();
}

bool EntityTreeNode::isOperation() const
{
    return entity->isOperation();
}

unsigned int EntityTreeNode::operation() const
{
    return entity->operation();
}

bool EntityTreeNode::hasParent() const
{
    return parent != 0;
}

int EntityTreeNode::numberOfChildren() const
{
    return children.size();
}

SpecificEntityConfiguration * EntityTreeNode::configuration()
{
    return _configuration;
}

glm::vec3 EntityTreeNode::position() const
{
    return entity->position();
}

Entity3D * EntityTreeNode::getEntity() const
{
    return entity;
}

cv::Mat EntityTreeNode::solidRendering()
{
    return _solidRendering;
}

cv::Mat EntityTreeNode::edgeDetection()
{
    return _edgeDetection;
}

void EntityTreeNode::setSolidRendering(cv::Mat solidRendering)
{
    _solidRendering = solidRendering;
}

void EntityTreeNode::setEdgeDetection(cv::Mat edgeDetection)
{
    _edgeDetection = edgeDetection;
}

void EntityTreeNode::setPosition(const glm::vec3 position)
{
    entity->setPosition(position);
}

void EntityTreeNode::addChild(EntityTreeNode * child)
{
    if(child->parent != 0)
    {
        child->parent->removeChild(child);
    }
    child->parent = this;
    children.append(child);

    if(this->entity->isOperation())
    {
        Operation * o = static_cast<Operation*>(entity);
        switch(numberOfChildren())
        {
        case 1:
            o->setLeftOperator(child->entity);
            break;
        case 2:
            o->setRightOperator(child->entity);
            break;
        }
    }
}

void EntityTreeNode::removeChild(EntityTreeNode * child)
{
    for(int i=0; i<children.size(); ++i)
    {
        if(children.at(i) == child)
        {
            if(this->entity->isOperation())
            {
                Operation * o = static_cast<Operation*>(entity);
                if(o->rightOperator() != 0 && o->rightOperator()->id() == child->id())
                {
                    o->setRightOperator(0);
                }
                else if(o->leftOperator() != 0 && o->leftOperator()->id() == child->id())
                {
                    o->setLeftOperator(o->rightOperator());
                    o->setRightOperator(0);
                }
            }
            children.remove(i);
            child->parent = 0;
        }
    }
}

void EntityTreeNode::applyConfigurationSetColor(glm::vec3 color)
{
    entity->setColor(color);
    foreach(EntityTreeNode * n, children)
    {
        n->applyConfigurationSetColor(color);
    }
}

void EntityTreeNode::applyConfigurationSetColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale)
{
    entity->setColorsCPs(position, resize, rotate, scale);
    foreach(EntityTreeNode * n, children)
    {
        n->applyConfigurationSetColorsCPs(position, resize, rotate, scale);
    }
}

QDomElement EntityTreeNode::toXML(QDomDocument * doc) const
{
    //out << "Exporting entity to XML" << endl;
    //entity->printToConsole();

    QDomElement node = doc->createElement("node");

    QDomElement type = doc->createElement("type");
    node.appendChild(type);
    QDomText typeTxt = doc->createTextNode(QString::number(int(entity->type())));
    type.appendChild(typeTxt);

    QDomElement name = doc->createElement("name");
    node.appendChild(name);
    QDomText nameTxt = doc->createTextNode(entity->name());
    name.appendChild(nameTxt);

    QDomElement visible = doc->createElement("visible");
    node.appendChild(visible);
    QDomText visibleTxt = doc->createTextNode(QString::number(int(entity->visible())));
    visible.appendChild(visibleTxt);

    QDomElement isOperation = doc->createElement("isOperation");
    node.appendChild(isOperation);
    QDomText isOperationTxt = doc->createTextNode(QString::number(int(entity->isOperation())));
    isOperation.appendChild(isOperationTxt);

    QDomElement operation = doc->createElement("operation");
    node.appendChild(operation);
    int temp = entity->isOperation() ? int(entity->operation()) : -1;
    QDomText operationTxt = doc->createTextNode(QString::number(temp));
    operation.appendChild(operationTxt);

    QDomElement model = doc->createElement("model");
    node.appendChild(model);
    for(int r=0; r<4; ++r)
    {
        for(int c=0; c<4; ++c)
        {
            QString elemNName = "elem" + QString::number((4*r)+c);
            QDomElement elemN = doc->createElement(elemNName);
            model.appendChild(elemN);

            QString element = QString::number(entity->model()[r][c]);
            QDomText modelTxt = doc->createTextNode(element);
            elemN.appendChild(modelTxt);
        }
    }

    switch(entity->type())
    {
    case OPERATION:
    {
        Operation * o = static_cast<Operation*>(entity);

        QDomElement controlPointsElem = doc->createElement("controlPoints");
        node.appendChild(controlPointsElem);

        QDomElement position = o->getControlPointPosition()->toXML(doc);
        controlPointsElem.appendChild(position);

        QDomElement rotateY = o->getControlPointRotateY()->toXML(doc);
        controlPointsElem.appendChild(rotateY);

        break;
    }

    case CYLINDER:
    {
        Cylinder * c = static_cast<Cylinder*>(entity);

        QDomElement controlPointsElem = doc->createElement("controlPoints");
        node.appendChild(controlPointsElem);

        QDomElement position = c->getControlPointPosition()->toXML(doc);
        controlPointsElem.appendChild(position);

        QDomElement rotateY = c->getControlPointRotateY()->toXML(doc);
        controlPointsElem.appendChild(rotateY);

        QDomElement resizeRadiusBottom = c->getControlPointResizeRadiusBottom()->toXML(doc);
        controlPointsElem.appendChild(resizeRadiusBottom);

        QDomElement resizeRadiusTop = c->getControlPointResizeRadiusTop()->toXML(doc);
        controlPointsElem.appendChild(resizeRadiusTop);

        QDomElement resizeHeight = c->getControlPointResizeHeight()->toXML(doc);
        controlPointsElem.appendChild(resizeHeight);

        break;
    }

    case PRISM:
    {
        Prism * p = static_cast<Prism*>(entity);

        QDomElement controlPointsElem = doc->createElement("controlPoints");
        node.appendChild(controlPointsElem);

        QDomElement position = p->getControlPointPosition()->toXML(doc);
        controlPointsElem.appendChild(position);

        QDomElement rotateY = p->getControlPointRotateY()->toXML(doc);
        controlPointsElem.appendChild(rotateY);

        QDomElement resizeX = p->getControlPointResizeX()->toXML(doc);
        controlPointsElem.appendChild(resizeX);

        QDomElement resizeZ = p->getControlPointResizeZ()->toXML(doc);
        controlPointsElem.appendChild(resizeZ);

        QDomElement resizeHeight = p->getControlPointResizeHeight()->toXML(doc);
        controlPointsElem.appendChild(resizeHeight);

        break;
    }
    }

    QDomElement childrenElem = doc->createElement("children");
    node.appendChild(childrenElem);

    foreach(EntityTreeNode * child, children)
    {
        QDomElement childNode = child->toXML(doc);
        childrenElem.appendChild(childNode);
    }

    return node;
}

void EntityTreeNode::correctEntityPositions()
{
    if(numberOfChildren() == 0)
    {
        // Positions already correct
    }
    else if(numberOfChildren() == 1)
    {
        children.at(0)->setPosition(entity->position());

        children.at(0)->correctEntityPositions();
    }
    else if(numberOfChildren() == 2)
    {
        glm::vec3 difference = children.at(1)->position() - children.at(0)->position();
        glm::vec3 newPos = entity->position() + difference;

        children.at(0)->setPosition(entity->position());
        children.at(1)->setPosition(newPos);

        children.at(0)->correctEntityPositions();
        children.at(1)->correctEntityPositions();
    }
}

EntityTreeNode * EntityTreeNode::find(Entity3D * toFind)
{
    if(toFind == entity)
    {
        return this;
    }
    else
    {
        EntityTreeNode * result = 0;
        foreach(EntityTreeNode * child, children)
        {
            result = child->find(toFind);
            if(result != 0)
            {
                // Found!
                return result;
            }
        }
        // Not found in its descendants
        return 0;
    }
}

