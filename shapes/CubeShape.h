#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "OpenGLShape.h"

class CubeShape : public OpenGLShape
{
public:
    CubeShape();
    CubeShape(int param1, int param2);
    ~CubeShape();

private: 
	Vertex leftTopFront = Vertex(-0.5f, 0.5f, 0.5f);
	Vertex rightTopFront = Vertex(0.5f, 0.5f, 0.5f);
	Vertex leftBottomFront = Vertex(-0.5f, -0.5f, 0.5f);
	Vertex rightBottomFront = Vertex(0.5f, -0.5f, 0.5f);

	Vertex leftTopBack = Vertex(-0.5f, 0.5f, -0.5f);
	Vertex rightTopBack = Vertex(0.5f, 0.5f, -0.5f);
	Vertex leftBottomBack = Vertex(-0.5f, -0.5f, -0.5f);
	Vertex rightBottomBack = Vertex(0.5f, -0.5f, -0.5f);

	std::vector<std::vector<Vertex>> cubeFaces = {
		{ leftTopFront, rightBottomFront, leftBottomFront, rightBottomFront },	// front face
		{ rightBottomBack, leftTopBack, rightTopBack, leftBottomBack },			// back face 
		{ leftTopBack, leftTopFront, leftTopBack, rightTopFront },				// top face
		{ leftBottomBack },					// bottom face
		{ leftTopBack },					// left face
		{ rightBottomFront },				// right face
	};

	std::vector<glm::vec3> cubeFaceNormals = {
		{ 0.f, 0.f, 1.f },	// front face
		{ 0.f, 0.f, -1.f },	// back face
		{ 0.f, 1.f, 0.f },	// top face
		{ 0.f, -1.f, 0.f },	// bottom face
		{ -1.f, 0.f, 0.f },	// left face
		{ 1.f, 0.f, 0.f },	// left face
	};

	std::vector<std::vector<int>> indexOrdering = {
		{ 0, 1, 2 }, // front
		{ 1, 0, 2 }, // back
		{ 1, 2, 0 }, // top
		{ 0, 2, 1 }, // bottom
		{ 2, 1, 0 }, // left
		{ 2, 0, 1 }, // right
	};
};

#endif // CUBESHAPE_H
