#ifndef TERRAINSCENE_H
#define TERRAINSCENE_H

#include "OpenGLScene.h"
#include "shapes/TerrainShape.h"
#include "shapes/ExampleShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/ConeShape.h"
#include "shapes/SphereShape.h"

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

    void setPhongUniforms(SupportCanvas3D *context);
    void setToonUniforms(SupportCanvas3D *context);
    void drawTrees();

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<CS123::GL::CS123Shader> m_toonShader;
    CS123::GL::CS123Shader *m_currentShader;

    std::unique_ptr<TerrainShape> m_terrain;

    CS123SceneLightData m_sceneLight;

    bool m_should_use_LOD = true;
    int m_LOD_count_threshold = 3;

    glm::vec3 m_backgroundColor;
    int m_numTrees = 1;
};

#endif // TERRAINSCENE_H
