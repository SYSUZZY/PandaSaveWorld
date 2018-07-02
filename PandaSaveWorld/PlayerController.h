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

	PlayerController(PhysicsEngine* physicsEngine, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 90);

	// Constructor with scalar values
	PlayerController(PhysicsEngine* physicsEngine, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw);

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Movement direction, float deltaTime);

	void RotatePlayer(float xoffset);

	// 初始化玩家
	void initPlayer();

	// 渲染玩家
	void renderPlayer(Camera *currentCamera, float deltaTime);

	// player attribution
	glm::vec3 Target;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	// player options
	float MovementSpeed;
	float MouseSensitivity;

private:

	// 更新玩家的方向
	void updatePlayerVectors();

	// 更新玩家的坐标
	void updatePlayerMovement();
};