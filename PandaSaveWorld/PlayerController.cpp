#include "PlayerController.h"

PlayerController::PlayerController(PhysicsEngine* physicsEngine, glm::vec3 position,
	glm::vec3 up, float yaw) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(MoveSpeed), MouseSensitivity(SENSITIVITY) {
	_physicsEngine = physicsEngine;
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	updatePlayerVectors();
}


PlayerController::PlayerController(PhysicsEngine * physicsEngine, float posX, float posY, 
	float posZ, float upX, float upY, float upZ, float yaw) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(MoveSpeed), MouseSensitivity(SENSITIVITY) {
	_physicsEngine = physicsEngine;
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	updatePlayerVectors();
}


void PlayerController::ProcessKeyboard(Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	Target = Position;

	if (direction == FORWARD)
		Target += Front * velocity;
	if (direction == BACKWARD)
		Target -= Front * velocity;
	if (direction == LEFT)
		Target -= Right * velocity;
	if (direction == RIGHT)
		Target += Right * velocity;
	if (direction == UP) {
		if (!_physicsEngine->isJumping)
			_physicsEngine->jumpAndUpdateVelocity();
		_physicsEngine->isJumping = true;
	}
	if (direction == LEFTROTATE)
		RotatePlayer(-10);
	if (direction == RIGHTROTATE)
		RotatePlayer(10);

	// 碰撞检测后更新Position
	_physicsEngine->checkOutsideCollision(Position, Target);
	_physicsEngine->checkInsideCollision(Position, Target);
	Position = Target;

	// std::cout << "x: " << Position.x << "y: " << Position.y << "z: " << Position.z << std::endl;
}

void PlayerController::RotatePlayer(float xoffset) {
	Yaw += xoffset;

	// Update Front, Right and Up Vectors using the updated Euler angles
	updatePlayerVectors();
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

	// 更新panda的位置
	updatePlayerMovement();
	std::cout << "x: " << Position.x << "y: " << Position.y << "z: " << Position.z << std::endl;

	// 更新骨骼动画
	glm::mat4 model_panda;
	model_panda = glm::translate(model_panda, Position);
	model_panda = glm::scale(model_panda, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
	model_panda = glm::rotate(model_panda, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model_panda = glm::rotate(model_panda, 90.0f - Yaw, glm::vec3(0.0f, 0.0f, 1.0f));
	pandaShader->setMat4("model", model_panda);

	Model *panda = getModel("panda");
	panda->OnDraw();
	panda->meshes[0].updateMesh();

	panda->Draw(*pandaShader);
}


void PlayerController::updatePlayerVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw));
	front.y = 0.0;
	front.z = sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void PlayerController::updatePlayerMovement() {
	_physicsEngine->updateItemMovement(Position, Target);
}
