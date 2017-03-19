/**
 * @file specificentityconfiguration.h
 * @brief SpecificEntityConfiguration header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 5/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef SPECIFICENTITYCONFIGURATION_H
#define SPECIFICENTITYCONFIGURATION_H

#include "configuration.h"

class SpecificEntityConfiguration
{
private:
    int _percentageSilhouetteDispersion;
    int _percentageInternalGeneration;

    bool _hasSpecificEdgeDetectionMethod;
    EdgeDetectionMethod _edgeDetectionMethod;

public:
    /**
     * @brief Default constructor.
     * Initializes the configuration to the default values.
     */
    SpecificEntityConfiguration();

    // Getters
    int percentageSilhouetteDispersion() const;
    int percentageInternalGeneration() const;
    bool hasSpecificEdgeDetectionMethod() const;
    EdgeDetectionMethod edgeDetectionMethod() const;

    // Setters
    void setPercentageSilhouetteDispersion(int percentageSilhouetteDispersion);
    void setPercentageInternalGeneration(int percentageInternalGeneration);
    void setHasSpecificEdgeDetectionMethod(bool hasSpecificEdgeDetectionMethod);
    void setEdgeDetectionMethod(EdgeDetectionMethod edgeDetectionMethod);

    bool isDefault() const;
};

#endif // SPECIFICENTITYCONFIGURATION_H
