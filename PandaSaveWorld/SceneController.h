#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Flame.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "PhysicsEngine.h"
#include "BaseController.h"

#include <vector>
#include <unordered_map>
#include <iostream>

class SceneController : public BaseController {
public:
	SceneController(PhysicsEngine* physicsEngine);
	// ��ʼ������
	void initScene();

	// ��Ⱦ����
	void renderScene(Camera *currentCamera, float deltaTime);

private:
	// ������ײ����
	void setSceneCollisionBox();
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;
};