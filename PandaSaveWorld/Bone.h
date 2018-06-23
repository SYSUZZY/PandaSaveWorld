#pragma once

#include"glm\glm.hpp"

class Bone {
public:
	char name[50];           // 例如 joint1，与 Scene->Animation->Channels 中的Channel的name对应;
	glm::mat4 offsetMatrix;  // 顶点坐标做成offsetmatrix 从模型空间到骨骼空间;
	glm::mat4 finalMatrix;
};