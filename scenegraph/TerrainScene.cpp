#include "TerrainScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include <sstream>
#include <set>
#include <random>
#include <iostream>
#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/TreeShape.cpp"
#include <glm/gtx/transform.hpp>

using namespace CS123::GL;

TerrainScene::TerrainScene() :
    m_terrain(std::make_unique<TerrainShape>(settings.shapeParameter1, settings.shapeParameter2)),
    m_sceneLight(),
    m_backgroundColor(0.8f, 0.93f, 0.96f)
{
    loadShaders();
    settingsChanged();
}

TerrainScene::~TerrainScene() {
}

void TerrainScene::loadShaders() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/toon.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/toon.frag");
    m_toonShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void TerrainScene::render(SupportCanvas3D *context) {
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_toonShader->bind();
    setLight();
    setToonUniforms();
    setMatrixUniforms(context);
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_toonShader->unbind();
}

void TerrainScene::setLight() {
    float sunPercent = (glm::clamp(settings.timeOfDay, 6.f, 18.f) - 6.f) / 12.f;
    float sunAngle = sunPercent * M_PI;

    m_sceneLight.dir = glm::rotate(sunAngle, glm::vec3(0, 0, 1)) * glm::vec4(1, 0, 0.25, 0);
    m_toonShader->setUniform("lightDir", m_sceneLight.dir);

    m_backgroundColor =  glm::mix(glm::vec3(0.56, 0.79, 0.94), glm::vec3(0.94, 0.8, 0.55), std::abs(1.f - 2 * sunPercent));
    m_toonShader->setUniform("backgroundColor", m_backgroundColor);
}

void TerrainScene::setMatrixUniforms(SupportCanvas3D *context) {
    CamtransCamera *camera = context->getCamtransCamera();
    m_toonShader->setUniform("WS_camPosition", camera->getPosition());
    m_toonShader->setUniform("p", camera->getProjectionMatrix());
    m_toonShader->setUniform("v", camera->getViewMatrix());
}

void TerrainScene::setToonUniforms() {
    m_toonShader->setUniform("shadowTint", glm::vec4(0.054, 0.058, 0.384, 0.2));
    m_toonShader->setUniform("highlightTint", glm::vec4(0.992, 0.878, 0.666, 0.2));
    m_toonShader->setUniform("outlineColor", glm::vec3(0.2));
}

void TerrainScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_toonShader->setUniform("m", glm::mat4(1));
//    std::unique_ptr<TreeShape> tree = std::make_unique<TreeShape>();
//    tree->draw();
//    m_toonShader->setUniform("objectColor", glm::vec3(0.3, 0.6, 0.4));
    m_toonShader->setUniform("terrain", true);
    m_toonShader->setUniform("m", glm::mat4(1));
    m_terrain->draw();
    drawTrees();



    return;
}

void TerrainScene::drawTrees() {
//    std::set<int> randIndices = this->generateRandIndices();
    glm::vec3 baseTree = {0, -0.5, 0};
    std::unique_ptr<TreeShape> tree = std::make_unique<TreeShape>();
 for (int vertexIndex:m_randIndices) {
     glm::vec3 location = m_terrain->getVertexAtIndex(vertexIndex);
     glm::vec3 translation = location - baseTree;
     glm::mat4x4 translationMatrix = glm::translate(translation);
     m_toonShader->setUniform("m", translationMatrix);
     m_toonShader->setUniform("terrain", false);
     m_toonShader->setUniform("objectColor", glm::vec3(0.8, 0.517, 0.141));
     tree->draw();

 }

}

/**
 * @brief TerrainScene::generateRandIndices -> NOT USING THIS ANYWHERE; but this generates
 * more random indices, but decided to generate pseudo random indices like the terrain
 * @return
 */
std::set<int> TerrainScene::generateRandIndices() {
    std::set<int> randIndices;
    std::random_device dev;
    std::mt19937 rng(dev());
    int numVertices = m_terrain->getVertexDataSize();
    int maxIndex = numVertices / 6;
    std::cout << "numVertices "<< numVertices << std::endl;
    std::cout << "index: "<< maxIndex << std::endl;
    while (randIndices.size() < m_numTrees) {
         std::uniform_int_distribution<std::mt19937::result_type> distRand(1,maxIndex); // distribution in range [1, 6]
         int index = distRand(rng);
         randIndices.insert(index);
         std::cout << index << std::endl;
    //     if (index % 2 == 0) {// check if even because odd indices are normals
    //         randIndices.insert(index);
    //         std::cout << index << std::endl;
    //     }
     }
     return randIndices;
}

std::set<int> TerrainScene::generatePseudoRandIndices() {
    std::set<int> randIndices;
    int numVertices = m_terrain->getVertexDataSize();
    int maxIndex = numVertices / 6;
    std::cout << "numVertices "<< numVertices << std::endl;
    std::cout << "index: "<< maxIndex << std::endl;
//index: 179998 //make 100 trees the max or something
    for (int i=0; i<m_numTrees; i++) {
         int index = 179 * (((i-1)*3 + 37)%101);
         if(index < maxIndex) {
             randIndices.insert(index);
             std::cout << index << std::endl;
         }
     }
     return randIndices;
}

void TerrainScene::settingsChanged() {
    m_randIndices = this->generatePseudoRandIndices();
    setToonUniforms();
    setLight();
}
