#include "SceneController.h"

SceneController::SceneController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void SceneController::initScene() {
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "scene");
	BaseController::loadModel("../res/model/scene/scene.obj", "scene");
	BaseController::setFlame("flame1", Flame());
	setSceneCollisionBox();
}

void SceneController::renderScene(Camera *currentCamera, float deltaTime) {
	// render scene
	Shader *sceneShader = getShader("scene");
	sceneShader->use();
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	sceneShader->setMat4("projection", projection);
	sceneShader->setMat4("view", view);
	glm::mat4 model_scene;
	model_scene = glm::translate(model_scene, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	sceneShader->setMat4("model", model_scene);
	Model *sceneModel = getModel("scene");
	sceneModel->Draw(*sceneShader);
					
	// render flame
	glm::mat4 model_flame;
	model_flame = glm::translate(model_flame, glm::vec3(0.0f, 0.0f, 0.0f));
	getFlame("flame1")->Render(deltaTime, model_flame, view, projection);
}

void SceneController::setSceneCollisionBox() {
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-80, -60), glm::vec2(132, 60));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-80, -1, -60), glm::vec3(132, 1, 60));
}