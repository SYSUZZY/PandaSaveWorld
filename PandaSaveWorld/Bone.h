#pragma once

#include"glm\glm.hpp"

class Bone {
public:
	char name[50];           // ���� joint1���� Scene->Animation->Channels �е�Channel��name��Ӧ;
	glm::mat4 offsetMatrix;  // ������������offsetmatrix ��ģ�Ϳռ䵽�����ռ�;
	glm::mat4 finalMatrix;
};