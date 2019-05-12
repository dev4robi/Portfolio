#ifndef __MISSILE_H__
#define __MISSILE_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : Missile.h(v1)
*	- Last Update : 2016.07.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 미사일에 대한 기초 클래스.
*
*	- Dependency : [FlightObject.h], Weapon.h, UI.h, GL_Effect.h
*
*   - Version(v1): 미사일 클래스 기초 설계 (2016.07.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Game/Object/FlightObject.h"

class WpnLockonMissile;
class JetFighter;
class LockonUI;
class TrailEffect;
class JetFlameEffect;
class Weapon;

class Missile : public FlightObject {
private:
	bool isBulletStyle;				// 미사일 혹은 총알 구분자
	REAL guideDelayTime;			// 유도 기능이 시작되기 까지 남은 지연 시간
	JetFighter *pFireJet;			// 미사일을 발사한 전투기
	JetFighter *pLockonTgt;			// 유도 대상 비행체 포인터
	Weapon *pFireWeapon;			// 발사한 무기의 포인터
	REAL damage;					// 미사일 피해량
	REAL dmgAoE;					// 미사일 피해 반경
	REAL homingLevel;				// 미사일 유도력 정도로 수가 낮을수록 고속 유도성능 감소 (0.0 ~ )
	
	TrailEffect *pMsslTrailEffect;	// 미사일 비행 자국 이펙트
	JetFlameEffect *pFlameEffect;	// 미사일 점화 이펙트
	// 여기에 속성 계속 추가... (충돌박스, 이펙트 등)
public:
	Missile(JetFighter *pMissileOwner, LockonUI *pLockonUI, Weapon *pWeapon, bool bulletStyle = false);	// 생성자
	Missile(Missile& obj);																				// 복사 생성자
	~Missile();																							// 소멸자

	JetFighter* getLockonTgt();			// 락온 타겟 포인터 반환
	void setDamage(REAL dmg);			// 미사일 피해량 설정
	REAL getDamage();					// 피사일 피해량 반환
	void setDmgAoE(REAL AoE);			// 미사일 피해 반경 설정
	REAL getDmgAoE();					// 미사일 피해 반경 반환
	bool getStyle();					// 총알 스타일의 미사일이면 true 반환

	virtual bool checkObjectCollision();	// 미사일 충돌을 확인
	virtual bool draw();					// 미사일 그리기
	virtual bool destory();					// 미사일 파괴
	virtual bool update(REAL timeElapsed);	// 미사일 업데이트
};

#endif