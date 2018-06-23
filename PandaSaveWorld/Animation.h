#pragma once
#include <vector>
#include <iostream>
#include "AnimationChannel.h"


class Animation {
public:
	char name[50];  // ����Ƭ����;
	double duration;  // ����ʱ��;
	double ticksPerSecond;  // ÿһ�����֡;
	int numChannels;  // ���ٸ�Channel��һ��Channel��һ���ؽڵĶ������ϡ�
	std::vector<AnimationChannel> channels;  // ���йؽڵĶ������ݼ���;
};