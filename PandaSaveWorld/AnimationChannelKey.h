#pragma once

#include"glm\glm.hpp"
#include"glm\gtc\quaternion.hpp"

#include<vector>

class AnimationChannelKeyVec3 {

public:
	double time;		// 时间节点;
	glm::vec3 keyData;	// 动画数据;

};

class AnimationChannelKeyQuat {

public:
	double time;		// 时间节点;
	glm::quat keyData;	// 动画数据;

};