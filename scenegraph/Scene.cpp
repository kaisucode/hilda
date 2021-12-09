#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
	// We need to set the global constants to one when we duplicate a scene,
	// otherwise the global constants will be double counted (squared)
	CS123SceneGlobalData global = { 1, 1, 1, 1};
	setGlobal(global);

	// TODO [INTERSECT]
	// Make sure to copy over the lights and the scenegraph from the old scene,
	// as well as any other member variables your new scene will need.

	m_global = scene.m_global;
	m_lights = scene.m_lights;
	m_transformedPrimitives = scene.m_transformedPrimitives;
}

Scene::~Scene()
{
	// Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
	// TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
	//   finishParsing()

	CS123SceneGlobalData global;
	parser->getGlobalData(global);
	sceneToFill->setGlobal(global);

	for (int i = 0; i < parser->getNumLights(); i++) {
		CS123SceneLightData lightData;
		parser->getLightData(i, lightData);
		sceneToFill->addLight(lightData);
	}

	CS123SceneNode *rootNode = parser->getRootNode();

	std::vector<CS123SceneTransformation*> previousTransformations;
	sceneToFill->recursivelyGetPrimitives(rootNode, previousTransformations); 
}

void Scene::recursivelyGetPrimitives(CS123SceneNode *node, std::vector<CS123SceneTransformation*> pastTransformations) {

	pastTransformations.insert(pastTransformations.end(), node->transformations.begin(), node->transformations.end());

	if (node->children.size()) { 
		for (int i = 0; i < node->children.size(); i++) {
			recursivelyGetPrimitives(node->children.at(i), pastTransformations);
		}
	}

	if (node->primitives.size()) {		// leaf node, reached primitive(s)
		for (int i = 0; i < node->primitives.size(); i++) {
			CS123ScenePrimitive *aPrimitive = node->primitives.at(i);

			// collapse transformations
			int lastIndex = pastTransformations.size() - 1;
			glm::mat4x4 finalTransformationMatrix = glm::mat4x4(1.0);

			for (int j = lastIndex; j >= 0; j--) {
				finalTransformationMatrix = apply3dTransformations(finalTransformationMatrix, pastTransformations.at(j));
			}

			this->addPrimitive(*aPrimitive, finalTransformationMatrix);
		}
	}

	return;
}

glm::mat4x4 Scene::apply3dTransformations(glm::mat4x4 ogMatrix, CS123SceneTransformation* aTransformation) {

	switch (aTransformation->type) {
		case TRANSFORMATION_TRANSLATE:
			ogMatrix = glm::translate(aTransformation->translate) * ogMatrix;
			break;
		case TRANSFORMATION_SCALE:
			ogMatrix = glm::scale(aTransformation->scale) * ogMatrix;
			break;
		case TRANSFORMATION_ROTATE: 
			ogMatrix = glm::rotate(aTransformation->angle, aTransformation->rotate) * ogMatrix;
			break;
		case TRANSFORMATION_MATRIX: 
			ogMatrix = aTransformation->matrix * ogMatrix;
			break;
			// default:
	}

	return ogMatrix;
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
	TransformedPrimitive aTransformedPrimitive;
	aTransformedPrimitive.primitive = scenePrimitive;
	aTransformedPrimitive.transformations = matrix;

	CS123SceneFileMap aTextureMap = scenePrimitive.material.textureMap;
	if (aTextureMap.isUsed && !aTextureMap.filename.empty()) {
		// load in aTextureMap.filename
		QString filename = QString::fromStdString(aTextureMap.filename);
		aTransformedPrimitive.textureImage = QImage(filename);
	}

	m_transformedPrimitives.push_back(aTransformedPrimitive);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
	this->m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
	this->m_global = global;
}

