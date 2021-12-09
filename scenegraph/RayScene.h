#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "GL/glew.h"
#include "Scene.h"
#include "IntersectPoint.h"
#include "RayShape/RayBaseShape.h"
#include "RayShape/RayCube.h"
#include "RayShape/RayCone.h"
#include "RayShape/RayCylinder.h"
#include "RayShape/RaySphere.h"
#include <memory>

#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();

	glm::vec4 getColor(glm::vec4 startingPoint, IntersectPoint intersectPt, int depth);
	glm::vec4 getNormalizedD(int j, int i, int W, int H, float thetaH, glm::vec4 eye);
	bool getClosestIntersection(IntersectPoint *intersectPt, glm::vec4 d, glm::vec4 pEye);

	int m_maxDepth = 5;
	float m_epsilon = 0.001f;
};

#endif // RAYSCENE_H
