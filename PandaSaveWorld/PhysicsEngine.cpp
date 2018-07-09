#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	velocity = glm::vec3(0.f, 0.f, 0.f);
	gravity = glm::vec3(0.f, GravityAcceleration, 0.f);
	accelerationUp = glm::vec3(0.f, 0.f, 0.f);

	isJumping = false;
}


PhysicsEngine::~PhysicsEngine() {}


void PhysicsEngine::setSceneOuterBoundary(glm::vec2 leftBackPoint, glm::vec2 rightFrontPoint) {
	// 设置包围盒：比空间外部边缘小一点
	leftBackPoint.x += BoundaryGap;
	leftBackPoint.y += BoundaryGap;
	rightFrontPoint.x -= BoundaryGap;
	rightFrontPoint.y -= BoundaryGap;

	outerBoundaryMin = leftBackPoint;
	outerBoundaryMax = rightFrontPoint;
}


void PhysicsEngine::setSceneInnerBoundary(glm::vec3 leftDownBackPoint, glm::vec3 rightUpFrontPoint) {
	// 设置包围盒：比空间内部边缘大一点
	leftDownBackPoint.x -= BoundaryGap;
	leftDownBackPoint.y -= BoundaryGap;
	leftDownBackPoint.z -= BoundaryGap;
	rightUpFrontPoint.x += BoundaryGap;
	rightUpFrontPoint.y += BoundaryGap;
	rightUpFrontPoint.z += BoundaryGap;

	innerBoundaryMin.push_back(leftDownBackPoint);
	innerBoundaryMax.push_back(rightUpFrontPoint);
}


void PhysicsEngine::checkOutsideCollision(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	checkOutsideCollisionXZ(cameraPos, targetPos);
}


void PhysicsEngine::checkOutsideCollisionXZ(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	// 如果目标位置出了包围盒，先放回来
	targetPos.x = targetPos.x < outerBoundaryMin.x ? outerBoundaryMin.x : targetPos.x > outerBoundaryMax.x ? outerBoundaryMax.x : targetPos.x;
	targetPos.z = targetPos.z < outerBoundaryMin.y ? outerBoundaryMin.y : targetPos.z > outerBoundaryMax.y ? outerBoundaryMax.y : targetPos.z;
	
	float distance = sqrt((cameraPos[0] - targetPos[0]) * (cameraPos[0] - targetPos[0]) +
		(cameraPos[2] - targetPos[2]) * (cameraPos[2] - targetPos[2]));

	// 若视点与目标距离太小，则固定目标位置，视点沿正对目标的逆方向移动
	if (distance <= 2.0f) {
		cameraPos[0] = 2.0f * (cameraPos[0] - targetPos[0]) / distance + targetPos[0];
		cameraPos[2] = 2.0f * (cameraPos[2] - targetPos[2]) / distance + targetPos[2];
	}
	bool flag = false;

	// 在检查视点是否出了包围盒。若则放回
	if (cameraPos[0] < outerBoundaryMin.x) {
		cameraPos[0] = outerBoundaryMin.x;
		flag = true;
	}
	if (cameraPos[0] > outerBoundaryMax.x) {
		cameraPos[0] = outerBoundaryMax.x;
		flag = true;
	}
	if (cameraPos[2] < outerBoundaryMin.y) {
		cameraPos[2] = outerBoundaryMin.y;
		flag = true;
	}
	if (cameraPos[2] > outerBoundaryMax.y) {
		cameraPos[2] = outerBoundaryMax.y;
		flag = true;
	}

	//重复上述远离两点距离的操作
	if (flag) {
		distance = sqrt((cameraPos[0] - targetPos[0])*(cameraPos[0] - targetPos[0]) +
			(cameraPos[2] - targetPos[2])*(cameraPos[2] - targetPos[2]));

		if (distance <= 2.0f) {
			targetPos[0] = 2.0f*(targetPos[0] - cameraPos[0]) / distance + cameraPos[0];
			targetPos[2] = 2.0f*(targetPos[2] - cameraPos[2]) / distance + cameraPos[2];
		}
	}
}


void PhysicsEngine::checkInsideCollision(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	// 后面可以在这里添加：预处理，排除当前肯定不会产生碰撞的物体
	for (int i = 0; i < innerBoundaryMin.size(); i++) {
		checkInsideCollisionXYZ(innerBoundaryMin[i], innerBoundaryMax[i], cameraPos, targetPos);
	}
}


void PhysicsEngine::checkInsideCollisionXYZ(glm::vec3 innerBoundaryMin, glm::vec3 innerBoundaryMax, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//当身体处于碰撞体垂直区域范围内，才进行XZ平面的碰撞检测
	if (!(cameraPos.y + phoenixHeight <= innerBoundaryMin.y || cameraPos.y >= innerBoundaryMax.y)) {
		checkInsideCollisionXZ(innerBoundaryMin, innerBoundaryMax, cameraPos, targetPos);
	}
}



double Direction(glm::vec2 pi, glm::vec2 pj, glm::vec2 pk) {
	return (pk.x - pi.x)*(pj.y - pi.y) - (pj.x - pi.x)*(pk.y - pi.y);
}


