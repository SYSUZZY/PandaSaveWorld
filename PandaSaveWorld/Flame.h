#pragma once
#include <GL/glew.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"


#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
	//����ٶ�
#define MAX_VELOC glm::vec3(0.0, 72.0, 0.0)
	//��С�ٶ�
#define MIN_VELOC glm::vec3(0.0, 70.0, 0.0)
	//�����С�ٶȲ��
#define DEL_VELOC glm::vec3(0.0, 2.0, 0.0)
	//���������
#define MAX_LIFE 2.0f*1000//ԭʼֵ2.0f*1000
	//�����������
#define MIN_LIFE 0.1f*1000//ԭʼֵ1.0f*1000
	//��ʼ�㾫���С
#define INIT_SIZE 30.0f;

const int MAX_PARTICLES = 18000;//�������ӷ���ϵͳ����������������ֵΪ18000
//��ʼ��������������
const int INIT_PARTICLES = 10000;//��ʼ��ʱ���ӷ���������ԭʼֵΪ10000

struct FlameParticle {
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetimeMills;  // ����
	float alpha;		  // alphaͨ��
	float size;			  // ���ӵ㾫���С
	float life;			  // ����
};

class Flame {

public:
	Flame();
	~Flame();
	void Render(float frametimeMills, glm::mat4& worldMatrix, glm::mat4 viewMatrix, glm::mat4& projectMatrix);
	void SetRaidus(float newRadius);
	glm::vec3 GetCenter();

private:
	bool InitFlame(glm::vec3& pos);
	void UpdateParticles(float frametimeMills);  // �������ӵ�λ�õ�
	void InitRandomTexture(unsigned int size);   // ����1ά�������
	void RenderParticles(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectMatrix);
	void GenInitLocation(FlameParticle partciles[], int nums);  // ���ɳ�ʼ����

	unsigned int mCurVBOIndex, mCurTransformFeedbackIndex;
	GLuint mParticleBuffers[2];  // ���ӷ���ϵͳ���������㻺����
	GLuint mParticleArrays[2];
	GLuint mTransformFeedbacks[2];  // ���ӷ���ϵͳ��Ӧ��TransformFeedback
	GLuint mRandomTexture;  // ���һά����
	CTexture mSparkTexture;  // Alpha����
	CTexture mStartTexture;
	float mTimer;  // ���ӷ������Ѿ������ʱ��
	bool mFirst;
	Shader* mUpdateShader;  // �������ӵ�GPUProgram
	Shader* mRenderShader;  // ��Ⱦ���ӵ�GPUProgram
	glm::vec3 center;// ��������λ��
	float radius;// ����뾶
};

