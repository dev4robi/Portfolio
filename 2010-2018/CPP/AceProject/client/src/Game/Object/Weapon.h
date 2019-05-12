#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Engine/StdHeader.h"

class JetFighter;
class GameWidget;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : Weapon.h(v1)
*	- Last Update : 2016.07.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 전투기가 보유할 무기 클래스.
*
*	- Dependency : [StdHeader.h], DrawManager.h
*
*   - Version(v1): 무기 클래스 기초 설계. (2016.07.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Weapon : public GameObject {
public:
	enum weapon_type {
		WEAPON_DEFAULT, WEAPON_GUN, WEAPON_MISSILE
	};

protected:
	weapon_type wpnType;		// 무기 종류
	int wpnID;					// 무기 아이디
	JetFighter *pWpnOwner;		// 무기를 소유한 전투기
	string wpnName;				// 무기 이름
	GLMmodel *pWpnModel;		// .obj 무기 모델 포인터
	
	int wpnQuantity;			// 현재 남은 수량
	int wpnMaxQuantity;			// 최대 수량
	REAL wpnAoE;				// 타격 범위
	REAL wpnDamage;				// 피해량
	REAL wpnFirePerSecond;		// 초당 무기 발사 수
	REAL wpnMissileSpeed;		// 발사체 속도
	REAL wpnMissileDuration;	// 발사체 지속 시간

	bool wpnIsReady;			// 무기 사용 가능 여부

public:
	Weapon(JetFighter *pOnwer, int id);	// 생성자
	Weapon(Weapon& obj);				// 복사 생성자
	~Weapon();							// 소멸자

	JetFighter* getWpnOwnerPtr();				// 무기 소유 전투기 포인터를 반환
	void setWpnType(weapon_type type);			// 무기 종류를 설정
	weapon_type getWpnType();					// 무기 종류를 반환
	void setQuantity(int quantity);				// 무기의 현재 수량을 설정
	int getQuantity();							// 무기의 현재 수량을 반환
	void setMaxQuantity(int quantity);			// 무기의 최대 수량을 설정
	int getMaxQuantity();						// 무기의 최대 수량을 반환
	void setWpnDamage(REAL damage);				// 무기의 피해량을 설정
	REAL getWpnDamage();						// 무기의 피해량을 반환
	void setFirePerSecond(REAL firePerSecond);	// 무기의 초당 발사량을 설정
	REAL getFirePerSecond();					// 무기의 초당 발사량을 반환
	void setWpnIsReady(bool ready);				// 무기의 준비 상태를 설정
	bool getWpnIsReady();						// 무기의 준비 상태를 반환
	void setWpnAoE(REAL aoe);					// 무기의 반경을 설정
	REAL getWpnAoE();							// 무기의 반경을 반환
	GLMmodel* getWpnModelPtr();					// 무기(미사일) 모델 포인터 반환
	REAL getMissileDuration();					// 미사일 지속 시간을 반환
	REAL getMissileSpeed();						// 미사일 속도를 반환


	virtual void drawWeapon();					// 무기 그리기
	virtual void fireWeapon();					// 무기를 발사(사용)
	virtual void update(REAL timeElapsed);		// 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : WpnLockonMissile(v2)
*	- Last Update : 2016.08.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 유도 미사일 및 기총 무기 클래스.
*
*	- Dependency : [StdHeader.h],  ResourceManager.h, JetFigher.h, GameWidget.h, UI.h, FlightObject.h, SoundManager.h
*
*   - Version(v1): 유도 미사일 클래스 기초 설계. (2016.07.20)
*	- Version(v2): 기관총 또한 구현할 수 있도록 클래스 기능 강화. (2016.08.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class LockonUI;
class Sound;

class WpnLockonMissile : public Weapon {
protected:
	GLuint lockonUITexture;					// 락온 UI 텍스처 번호
	list<JetFighter*> wpnLockonTgtList;		// 락온 타겟 전투기 리스트
	list<LockonUI*> wpnLockonUIList;		// 락온 UI 리스트

	bool wpnTgtChangeRequested;			// 타겟 변경 요청 여부를 저장
	int wpnNextFireMissileNum;			// 다음에 발사될 미사일 번호
	int wpnLockonCount;					// 동시에 락온할 수 있는 전투기 개수 (항상 미사일 개수보다 적거나 같다)
	int wpnMissileCount;				// 미사일 개수					
	vector<REAL> wpnFireLeftTime;		// 미사일별 남은 발사시간
	REAL wpnLockonSpeed;				// 미사일 락온 속도
	REAL wpnLockonRadOnScr;				// 화면에서의 락온 범위 반지름
	REAL wpnLockonRange;				// 락온 범위

	string wpnFireSoundName;			// 무기 발사 사운드파일 이름
	Sound *pWpnFireSound;				// 무기 발사 사운드 (루프하는 사운드의 경우 동적 생성 후 사운드 소스를 보관)

public:
	WpnLockonMissile(JetFighter *pOwner, int id);	// 생성자
	WpnLockonMissile(WpnLockonMissile& obj);		// 복사 생성자
	~WpnLockonMissile();							// 소멸자

	void wpnTgtChangeRequest();						// 타겟 변경을 요청
	int getWpnMissileCount();						// 미사일 개수를 반환

	list<LockonUI*>* getWpnLockonUIListPtr();		// 락온 UI 리스트 포인터 반환
	vector<REAL>* getWpnFireLeftTimePtr();			// 미사일별 남은 발사시간이 저장된 벡터 포인터 반환

	virtual void drawWeapon();						// 무기 그리기
	virtual void fireWeapon();						// 무기 발사
	virtual void stopFireWeapon();					// 무기 발사 중지
	virtual void update(REAL timeElapsed);			// 업데이트

private:
	void updateRadarLockonIsLocked(LockonUI *pLockonUI, bool isLocked);			// 레이더 락온 UI의 lockedByWeapon 을 변경하여 색상 변경을 적용하기 위함
	bool isJetInLockonRad(JetFighter *jet);										// 매개변수로 전달된 전투기가 화면상 락온 가능 범위내에 있으면 true반환
	bool isTargtInLockonList(JetFighter *target);								// 타겟 전투기가 wpnLockonTgtList에 포함되어 있는지 여부를 반환
	void initLockonUI(LockonUI *pObj);											// 락온 UI객체의 정보를 초기화시킴
	void initLockonUIAll();														// 모든 UI객체의 정보를 초기화시킴
	int getLockonUITgtCount();													// 락온 UI가 락온하고 있는 대상의 수를 반환
};

#endif