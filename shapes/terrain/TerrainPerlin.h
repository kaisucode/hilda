#ifndef TERRAINPERLIN_H
#define TERRAINPERLIN_H

#include "TerrainBase.h"


class TerrainPerlin : public TerrainBase
{
public:
    TerrainPerlin();
    TerrainPerlin(int param1, int param2);
    ~TerrainPerlin();

	const float m_centerRadius = 2;

private: 
	virtual glm::vec3 getPosition(int row, int col) override;
	virtual float randValue(int row, int col) override;
	virtual float getNoise(glm::vec3 x) override;

	float hash(glm::vec3 p);
	float noised(glm::vec3 x);
};

#endif // TERRAINPERLIN_H
