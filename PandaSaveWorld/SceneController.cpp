#include "SceneController.h"

SceneController::SceneController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void SceneController::initScene() {
	BaseController::compileShader("../res/shader/scene_model_loading_s.vs", "../res/shader/scene_model_loading_s.fs", "scene");
	BaseController::compileShader("../res/shader/simpleDepthShader.vs", "../res/shader/simpleDepthShader.fs", "depth");
	BaseController::loadModel("../res/model/scene/scene.obj", "scene");
	setSceneCollisionBox();

	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void SceneController::renderScene(Camera *currentCamera, float deltaTime) {
	Model *sceneModel = getModel("scene");
	// lighting
	//点光源
	glm::vec3 lightPos(50.0f, 40.0f, -114.0f);
	//平行光
	glm::vec3 lightDir(-85.0f, -53.0f, -124.0f);
	//从光源角度渲染深度
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat size = 128*2;
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightProjection = glm::ortho(-size, size, -size, size, 0.1f, 1000.0f);

	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	Shader *depthShader = getShader("depth");
	depthShader->use();
	depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	sceneModel->Draw(*depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render scene
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader *sceneShader = getShader("scene");
	sceneShader->use();
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	sceneShader->setMat4("projection", projection);
	sceneShader->setMat4("view", view);
	glm::mat4 model_scene;
	model_scene = glm::translate(model_scene, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	sceneShader->setMat4("model", model_scene);

	//传入光照数据
	sceneShader->setVec3("lightPos", lightPos);
	//sceneShader->setVec3("lightDirection", lightDir);
	sceneShader->setVec3("viewPos", currentCamera->Position);
	// light properties
	sceneShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	sceneShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	sceneShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	sceneShader->setFloat("shininess", 32.0f);
	sceneShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(sceneShader->ID, "depthMap"),depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	sceneModel->Draw(*sceneShader);
}

void SceneController::setSceneCollisionBox() {
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-80, -60), glm::vec2(132, 60));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-80, -1, -60), glm::vec3(132, 1, 60));
}