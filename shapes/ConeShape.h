#ifndef CONESHAPE_H
#define CONESHAPE_H

#include "OpenGLShape.h"

class ConeShape : public OpenGLShape
{
public:
    ConeShape();
    ConeShape(int param1, int param2);
    ~ConeShape();
	void buildSides(std::vector<Vertex> circumferenceVertices);
};

#endif // CONESHAPE_H
