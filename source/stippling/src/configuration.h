/**
 * @file configuration.h
 * @brief Configuration header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 18/December/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#include "util.h"

#include <QColor>


enum EdgeDetectionMethod { SOBEL, CANNY };

/**
 * @brief Configuration class.
 * Represents a viewing configuration for the program.
 */
class Configuration
{
private:
    // Color patterns
    QColor _entityWires; /**< Entity wires color. */
    QColor _positionCP; /**< Position control point color. */
    QColor _resizeCP; /**< Resize control point color. */
    QColor _rotateCP; /**< Rotate control point color. */
    QColor _scaleCP; /**< Scale control point color. */

    QColor _floorWires; /**< Floor wires color. */

    int _packingFactor; /**< Packing factor (min = 1, max = stippling dot size - 1). */
    unsigned int _rngSeed; /**< Random Number Generator seed. */
    unsigned int _stippleDotDispersion; /**< Stipple dot dispersion (maximum offset applied to the position). */
    int _unmodelledStipplingChance; /**< Percentage of the unmodelled image that will result of Stipple dots generation. */
    int _modelledStipplingChance; /**< Percentage of the modelled image that will result of Stipple dots generation. */

    EdgeDetectionMethod _edgeDetectionMethod;

    bool _useTileRendering;
    int _tileWidth;
    int _tileHeight;


public:
    /**
     * @brief Default constructor.
     * Initializes the configuration to the default values.
     */
    Configuration();

    // Getters
    glm::vec3 entityWires() const;
    glm::vec3 positionCP() const;
    glm::vec3 resizeCP() const;
    glm::vec3 rotateCP() const;
    glm::vec3 scaleCP() const;

    glm::vec3 floorWires() const;

    int packingFactor() const;

    unsigned int rngSeed() const;
    unsigned int stippleDotDispersion() const;

    int unmodelledStipplingChance() const;
    int modelledStipplingChance() const;

    EdgeDetectionMethod edgeDetectionMethod() const;

    bool useTileRendering() const;
    int tileWidth() const;
    int tileHeight() const;

    // Setters
    void setEntityWires(QColor color);
    void setPositionCP(QColor color);
    void setResizeCP(QColor color);
    void setRotateCP(QColor color);
    void setScaleCP(QColor color);

    void setFloorWires(QColor color);

    void setPackingFactor(int packingFactor);

    void setRngSeed(unsigned int rngSeed);
    void setStippleDotDispersion(unsigned int stippleDotDispersion);

    void setUnmodelledStipplingChance(int unmodelledStipplingChance);
    void setModelledStipplingChance(int modelledStipplingChance);

    void setEdgeDetectionMethod(EdgeDetectionMethod edgeDetectionMethod);

    void setUseTileRendering(bool useTileRendering);
    void setTileWidth(int tileWidth);
    void setTileHeight(int tileHeight);
};

#endif // CONFIGURATION_H
