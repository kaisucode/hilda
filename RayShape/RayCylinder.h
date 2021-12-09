#ifndef RAYCYLINDER_H
#define RAYCYLINDER_H

#include "RayBaseShape.h"


class RayCylinder : public RayBaseShape 
{
public: 
	RayCylinder(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive);
	virtual ~RayCylinder();

	virtual bool findIntersection() override;
	virtual glm::vec4 getSurfaceNormals(glm::vec4 intersectPt) override;

};

#endif // RAYCYLINDER_H

