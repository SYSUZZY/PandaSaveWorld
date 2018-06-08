#include "PlayerController.h"

PlayerController::PlayerController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void PlayerController::initPlayer() {
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "panda");
	BaseController::loadModel("../res/model/Panda_animation/panda.FBX", "panda");
}

void PlayerController::renderPlayer(Camera * currentCamera, float deltaTime) {
	// render panda
	Shader *pandaShader = getShader("panda");
	pandaShader->use();
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	pandaShader->setMat4("projection", projection);
	pandaShader->setMat4("view", view);
	glm::mat4 model_panda;
	model_panda = glm::translate(model_panda, currentCamera->Position); // translate it down so it's at the center of the scene
	model_panda = glm::scale(model_panda, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
	pandaShader->setMat4("model", model_panda);
	getModel("panda")->Draw(*pandaShader);
}

void PlayerController::setPlayerCollisionBox() {

}
