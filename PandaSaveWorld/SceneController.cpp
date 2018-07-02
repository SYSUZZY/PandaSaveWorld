#include "SceneController.h"

SceneController::SceneController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void SceneController::initScene() {
	BaseController::compileShader("../res/shader/scene_model_loading_s.vs", "../res/shader/scene_model_loading_s.fs", "scene");
	BaseController::loadModel("../res/model/scene/scene.obj", "scene");
	setSceneCollisionBox();
}

void SceneController::renderScene(Camera *currentCamera, float deltaTime) {
	// lighting
	//点光源
	glm::vec3 lightPos(0.0f, 50.0f, 50.0f);
	//平行光
	glm::vec3 lightDir(122.0f, 155.0f, 0.5f);
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

	//传入光照数据
	//sceneShader->setVec3("light.position", lightPos);
	//sceneShader->setVec3("light.direction", lightDir);
	sceneShader->setVec3("lightPos", lightPos);
	sceneShader->setVec3("lightDirection", lightDir);
	sceneShader->setVec3("viewPos", currentCamera->Position);
	// light properties
	sceneShader->setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
	sceneShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	sceneShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	sceneShader->setFloat("shininess", 32.0f);

	Model *sceneModel = getModel("scene");
	sceneModel->Draw(*sceneShader);
}

void SceneController::setSceneCollisionBox() {
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-80, -60), glm::vec2(132, 60));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-80, -1, -60), glm::vec3(132, 1, 60));
}