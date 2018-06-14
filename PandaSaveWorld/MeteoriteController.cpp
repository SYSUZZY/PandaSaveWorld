#include "MeteoriteController.h"

MeteoriteController::MeteoriteController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void MeteoriteController::initMeteorite() {
	// 初始化变量
	speed = 20.0f;
	from = glm::vec3(0.0f, 0.0f, 0.0f);
	dest = glm::vec3(0.0f, 100.0f, -50.0f);
	dir = glm::normalize(from - dest);

	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/1.model_loading.fs", "meteorite");
	BaseController::loadModel("../res/model/Meteorite/stg600_obj_meteoriteB01.obj", "meteorite");
	BaseController::setFlame("flame1", Flame());
}

void MeteoriteController::renderMeteorite(Camera * currentCamera, float deltaTime) {
	// render meteorite
	Shader *meteoriteShader = getShader("meteorite");
	glm::mat4 model_meteorite, model_flame;
	model_meteorite = glm::translate(model_meteorite, dest);
	model_flame = model_meteorite;
	model_meteorite = glm::scale(model_meteorite, glm::vec3(0.1f, 0.1f, 0.1f));
	if (curTime <= 55) {// 55为实验累计时间，到达55时会到达地面(0.0f, 0.0f, 0.0f)处
		curTime += deltaTime;// 累计时间，实现陨石移动
	}
	model_meteorite = glm::translate(model_meteorite, dir * curTime * speed);//逐帧平滑移动
	meteoriteShader->setMat4("model", model_meteorite);
	getModel("meteorite")->Draw(*meteoriteShader);

	// render flame
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	model_flame = glm::translate(model_flame, glm::vec3(0.5f, -2.3f, 0.5f));//实验最佳偏移值
	model_flame = glm::translate(model_flame, dir * curTime * speed / 10.0f);//火焰跟随陨石平滑移动
	getFlame("flame1")->Render(deltaTime, model_flame, view, projection);
}

void MeteoriteController::setMeteoriteCollisionBox()
{
}
