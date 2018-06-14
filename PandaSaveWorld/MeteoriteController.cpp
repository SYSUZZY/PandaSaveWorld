#include "MeteoriteController.h"

MeteoriteController::MeteoriteController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void MeteoriteController::initMeteorite() {
	speed = 20.0f;
	dir = glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(0.0f, 50.0f, -50.0f);
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "meteorite");
	BaseController::loadModel("../res/model/Meteorite/stg600_obj_meteoriteB01.obj", "meteorite");
	BaseController::setFlame("flame1", Flame());
}

void MeteoriteController::renderMeteorite(Camera * currentCamera, float deltaTime) {
	// render meteorite
	Shader *meteoriteShader = getShader("meteorite");
	glm::mat4 model_meteorite, model_flame;
	model_meteorite = glm::translate(model_meteorite, glm::vec3(0.0f, 50.0f, -50.0f));
	model_flame = model_meteorite;
	model_meteorite = glm::scale(model_meteorite, glm::vec3(0.1f, 0.1f, 0.1f));
	dir = glm::normalize(dir);
	curTime += deltaTime;
	model_meteorite = glm::translate(model_meteorite, dir * curTime * speed);
	meteoriteShader->setMat4("model", model_meteorite);
	getModel("meteorite")->Draw(*meteoriteShader);

	// render flame
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	model_flame = glm::translate(model_flame, glm::vec3(0.0f, -2.0f, 0.0f));
	model_flame = glm::translate(model_flame, dir * curTime * speed / 10.0f);//»ðÑæ¸úËæÔÉÊ¯ÒÆ¶¯
	getFlame("flame1")->Render(deltaTime, model_flame, view, projection);
}

void MeteoriteController::setMeteoriteCollisionBox()
{
}
