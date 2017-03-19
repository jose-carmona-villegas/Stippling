#ifndef ENTITYTREECONTROLLER_H
#define ENTITYTREECONTROLLER_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

#include <QQueue>
#include <QDomDocument>

#include "entitytreenode.h"
#include "rootentity.h"

class EntityTreeNode;

// As a QObject due to the need for signal/slot interactions...
class EntityTreeController : public QObject
{
    Q_OBJECT

private:
    EntityTreeNode * _root;
    EntityTreeNode * _selected;

    void traverseDepthFirst_kernel(QVector<EntityTreeNode*> * visitedNodes, EntityTreeNode * start) const;
    void traverseBreadthFirst_kernel(QVector<EntityTreeNode*> * visitedNodes, EntityTreeNode * start) const;

public:
    EntityTreeController();
    ~EntityTreeController();

    QVector<EntityTreeNode*> getFirstLevelEntities() const;
    QVector<Entity3D*> * getVisibleEntities() const;
    QVector<Entity3D*> * getAllEntities() const;
    QVector<Entity3D*> * getNonConflictingEntities(Entity3D * start) const;
    EntityTreeNode * find(Entity3D * toFind) const;

    Entity3D * selected() const;
    EntityTreeNode * selectedNode() const;

    bool isSelected(EntityTreeNode * node);

    void select(const int id);
    void deselect();

    void setName(EntityTreeNode * node, const QString name);
    void setVisible(EntityTreeNode * node, const bool visible);
    void setPaintMode(EntityTreeNode * node, const PaintMode paintMode);
    void setOperation(EntityTreeNode * node, const unsigned int operation);

    void addPrimitive(Entity3D * primitive);

    void deleteSelected();

    void applyConfigurationSetColor(glm::vec3 color);
    void applyConfigurationSetColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale);

    void pruneAndGraft(EntityTreeNode * toPrune, EntityTreeNode * graftHost);
    void pruneAndGraftAsPrimitive(EntityTreeNode * toPrune);

    void clear();

    QVector<EntityTreeNode*> * traverseDepthFirst(EntityTreeNode * start) const;
    QVector<EntityTreeNode*> * traverseBreadthFirst(EntityTreeNode * start) const;
    QVector<EntityTreeNode*> * traverseDepthFirst() const;
    QVector<EntityTreeNode*> * traverseBreadthFirst() const;

	QString toXML() const;

    void correctEntityPositions();

    void emitModifiedInformation();

signals:
    void modifiedTree();
    void modifiedInformation();
};

#endif // ENTITYTREECONTROLLER_H
