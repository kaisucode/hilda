#ifndef RAY_H
#define RAY_H

#include "Scene.h"
#include "glm/glm.hpp"
#include "GL/glew.h"

#include <vector>


class IntersectPoint {
public:
	IntersectPoint();
	IntersectPoint(glm::vec4 p, glm::vec4 d, GLfloat t, glm::vec4 n);
	IntersectPoint(glm::vec4 p, glm::vec4 d, GLfloat t, glm::vec4 n, TransformedPrimitive transformedPrimitive);
	IntersectPoint(glm::vec4 p, glm::vec4 d, GLfloat t, glm::vec4 n,\
			TransformedPrimitive transformedPrimitive, glm::vec2 uv);
    ~IntersectPoint();

	glm::vec4 getPointOfIntersection();
	glm::vec4 getPointOfIntersectionWorldSpace();
	glm::vec4 getNormalWorldSpace();
	glm::vec2 getST();
	glm::vec4 getTextureColor();

	glm::vec4 p;
	glm::vec4 d;
	glm::vec4 n;
	GLfloat t;
	TransformedPrimitive transformedPrimitive;
	bool isValid;
	glm::vec2 uv;
};

#endif // RAY_H
