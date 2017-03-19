/**
 * @file configuration.cpp
 * @brief Configuration source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 18/December/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "configuration.h"

Configuration::Configuration()
{
    // Default configuration
    _entityWires = QColor(0,255,0);
    _positionCP = QColor(255,0,0);
    _resizeCP = QColor(0,0,255);
    _rotateCP = QColor(0,0,0);
    _scaleCP = QColor(0,70,70);

    _floorWires = QColor(0,70,0);

    _packingFactor = 3;
    _rngSeed = 0;
    _stippleDotDispersion = 0;

    _unmodelledStipplingChance = 15;
    _modelledStipplingChance = 75;

    _edgeDetectionMethod = SOBEL;

    _useTileRendering = false; // Single tile mode by default
    _tileWidth = 3000;
    _tileHeight = 3000;
}





glm::vec3 Configuration::entityWires() const
{
    return Util::toGlmVec3(_entityWires);
}

glm::vec3 Configuration::positionCP() const
{
    return Util::toGlmVec3(_positionCP);
}

glm::vec3 Configuration::resizeCP() const
{
    return Util::toGlmVec3(_resizeCP);
}

glm::vec3 Configuration::rotateCP() const
{
    return Util::toGlmVec3(_rotateCP);
}

glm::vec3 Configuration::scaleCP() const
{
    return Util::toGlmVec3(_scaleCP);
}

glm::vec3 Configuration::floorWires() const
{
    return Util::toGlmVec3(_floorWires);
}

int Configuration::packingFactor() const
{
    return _packingFactor;
}

unsigned int Configuration::rngSeed() const
{
    return _rngSeed;
}

unsigned int Configuration::stippleDotDispersion() const
{
    return _stippleDotDispersion;
}

int Configuration::unmodelledStipplingChance() const
{
    return _unmodelledStipplingChance;
}

int Configuration::modelledStipplingChance() const
{
    return _modelledStipplingChance;
}

EdgeDetectionMethod Configuration::edgeDetectionMethod() const
{
    return _edgeDetectionMethod;
}

bool Configuration::useTileRendering() const
{
    return _useTileRendering;
}

int Configuration::tileWidth() const
{
    return _tileWidth;
}

int Configuration::tileHeight() const
{
    return _tileHeight;
}











void Configuration::setEntityWires(QColor color)
{
    _entityWires = color;
}

void Configuration::setPositionCP(QColor color)
{
    _positionCP = color;
}

void Configuration::setResizeCP(QColor color)
{
    _resizeCP = color;
}

void Configuration::setRotateCP(QColor color)
{
    _rotateCP = color;
}

void Configuration::setScaleCP(QColor color)
{
    _scaleCP = color;
}

void Configuration::setFloorWires(QColor color)
{
    _floorWires = color;
}

void Configuration::setPackingFactor(int packingFactor)
{
    _packingFactor = packingFactor;
}

void Configuration::setRngSeed(unsigned int rngSeed)
{
    _rngSeed = rngSeed;
}

void Configuration::setStippleDotDispersion(unsigned int stippleDotDispersion)
{
    _stippleDotDispersion = stippleDotDispersion;
}

void Configuration::setUnmodelledStipplingChance(int unmodelledStipplingChance)
{
    _unmodelledStipplingChance = unmodelledStipplingChance;
}

void Configuration::setModelledStipplingChance(int modelledStipplingChance)
{
    _modelledStipplingChance = modelledStipplingChance;
}

void Configuration::setEdgeDetectionMethod(EdgeDetectionMethod edgeDetectionMethod)
{
    _edgeDetectionMethod = edgeDetectionMethod;
}

void Configuration::setUseTileRendering(bool useTileRendering)
{
    _useTileRendering = useTileRendering;
}

void Configuration::setTileWidth(int tileWidth)
{
    _tileWidth = tileWidth;
}

void Configuration::setTileHeight(int tileHeight)
{
    _tileHeight = tileHeight;
}


