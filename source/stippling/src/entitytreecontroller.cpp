#include "entitytreecontroller.h"

void EntityTreeController::traverseDepthFirst_kernel(QVector<EntityTreeNode*> * visitedNodes, EntityTreeNode * start) const
{
    visitedNodes->append(start);

    foreach(EntityTreeNode * c, start->children)
    {
        traverseDepthFirst_kernel(visitedNodes, c);
    }
}

void EntityTreeController::traverseBreadthFirst_kernel(QVector<EntityTreeNode*> * visitedNodes, EntityTreeNode * start) const
{
    QQueue<EntityTreeNode*> q;

    if(start != 0)
    {
        visitedNodes->append(start);
        q.append(start->children.toList());

        while(!q.empty())
        {
            EntityTreeNode * n = q.dequeue();
            if(n != 0)
            {
                visitedNodes->append(n);
                q.append(n->children.toList());
            }
        }
    }
}

EntityTreeController::EntityTreeController()
{
    Entity3D * temp = new RootEntity();
    _root = new EntityTreeNode(temp, 0);

    _selected = 0;

    emit modifiedTree();
}

EntityTreeController::~EntityTreeController()
{
    _selected = 0;

    if(_root != 0)
    {
        delete _root;
        _root = 0;
    }
}

QVector<EntityTreeNode*> EntityTreeController::getFirstLevelEntities() const
{
    QVector<EntityTreeNode*> firstLevel;

    foreach(EntityTreeNode * child, _root->children)
    {
        firstLevel.append(child);
    }

    return firstLevel;
}

