#pragma once

#define VERTEX_MAX_BONE 4

class Weight
{
public:
	int boneid;  // ����id,Ҫ�ҵ���Ӧ��Bone��ȡBone�е�offsetMatrix;
	float weight;  // Ȩ��,���ڽ���������ı任��ϳ�һ���任����,һ����������й���Ȩ��֮�ͱ���Ϊ1;

	Weight() {
		weight = 0;
		boneid = 0;
	}
};