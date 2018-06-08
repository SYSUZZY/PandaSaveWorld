#include "MeteoriteController.h"

MeteoriteController::MeteoriteController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void MeteoriteController::initMeteorite() {
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "meteorite");
	BaseController::loadModel("../res/model/Meteorite/stg600_obj_meteoriteB01.obj", "meteorite");
}

void MeteoriteController::renderMeteorite(Camera * currentCamera, float deltaTime) {
	// render meteorite
	Shader *meteoriteShader = getShader("meteorite");
	glm::mat4 model_meteorite;
	model_meteorite = glm::translate(model_meteorite, glm::vec3(0.0f, 50.0f, -50.0f));
	meteoriteShader->setMat4("model", model_meteorite);
	getModel("meteorite")->Draw(*meteoriteShader);
}

void MeteoriteController::setMeteoriteCollisionBox()
{
}
