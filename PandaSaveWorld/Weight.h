#pragma once

#define VERTEX_MAX_BONE 4

class Weight
{
public:
	int boneid;  // 骨骼id,要找到对应的Bone，取Bone中的offsetMatrix;
	float weight;  // 权重,用于将多个骨骼的变换组合成一个变换矩阵,一个顶点的所有骨骼权重之和必须为1;

	Weight() {
		weight = 0;
		boneid = 0;
	}
};