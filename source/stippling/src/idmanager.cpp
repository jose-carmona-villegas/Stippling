/**
 * @file idmanager.cpp
 * @brief IDManager class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 14/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "idmanager.h"
#include "util.h"
IDManager::IDManager()
{
    _backgroundID = decodeID(glm::vec3(0,0,0));
    _invalidID = 1;
    _highestUnassignedID = 2;

    /*
    out << "IDManager constructed by default" << endl;
    out << "IDManager backgroundID: " << _backgroundID << endl;
    out << "IDManager invalidID: " << _invalidID << endl;
    */
}

IDManager::IDManager(glm::vec4 backgroundColor)
{
    _backgroundID = decodeID(glm::vec3(backgroundColor.r*255.0f,backgroundColor.g*255.0f,backgroundColor.b*255.0f));
    _invalidID = _backgroundID + 1;
    if(_backgroundID == 0)
    {
        _highestUnassignedID = 2;
    }
    else
    {
        _highestUnassignedID = 0;
    }

    /*
    out << "IDManager constructed using assigned background color" << endl;
    out << "IDManager backgroundID: " << _backgroundID << endl;
    out << "IDManager invalidID: " << _invalidID << endl;
    */
}

IDManager::~IDManager()
{
    //out << "IDManager destroyed." << endl;
}

int IDManager::NONE() const
{
    return _invalidID;
}

int IDManager::BackgroundID() const
{
    return _backgroundID;
}

int IDManager::getNewID()
{
    while( _highestUnassignedID == _backgroundID || _highestUnassignedID == _invalidID )
    {
        _highestUnassignedID += 1;
    }

    int newID = _highestUnassignedID;
    // Increment the id counter so _highestUnassignedID shows the next free id.
    _highestUnassignedID += 1;

    //out << "IDManager returns new id: " << newID << endl;
    return newID;
}

glm::vec3 IDManager::encodeID(int id)
{
    // Convert "id", the integer mesh ID, into an RGB color
    int r = (id & 0x000000FF) >>  0;
    int g = (id & 0x0000FF00) >>  8;
    int b = (id & 0x00FF0000) >> 16;

    return glm::vec3(r/255.0f,g/255.0f,b/255.0f);
}

int IDManager::decodeID(glm::vec3 data)
{
    // Convert the RGB color back to an integer ID
    return int( (data[0]) + (data[1] * 256) + (data[2] * 256*256) );
}

int IDManager::decodeID(glm::vec4 data)
{
    return decodeID(glm::vec3(data.r, data.g, data.b));
}