bool OnSegment(glm::vec2 pi, glm::vec2 pj, glm::vec2 pk) {
	if ((min(pi.x, pj.x) <= pk.x) && (pk.x <= max(pi.x, pj.x))
		&& (min(pi.y, pj.y) <= pk.y) && (pk.y <= max(pi.y, pj.y)))
		return true;
	else return false;
}


//检测线段相交快速算法
bool SegmentIntersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
	int d1, d2, d3, d4;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3>0 && d4 < 0) || (d3 < 0 && d4>0)))
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	else
		return false;
}


void PhysicsEngine::checkInsideCollisionXZ(glm::vec3 innerBoundaryMin, glm::vec3 innerBoundaryMax, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	float tarX = targetPos.x, camX = cameraPos.x, tarZ = targetPos.z, camZ = cameraPos.z;
	float len = sqrt((camX - tarX)*(camX - tarX) + (camZ - tarZ)*(camZ - tarZ));

	glm::vec2 camPoint(camX, camZ), tarPoint(tarX, tarZ);
	glm::vec2 leftBack(innerBoundaryMin.x, innerBoundaryMin.z), leftFront(innerBoundaryMin.x, innerBoundaryMax.z);
	glm::vec2 rightBack(innerBoundaryMax.x, innerBoundaryMin.z), rightFront(innerBoundaryMax.x, innerBoundaryMax.z);

	if (SegmentIntersect(camPoint, tarPoint, leftFront, rightFront)) {
		if (targetPos.z < cameraPos.z) {
			printf("1\n");

			//利用相似三角形原理计算，
			//仅改变z坐标
			targetPos.z = innerBoundaryMax.z;
			cameraPos.z += (targetPos.z - tarZ);
		}
		else if (targetPos.z > cameraPos.z) {
			printf("2\n");
			cameraPos.z = innerBoundaryMax.z;
			targetPos.z += (cameraPos.z - camZ);
		}
	}
	else if (SegmentIntersect(camPoint, tarPoint, rightBack, rightFront)) {
		if (targetPos.x < cameraPos.x) {
			printf("3\n");
			targetPos.x = innerBoundaryMax.x;
			cameraPos.x += (targetPos.x - tarX);
		}
		else if (targetPos.x > cameraPos.x) {
			printf("4\n");
			cameraPos.x = innerBoundaryMax.x;
			targetPos.x += (cameraPos.x - camX);
		}
	}
	else if (SegmentIntersect(camPoint, tarPoint, leftBack, rightBack)) {
		if (targetPos.z > cameraPos.z) {
			printf("5\n");
			targetPos.z = innerBoundaryMin.z;
			cameraPos.z += (targetPos.z - tarZ);
		}
		else if (targetPos.z < cameraPos.z) {
			printf("6\n");
			cameraPos.z = innerBoundaryMin.z;
			targetPos.z += (cameraPos.z - camZ);
		}
	}
	else if (SegmentIntersect(camPoint, tarPoint, leftBack, leftFront)) {
		if (targetPos.x > cameraPos.x) {
			printf("7\n");
			targetPos.x = innerBoundaryMin.x;
			cameraPos.x += (targetPos.x - tarX);
		}
		else if (targetPos.x < cameraPos.x) {
			printf("8\n");
			cameraPos.x = innerBoundaryMin.x;
			targetPos.x += (cameraPos.x - camX);
		}
	}
}



void PhysicsEngine::jumpAndUpdateVelocity() {
	velocity += glm::vec3(0.f, JumpInitialSpeed, 0.f);
	accelerationUp.y = 0.f;
}


//判断在xz平面，相机位置是否位于碰撞体内部
bool insideTheCollider(glm::vec3 _cameraPos, glm::vec3 _innerMin, glm::vec3 _innerMax) {
	float camX = _cameraPos.x;
	float camZ = _cameraPos.z;
	float minX = _innerMin.x;
	float minZ = _innerMin.z;
	float maxX = _innerMax.x;
	float maxZ = _innerMax.z;

	if (minX <= camX && camX <= maxX && minZ <= camZ && camZ <= maxZ)
		return true;
	else
		return false;
}

void PhysicsEngine::updateItemMovement(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	glm::vec3 acceleration = gravity + accelerationUp;
	velocity += acceleration * GravityFactor;
	cameraPos += velocity * JumpFactor;
	targetPos += velocity * JumpFactor;

	if (cameraPos.y < 2.0f) {
		cout << cameraPos.y << endl;
	}
	cout << cameraPos.y << endl;
	//检测所有碰撞体
	for (int i = 0; i < innerBoundaryMin.size(); i++) {
		//如果在XZ平面进入碰撞体所在区域
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i])) {
			if (cameraPos.y <= innerBoundaryMax[i][1]
				&& cameraPos.y + phoenixHeight >= innerBoundaryMax[i][1]) {              // 脚接触到碰撞体顶部
																		 // cout << "touch the top of collider" << endl;
				isJumping = false;
				accelerationUp.y = -GravityAcceleration;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i][1];
				break;
			}

			if (cameraPos.y + phoenixHeight >= innerBoundaryMin[i][1] &&
				cameraPos.y <= innerBoundaryMin[i][1]) {    // 头接触到碰撞体底部
																		  // cout << "touch the bottom of collider" << endl;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMin[i][1];
				break;
			}
		}
		else {
			accelerationUp.y = 0.f;
		}
	}
}