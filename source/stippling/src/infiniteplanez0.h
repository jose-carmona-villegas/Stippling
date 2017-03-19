#ifndef INFINITEPLANEZ0_H
#define INFINITEPLANEZ0_H

#include <infiniteplane.h>

class InfinitePlaneZ0 : public InfinitePlane
{
public:
    InfinitePlaneZ0();
    InfinitePlaneZ0(int id, glm::vec3 selectionColor, GLuint renderShader, GLuint pickingShader);
};

#endif // INFINITEPLANEZ0_H
