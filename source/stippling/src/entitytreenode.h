#ifndef ENTITYTREENODE_H
#define ENTITYTREENODE_H

#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "entitytreecontroller.h"
#include "entity3d.h"
#include "operation.h"
#include "cylinder.h"
#include "prism.h"
#include "specificentityconfiguration.h"

class EntityTreeNode
{
    friend class EntityTreeController;
private:
    Entity3D * entity;
    EntityTreeNode * parent;
    SpecificEntityConfiguration * _configuration;
    cv::Mat _solidRendering;
    cv::Mat _edgeDetection;

public:
    QVector<EntityTreeNode*> children;
public:
    EntityTreeNode(Entity3D * entity, EntityTreeNode * parent = 0);
    ~EntityTreeNode();

    int id() const;
    EntityType type() const;
    QString name() const;
    bool visible() const;
    PaintMode paintMode() const;
    bool isOperation() const;
    unsigned int operation() const;
    bool hasParent() const;
    int numberOfChildren() const;
    SpecificEntityConfiguration * configuration();
    glm::vec3 position() const;
    Entity3D * getEntity() const;
    cv::Mat solidRendering();
    cv::Mat edgeDetection();

    void setSolidRendering(cv::Mat solidRendering);
    void setEdgeDetection(cv::Mat edgeDetection);
    void setPosition(const glm::vec3 position);

    void addChild(EntityTreeNode * child);
    void removeChild(EntityTreeNode * child);

    void applyConfigurationSetColor(glm::vec3 color);
    void applyConfigurationSetColorsCPs(glm::vec3 position, glm::vec3 resize, glm::vec3 rotate, glm::vec3 scale);

    QDomElement toXML(QDomDocument * doc) const;

    void correctEntityPositions();

    EntityTreeNode * find(Entity3D * toFind);
};

#endif // ENTITYTREENODE_H
