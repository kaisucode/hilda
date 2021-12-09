#include "RayCone.h"

RayCone::RayCone(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive): 
	RayBaseShape(p, d, transformedPrimitive)
{
}

RayCone::~RayCone()
{
}

bool RayCone::findIntersection() {

	GLfloat A = (d.x * d.x + d.z * d.z - 0.25 * d.y * d.y);
	GLfloat B = 2.0 * (p.x * d.x + p.z * d.z) - 0.5 * p.y * d.y + 0.25 * d.y;
	GLfloat C = (p.x * p.x + p.z * p.z - 0.25 * (p.y * p.y - p.y) - 0.25 * 0.25);

	// check cone sides
	std::vector<IntersectPoint> intersectPoints = getValidTsFromQuadratic(A, B, C, 0.5);

	// check for t4, intersection with bottom cap
	GLfloat t4 = ((-0.5) - p.y) / d.y;
	if (pointOnCapIsValid((p + d * t4), 0.5)) {
		glm::vec4 n = glm::vec4(0, -1, 0, 0);
		glm::vec2 uv = getPlaneUV(p + d * t4, n);
		addIntersectPoint(intersectPoints, t4, n, uv);
	}

	return setMinPositiveT(intersectPoints);
}


glm::vec4 RayCone::getSurfaceNormals(glm::vec4 intersectPt) {

	glm::vec4 surfaceNormal = glm::vec4(\
			2 * intersectPt.x,\
			-0.5f * intersectPt.y + 0.25f,\
			2 * intersectPt.z, 0);

	return surfaceNormal;
}




