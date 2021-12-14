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

/**
 * @brief TerrainScene::TerrainScene scene used for final project to display terrain generation with trees
 */
TerrainScene::TerrainScene() :
    m_terrain(std::make_unique<TerrainLab>(settings.shapeParameter1, settings.shapeParameter2)),
    terrainType(TERRAIN_LAB),
    m_sceneLight(),
    m_backgroundColor(0.8f, 0.93f, 0.96f),
    m_numTrees(settings.numberOfTrees),
    m_randIndices()
{

    settings.terrainType = this->terrainType;

    loadShaders();
    m_randIndices.reserve(settings.maxTreeNum);
    generatePseudoRandIndices();
    settingsChanged();
}

TerrainScene::~TerrainScene() {
}

/**
 * @brief TerrainScene::loadShaders loads shader files
 */
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
    setCameraUniforms(context);
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_toonShader->unbind();
}

/**
 * @brief TerrainScene::setLight updates light angle and background color tint according to time slider
 */
void TerrainScene::setLight() {
    float sunPercent = (glm::clamp(settings.timeOfDay, 6.f, 18.f) - 6.f) / 12.f;
    float sunAngle = sunPercent * M_PI;

    m_sceneLight.dir = glm::rotate(sunAngle, glm::vec3(0, 0, 1)) * glm::vec4(1, 0, 0.25, 0);
    m_toonShader->setUniform("lightDir", m_sceneLight.dir);

    m_backgroundColor =  glm::mix(glm::vec3(0.56, 0.79, 0.94), glm::vec3(0.94, 0.8, 0.55), std::abs(1.f - 2 * sunPercent));
    m_toonShader->setUniform("backgroundColor", m_backgroundColor);
}

/**
 * @brief TerrainScene::setCameraUniforms sets projection and view matrices and position of camera
 * @param context
 */
void TerrainScene::setCameraUniforms(SupportCanvas3D *context) {
	CamtransCamera *camera = context->getCamtransCamera();
	m_toonShader->setUniform("WS_camPosition", camera->getPosition());
    m_toonShader->setUniform("p", context->getCamera()->getProjectionMatrix());
    m_toonShader->setUniform("v", context->getCamera()->getViewMatrix());
}


void TerrainScene::setToonUniforms() {
    m_toonShader->setUniform("shadowTint", glm::vec4(0.054, 0.058, 0.384, 0.2));
    m_toonShader->setUniform("highlightTint", glm::vec4(0.992, 0.878, 0.666, 0.2));
    m_toonShader->setUniform("useOutlines", settings.useOutlines);
    m_toonShader->setUniform("outlineColor", glm::vec3(0.2));
    m_toonShader->setUniform("outlineWeight", settings.outlineWeight);
}

void TerrainScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    drawTerrain();
    drawTrees();

    return;
}

void TerrainScene::drawTerrain() {
    m_toonShader->setUniform("m", glm::mat4(1.0f));
    m_toonShader->setUniform("terrain", true);
    m_terrain->draw();
}

void TerrainScene::drawTrees() {
    float scaleFactor = 0.25f;
    glm::vec3 baseTree = {0, -scaleFactor * 0.5f, 0};
    std::unique_ptr<TreeShape> tree = std::make_unique<TreeShape>();

    m_toonShader->setUniform("terrain", false);
    m_toonShader->setUniform("objectColor", glm::vec3(0.090, 0.368, 0.098));
    float treeOutline = 2.f * scaleFactor * settings.outlineWeight;
    m_toonShader->setUniform("outlineWeight", treeOutline);
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(scaleFactor));

    for (int i = 0; i < m_numTrees; i++) {
        int vertexIndex = m_randIndices[i];
        glm::vec3 location = m_terrain->getVertexAtIndex(vertexIndex);
        glm::vec3 translation = location - baseTree;
        glm::mat4x4 modelMatrix = glm::translate(translation) * scaleMatrix;
        m_toonShader->setUniform("m", modelMatrix);
        tree->draw();
    }

}

void TerrainScene::generatePseudoRandIndices() {
    m_randIndices.clear();
    int numVertices = m_terrain->getVertexDataSize();
    int maxIndex = numVertices / 6;
//index: 179998 //make 100 trees the max or something
    for (int i=1; i<settings.maxTreeNum+1; i++) {
         int index = (6 * ((((17999 % i)*16932) + 3731)))%numVertices;
         if(index < numVertices) {
             m_randIndices.push_back(index);
         }
     }
}

void TerrainScene::settingsChanged() {
    setToonUniforms();
    setLight();
    if (this->terrainType != settings.terrainType) {
		// update terrain
		switch (settings.terrainType) {
			case TERRAIN_LAB:
				m_terrain = std::make_unique<TerrainLab>(settings.shapeParameter1, settings.shapeParameter2);
				break;
			case TERRAIN_LAKE:
				m_terrain = std::make_unique<TerrainBowl>(settings.shapeParameter1, settings.shapeParameter2);
				break;
			case TERRAIN_CLIFF:
				m_terrain = std::make_unique<TerrainCliff>(settings.shapeParameter1, settings.shapeParameter2);
				break;
			default:
				m_terrain = std::make_unique<TerrainBowl>(settings.shapeParameter1, settings.shapeParameter2);
        }

        this->terrainType = settings.terrainType;
        generatePseudoRandIndices();
	}
    if (this->m_numTrees != settings.numberOfTrees) {
        m_numTrees = settings.numberOfTrees;
    }

}
