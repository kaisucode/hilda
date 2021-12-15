#ifndef TREESHAPE_H
#define TREESHAPE_H

#include "shapes/OpenGLShape.h"

class TreeShape : public OpenGLShape
{
public:
    TreeShape();
    TreeShape(std::vector<GLfloat> *vertexData);
    ~TreeShape();
    void buildSides(std::vector<Vertex> circumferencevertices);
};

#endif // TREESHAPE_H


