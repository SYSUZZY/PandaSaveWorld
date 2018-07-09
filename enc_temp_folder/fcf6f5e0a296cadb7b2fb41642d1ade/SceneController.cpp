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
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-80, -60), glm::vec2(132, 60));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-80, -1, -60), glm::vec3(132, 0, 60));

	// Î§Ç½£¨1£©Åö×²Ìå
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-25.5, 1, 5.5), glm::vec3(-11.5, 10, 15.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-31.5, 1, 7.5), glm::vec3(-25.5, 5, 12.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-45.5, 1, 5.5), glm::vec3( - 31.5, 10, 15.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-51.5, 1, 7.5), glm::vec3(-45.5, 5, 12.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-65.5, 1, 5.5), glm::vec3(-51.5, 10, 15.5));

	_physicsEngine->setSceneInnerBoundary(glm::vec3(-70, 1, 8.5), glm::vec3(-65.5, 4.75, 52.5));

	_physicsEngine->setSceneInnerBoundary(glm::vec3(-63, 1, 48.5), glm::vec3(-48, 4.75, 53.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-48, 1, 46.5), glm::vec3(-29.5, 6.5, 55));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-44.5, 1, 43.5), glm::vec3(-32.5, 5, 46.5));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-29, 1, 49.5), glm::vec3(-12, 4.75, 53.5));

	_physicsEngine->setSceneInnerBoundary(glm::vec3(-11.5, -1, 8.5), glm::vec3(-7.5, 4.75, 52.5));

	// Î§Ç½£¨2£©Åö×²Ìå
}