/**
 * @file camera.h
 * @brief Camera class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 8/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <util.h>

#include <QString>

/**
 * @brief Camera class.
 * Represents a camera. Holds the factory parameters that define the camera.
 */
class Camera
{
public:
    QString name; /**< Name (usually the brand and model). */
    float sensorSizeX; /**< Horizontal sensor size, measured in milimeters. */
    float sensorSizeY; /**< Vertical sensor size, measured in milimiters. */
    float cropFactor; /**< Crop factor (or Focal Length Multiplier). A unitless factor. */

    bool forcedFOV; /**< Flag that indicates if the FOV should be calculated. */
    float fov; /**< Vertical FOV (Vertical Field of view), measured in degrees. */

    /**
     * @brief Default constructor.
     * The camera is initialized to empty values.
     */
    Camera();

    /**
     * @brief Constructor.
     * Initializes the camera to the given values, without a forced FOV.
     */
    Camera(QString name, float sensorSizeX, float sensorSizeY, float cropFactor);

    /**
     * @brief Constructor.
     * Initializes a custom camera, with the specified FOV as forced.
     */
    Camera(float fov);
};

#endif // CAMERA_H
