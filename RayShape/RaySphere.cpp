#include "RaySphere.h"

RaySphere::RaySphere(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive): 
	RayBaseShape(p, d, transformedPrimitive)
{
}

RaySphere::~RaySphere()
{
}

bool RaySphere::findIntersection() {

	GLfloat A = (d.x * d.x + d.y * d.y + d.z * d.z);
	GLfloat B = 2.0 * (p.x * d.x + p.y * d.y + p.z * d.z);
	GLfloat C = (p.x * p.x + p.y * p.y + p.z * p.z - 0.5 * 0.5);

	std::vector<IntersectPoint> intersectPoints = getValidTsFromQuadratic(A, B, C, 0.5);

	for (int i = 0; i < intersectPoints.size(); i++) {

		glm::vec4 pdt = intersectPoints[i].getPointOfIntersection();

		float r = 0.5f;
		float phi = asin(pdt.y / r);
		float v = (0.5f - (phi / M_PI));

		// reassign u, v
		if (v == 0.0f || v == 1.0f) {
			intersectPoints[i].uv[0] = 0.5f;
		}
		intersectPoints[i].uv[1] = v;
	}

	return setMinPositiveT(intersectPoints);
}

glm::vec4 RaySphere::getSurfaceNormals(glm::vec4 intersectPt) {
	glm::vec4 surfaceNormal = glm::vec4(\
			2 * intersectPt.x,\
			2 * intersectPt.y, \
			2 * intersectPt.z, 0);
	return surfaceNormal;
}

