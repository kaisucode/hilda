#ifndef RayBaseShape_H
#define RayBaseShape_H

#include "Scene.h"
#include "IntersectPoint.h"
#include "GL/glew.h"
#include "CS123SceneData.h"

#include <iostream>
#include <cmath>
#include <vector>

class RayBaseShape {
public: 
	RayBaseShape(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive);
	~RayBaseShape();

	virtual glm::vec4 getSurfaceNormals(glm::vec4 intersectPt) { return glm::vec4(1); };
	virtual bool findIntersection() { return false; };

	GLfloat getTForPlane(glm::vec4 pt, glm::vec4 d, glm::vec4 norm, glm::vec3 planePt);
	bool setMinPositiveT(std::vector<IntersectPoint> intersectPoints);

	bool pointOnCapIsValid(glm::vec4 ptOfIntersect, GLfloat r);
	std::vector<IntersectPoint> getValidTsFromQuadratic(GLfloat A, GLfloat B, GLfloat C,\
			GLfloat r);
	void addIntersectPoint(std::vector<IntersectPoint> &intersectPoints, GLfloat tValue, glm::vec2 uv);
	void addIntersectPoint(std::vector<IntersectPoint> &intersectPoints, GLfloat tValue, glm::vec4 surfaceNormal, glm::vec2 uv);

	glm::vec2 getPlaneUV(glm::vec4 ptOfIntersect, glm::vec4 n);

	glm::vec4 p;
	glm::vec4 d;
	TransformedPrimitive transformedPrimitive;
	GLfloat t;
	glm::vec4 n;

	IntersectPoint minIntersectPt;

	GLfloat EPSILON = 0.00001;
};


#endif // RayBaseShape_H

