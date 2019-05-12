#include "Engine/CollisionSytem.h"

/*************************************************************************************************************/
/********************************************** CollisionObject **********************************************/
/*************************************************************************************************************/

list<CollisionObject*> g_colList;

CollisionObject::CollisionObject(int owner, Vector3 _pivot, Vector3 _offset) : GameObject() {
	type = collision_type::COL_UNDEFINED;
	ownerNum = owner;
	pivot = _pivot;
	offset = _offset;
	center = pivot + offset;
	transform.identity();
	
	//g_colList.push_back(this); 그리기 작업을 수행하려면 주석 해제
}

CollisionObject::CollisionObject(CollisionObject &obj) : GameObject(obj) {
	ownerNum = obj.ownerNum;
	center = obj.center;
	type = obj.type;
	
	//g_colList.push_back(this); 그리기 작업을 수행하려면 주석 해제
}

CollisionObject::~CollisionObject() { 
	//g_colList.remove(this); 그리기 작업을 수행하려면 주석 해제
}

Quaternion CollisionObject::getTransform() {
	return transform;
}

int CollisionObject::getOwnerNum() {
	return ownerNum;
}

void CollisionObject::setOwnerNum(int num) {
	ownerNum = num;
}

Vector3 CollisionObject::getCenter() {
	return center;
}

void CollisionObject::setCenter(Vector3 pos) {
	center = pos;
}

CollisionObject::collision_type CollisionObject::getType() {
	return type;
}

void CollisionObject::draw() {
	// 가상 함수 . . . 
}

void CollisionObject::update(Vector3 newPivot, Quaternion *pNewTrans) {
	// 가상 함수 . . .
}

/*************************************************************************************************************/
/********************************************** Collision_Sphere *********************************************/
/*************************************************************************************************************/

Collision_Sphere::Collision_Sphere(int owner, Vector3 _pivot, Vector3 _offset, REAL rad) : CollisionObject(owner, _pivot, _offset) {
	type = collision_type::COL_SPHERE;
	radius = rad;
}

Collision_Sphere::Collision_Sphere(Collision_Sphere &obj) : CollisionObject(obj) {
	type = obj.type;
	radius = obj.radius;
}

Collision_Sphere::~Collision_Sphere() { }

void Collision_Sphere::setRadius(REAL rad) {
	radius = rad;
}

REAL Collision_Sphere::getRadius() {
	return radius;
}

bool Collision_Sphere::detectCollision(Vector3 vec) {
	// 벡터(점)의 위치와 구체 중심의 위치의 거리가 반지름보다 작거나 같으면 충돌
	if ((vec - center).magnitude() <= radius) {
		return true;
	}

	return false;
}

bool Collision_Sphere::detectCollision(Collision_Sphere *pSphere) {
	// 구체와 구체 사이의 거리가 각 구체의 반지름을 더한 것보다 작거나 같으면 충돌
	if (((pSphere->center - center).magnitude()) <= (pSphere->radius + radius)) {
		return true;
	}

	return false;
}

