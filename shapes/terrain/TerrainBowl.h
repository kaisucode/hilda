#ifndef TERRAINBOWL_H
#define TERRAINBOWL_H

#include "TerrainBase.h"

class TerrainBowl : public TerrainBase
{
	public: 
	TerrainBowl();
	TerrainBowl(int param1, int param2);
	~TerrainBowl();
private: 
	virtual glm::vec3 getPosition(int row, int col) override;
	virtual float randValue(int row, int col) override;
	virtual float getNoise(glm::vec3 x) override;

};

#endif // TERRAINBOWL_H
