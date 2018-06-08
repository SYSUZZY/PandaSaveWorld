#include "BaseController.h"


Shader * BaseController::getShader(string name) {
	Shader* shaderPtr = &shaderArr[shaderIndex[name]];
	return shaderPtr;
}

void BaseController::setShader(string name, Shader & shader) {
	shaderIndex[name] = shaderArr.size();
	shaderArr.push_back(shader);
}

Model * BaseController::getModel(string name) {
	Model* modelPtr = &modelArr[modelIndex[name]];
	return modelPtr;
}

void BaseController::setModel(string name, Model & model) {
	modelIndex[name] = modelArr.size();
	modelArr.push_back(model);
}

Flame * BaseController::getFlame(string name) {
	Flame* flamePtr = &flameArr[flameIndex[name]];
	return flamePtr;
}

void BaseController::setFlame(string name, Flame & flame) {
	flameIndex[name] = flameArr.size();
	flameArr.push_back(flame);
}

void BaseController::compileShader(string vsPath, string fsPath, string name) {
	Shader shader(vsPath.c_str(), fsPath.c_str());
	setShader(name, shader);
}

void BaseController::loadModel(string path, string name) {
	std::cout << "Start loading " << name << " model." << std::endl;
	Model model(path);
	setModel(name, model);
	std::cout << name << " complete" << endl;
}