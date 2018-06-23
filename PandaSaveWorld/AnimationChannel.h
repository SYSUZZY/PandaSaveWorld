#pragma once
#include "AnimationChannelKey.h"

class AnimationChannel {

public:
	char nodeName[50];									// �ؽ�����,���Channel�������ĸ��ڵ�Ķ�������;
	int numPositionKeys;								// λ�ƹؼ�֡����;
	std::vector<AnimationChannelKeyVec3> positionKeys;  // λ�ƹؼ�֡����;
	int numRotationKeys;								// ��ת�ؼ�֡����;
	std::vector<AnimationChannelKeyQuat> rotationKeys;  // ��ת�ؼ�֡����;
	int numScalingKeys;                                 // ���Źؼ�֡����;
	std::vector<AnimationChannelKeyVec3> scalingKeys;   // ���Źؼ�֡����;

};