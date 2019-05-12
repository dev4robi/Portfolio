#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CollisionObject(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 충돌 오브젝트들의 최상위 클레스.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : 클래스 기초 정의.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class CollisionObject;
extern list<CollisionObject*> g_colList; // 모든 충돌감지 오브젝트들을 보관하기 위한 전역 리스트

class CollisionObject : public GameObject {
public:
	enum collision_type {
		COL_UNDEFINED, COL_SPHERE, COL_AABB, COL_OBB
	};

protected:
	int ownerNum;				// 소유주 번호
	Vector3 pivot;				// 피벗 좌표
	Vector3 offset;				// 오프셋 정보
	Vector3 center;				// 중심 좌표 (충돌을 체크할 실제 중심좌표)
	Quaternion transform;		// 쿼터니언 트랜스폼
	collision_type type;		// 충돌 체크 종류

public:
	CollisionObject(int owner, Vector3 _pivot, Vector3 _offset);	// 생성자
	CollisionObject(CollisionObject &obj);							// 복사 생성자
	~CollisionObject();												// 소멸자

	Quaternion getTransform();			// 오브젝트의 트랜스폼 쿼터니언 반환
	int getOwnerNum();					// 소유주 번호를 반환
	void setOwnerNum(int num);			// 소유주 번호를 설정
	Vector3 getCenter();				// 오브젝트 중심을 반환
	void setCenter(Vector3 pos);		// 오브젝트 중심을 설정
	collision_type getType();			// 충돌 오브젝트 종류를 반환

	virtual void draw();											// 충돌 영역을 그림(가상 함수)
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// 충돌체 center 좌표를 업데이트
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Collision_Sphere(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 구형 충돌 확인을 담당.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : 클래스 기초 정의.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Collision_Sphere : public CollisionObject {
protected:
	REAL radius;		// 구체 충돌 반경(반지름)
	
public:
	Collision_Sphere(int owner, Vector3 _pivot, Vector3 _offset, REAL rad);	// 생성자
	Collision_Sphere(Collision_Sphere &obj);								// 복사 생성자
	~Collision_Sphere();													// 소멸자
	
	void setRadius(REAL rad);							// 반지름 설정
	REAL getRadius();									// 반지름 반환
	bool detectCollision(Vector3 vec);					// 벡터와 충돌하는지 여부를 반환 (충돌시: true)
	bool detectCollision(Collision_Sphere *pSphere);	// 다른 구체와 충돌하는지 여부를 반환 (충돌시: true)

	virtual void draw();											// 충돌 구체를 그려줌
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// 충돌감지 구체 center 좌표를 업데이트
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Collision_Sphere(v1)
*	- Last Update : 2016.07.28
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 구형 충돌 확인을 담당.
*
*	- Dependency : [StdHeader.h]
*
*	- Version(v1 - 16.07.28) : 클래스 기초 정의.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Collision_OBB : public CollisionObject {
protected:
	Vector3 lengthXYZ;		// 충돌 상자의 폭과 너비 (X, Y, Z)

public:
	Collision_OBB(int owner, Vector3 _pivot, Vector3 _offset, Vector3 length);	// 생성자
	Collision_OBB(Collision_OBB &obj);											// 복사 생성자
	~Collision_OBB();															// 소멸자

	bool detectCollision(Collision_OBB *pOBB);		// OBB끼리의 충돌을 탐지 (충돌 시 true 반환)
	Vector3 getLengthXYZ();							// OBB의 폭, 너비, 높이 정보를 담은 벡터를 반환

	virtual void draw();											// 충돌 구체를 그려줌
	virtual void update(Vector3 newPivot, Quaternion *pNewTrans);	// 충돌감지 구체 center 좌표를 업데이트
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

#endif