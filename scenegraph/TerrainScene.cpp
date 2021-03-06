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
#include <glm/gtx/transform.hpp>

using namespace CS123::GL;

/**
 * @brief TerrainScene::TerrainScene scene used for final project to display terrain generation with trees
 */
TerrainScene::TerrainScene() :
    m_terrain(std::make_unique<TerrainLab>(settings.shapeParameter1, settings.shapeParameter2)),
    m_tree(std::make_unique<BasicTree>()),
    m_skyColorOne(0.56, 0.79, 0.94),
    m_skyColorTwo(0.94, 0.8, 0.55),
    m_backgroundColor(),
    m_shadowTint(0.054, 0.058, 0.384, 0.2),
    m_highlightTint(0.992, 0.878, 0.666, 0.2),
    m_outlineColor(0.2),
    m_numTrees(settings.numberOfTrees),
    m_randVertices(),
    m_terrainType(settings.terrainType),
    m_treeScale(0.25f)
{
    loadShaders();
    m_randVertices.reserve(settings.maxTreeNum);
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

    m_lightDir = glm::rotate(sunAngle, glm::vec3(0, 0, 1)) * glm::vec4(1, 0, 0.2, 0);
    m_toonShader->setUniform("lightDir", m_lightDir);

    m_backgroundColor =  glm::mix(m_skyColorOne, m_skyColorTwo, std::abs(1.f - 2 * sunPercent));
    m_toonShader->setUniform("backgroundColor", m_backgroundColor);
}

/**
 * @brief TerrainScene::setCameraUniforms sets projection and view matrices and position of camera
 * @param context
 */
void TerrainScene::setCameraUniforms(SupportCanvas3D *context) {
	CamtransCamera *camera = context->getCamtransCamera();
	m_toonShader->setUniform("WS_camPosition", camera->getPosition());
    m_toonShader->setUniform("p", camera->getProjectionMatrix());
    m_toonShader->setUniform("v", camera->getViewMatrix());
}


void TerrainScene::setToonUniforms() {
    m_toonShader->setUniform("shadowTint", m_shadowTint);
    m_toonShader->setUniform("highlightTint", m_highlightTint);
    m_toonShader->setUniform("useOutlines", settings.useOutlines);
    m_toonShader->setUniform("outlineColor", m_outlineColor);
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
    std::unique_ptr<BasicTree> tree = std::make_unique<BasicTree>();

    for (int i = 0; i < m_numTrees; i++) {
        tree->draw(m_toonShader, m_randVertices[i]);
    }
}

void TerrainScene::generatePseudoRandIndices() {
    m_randVertices.clear();
    int numVertices = m_terrain->getVertexDataSize();
    int i = 0;
    int numTrees = 0;
    while (numTrees < settings.maxTreeNum) {
        i++;
        int index = (6 * ((((17999 % i)*16932) + 3731)))%numVertices;
        glm::vec3 location = m_terrain->getVertexAtIndex(index);

        if (location.y > -0.2 && location.y < 0.3) {
            m_randVertices.push_back(location);
            numTrees++;
        }
    }
}

void TerrainScene::settingsChanged() {
    setToonUniforms();
    setLight();
    if (this->m_terrainType != settings.terrainType) {
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
        this->m_terrainType = settings.terrainType;
        generatePseudoRandIndices();
	}
    if (this->m_numTrees != settings.numberOfTrees) {
        m_numTrees = settings.numberOfTrees;
    }

}
