#ifndef RAYCUBE_H
#define RAYCUBE_H

#include "RayBaseShape.h"


class RayCube : public RayBaseShape 
{
public: 
	RayCube(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive);
	virtual ~RayCube();

	virtual bool findIntersection() override;
	virtual glm::vec4 getSurfaceNormals(glm::vec4 intersectPt) override { return glm::vec4(0); };
};

#endif // RAYCUBE_H

