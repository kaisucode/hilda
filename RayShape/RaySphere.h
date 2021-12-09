#ifndef RAYSPHERE_H
#define RAYSPHERE_H

#include "RayBaseShape.h"


class RaySphere : public RayBaseShape 
{
public: 
	RaySphere(glm::vec4 p, glm::vec4 d, TransformedPrimitive transformedPrimitive);
	virtual ~RaySphere();

	virtual bool findIntersection() override;
	virtual glm::vec4 getSurfaceNormals(glm::vec4 intersectPt) override;

};

#endif // RAYSPHERE_H

