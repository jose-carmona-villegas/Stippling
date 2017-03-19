/**
 * @file camera.cpp
 * @brief Camera class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 8/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(QString name, float sensorSizeX, float sensorSizeY, float cropFactor)
{
    this->name = name;
    this->sensorSizeX = sensorSizeX;
    this->sensorSizeY = sensorSizeY;
    this->cropFactor = cropFactor;
    forcedFOV = false;
}

Camera::Camera(float fov)
{
    this->name = "Custom camera";
    forcedFOV = true;
    this->fov = fov;
}
