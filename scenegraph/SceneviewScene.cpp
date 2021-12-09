#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
	settingsChanged();

    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
	setGlobalData();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setGlobalData(){
    // [TODO] pass global data to shader.vert using m_phongShader
    m_phongShader->setUniform("ka", m_global.ka);
    m_phongShader->setUniform("kd", m_global.kd);
    m_phongShader->setUniform("ks", m_global.ks);
    // m_phongShader->setUniform("kt", m_global.kt);
}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
	m_phongShader->setUniform("isShapeScene", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
	
	for (int i = 0; i < this->m_lights.size(); i++) {
		CS123SceneLightData aLight = m_lights.at(i);
		m_phongShader->setLight(aLight);
	}
	return;
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

	for (int i = 0; i < this->m_transformedPrimitives.size(); ++i) {
		TransformedPrimitive aTransformedPrimitive = this->m_transformedPrimitives.at(i);
		m_phongShader->applyMaterial(aTransformedPrimitive.primitive.material);
		m_phongShader->setUniform("m", aTransformedPrimitive.transformations);

		switch(aTransformedPrimitive.primitive.type) {
			case PrimitiveType::PRIMITIVE_CUBE: 
				m_cube->draw();
				break;
			case PrimitiveType::PRIMITIVE_CONE: 
				m_cone->draw();
				break;
			case PrimitiveType::PRIMITIVE_SPHERE: 
				m_sphere->draw();
				break;
			case PrimitiveType::PRIMITIVE_CYLINDER: 
				m_cylinder->draw();
				break;
			default: 
				m_example->draw();
		}

	}

	return;
}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.

	int param1 = settings.shapeParameter1;
	int param2 = settings.shapeParameter2;

	if (m_should_use_LOD && this->m_transformedPrimitives.size() > m_LOD_count_threshold) {
		param1 = std::min(param1, 10);
		param2 = std::min(param2, 10);
	}

	// reconstruct primitives with the correct tesselation values
	m_cube = std::make_unique<CubeShape>(param1, param2);
	m_cone = std::make_unique<ConeShape>(param1, param2);
	m_sphere = std::make_unique<SphereShape>(param1, param2);
	m_cylinder = std::make_unique<CylinderShape>(param1, param2);
	m_example = std::make_unique<ExampleShape>(param1, param2);
}