QVector<Entity3D*> * EntityTreeController::getVisibleEntities() const
{
    QVector<Entity3D*> * visibleEntities = new QVector<Entity3D*>();

    // Traverse the tree gathering all visible entities
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        if(n->entity->visible())
        {
            visibleEntities->append(n->entity);
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    return visibleEntities;
}

QVector<Entity3D*> * EntityTreeController::getAllEntities() const
{
    QVector<Entity3D*> * allEntities = new QVector<Entity3D*>();

    // Traverse the tree gathering all visible entities
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        allEntities->append(n->entity);
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    return allEntities;
}

QVector<Entity3D*> * EntityTreeController::getNonConflictingEntities(Entity3D * start) const
{
    QVector<Entity3D*> * nonConflictingEntities = new QVector<Entity3D*>();

    // Find the node that owns the starting entity
    EntityTreeNode * startNode = find(start);

    // Collect the non conflicting entities
    if(startNode != 0)
    {
        nonConflictingEntities->append(startNode->entity);

        EntityTreeNode * comingFrom = startNode;
        EntityTreeNode * current = startNode->parent;
        bool finished = (current == 0);
        while(!finished)
        {
            foreach(EntityTreeNode * child, current->children)
            {
                if(child != comingFrom)
                {
                    nonConflictingEntities->append(child->entity);
                }
            }

            comingFrom = current;
            current = comingFrom->parent;

            finished = (current == 0);
        }
    }

    return nonConflictingEntities;
}

EntityTreeNode * EntityTreeController::find(Entity3D * toFind) const
{
    return _root->find(toFind);
}

Entity3D * EntityTreeController::selected() const
{
    if(_selected != 0)
    {
        return _selected->entity;
    }
    else
    {
        return 0;
    }
}

EntityTreeNode * EntityTreeController::selectedNode() const
{
    return _selected;
}

bool EntityTreeController::isSelected(EntityTreeNode * node)
{
    return (node == _selected);
}

void EntityTreeController::select(const int id)
{
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode *  node, *nodes)
    {
        if(node->entity->id() == id)
        {
            _selected = node;
            break;
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    /*
    if(_selected != 0 && _selected->entity != 0)
    {
        out << "Selected " << _selected->entity->name() << endl;
    }
    else
    {
        out << "Selected None" << endl;
    }
    */

    emit modifiedInformation();
}

void EntityTreeController::deselect()
{
    _selected = 0;

    emit modifiedInformation();
}

void EntityTreeController::setName(EntityTreeNode * node, const QString name)
{
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        if(n == node)
        {
            if(n->entity != 0)
            {
                n->entity->setName(name);
            }
            break;
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    emit modifiedInformation();
}

void EntityTreeController::setVisible(EntityTreeNode * node, const bool visible)
{
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        if(n == node)
        {
            if(n->entity != 0)
            {
                n->entity->setVisible(visible);
            }
            break;
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    emit modifiedInformation();
}

void EntityTreeController::setPaintMode(EntityTreeNode *node, const PaintMode paintMode)
{
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        if(n == node)
        {
            if(n->entity != 0)
            {
                n->entity->setPaintMode(paintMode);
            }
            break;
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    emit modifiedInformation();
}

void EntityTreeController::setOperation(EntityTreeNode * node, const unsigned int operation)
{
    QVector<EntityTreeNode*> * nodes = traverseBreadthFirst(_root);

    foreach(EntityTreeNode * n, *nodes)
    {
        if(n == node)
        {
            if(n->entity != 0)
            {
                n->entity->setOperation(operation);
            }
            break;
        }
    }

    nodes->clear();
    if(nodes != 0)
    {
        delete nodes;
        nodes = 0;
    }

    emit modifiedInformation();
}

void EntityTreeController::addPrimitive(Entity3D * primitive)
{
    EntityTreeNode * node = new EntityTreeNode(primitive);

    _root->addChild(node);

    emit modifiedTree();
}

void EntityTreeController::deleteSelected()
{
    EntityTreeNode * toDelete = selectedNode();

    if(toDelete != 0)
    {
        deselect();

        Entity3D * temp = new RootEntity();
        EntityTreeNode * tempHost = new EntityTreeNode(temp, 0);

        // Prune the branch and graft it on the temporary host.
        // The method emits the modified signal.
        pruneAndGraft(toDelete, tempHost);

        // Finally delete the host and its descendants recursively.
        delete tempHost;
        tempHost = 0;
    }
}

void EntityTreeController::applyConfigurationSetColor(glm::vec3 color)
{
    _root->applyConfigurationSetColor(color);
}

void EntityTreeController::applyConfigurationSetColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale)
{
    _root->applyConfigurationSetColorsCPs(position, resize, rotate, scale);
}

void EntityTreeController::pruneAndGraft(EntityTreeNode * toPrune, EntityTreeNode * graftHost)
{
    graftHost->addChild(toPrune);

    emit modifiedTree();
}

void EntityTreeController::pruneAndGraftAsPrimitive(EntityTreeNode * toPrune)
{
    pruneAndGraft(toPrune, _root);

    // pruneAndGraft emits the modifiedTree signal
}

void EntityTreeController::clear()
{
    //out << "Clearing EntityTreeController" << endl;

    foreach(EntityTreeNode * child, _root->children)
    {
        if(child != 0)
        {
            delete child;
            child = 0;
        }
    }

    _root->children.clear();

    _selected = 0;

    //out << "Cleared EntityTreeController" << endl;

    emit modifiedTree();
}

QVector<EntityTreeNode*> * EntityTreeController::traverseDepthFirst(EntityTreeNode * start) const
{
    QVector<EntityTreeNode*> * traversal = new QVector<EntityTreeNode*>();

    traverseDepthFirst_kernel(traversal, start);

    return traversal;
}

QVector<EntityTreeNode*> * EntityTreeController::traverseBreadthFirst(EntityTreeNode * start) const
{
    QVector<EntityTreeNode*> * traversal = new QVector<EntityTreeNode*>();

    traverseBreadthFirst_kernel(traversal, start);

    return traversal;
}

QVector<EntityTreeNode*> * EntityTreeController::traverseDepthFirst() const
{
    return traverseDepthFirst(_root);
}

QVector<EntityTreeNode*> * EntityTreeController::traverseBreadthFirst() const
{
    return traverseBreadthFirst(_root);
}

QString EntityTreeController::toXML() const
{
	QDomDocument doc;
	QDomProcessingInstruction instr = doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	doc.appendChild(instr);

	// generate csgtree tag as root
    QDomElement csgTreeElement = doc.createElement("csgtree");
    doc.appendChild(csgTreeElement);

    QDomElement rootElement = _root->toXML(&doc);

    csgTreeElement.appendChild(rootElement);
	
	return doc.toString();
}

void EntityTreeController::correctEntityPositions()
{
    QVector<EntityTreeNode *> firstLevel = getFirstLevelEntities();

    foreach(EntityTreeNode * node, firstLevel)
    {
        node->correctEntityPositions();
    }
}

void EntityTreeController::emitModifiedInformation()
{
    emit modifiedInformation();
}

