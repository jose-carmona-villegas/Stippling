#ifndef ROOTENTITY_H
#define ROOTENTITY_H

#include "entity3d.h"

/**
 * @brief RootEntity class
 * Represents a special type of Entity3D, not renderable and only used as support for the EntityTreeWidget, since a root node is required.
 */
class RootEntity : public Entity3D
{
protected:
    /**
     * @brief Recalculates the root entity's geometry.
     * Not renderable, so no actual calculations are done. Redefined for compatibility reasons.
     */
    void recalculateGeometry();

    /**
     * @brief Specific interactions. Called after the generic (Entity3D) interactions are processed.
     * Not renderable, so no actual calculations are done. Redefined for compatibility reasons.
     * @param Control Point being interacted with.
     */
    void interactSpecifically(ControlPoint * cp);

public:
    /**
     * @brief Default constructor.
     */
    RootEntity();

    /**
     * @brief Destructor.
     */
    ~RootEntity();

};

#endif // ROOTENTITY_H
