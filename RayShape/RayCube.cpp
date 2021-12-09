#include "RayCube.h"

RayCube::RayCube(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive): 
	RayBaseShape(p, d, transformedPrimitive)
{
}

RayCube::~RayCube()
{
}

bool RayCube::findIntersection() {

	std::vector<IntersectPoint> intersectPoints;

	std::vector<glm::vec4> norms = { 
		glm::vec4(0, 1, 0, 0), 
		glm::vec4(0, -1, 0, 0), 
		glm::vec4(1, 0, 0, 0), 
		glm::vec4(-1, 0, 0, 0), 
		glm::vec4(0, 0, 1, 0), 
		glm::vec4(0, 0, -1, 0), 
	};

	std::vector<glm::vec3> planePts = { 
		glm::vec3(0, 0.5, 0), 
		glm::vec3(0, -0.5, 0), 
		glm::vec3(0.5, 0, 0), 
		glm::vec3(-0.5, 0, 0), 
		glm::vec3(0, 0, 0.5), 
		glm::vec3(0, 0, -0.5), 
	};

	for (int i = 0; i < norms.size(); i++) {
		GLfloat aTValue = getTForPlane(p, d, norms.at(i), planePts.at(i));

		glm::vec4 ptOfIntersect = p + d * aTValue;

		bool onX = std::abs(norms.at(i).x) &&
			ptOfIntersect.y <= 0.5 && ptOfIntersect.y >= -0.5 &&
			ptOfIntersect.z <= 0.5 && ptOfIntersect.z >= -0.5;

		bool onY = std::abs(norms.at(i).y) &&
			ptOfIntersect.x <= 0.5 && ptOfIntersect.x >= -0.5 &&
			ptOfIntersect.z <= 0.5 && ptOfIntersect.z >= -0.5;

		bool onZ = std::abs(norms.at(i).z) &&
			ptOfIntersect.x <= 0.5 && ptOfIntersect.x >= -0.5 &&
			ptOfIntersect.y <= 0.5 && ptOfIntersect.y >= -0.5;

		if (aTValue > 0 && onX || onY || onZ) {
			// addIntersectPoint(intersectPoints, aTValue, norms.at(i));
			glm::vec2 uv = getPlaneUV(ptOfIntersect, norms.at(i));
			addIntersectPoint(intersectPoints, aTValue, norms.at(i), uv);
		}
	}

	return setMinPositiveT(intersectPoints);
}

