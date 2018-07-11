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
	// 地板碰撞体
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-1600, -2700), glm::vec2(2600, 370));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1600, -14, -2700), glm::vec3(2600, -10, 370));

	// 第一层
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1693,  -12, - 2465), glm::vec3(1759, 697, - 1978));

	// 第二层
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-650, 235, -2500), glm::vec3(1380, 360, -1450));

	// 祭台
	_physicsEngine->setSceneInnerBoundary(glm::vec3(106, 363, -1865), glm::vec3(314, 400, -1710));

	// 第三层
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-879, 104, -2008), glm::vec3(1827, 235, -1435));

	// 第四层
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1400, -10, -2400), glm::vec3(2925, 104, 10));
}