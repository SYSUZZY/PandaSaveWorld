#pragma once
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "model.h"
#include "BaseController.h"

#include <vector>
#include <iostream>

class MeteoriteController : public BaseController {
public:
	MeteoriteController(PhysicsEngine* physicsEngine);

	// ��ʼ����ʯ
	void initMeteorite();

	// ��Ⱦ��ʯ
	void renderMeteorite(Camera *currentCamera, float deltaTime);

	float speed;
	float curTime;
	glm::vec3 dir;
private:

	// ������ײ����(δ���)
	void setMeteoriteCollisionBox();
};