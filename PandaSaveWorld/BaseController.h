#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Flame.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "PhysicsEngine.h"

#include <vector>
#include <unordered_map>
#include <iostream>

// settings
#define SCR_WIDTH 1000
#define SCR_HEIGHT 1000

class BaseController {

public:
	// 获得指定的shader
	Shader* getShader(string name);

	// 设置指定的shader
	void setShader(string name, Shader &shader);

	// 获得指定的model
	Model* getModel(string name);

	// 设置指定的model
	void setModel(string name, Model &model);

	// 获得指定的粒子系统
	Flame* getFlame(string name);

	// 设置指定的粒子系统
	void setFlame(string name, Flame &flame);

	PhysicsEngine *_physicsEngine;

protected:
	// 编译shader
	void compileShader(string vsPath, string fsPath, string name);

	// 加载模型
	void loadModel(string path, string name);

	std::vector<Shader> shaderArr;
	std::map<string, int> shaderIndex;
	std::vector<Model> modelArr;
	std::map<string, int> modelIndex;
	std::vector<Flame> flameArr;
	std::map<string, int> flameIndex;
};