#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	velocity = glm::vec3(0.f, 0.f, 0.f);
	gravity = glm::vec3(0.f, GravityAcceleration, 0.f);
	accelerationUp = glm::vec3(0.f, 0.f, 0.f);

	isJumping = false;
}


PhysicsEngine::~PhysicsEngine() {}


void PhysicsEngine::setSceneOuterBoundary(glm::vec2 leftBackPoint, glm::vec2 rightFrontPoint) {
	// ���ð�Χ�У��ȿռ��ⲿ��ԵСһ��
	leftBackPoint.x += BoundaryGap;
	leftBackPoint.y += BoundaryGap;
	rightFrontPoint.x -= BoundaryGap;
	rightFrontPoint.y -= BoundaryGap;

	outerBoundaryMin = leftBackPoint;
	outerBoundaryMax = rightFrontPoint;
}


void PhysicsEngine::setSceneInnerBoundary(glm::vec3 leftDownBackPoint, glm::vec3 rightUpFrontPoint) {
	// ���ð�Χ�У��ȿռ��ڲ���Ե��һ��
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
	// ���Ŀ��λ�ó��˰�Χ�У��ȷŻ���
	targetPos.x = targetPos.x < outerBoundaryMin.x ? outerBoundaryMin.x : targetPos.x > outerBoundaryMax.x ? outerBoundaryMax.x : targetPos.x;
	targetPos.z = targetPos.z < outerBoundaryMin.y ? outerBoundaryMin.y : targetPos.z > outerBoundaryMax.y ? outerBoundaryMax.y : targetPos.z;
	
	float distance = sqrt((cameraPos[0] - targetPos[0]) * (cameraPos[0] - targetPos[0]) +
		(cameraPos[2] - targetPos[2]) * (cameraPos[2] - targetPos[2]));

	// ���ӵ���Ŀ�����̫С����̶�Ŀ��λ�ã��ӵ�������Ŀ����淽���ƶ�
	if (distance <= 2.0f) {
		cameraPos[0] = 2.0f * (cameraPos[0] - targetPos[0]) / distance + targetPos[0];
		cameraPos[2] = 2.0f * (cameraPos[2] - targetPos[2]) / distance + targetPos[2];
	}
	bool flag = false;

	// �ڼ���ӵ��Ƿ���˰�Χ�С�����Ż�
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

	//�ظ�����Զ���������Ĳ���
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
	// ���������������ӣ�Ԥ�����ų���ǰ�϶����������ײ������
	for (int i = 0; i < innerBoundaryMin.size(); i++) {
		checkInsideCollisionXYZ(innerBoundaryMin[i], innerBoundaryMax[i], cameraPos, targetPos);
	}
}


void PhysicsEngine::checkInsideCollisionXYZ(glm::vec3 innerBoundaryMin, glm::vec3 innerBoundaryMax, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//�����崦����ײ�崹ֱ����Χ�ڣ��Ž���XZƽ�����ײ���
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


//����߶��ཻ�����㷨
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

			//��������������ԭ����㣬
			//���ı�z����
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


//�ж���xzƽ�棬���λ���Ƿ�λ����ײ���ڲ�
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
	//���������ײ��
	for (int i = 0; i < innerBoundaryMin.size(); i++) {
		//�����XZƽ�������ײ����������
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i])) {
			if (cameraPos.y <= innerBoundaryMax[i][1]
				&& cameraPos.y + phoenixHeight >= innerBoundaryMax[i][1]) {              // �ŽӴ�����ײ�嶥��
																		 // cout << "touch the top of collider" << endl;
				isJumping = false;
				accelerationUp.y = -GravityAcceleration;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i][1];
				break;
			}

			if (cameraPos.y + phoenixHeight >= innerBoundaryMin[i][1] &&
				cameraPos.y <= innerBoundaryMin[i][1]) {    // ͷ�Ӵ�����ײ��ײ�
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