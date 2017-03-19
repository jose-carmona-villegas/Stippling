#ifndef INFINITEPLANEY0_H
#define INFINITEPLANEY0_H

#include <infiniteplane.h>

// The infinite plane is already a Y=0 plane. Subclassed only for clarity's sake (when using other subclasses).
class InfinitePlaneY0 : public InfinitePlane
{
public:
    InfinitePlaneY0();
    InfinitePlaneY0(int id, glm::vec3 selectionColor, GLuint renderShader, GLuint pickingShader);
};

#endif // INFINITEPLANEY0_H
