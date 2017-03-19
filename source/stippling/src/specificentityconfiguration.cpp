/**
 * @file specificentityconfiguration.cpp
 * @brief SpecificEntityConfiguration source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 5/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "specificentityconfiguration.h"

SpecificEntityConfiguration::SpecificEntityConfiguration()
{
    // Default configuration
    _percentageSilhouetteDispersion = 0;
    _percentageInternalGeneration = -1;
    _hasSpecificEdgeDetectionMethod = false;
    _edgeDetectionMethod = SOBEL;
}





int SpecificEntityConfiguration::percentageSilhouetteDispersion() const
{
    return _percentageSilhouetteDispersion;
}

int SpecificEntityConfiguration::percentageInternalGeneration() const
{
    return _percentageInternalGeneration;
}

bool SpecificEntityConfiguration::hasSpecificEdgeDetectionMethod() const
{
    return _hasSpecificEdgeDetectionMethod;
}

EdgeDetectionMethod SpecificEntityConfiguration::edgeDetectionMethod() const
{
    return _edgeDetectionMethod;
}






void SpecificEntityConfiguration::setPercentageSilhouetteDispersion(int percentageSilhouetteDispersion)
{
    _percentageSilhouetteDispersion = percentageSilhouetteDispersion;
}

void SpecificEntityConfiguration::setPercentageInternalGeneration(int percentageInternalGeneration)
{
    _percentageInternalGeneration = percentageInternalGeneration;
}

void SpecificEntityConfiguration::setHasSpecificEdgeDetectionMethod(bool hasSpecificEdgeDetectionMethod)
{
    _hasSpecificEdgeDetectionMethod = hasSpecificEdgeDetectionMethod;
}

void SpecificEntityConfiguration::setEdgeDetectionMethod(EdgeDetectionMethod edgeDetectionMethod)
{
    _edgeDetectionMethod = edgeDetectionMethod;
}




bool SpecificEntityConfiguration::isDefault() const
{
    bool isDefault = true;
    if(_percentageSilhouetteDispersion != 0)
    {
        isDefault = false;
    }
    if(_percentageInternalGeneration != -1)
    {
        isDefault = false;
    }
    if(_hasSpecificEdgeDetectionMethod)
    {
        isDefault = false;
    }
    return isDefault;
}
