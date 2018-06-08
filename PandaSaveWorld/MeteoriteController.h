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

	// 初始化陨石
	void initMeteorite();

	// 渲染陨石
	void renderMeteorite(Camera *currentCamera, float deltaTime);

private:

	// 设置碰撞盒子(未完成)
	void setMeteoriteCollisionBox();
};