#pragma once

#include"glm\glm.hpp"
#include"glm\gtc\quaternion.hpp"

#include<vector>

class AnimationChannelKeyVec3 {

public:
	double time;		// ʱ��ڵ�;
	glm::vec3 keyData;	// ��������;

};

class AnimationChannelKeyQuat {

public:
	double time;		// ʱ��ڵ�;
	glm::quat keyData;	// ��������;

};