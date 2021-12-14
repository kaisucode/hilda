#ifndef TERRAINCLIFF_H
#define TERRAINCLIFF_H

#include "TerrainBase.h"


class TerrainCliff : public TerrainBase
{
public:
    TerrainCliff();
    TerrainCliff(int param1, int param2);
    ~TerrainCliff();

private: 
	virtual glm::vec3 getPosition(int row, int col) override;
	virtual float randValue(int row, int col) override;
	virtual float getNoise(glm::vec3 x) override;
};

#endif // TERRAINCLIFF_H
