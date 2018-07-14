#include "SceneController.h"

SceneController::SceneController(PhysicsEngine* physicsEngine) {
	_physicsEngine = physicsEngine;
}

void SceneController::initScene() {
	BaseController::compileShader("../res/shader/scene_model_loading_s.vs", "../res/shader/scene_model_loading_s.fs", "scene");
	BaseController::compileShader("../res/shader/simpleDepthShader.vs", "../res/shader/simpleDepthShader.fs", "depth");
	BaseController::compileShader("../res/shader/1.model_loading.vs", "../res/shader/shadow_debug.fs", "debug");
	BaseController::loadModel("../res/model2/newScene/newScene.obj", "scene");

	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	/*float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader *sceneShader = getShader("scene");
	sceneShader->use();
	sceneShader->setInt("shadowMap", 0);

	/*Shader *debugshader = getShader("debug");
	debugshader->use();
	debugshader->setInt("depthMap", 0);*/

	setSceneCollisionBox();
}

void SceneController::renderScene(Camera *currentCamera, float deltaTime) {
	// light properity
	//glm::vec3 lightPos(300.0f, 430.0f, -1380.0f);
	//glm::vec3 lightPos(-200.0f, 2300.0f, -3000.0f);
	//glm::vec3 lightPos(1703.0f,2539.0f, -1988.0f);
	glm::vec3 lightPos(80.0f, 2300.0f, -3780.0f);
	Model *sceneModel = getModel("scene");

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = -5000.0f, far_plane = 5000.0f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-5000.0f,5000.0f, -5000.0f, 5000.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	Shader *simpleDepthShader = getShader("depth");
	simpleDepthShader->use();
	simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	sceneModel->Draw(*simpleDepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader *sceneShader = getShader("scene");
	sceneShader->use();
	glm::mat4 projection = glm::perspective(currentCamera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = currentCamera->GetViewMatrix();
	sceneShader->setMat4("projection", projection);
	sceneShader->setMat4("view", view);
	glm::mat4 model_scene;
	model_scene = glm::translate(model_scene, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model_scene = glm::scale(model_scene, glm::vec3(0.5f, 0.5f, 0.5f));
	sceneShader->setMat4("model", model_scene);
	sceneShader->setVec3("viewPos", currentCamera->Position);
	sceneShader->setVec3("lightPos", lightPos);
	sceneShader->setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
	sceneShader->setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
	sceneShader->setVec3("light.specular", 0.8f, 0.8f, 0.8f);
	sceneShader->setFloat("shininess", 32.0f);
	sceneShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	sceneModel->DrawScene(*sceneShader, depthMap);
	//sceneModel->Draw(*sceneShader);

	/*Shader *debugshader = getShader("debug");
	debugshader->use();
	debugshader->setFloat("near_plane", near_plane);
	debugshader->setFloat("far_plane", far_plane);
	sceneModel->shadowDebug(*debugshader, depthMap);*/

}

void SceneController::setSceneCollisionBox() {
	// µØ°åÅö×²Ìå
	_physicsEngine->setSceneOuterBoundary(glm::vec2(-1600, -2700), glm::vec2(2600, 370));
	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1600, -14, -2700), glm::vec3(2600, -10, 370));

	_physicsEngine->setSceneInnerBoundary(glm::vec3(-1160, -10, -600), glm::vec3(2600, 5, -200));
}