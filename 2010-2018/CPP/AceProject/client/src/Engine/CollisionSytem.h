#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CollisionObject(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �浹 ������Ʈ���� �ֻ��� Ŭ����.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : Ŭ���� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class CollisionObject;
extern list<CollisionObject*> g_colList; // ��� �浹���� ������Ʈ���� �����ϱ� ���� ���� ����Ʈ

class CollisionObject : public GameObject {
public:
	enum collision_type {
		COL_UNDEFINED, COL_SPHERE, COL_AABB, COL_OBB
	};

protected:
	int ownerNum;				// ������ ��ȣ
	Vector3 pivot;				// �ǹ� ��ǥ
	Vector3 offset;				// ������ ����
	Vector3 center;				// �߽� ��ǥ (�浹�� üũ�� ���� �߽���ǥ)
	Quaternion transform;		// ���ʹϾ� Ʈ������
	collision_type type;		// �浹 üũ ����

public:
	CollisionObject(int owner, Vector3 _pivot, Vector3 _offset);	// ������
	CollisionObject(CollisionObject &obj);							// ���� ������
	~CollisionObject();												// �Ҹ���

	Quaternion getTransform();			// ������Ʈ�� Ʈ������ ���ʹϾ� ��ȯ
	int getOwnerNum();					// ������ ��ȣ�� ��ȯ
	void setOwnerNum(int num);			// ������ ��ȣ�� ����
	Vector3 getCenter();				// ������Ʈ �߽��� ��ȯ
	void setCenter(Vector3 pos);		// ������Ʈ �߽��� ����
	collision_type getType();			// �浹 ������Ʈ ������ ��ȯ

	virtual void draw();											// �浹 ������ �׸�(���� �Լ�)
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// �浹ü center ��ǥ�� ������Ʈ
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Collision_Sphere(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �浹 Ȯ���� ���.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : Ŭ���� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Collision_Sphere : public CollisionObject {
protected:
	REAL radius;		// ��ü �浹 �ݰ�(������)
	
public:
	Collision_Sphere(int owner, Vector3 _pivot, Vector3 _offset, REAL rad);	// ������
	Collision_Sphere(Collision_Sphere &obj);								// ���� ������
	~Collision_Sphere();													// �Ҹ���
	
	void setRadius(REAL rad);							// ������ ����
	REAL getRadius();									// ������ ��ȯ
	bool detectCollision(Vector3 vec);					// ���Ϳ� �浹�ϴ��� ���θ� ��ȯ (�浹��: true)
	bool detectCollision(Collision_Sphere *pSphere);	// �ٸ� ��ü�� �浹�ϴ��� ���θ� ��ȯ (�浹��: true)

	virtual void draw();											// �浹 ��ü�� �׷���
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// �浹���� ��ü center ��ǥ�� ������Ʈ
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Collision_Sphere(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �浹 Ȯ���� ���.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : Ŭ���� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Collision_OBB : public CollisionObject {
protected:
	Vector3 lengthXYZ;		// �浹 ������ ���� �ʺ� (X, Y, Z)

public:
	Collision_OBB(int owner, Vector3 _pivot, Vector3 _offset, Vector3 length);	// ������
	Collision_OBB(Collision_OBB &obj);											// ���� ������
	~Collision_OBB();															// �Ҹ���

	bool detectCollision(Collision_OBB *pOBB);		// OBB������ �浹�� Ž�� (�浹 �� true ��ȯ)
	Vector3 getLengthXYZ();							// OBB�� ��, �ʺ�, ���� ������ ���� ���͸� ��ȯ

	virtual void draw();											// �浹 ��ü�� �׷���
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// �浹���� ��ü center ��ǥ�� ������Ʈ
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

#endif