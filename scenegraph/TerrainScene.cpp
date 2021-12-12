#include "TerrainScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include <sstream>

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"

using namespace CS123::GL;

TerrainScene::TerrainScene() :
    m_sceneLight(),
    m_backgroundColor(0.8f, 0.93f, 0.96f)
{
    loadShaders();
    settingsChanged();
    setLight();
}

TerrainScene::~TerrainScene() {
    delete m_currentShader;
}

void TerrainScene::loadShaders() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/toon.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/toon.frag");
    m_toonShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
    vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void TerrainScene::render(SupportCanvas3D *context) {
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_currentShader->bind();

    if (settings.useToonShader) {
        setToonUniforms(context);
    }
    else {
        setPhongUniforms(context);
    }

    setMatrixUniforms(context);
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_currentShader->unbind();

}

void TerrainScene::setLight() {
    memset(&m_sceneLight, 0, sizeof(m_sceneLight));
    m_sceneLight.type = LightType::LIGHT_DIRECTIONAL;
    m_sceneLight.color.r = m_sceneLight.color.g = m_sceneLight.color.b = 1;
    m_sceneLight.id = 0;
    m_sceneLight.dir = glm::normalize(glm::vec4(-1, -1, -1, 0));
    m_phongShader->setLight(m_sceneLight);
}

void TerrainScene::setMatrixUniforms(SupportCanvas3D *context) {
    m_currentShader->setUniform("p", context->getCamera()->getProjectionMatrix());
    m_currentShader->setUniform("v", context->getCamera()->getViewMatrix());
}

void TerrainScene::setPhongUniforms(SupportCanvas3D *context) {
    m_phongShader->setUniform("ka", m_global.ka);
    m_phongShader->setUniform("kd", m_global.kd);
    m_phongShader->setUniform("ks", m_global.ks);
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("isShapeScene", false);
}

void TerrainScene::setToonUniforms(SupportCanvas3D *context) {
    CamtransCamera *camera = context->getCamtransCamera();
    m_toonShader->setUniform("WS_camPosition", camera->getPosition());
    m_toonShader->setUniform("backgroundColor", m_backgroundColor);
    m_toonShader->setUniform("lightDir", m_sceneLight.dir);
}

void TerrainScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (settings.useToonShader) {
        m_toonShader->setUniform("shadowTint", glm::vec4(0.054, 0.058, 0.384, 0.2));
        m_toonShader->setUniform("highlightTint", glm::vec4(0.984, 0.792, 0.439, 0.2));
        m_toonShader->setUniform("objectColor", glm::vec3(0.3, 0.6, 0.4));
        m_toonShader->setUniform("outlineColor", glm::vec3(0.2));
    }
    else {
        CS123SceneMaterial material;
        material.cAmbient.r = 0.2f;
        material.cAmbient.g = 0.1f;
        material.cDiffuse.r = 1.0f;
        material.cDiffuse.g = 0.5f;
        material.cSpecular.r = material.cSpecular.g = material.cSpecular.b = 1;
        material.shininess = 64;
        m_phongShader->applyMaterial(material);
    }

    m_currentShader->setUniform("m", glm::mat4(1));
    m_terrain->draw();

    return;
}

void TerrainScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.

    int param1 = settings.shapeParameter1;
    int param2 = settings.shapeParameter2;

    if (m_should_use_LOD && this->m_transformedPrimitives.size() > m_LOD_count_threshold) {
        param1 = std::min(param1, 10);
        param2 = std::min(param2, 10);
    }

    m_terrain = std::make_unique<TerrainShape>(param1, param2);

    if (settings.useToonShader) {
        m_currentShader = m_toonShader.get();
    }
    else {
        m_currentShader = m_phongShader.get();
    }
}
