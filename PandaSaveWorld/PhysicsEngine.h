#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

using namespace std;

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))

// 由于改用飞行的凤凰，因此重力和跳跃的系数置为零，倘若要检查重力的跳跃的功能，可以手动更改这部分代码，将数值设置为不为零即可
#define phoenixHeight 1.0f           // 玩家头到脚的高度
#define GravityAcceleration -9.8f
#define MoveSpeed 50.0f            // 玩家移动速度
#define BoundaryGap 0.1f           // 碰撞间距
#define JumpInitialSpeed 0.0f     // 起跳初速度
#define JumpFactor 0.0f           // 跳起速度系数
#define GravityFactor 0.0f        // 下落速度系数

class PhysicsEngine {

public:
	PhysicsEngine();
	~PhysicsEngine();

	// 设置空间外部边缘
	void setSceneOuterBoundary(glm::vec2 leftBackPoint, glm::vec2 rightFrontPoint);


	// 设置空间内部边缘
	void setSceneInnerBoundary(glm::vec3 leftBackDownPoint, glm::vec3 rightFrontUpPoint);


	// 外部碰撞检测
	void checkOutsideCollision(glm::vec3 & cameraPos, glm::vec3 & targetPos);


	// 内部碰撞检测
	void checkInsideCollision(glm::vec3 & cameraPos, glm::vec3 & targetPos);


	// 是否在跳跃
	bool isJumping;


	void jumpAndUpdateVelocity();    //按下space跳跃时调用
									 //每帧绘制的时候更新摄像机垂直方向移动

	void updateItemMovement(glm::vec3 & cameraPos, glm::vec3 & targetPos);

private:
	// 空间外部边缘碰撞检测
	void checkOutsideCollisionXZ(glm::vec3 & cameraPos, glm::vec3 & targetPos);


	// 空间内部边缘碰撞检测（考虑XYZ）
	void checkInsideCollisionXYZ(glm::vec3 innerBoundaryMin, glm::vec3 innerBoundaryMax, glm::vec3 & cameraPos, glm::vec3 & targetPos);


	// 空间内部边缘碰撞检测（考虑XZ）
	void checkInsideCollisionXZ(glm::vec3 innerBoundaryMin, glm::vec3 innerBoundaryMax, glm::vec3 & cameraPos, glm::vec3 & targetPos);


	glm::vec3 velocity;        // 垂直方向速度
	glm::vec3 gravity;         // 重力加速度
	glm::vec3 accelerationUp;  // 方向向上的加速度

	glm::vec2 outerBoundaryMin;          // 场景小的xz坐标
	glm::vec2 outerBoundaryMax;		     // 场景大的xz坐标
	vector<glm::vec3> innerBoundaryMin;  // 碰撞器小的x/y/z坐标
	vector<glm::vec3> innerBoundaryMax;  // 碰撞器大的x/y/z坐标
};

#endif // !PHYSICSENGINE_H