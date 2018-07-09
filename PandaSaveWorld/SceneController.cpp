#include "SceneController.h"

SceneController::SceneController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void SceneController::initScene() {
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "scene");
	BaseController::loadModel("../res/model2/newScene/newScene.obj", "scene");
	setSceneCollisionBox();
}

void SceneController::renderScene(Camera *currentCamera, float deltaTime) {
	// render scene
	Shader *sceneShader = getShader("scene");
	sceneShader->use();
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	sceneShader->setMat4("projection", projection);
	sceneShader->setMat4("view", view);
	glm::mat4 model_scene;
	model_scene = glm::translate(model_scene, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model_scene = glm::scale(model_scene, glm::vec3(0.5f, 0.5f, 0.5f));
	sceneShader->setMat4("model", model_scene);
	Model *sceneModel = getModel("scene");
	sceneModel->Draw(*sceneShader);
}

void SceneController::setSceneCollisionBox() {
	// µØ°åÅö×²Ìå
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-1600, -2700), glm::vec2(2600, 370));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1600, -14, -2700), glm::vec3(2600, -10, 370));

	//_physicsEngine->setSceneInnerBoundary(glm::vec3(-1600, -10, -600), glm::vec3(-, -10, 370));
}