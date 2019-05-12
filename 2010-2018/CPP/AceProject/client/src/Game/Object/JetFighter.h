#ifndef __JET_FIGHTER_H__
#define __JET_FIGHTER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : JetFighter.h(v2)
*	- Last Update : 2016.07.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 비행체(전투기, 미사일)에 대한 기초 클래스.
*
*	- Dependency : [FlightObject.h], Weapon.h, Player.h, UI.h
*
*   - Version(v1): 전투기 클래스 기초 설계 (2016.05.??)
*   - Version(v2): 전투기 클래스 멤버 추가. (2016.07.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Game/Object/FlightObject.h"

class Player;
class Weapon;
class WpnGatlingGun;
class WpnLockonMissile;
class TrailEffect;
class JetFlameEffect;
class Sound;
class GamePacket;

class JetFighter : public FlightObject {
public:
	enum jet_warning {
		JET_LOCKON_NONE, JET_LOCKON_BY_ENEMY, JET_LOCKON_BY_MISSILE
	};

protected:
	// 최대 고도
	static const int JET_MAX_ALTITUDE = 10000;

	// 기체명
	string craftName;				// 비행기 명칭

	// 경고 및 상태
	REAL jetOffcourseTime;			// 기체 코스 벗어난 경과 시간
	jet_warning jetWarningLockon;	// 기체 락온 경고 3상태
	bool jetWarningAltitude;		// 기체 낮은 고도 경고
	bool jetWarningStall;			// 기체 실속 경고

	// 레이더
	REAL radarRadius;				// 레이더 포착 범위
	list<FlightObject*> radarList;	// 레이더에 포착된 비행체 포인터 리스트

	// 무장
	WpnLockonMissile *pBaseWeapon;	// 주무장 (기관총)
	WpnLockonMissile *pFirstWeapon;	// 주무장 (미사일)

	// 리스폰
	bool jetUseRespawn;				// 부활 사용 여부
	REAL jetRespawnLeftTime;		// 부활 남은 시간
	REAL jetRespawnReqTime;			// 부활 요구 시간

	// 부가 정보
	bool jetIsRolling;				// 전투기 롤링 상태 여부
	REAL jetRollingTime;			// 전투기 롤링 시간

	// 이펙트
	TrailEffect *pWingTrail[2];		// 날개 트레일
	JetFlameEffect *pFlameLeft;		// 좌측 엔진 출력 불꽃
	JetFlameEffect *pFlameRight;	// 우측 엔진 출력 불꽃

	// 사운드
	Sound *pSndEngine;	// 엔진 출력 사운드 (루프)

public:
	JetFighter(Vector3 pos, Player *_player, char *_craftName);	// 생성자
	JetFighter(JetFighter& _obj);								// 복사 생성자
	~JetFighter();												// 소멸자

	// Getter & Setter
	string* getCraftNamePtr();						// 비행기 명칭 포인터를 반환
	list<FlightObject*> *getRadarListPtr();			// 레이더 리스트 포인터를 반환
	void setRespawnLeftTime(REAL time);				// 부활 대기 시간 설정
	REAL getRespawnLeftTime();						// 부활 대기 시간 반환
	REAL getRespawnReqTime();						// 부활 요구 시간 반환 
	REAL getOffCourseTime();						// 코스를 이탈한 시간을 반환
	void setOffCourseTime(REAL time);				// 코스를 이탈한 시간을 설정
	void setLockonWarningState(jet_warning state);	// 락온 경고 상태를 설정
	jet_warning getLockonWarningState();			// 락온 경고 상태를 반환
	void setAltitudeWarningState(bool warning);		// 고도 경고 상태를 설정
	bool getAltitudeWarningState();					// 고도 경고 상태를 반환
	void setStallWarningState(bool warning);		// 실속 경고 상태를 설정
	bool getStallWarningState();					// 실속 경고 상태를 반환
	void setRadarRadius(REAL rad);					// 레이더 반경 설정
	REAL getRadarRadius();							// 레이더 반경 반환
	WpnLockonMissile* getBaseWeaponPtr();			// 주무장(기관총) 포인터 반환
	WpnLockonMissile* getFirstWeaponPtr();			// 주무장(미사일) 포인터 반환
	bool getJetIsRolling();							// 전투기 롤링 여부 반환
	void setJetIsRolling(bool rolling);				// 전투기 롤링 여부 설정

	// Method
	void respawnFigther(Vector3 pos, Vector3 direction);	// 전투기를 부활시킴
	char* toGamePacket(GamePacket *pPacket);				// 게임 패킷을 필요한 내용으로 채움
	char* fromGamePacket(GamePacket *pPacket);				// 클래스 내부 맴버를 패킷 내용으로 채움

	// Virtual Function
	virtual bool checkObjectCollision();			// 전투기 충돌을 확인
	virtual bool draw();							// 전투기를 그림
	virtual bool destroy();							// 비행체를 파괴함
	virtual bool update(REAL timeElapsed);			// 비행체를 업데이트

private:
	// Update
	void updateOffCourse(REAL timeElapsed);		// 코스 이탈 업데이트
	void updateRadar();							// 레이더 리스트 업데이트
	void updateWarning();						// 경고 상태 업데이트
	void updateBoost(REAL timeElapsed);			// 부스터 업데이트
	void updateTrail(REAL timeElapsed);			// 트레일 이펙트 업데이트
	void updateFlameColor(REAL timeElapsed);	// 엔진 사용량에 따른 불꽃 이펙트 색상 업데이트
};

#endif