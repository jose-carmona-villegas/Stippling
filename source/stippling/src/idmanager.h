/**
 * @file idmanager.h
 * @brief IDManager class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 14/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef IDMANAGER_H
#define IDMANAGER_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

#include "util.h"

/**
 * @brief IDManager class.
 * Manages IDs and encodes/decodes their associated color.
 * Keeps track of the background color and generates new unused IDs
 * (different both to the background and to any previously generated IDs).
 */
class IDManager
{
private:
    int _invalidID; /**< Invalid ID (reserved id, to represent no id entities). */
    int _backgroundID; /**< Background ID (reserved id corresponding to the background color). */
    int _highestUnassignedID; /**< Highest unassigned ID (this pointer always indicates an id that can be used). */

public:
    /**
     * @brief Default constructor.
     * Reserves the IDs: 0 for background (black), and 1 for no id. The first usable id is 2.
     */
    IDManager();

    /**
     * @brief Custom background color constructor.
     * Reserves the custom background id for the the background, and the next one for the invalid id.
     * @param backgroundColor RGBA normalized color (varying in [0,1]).
     * At this current state of implementation the Alpha channel is not used when encoding.
     */
    IDManager(glm::vec4 backgroundColor);

    /**
     * @brief Destructor
     */
    ~IDManager();

    /**
     * @brief Returns the id that designates entities with no id.
     * @return The id that designates entities with no id.
     */
    int NONE() const;

    /**
     * @brief Returns the background id.
     * @return The background id.
     */
    int BackgroundID() const;

    /**
     * @brief Returns a new unused valid id.
     * @return A new unused valid id.
     */
    int getNewID();

    /**
     * @brief Encodes the provided id as a color.
     * Returns normalized rgb color (varying in [0,1])
     * @param The id to be encoded as a color.
     * @return The color associated to the id.
     */
    glm::vec3 encodeID(int id);

    /**
     * @brief Dencodes the provided id as a color.
     * Requires rgb color varying in [0,255]
     * @param The color to be decoded as an id.
     * @return The id associated to the color.
     */
    int decodeID(glm::vec3 data);

    /**
     * @brief Dencodes the provided id as a color.
     * At the current state of implementation, the Alpha channel is not used.
     * @param The color to be decoded as an id.
     * @return The id associated to the color.
     */
    int decodeID(glm::vec4 data);
};

#endif // IDMANAGER_H
