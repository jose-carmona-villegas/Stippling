#include "rootentity.h"

RootEntity::RootEntity() : Entity3D()
{
    _type = ROOT;

    _wireframe = GLEntity();
    _solid = GLEntity();
    setName("Root node");
    setVisible(false);

    lines = 0;
}

RootEntity::~RootEntity()
{

}

void RootEntity::recalculateGeometry()
{

}

void RootEntity::interactSpecifically(ControlPoint * cp)
{

}

