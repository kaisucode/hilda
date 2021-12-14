#ifndef TERRAINLAB_H
#define TERRAINLAB_H

#include "TerrainBase.h"

class TerrainLab : public TerrainBase
{
	public: 
	TerrainLab();
	TerrainLab(int param1, int param2);
	~TerrainLab();
private: 
	virtual glm::vec3 getPosition(int row, int col) override;
	virtual float randValue(int row, int col) override;
	virtual float getNoise(glm::vec3 x) override;

};

#endif // TERRAINLAB_H
