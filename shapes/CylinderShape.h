#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "OpenGLShape.h"

class CylinderShape : public OpenGLShape
{
public:
    CylinderShape();
    CylinderShape(int param1, int param2);
    ~CylinderShape();
	void buildSides(std::vector<Vertex> circumferenceVertices);
};

#endif // CYLINDERSHAPE_H
