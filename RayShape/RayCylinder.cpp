#include "RayCylinder.h"

RayCylinder::RayCylinder(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive): 
	RayBaseShape(p, d, transformedPrimitive)
{
}

RayCylinder::~RayCylinder()
{
}

bool RayCylinder::findIntersection() {

	// check with infinite cylinder
	GLfloat A = (d.x * d.x + d.z * d.z);
	GLfloat B = 2.0 * (p.x * d.x + p.z * d.z);
	GLfloat C = (p.x * p.x + p.z * p.z - 0.5 * 0.5);

	// check cone sides
	// std::vector<std::pair<GLfloat, glm::vec4>> tValuesAndNormals = getValidTsFromQuadratic(A, B, C, 0.5);
	std::vector<IntersectPoint> intersectPoints = getValidTsFromQuadratic(A, B, C, 0.5);

	// check for t3, intersection with top cap
	GLfloat t3 = ((0.5) - p.y) / d.y;
	if (pointOnCapIsValid((p + d * t3), 0.5)) {
		glm::vec4 n = glm::vec4(0, 1, 0, 0);
		glm::vec2 uv = getPlaneUV(p + d * t3, n);
		addIntersectPoint(intersectPoints, t3, n, uv);
	}

	// check for t4, intersection with bottom cap
	GLfloat t4 = ((-0.5) - p.y) / d.y;
	if (pointOnCapIsValid((p + d * t4), 0.5)) {
		glm::vec4 n = glm::vec4(0, -1, 0, 0);
		glm::vec2 uv = getPlaneUV(p + d * t4, n);
		addIntersectPoint(intersectPoints, t4, n, uv);
	}
	
	return setMinPositiveT(intersectPoints);
}

glm::vec4 RayCylinder::getSurfaceNormals(glm::vec4 intersectPt) {

	glm::vec4 surfaceNormal = glm::vec4(\
			2 * intersectPt.x, 0,\
			2 * intersectPt.z, 0);
	return surfaceNormal;
}


