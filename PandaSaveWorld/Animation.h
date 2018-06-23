#pragma once
#include <vector>
#include <iostream>
#include "AnimationChannel.h"


class Animation {
public:
	char name[50];  // 动画片段名;
	double duration;  // 持续时间;
	double ticksPerSecond;  // 每一秒多少帧;
	int numChannels;  // 多少个Channel，一个Channel是一个关节的动作集合。
	std::vector<AnimationChannel> channels;  // 所有关节的动作数据集合;
};