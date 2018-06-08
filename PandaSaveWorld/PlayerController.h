#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "model.h"
#include "BaseController.h"

#include <vector>
#include <iostream>

class PlayerController : public BaseController {
public:
	PlayerController(PhysicsEngine* physicsEngine);

	// 初始化玩家
	void initPlayer();

	// 渲染玩家
	void renderPlayer(Camera *currentCamera, float deltaTime);

private:

	// 设置碰撞盒子(未完成)
	void setPlayerCollisionBox();
};