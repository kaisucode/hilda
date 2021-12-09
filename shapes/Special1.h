#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "OpenGLShape.h"

class SphereShape : public OpenGLShape
{
public:
    SphereShape();
    SphereShape(int param1, int param2);
    ~SphereShape();
	void buildSides(std::vector<Vertex> circumferenceVertices);
};

#endif // SPHERESHAPE_H
