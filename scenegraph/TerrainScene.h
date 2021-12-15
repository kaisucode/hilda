#ifndef TERRAINSCENE_H
#define TERRAINSCENE_H

#include "OpenGLScene.h"
// #include "shapes/TerrainShape.h"

#include "shapes/terrain/TerrainBase.h"
#include "shapes/terrain/TerrainBowl.h"
#include "shapes/terrain/TerrainCliff.h"
#include "shapes/terrain/TerrainLab.h"
#include "shapes/terrain/BasicTree.h"

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
    void setCameraUniforms(SupportCanvas3D *context);
    void renderGeometry();

    void setToonUniforms();
    void drawTerrain();
    void drawTrees();
    void generatePseudoRandIndices();

    std::unique_ptr<CS123::GL::CS123Shader> m_toonShader;

    std::unique_ptr<TerrainBase> m_terrain;
    std::unique_ptr<BasicTree> m_tree;

    glm::vec4 m_sceneLight;

    glm::vec3 m_backgroundColor;
    int m_numTrees;
    std::vector<glm::vec3> m_randVertices;
    std::uniform_int_distribution<std::mt19937::result_type> m_distRand();

    int m_terrainType;

    float m_treeScale;
};

#endif // TERRAINSCENE_H
