#ifndef RAYCONE_H
#define RAYCONE_H

#include "RayBaseShape.h"


class RayCone : public RayBaseShape 
{
public: 
	RayCone(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive);
	virtual ~RayCone();

	virtual bool findIntersection() override;
	virtual glm::vec4 getSurfaceNormals(glm::vec4 intersectPt) override;

};

#endif // RAYCONE_H

