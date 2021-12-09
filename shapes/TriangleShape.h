#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include <vector>
#include "shapes/Vertex.h"

// #include<memory>
// #include <vector>

class TriangleShape
{
public: 
	TriangleShape();
	TriangleShape(Vertex p1, Vertex p2, Vertex p3);
	~TriangleShape();
	glm::vec3 getSurfaceNormal();

	std::vector<Vertex> v;

private: 
	// Vertex m_p1;
	// Vertex m_p2;
	// Vertex m_p3;
	glm::vec3 surface_normal;
};

#endif // TRIANGLESHAPE_H
