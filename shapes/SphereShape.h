#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "OpenGLShape.h"

class SphereShape : public OpenGLShape
{
public:
    SphereShape();
    SphereShape(int param1, int param2);
    ~SphereShape();
	void buildSides();
	Vertex getSphereCoordinates(GLfloat phi, GLfloat theta);
};

#endif // SPHERESHAPE_H