void Collision_Sphere::draw() {
	glMatrixMode(GL_MODELVIEW);

	// 월드 좌표 이동
	glTranslatef(center.x, center.y, center.z);

	// 회전 행렬 매트릭스
	glMultMatrixf(&transform.toMatrix4()[0][0]);

	// 와이어프레임 구체 그리기
	glColor4f(1.0, 0.5, 0.153, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLUquadric *pObj = gluNewQuadric();
	gluSphere(pObj, radius, 20, 20);
	gluDeleteQuadric(pObj);
	pObj = NULL;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Collision_Sphere::update(Vector3 newPivot, Quaternion *pNewTrans) {
	pivot = newPivot;
	transform = (*pNewTrans);
	
	Matrix4 transMat = transform.toMatrix4();
	center = pivot + (offset * transMat);
}

/*************************************************************************************************************/
/*********************************************** Collision_OBB ***********************************************/
/*************************************************************************************************************/

Collision_OBB::Collision_OBB(int owner, Vector3 _pivot, Vector3 _offset, Vector3 length) : CollisionObject(owner, _pivot, _offset) {
	type = collision_type::COL_OBB;
	lengthXYZ = length;
}

Collision_OBB::Collision_OBB(Collision_OBB &obj) : CollisionObject(obj) {
	type = obj.type;
	lengthXYZ = obj.lengthXYZ;
}

Collision_OBB::~Collision_OBB() {

}

Vector3 Collision_OBB::getLengthXYZ() {
	return lengthXYZ;
}

void Collision_OBB::draw() {
	glMatrixMode(GL_MODELVIEW);

	// 월드 좌표 이동
	glTranslatef(center.x, center.y, center.z);

	// 회전 행렬 매트릭스
	glMultMatrixf(&transform.toMatrix4()[0][0]);

	// 와이어프레임 육면체 그리기
	glColor4f(1.0, 0.5, 0.153, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	REAL x = lengthXYZ.x, y = lengthXYZ.y, z = lengthXYZ.z;
	glBegin(GL_QUADS);
	{
		glVertex3f(-x,  y, -z); // 상
		glVertex3f(-x,  y,  z);
		glVertex3f( x,  y,  z);
		glVertex3f( x,  y, -z);

		glVertex3f(-x,  y, -z); // 후
		glVertex3f(-x, -y, -z);
		glVertex3f( x, -y, -z);
		glVertex3f( x,  y, -z);

		glVertex3f(-x,  y, -z); // 좌
		glVertex3f(-x, -y, -z);
		glVertex3f(-x, -y,  z);
		glVertex3f(-x,  y,  z);

		glVertex3f(-x,  y,  z); // 앞
		glVertex3f(-x, -y,  z);
		glVertex3f( x, -y,  z);
		glVertex3f( x,  y,  z);

		glVertex3f( x,  y,  z); // 우
		glVertex3f( x, -y,  z);
		glVertex3f( x, -y, -z);
		glVertex3f( x,  y, -z);

		glVertex3f(-x, -y,  z); // 하
		glVertex3f(-x, -y, -z);
		glVertex3f( x, -y, -z);
		glVertex3f( x, -y,  z);
	}
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Collision_OBB::update(Vector3 newPivot, Quaternion *pNewTrans) {
	pivot = newPivot;
	transform = (*pNewTrans);

	Matrix4 transMat = transform.toMatrix4();
	center = pivot + (offset * transMat);
}

bool Collision_OBB::detectCollision(Collision_OBB *pOBB) {
/*	// OLD CODE //
	// compute difference of box centers,D=C1-C0
	Vector3 dist(center - pOBB->center);

	REAL C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	REAL absC[3][3]; //|c_{ij}|
	REAL AD[3];      //Dot(A_i,D)
	REAL R0, R1, R;    //interval radii and distance between centers
	REAL R01;        //=R0+R1


	Matrix4 axis1 = transform.toMatrix4(), axis2 = pOBB->transform.toMatrix4();

	//A0
	C[0][0] = Vector3::dot(Vector3(axis1[0][0], axis1[0][1], axis1[0][2]), Vector3(axis2[0][0], axis2[0][1], axis2[0][2]));
	C[0][1] = Vector3::dot(Vector3(axis1[0][0], axis1[0][1], axis1[0][2]), Vector3(axis2[1][0], axis2[1][1], axis2[1][2]));
	C[0][2] = Vector3::dot(Vector3(axis1[0][0], axis1[0][1], axis1[0][2]), Vector3(axis2[2][0], axis2[2][1], axis2[2][2]));
	AD[0] = Vector3::dot(Vector3(axis1[0][0], axis1[0][1], axis1[0][2]), dist);
	absC[0][0] = (REAL)fabsf(C[0][0]);
	absC[0][1] = (REAL)fabsf(C[0][1]);
	absC[0][2] = (REAL)fabsf(C[0][2]);
	R = (REAL)fabsf(AD[0]);
	R1 = lengthXYZ.x * absC[0][0] + pOBB->lengthXYZ.y * absC[0][1] + pOBB->lengthXYZ.z * absC[0][2];
	R01 = lengthXYZ.x + R1;
	if (R > R01) return false;

	//A1
	C[1][0] = Vector3::dot(Vector3(axis1[1][0], axis1[1][1], axis1[1][2]), Vector3(axis2[0][0], axis2[0][1], axis2[0][2]));
	C[1][1] = Vector3::dot(Vector3(axis1[1][0], axis1[1][1], axis1[1][2]), Vector3(axis2[1][0], axis2[1][1], axis2[1][2]));
	C[1][2] = Vector3::dot(Vector3(axis1[1][0], axis1[1][1], axis1[1][2]), Vector3(axis2[2][0], axis2[2][1], axis2[2][2]));
	AD[1] = Vector3::dot(Vector3(axis1[1][0], axis1[1][1], axis1[1][2]), dist);
	absC[1][0] = (REAL)fabsf(C[1][0]);
	absC[1][1] = (REAL)fabsf(C[1][1]);
	absC[1][2] = (REAL)fabsf(C[1][2]);
	R = (REAL)fabsf(AD[1]);
	R1 = lengthXYZ.x * absC[1][0] + pOBB->lengthXYZ.y * absC[1][1] + pOBB->lengthXYZ.z * absC[1][2];
	R01 = lengthXYZ.y + R1;
	if (R > R01) return false;

	//A2
	C[2][0] = Vector3::dot(Vector3(axis1[2][0], axis1[2][1], axis1[2][2]), Vector3(axis2[0][0], axis2[0][1], axis2[0][2]));
	C[2][1] = Vector3::dot(Vector3(axis1[2][0], axis1[2][1], axis1[2][2]), Vector3(axis2[1][0], axis2[1][1], axis2[1][2]));
	C[2][2] = Vector3::dot(Vector3(axis1[2][0], axis1[2][1], axis1[2][2]), Vector3(axis2[2][0], axis2[2][1], axis2[2][2]));
	AD[2] = Vector3::dot(Vector3(axis1[2][0], axis1[2][1], axis1[2][2]), dist);
	absC[2][0] = (REAL)fabsf(C[2][0]);
	absC[2][1] = (REAL)fabsf(C[2][1]);
	absC[2][2] = (REAL)fabsf(C[2][2]);
	R = (REAL)fabsf(AD[2]);
	R1 = lengthXYZ.x * absC[2][0] + pOBB->lengthXYZ.y * absC[2][1] + pOBB->lengthXYZ.z * absC[2][2];
	R01 = lengthXYZ.z + R1;
	if (R > R01) return false;

	//B0
	R = (REAL)fabsf(Vector3::dot(Vector3(axis1[0][0], axis1[0][1], axis1[0][2]), dist));
	R0 = lengthXYZ.x * absC[0][0] + lengthXYZ.y * absC[1][0] + lengthXYZ.z * absC[2][0];
	R01 = R0 + pOBB->lengthXYZ.x;
	if (R > R01) return false;

	//B1
	R = (REAL)fabsf(Vector3::dot(Vector3(axis1[1][0], axis1[1][1], axis1[1][2]), dist));
	R0 = lengthXYZ.x * absC[0][1] + lengthXYZ.y * absC[1][1] + lengthXYZ.z * absC[2][1];
	R01 = R0 + pOBB->lengthXYZ.y;
	if (R > R01) return false;

	//B2
	R = (REAL)fabsf(Vector3::dot(Vector3(axis1[2][0], axis1[2][1], axis1[2][2]), dist));
	R0 = lengthXYZ.x * absC[0][2] + lengthXYZ.y * absC[1][2] + lengthXYZ.z * absC[2][2];
	R01 = R0 + pOBB->lengthXYZ.z;
	if (R > R01) return false;

	//A0xB0
	R = (REAL)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = lengthXYZ.y * absC[2][0] + lengthXYZ.z * absC[1][0];
	R1 = pOBB->lengthXYZ.y * absC[0][2] + pOBB->lengthXYZ.z * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A0xB1
	R = (REAL)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = lengthXYZ.y * absC[2][1] + lengthXYZ.z * absC[1][1];
	R1 = pOBB->lengthXYZ.x * absC[0][2] + pOBB->lengthXYZ.z * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A0xB2
	R = (REAL)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = lengthXYZ.y * absC[2][2] + lengthXYZ.z * absC[1][2];
	R1 = pOBB->lengthXYZ.x * absC[0][1] + pOBB->lengthXYZ.y * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB0
	R = (REAL)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = lengthXYZ.x * absC[2][0] + lengthXYZ.z * absC[0][0];
	R1 = pOBB->lengthXYZ.y * absC[1][2] + pOBB->lengthXYZ.z * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB1
	R = (REAL)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = lengthXYZ.x * absC[2][1] + lengthXYZ.z * absC[0][1];
	R1 = pOBB->lengthXYZ.x * absC[1][2] + pOBB->lengthXYZ.z * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB2
	R = (REAL)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = lengthXYZ.x * absC[2][2] + lengthXYZ.z * absC[0][2];
	R1 = pOBB->lengthXYZ.x * absC[1][1] + pOBB->lengthXYZ.y * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB0
	R = (REAL)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = lengthXYZ.x * absC[1][0] + lengthXYZ.y * absC[0][0];
	R1 = pOBB->lengthXYZ.y * absC[2][2] + pOBB->lengthXYZ.z * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB1
	R = (REAL)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = lengthXYZ.x * absC[1][1] + lengthXYZ.y * absC[0][1];
	R1 = pOBB->lengthXYZ.x * absC[2][2] + pOBB->lengthXYZ.z * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB2
	R = (REAL)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = lengthXYZ.x * absC[1][2] + lengthXYZ.y * absC[0][2];
	R1 = pOBB->lengthXYZ.x * absC[2][1] + pOBB->lengthXYZ.y * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	return true;
*/
	// NEW CODE //
	Vector3 dist(center - pOBB->center); // compute difference of box centers, dist = (this->center - pOBB->center)
	REAL C[3][3];		// matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	REAL absC[3][3];	// |c_{ij}|
	REAL AD[3];			// Dot(A_i,D)
	REAL R0, R1, R;		// interval radii and distance between centers
	REAL R01;			// =R0+R1
	Matrix4 axis1 = transform.toMatrix4(), axis2 = pOBB->transform.toMatrix4();
	Vector3 axis1Vec1(axis1[0][0], axis1[0][1], axis1[0][2]), axis1Vec2(axis1[1][0], axis1[1][1], axis1[1][2]), axis1Vec3(axis1[2][0], axis1[2][1], axis1[2][2]);
	Vector3 axis2Vec1(axis2[0][0], axis2[0][1], axis2[0][2]), axis2Vec2(axis2[1][0], axis2[1][1], axis2[1][2]), axis2Vec3(axis2[2][0], axis2[2][1], axis2[2][2]);
	
	//A0
	C[0][0] = Vector3::dot(axis1Vec1, axis2Vec1);
	C[0][1] = Vector3::dot(axis1Vec1, axis2Vec2);
	C[0][2] = Vector3::dot(axis1Vec1, axis2Vec3);
	AD[0] = Vector3::dot(axis1Vec1, dist);
	absC[0][0] = (REAL)fabsf(C[0][0]);
	absC[0][1] = (REAL)fabsf(C[0][1]);
	absC[0][2] = (REAL)fabsf(C[0][2]);
	R = (REAL)fabsf(AD[0]);
	R1 = lengthXYZ.x * absC[0][0] + pOBB->lengthXYZ.y * absC[0][1] + pOBB->lengthXYZ.z * absC[0][2];
	R01 = lengthXYZ.x + R1;
	if (R > R01) return false;

	//A1
	C[1][0] = Vector3::dot(axis1Vec2, axis2Vec1);
	C[1][1] = Vector3::dot(axis1Vec2, axis2Vec2);
	C[1][2] = Vector3::dot(axis1Vec2, axis2Vec3);
	AD[1] = Vector3::dot(axis1Vec2, dist);
	absC[1][0] = (REAL)fabsf(C[1][0]);
	absC[1][1] = (REAL)fabsf(C[1][1]);
	absC[1][2] = (REAL)fabsf(C[1][2]);
	R = (REAL)fabsf(AD[1]);
	R1 = lengthXYZ.x * absC[1][0] + pOBB->lengthXYZ.y * absC[1][1] + pOBB->lengthXYZ.z * absC[1][2];
	R01 = lengthXYZ.y + R1;
	if (R > R01) return false;

	//A2
	C[2][0] = Vector3::dot(axis1Vec3, axis2Vec1);
	C[2][1] = Vector3::dot(axis1Vec3, axis2Vec2);
	C[2][2] = Vector3::dot(axis1Vec3, axis2Vec3);
	AD[2] = Vector3::dot(axis1Vec3, dist);
	absC[2][0] = (REAL)fabsf(C[2][0]);
	absC[2][1] = (REAL)fabsf(C[2][1]);
	absC[2][2] = (REAL)fabsf(C[2][2]);
	R = (REAL)fabsf(AD[2]);
	R1 = lengthXYZ.x * absC[2][0] + pOBB->lengthXYZ.y * absC[2][1] + pOBB->lengthXYZ.z * absC[2][2];
	R01 = lengthXYZ.z + R1;
	if (R > R01) return false;

	//B0
	R = (REAL)fabsf(Vector3::dot(axis1Vec1, dist));
	R0 = lengthXYZ.x * absC[0][0] + lengthXYZ.y * absC[1][0] + lengthXYZ.z * absC[2][0];
	R01 = R0 + pOBB->lengthXYZ.x;
	if (R > R01) return false;

	//B1
	R = (REAL)fabsf(Vector3::dot(axis1Vec2, dist));
	R0 = lengthXYZ.x * absC[0][1] + lengthXYZ.y * absC[1][1] + lengthXYZ.z * absC[2][1];
	R01 = R0 + pOBB->lengthXYZ.y;
	if (R > R01) return false;

	//B2
	R = (REAL)fabsf(Vector3::dot(axis1Vec1, dist));
	R0 = lengthXYZ.x * absC[0][2] + lengthXYZ.y * absC[1][2] + lengthXYZ.z * absC[2][2];
	R01 = R0 + pOBB->lengthXYZ.z;
	if (R > R01) return false;

	//A0xB0
	R = (REAL)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = lengthXYZ.y * absC[2][0] + lengthXYZ.z * absC[1][0];
	R1 = pOBB->lengthXYZ.y * absC[0][2] + pOBB->lengthXYZ.z * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A0xB1
	R = (REAL)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = lengthXYZ.y * absC[2][1] + lengthXYZ.z * absC[1][1];
	R1 = pOBB->lengthXYZ.x * absC[0][2] + pOBB->lengthXYZ.z * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A0xB2
	R = (REAL)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = lengthXYZ.y * absC[2][2] + lengthXYZ.z * absC[1][2];
	R1 = pOBB->lengthXYZ.x * absC[0][1] + pOBB->lengthXYZ.y * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB0
	R = (REAL)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = lengthXYZ.x * absC[2][0] + lengthXYZ.z * absC[0][0];
	R1 = pOBB->lengthXYZ.y * absC[1][2] + pOBB->lengthXYZ.z * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB1
	R = (REAL)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = lengthXYZ.x * absC[2][1] + lengthXYZ.z * absC[0][1];
	R1 = pOBB->lengthXYZ.x * absC[1][2] + pOBB->lengthXYZ.z * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A1xB2
	R = (REAL)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = lengthXYZ.x * absC[2][2] + lengthXYZ.z * absC[0][2];
	R1 = pOBB->lengthXYZ.x * absC[1][1] + pOBB->lengthXYZ.y * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB0
	R = (REAL)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = lengthXYZ.x * absC[1][0] + lengthXYZ.y * absC[0][0];
	R1 = pOBB->lengthXYZ.y * absC[2][2] + pOBB->lengthXYZ.z * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB1
	R = (REAL)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = lengthXYZ.x * absC[1][1] + lengthXYZ.y * absC[0][1];
	R1 = pOBB->lengthXYZ.x * absC[2][2] + pOBB->lengthXYZ.z * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	//A2xB2
	R = (REAL)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = lengthXYZ.x * absC[1][2] + lengthXYZ.y * absC[0][2];
	R1 = pOBB->lengthXYZ.x * absC[2][1] + pOBB->lengthXYZ.y * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) return false;

	return true;
}
