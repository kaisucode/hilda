#ifndef TERRAINSCENE_H
#define TERRAINSCENE_H

#include "OpenGLScene.h"
// #include "shapes/TerrainShape.h"

#include "shapes/terrain/TerrainBase.h"
#include "shapes/terrain/TerrainBowl.h"
#include "shapes/terrain/TerrainCliff.h"
#include "shapes/terrain/TerrainLab.h"

#include "shapes/ExampleShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/ConeShape.h"
#include "shapes/SphereShape.h"
#include <set>
#include <random>
#include <iostream>

namespace CS123 { namespace GL {

    class Shader;
    class CS123Shader;
    class Texture2D;
}}

class TerrainScene : public OpenGLScene
{
public:
    TerrainScene();
    virtual ~TerrainScene();

    virtual void render(SupportCanvas3D *context) override;
    virtual void settingsChanged() override;

private:
    void setLight();

    void loadShaders();
    void setMatrixUniforms(SupportCanvas3D *context);
    void renderGeometry();

    void setToonUniforms();
    void drawTrees();
    std::set<int> generateRandIndices();
    std::set<int> generatePseudoRandIndices();

    std::unique_ptr<CS123::GL::CS123Shader> m_toonShader;

    std::unique_ptr<TerrainBase> m_terrain;

    CS123SceneLightData m_sceneLight;

    bool m_should_use_LOD = true;
    int m_LOD_count_threshold = 3;

    glm::vec3 m_backgroundColor;
    int m_numTrees = 10;
    std::set<int> m_randIndices;

	int terrainType;
};

#endif // TERRAINSCENE_H
