
#include "RayBaseShape.h"

RayBaseShape::RayBaseShape(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive)
{
	this->p = p;
	this->d = d;
	this->transformedPrimitive = transformedPrimitive;
	this->t = -1.0;
	this->n = glm::vec4(0);
	this->minIntersectPt = IntersectPoint();
}

RayBaseShape::~RayBaseShape()
{

}

bool RayBaseShape::setMinPositiveT(std::vector<IntersectPoint> intersectPoints) {

	if (intersectPoints.size() == 0) {
		return false;
	}

	// find the minimum positive value
	IntersectPoint validMin = intersectPoints.at(0);
	bool nothingFound = true;
	for (int i = 0; i < intersectPoints.size(); i++) {
		if (intersectPoints.at(i).t > 0 && (nothingFound || intersectPoints.at(i).t <= validMin.t)) {
			validMin = intersectPoints.at(i);
			nothingFound = false;
		}
	}

	if (!nothingFound) {
		this->minIntersectPt = validMin;
	}

	return !nothingFound;
}

bool RayBaseShape::pointOnCapIsValid(glm::vec4 ptOfIntersect, GLfloat r) {
	return (ptOfIntersect.x * ptOfIntersect.x + ptOfIntersect.z * ptOfIntersect.z <= r * r);
}

GLfloat RayBaseShape::getTForPlane(glm::vec4 pt, glm::vec4 d, glm::vec4 norm, glm::vec3 planePt) {
	GLfloat a = (norm.x * planePt.x + norm.y * planePt.y + norm.z * planePt.z);
	GLfloat b = (norm.x * pt.x + norm.y * pt.y + norm.z * pt.z);
	GLfloat c = (norm.x * d.x + norm.y * d.y + norm.z * d.z);

	GLfloat results = (a - b) / c;
	return results;
}


std::vector<IntersectPoint> RayBaseShape::getValidTsFromQuadratic(GLfloat A, GLfloat B, GLfloat C,\
		GLfloat r) {

	std::vector<IntersectPoint> intersectPoints;

	GLfloat D = B * B - 4 * A * C;
	if (D < 0) {
		return intersectPoints;
	}

	GLfloat t1 = ((-1.0) * B + glm::sqrt(D)) / (2 * A);
	GLfloat t2 = ((-1.0) * B - glm::sqrt(D)) / (2 * A);

	GLfloat y1 = p.y + d.y * t1;
	GLfloat y2 = p.y + d.y * t2;

	glm::vec2 uv = glm::vec2(0);

	// use epsilons to make the bounds a little extra and more forgiving
	if (!((y1 > r + EPSILON) || (y1 < -r - EPSILON))) {
		glm::vec4 pdt = p + d * t1;
		float theta = atan2(pdt.z, pdt.x);
		uv[0] = -theta / (2.0 * M_PI);
		uv[1] = 0.5 - pdt.y;

		if (theta >= 0) {
			uv[0] += 1;
		}

		addIntersectPoint(intersectPoints, t1, uv);
	} 
	if (!((y2 > r + EPSILON) || (y2 < -r - EPSILON))) {
		glm::vec4 pdt = p + d * t2;
		float theta = atan2(pdt.z, pdt.x);
		uv[0] = -theta / (2.0 * M_PI);
		uv[1] = 0.5 - pdt.y;

		if (theta >= 0) {
			uv[0] += 1;
		}
		addIntersectPoint(intersectPoints, t2, uv);
	} 

	return intersectPoints;
}

void RayBaseShape::addIntersectPoint(std::vector<IntersectPoint> &intersectPoints, GLfloat tValue, glm::vec2 uv) {
	glm::vec4 normalizedIntersectionPoint = glm::normalize(p + d * tValue);
	glm::vec4 surfaceNormal = getSurfaceNormals(normalizedIntersectionPoint);
	IntersectPoint anIntersectPt(p, d, tValue, surfaceNormal, transformedPrimitive, uv);
	intersectPoints.push_back(anIntersectPt);
}

// for cube
void RayBaseShape::addIntersectPoint(std::vector<IntersectPoint> &intersectPoints, GLfloat tValue, glm::vec4 surfaceNormal, glm::vec2 uv) {
	IntersectPoint anIntersectPt(p, d, tValue, surfaceNormal, transformedPrimitive, uv);
	intersectPoints.push_back(anIntersectPt);
}

glm::vec2 RayBaseShape::getPlaneUV(glm::vec4 ptOfIntersect, glm::vec4 n) {
	glm::vec2 uv(0);

	if (n.x == 1) {
		uv[0] = -ptOfIntersect.z;
		uv[1] = -ptOfIntersect.y;
	}
	else if (n.x == -1) {
		uv[0] = ptOfIntersect.z;
		uv[1] = -ptOfIntersect.y;
	}
	else if (n.y == 1) {
		uv[0] = ptOfIntersect.x;
		uv[1] = ptOfIntersect.z;
	}
	else if (n.y == -1) {
		uv[0] = ptOfIntersect.x;
		uv[1] = -ptOfIntersect.z;
	}
	else if (n.z == 1) {
		uv[0] = ptOfIntersect.x;
		uv[1] = -ptOfIntersect.y;
	}
	else if (n.z == -1) {
		uv[0] = -ptOfIntersect.x;
		uv[1] = -ptOfIntersect.y;
	}
	uv[0] += 0.5;
	uv[1] += 0.5;

	return uv;
}


