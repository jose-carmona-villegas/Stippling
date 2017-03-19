/**
 * @file enums.h
 * @brief Enums header file
 * Contains enumeration declarations common to multiple classes.
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 14/November/2013
 *
 * @thanks Germán Arroyo <arroyo@ugr.es>
 *
 */

#ifndef ENUMS_H
#define ENUMS_H

enum ProjectionMode { PERSPECTIVE, ORTHOGRAPHIC };
enum SelectionMode { ON, OFF };
enum PaintMode { ENTITY, TRANSLATION, RESIZE, ROTATION, SCALE, ANY_CP };
enum EntitiesRenderMode {WIREFRAME, DARK_SOLID, ILLUMINATED_SOLID, CPS_ONLY};
enum FloorRenderMode { GRID, SOLID, GRID_SOLID, NO_FLOOR };
enum EntityType { ABSTRACT, ROOT, OPERATION, CYLINDER, PRISM };

#endif // ENUMS_H
