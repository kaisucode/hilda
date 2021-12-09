#include "TriangleShape.h"

TriangleShape::TriangleShape()
{
}

TriangleShape::TriangleShape(Vertex p1, Vertex p2, Vertex p3) 
{
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
}

TriangleShape::~TriangleShape()
{
}

glm::vec3 TriangleShape::getSurfaceNormal() 
{
	glm::vec3 u = this->v.at(1).pos - this->v.at(0).pos;
	glm::vec3 v = this->v.at(2).pos - this->v.at(0).pos;
	
	GLfloat normalX = u.y * v.z - u.z * v.y;
	GLfloat normalY = u.z * v.x - u.x * v.z;
	GLfloat normalZ = u.x * v.y - u.y * v.x;

	glm::vec3 normalized(normalX, normalY, normalZ);

	return normalized;
}

