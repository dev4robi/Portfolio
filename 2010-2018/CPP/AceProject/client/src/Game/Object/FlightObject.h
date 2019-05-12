#ifndef __FLIGHT_OBJECT_H__
#define __FLIGHT_OBJECT_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : FlightObject.h
*	- Last Update : 2016.05.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 비행체(전투기, 미사일)에 대한 기초 클래스.
*
*	- Dependency : [StdHeader.h, GameWidget.h]
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"
#include "Engine/CollisionSytem.h"

class Player;

class FlightObject : public GameWidget {
public:
	// 상태 열거형
	enum object_state { // 생존, 사망 등의 상태 열거형
		STATE_ALIVE, STATE_DEAD
	};

	// 기본 정보
	Player *pOwningPlayer;		// 소유주 플레이어 포인터

	// 모델 관련 정보
	GLMmodel *pModel;			// 모델 정보
	REAL rotateOffset[3];		// 회전 오프셋 x(pitch), y(yaw), z(roll)
	REAL pivotOffset[3];		// 피봇 오프셋 x, y, z
	REAL scale[3];				// 크기 x, y, z
	REAL color[4];				// 색상 r, g, b, a	

	// 비행체 관련 정보
	Vector3 directVector;		// 비행기의 비행 방향 벡터
	Vector3 upVector;			// 비행기의 비행 방향과 수직인 방향 벡터
	Vector3 rightVector;		// 비행기의 방향 벡터/업벡터와 수평 방향 벡터
	Quaternion transform;		// 비행체 회전 매트릭스를 위한 사원수
	REAL pitch, yaw, roll;		// 피치각 요각 롤각 (updatePitchRollYaw 함수로 갱신)
	REAL HP, maxHP;				// 비행체 생명력

	// 비행체 상태 관련 정보
	object_state objectState;	// 비행체 상태 정보
	bool isAccelerating;		// 비행체 가속 중
	bool isDeaccelerating;		// 비행체 감속 중

	// 비행체 속도 관련 정보
	REAL currentSpeed, maxSpeed;		// 현재 속도, 최대 속도
	REAL currentEngPower;				// 현재 엔진 파워 (0.0 ~ 100.0)
	REAL currentBoost;					// 남은 부스터 수치 (0.0 ~ 100.0)
	REAL boostLeftTime;					// 부스터 남은 시간
	REAL engPowerInc, engPowerDec;		// 초당 파워 초당 증감분
	REAL engOfferSpeed;					// 엔진 파워가 최고일 때 엔진이 제공하는 초당 속도 (currentSpeed = engOfferSpeed * currentEngPower)

	// 비행체 관련 상수
	REAL default_pitchTurn;				// 피치 변경 최대 속도
	REAL default_yawTurn;				// 요 변경 최대 속도
	REAL default_rollTurn;				// 롤 변경 최대 속도
	REAL default_maxSpeed;				// 기본 - 엔진 속도
	REAL default_engOfferSpeed;			// 기본 - 엔진 제공 속도
	REAL default_boostOfferTime;		// 기본 - 부스터 지속 시간

	// 충돌 관련 객체
	vector<CollisionObject*> collisionAry; // 충돌체 배열

	// 여기에 속성 계속 추가 . . .
public:
	// Cons & Des
	FlightObject(Vector3 pos);			// 생성자
	FlightObject(FlightObject& _obj);	// 복사 생성자
	~FlightObject();					// 소멸자

	// Method
	void acceleration(REAL timeElapsed);				// 비행체 엔진 가속
	void deacceleration(REAL timeElapsed);				// 속도 감속
	void boosting(REAL timeElapsed);					// 부스터 사용
	void pitching(REAL angle, bool spdDecomp = false);	// angle만큼 피칭
	void yawing(REAL angle, bool spdDecomp = false);	// angle만큼 요잉
	void rolling(REAL angle, bool spdDecomp = false);	// angle만큼 롤링
	object_state getState();							// 비행체 상태 반환
	void setState(object_state state);					// 비행체 상태 설정
	REAL dealtDamage(FlightObject *pTgt, REAL amount, bool suicide, bool msgToNetwork = true);	// 비행체에게 피해를 입히고 피해입은 비행체의 남은 생명력을 반환
	void dealtDamage_Net(short targetPlayerNum, REAL amount, bool suicide);						// 피해입은 대상, 피해량, 자살 여부등을 네트워크 패킷에 채워넣음 (네트워크 동기화 메시지)
	void updateCollisionPos(Vector3 _position, Quaternion *_pTransform);						// 충돌체들 위치를 업데이트
	static REAL distanceBetweenObj(FlightObject *obj1, FlightObject *obj2);						// 두 비행체사이의 거리를 반환
	static bool isObjPlaceFront(FlightObject *obj1, FlightObject *obj2);						// 비행체 2가 비행체1 앞에 있으면 true반환
	Vector3 getUpdatedPosition(REAL timeElapsed, int cnt = 1);									// cnt번 업데이트된 위치(비행체의 다음 업데이트 위치)를 반환

protected:
	void updatePosition(REAL timeElapsed);			// 위치 정보 갱신
	void updatePitchRollYaw(REAL timeElapsed);		// 피치, 롤, 요 정보를 갱신
	void updateSpeed(REAL timeElapsed);				// 비행체의 속도 업데이트

private:
	void killFighter(FlightObject *pKiller);		// 전투기 파괴 (dealtDamage 함수에서 내부적으로 호출)

	// Virtual Func
	virtual bool checkObjectCollision();			// 충돌을 검사 (가상 함수로 클래스마다 별도의 충돌 검사 가능)
	virtual bool draw();							// 비행체(위젯)를 그림
	virtual bool destroy();							// 비행체를 파괴함
	virtual bool update(REAL timeElapsed);			// 비행체를 업데이트
};

#endif