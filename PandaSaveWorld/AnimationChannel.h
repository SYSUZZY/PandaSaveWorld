#pragma once
#include "AnimationChannelKey.h"

class AnimationChannel {

public:
	char nodeName[50];									// 关节名字,这个Channel是属于哪个节点的动作集合;
	int numPositionKeys;								// 位移关键帧数量;
	std::vector<AnimationChannelKeyVec3> positionKeys;  // 位移关键帧数据;
	int numRotationKeys;								// 旋转关键帧数量;
	std::vector<AnimationChannelKeyQuat> rotationKeys;  // 旋转关键帧数据;
	int numScalingKeys;                                 // 缩放关键帧数量;
	std::vector<AnimationChannelKeyVec3> scalingKeys;   // 缩放关键帧数据;

};