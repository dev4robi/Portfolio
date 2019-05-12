#ifndef __UI_H__
#define __UI_H__

#include "Engine/GameWidget.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	< Content of UI.h >
*
*	 0. UImaster(v4) - 2016.08.23
*	 1. RaderLockon(v1) - 2016.07.12
*	 2. SpeedAltMeter(v1) - 2016.07.18
*	 3. RadarMap(v1) - 2016.07.18
*	 4. LockonUI(v1) - 2016.07.23
*	 5. StateSys(v2) - 2016.08.11
*	 6. WarningSys(v1) - 2016.08.11
*	 7. RespawnUI(v1) - 2016.08.18
*	 8. ScoreBoard(v1) - 2016.08.20
*	 9. MissionMsg(v1) - 2016.08.23
*	10. ScreenEffect(v1) - 2016.10.10
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ui {
	enum UI_COLOR_ENUM {
		cyan = 0, red, blue, white
	};

	inline Vector3 GetSavedUIColor(UI_COLOR_ENUM status) {
		Vector3 rtVec;

		switch (status) {
		case UI_COLOR_ENUM::cyan:	rtVec.x = 0.33; rtVec.y = 1.00; rtVec.z = 1.00; break; // 청록
		case UI_COLOR_ENUM::red:	rtVec.x = 1.00; rtVec.y = 0.15; rtVec.z = 0.15; break; // 빨강
		case UI_COLOR_ENUM::blue:	rtVec.x = 0.00; rtVec.y = 0.00; rtVec.z = 1.00; break; // 파랑
		case UI_COLOR_ENUM::white:	rtVec.x = 1.00; rtVec.y = 1.00; rtVec.z = 1.00; break; // 하얀
		default: rtVec.x = 1.0, rtVec.y = 1.0, rtVec.z = 1.0;
		}

		return rtVec;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RaderLockon(v2)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 레이더에 감지된 비행체 위에 그려지는 유저 인터페이스.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h
*
*   - Version(v1): 레이더 락온 클래스 기초 설계. (2016.07.12)
*	- Version(v2): 화면 범위를 벗어난 대상을 그릴 때 사용할 화살표 텍스처 맴버 추가 및 로직 구현. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UImaster;
class Player;
class JetFighter;

class RadarLockon : public GameWidget {
public:
	enum RadarLockonType {
		ally = 0, enemy
	};

private:
	REAL uiScale;				// UI 크기
	JetFighter *targetFighter;	// 레이더 락온이 된 타겟
	JetFighter *ownerFighter;	// 레이더 락온을 하는 타겟
	bool lockedByWeapon;		// 무기로 인해 락온된 상태 여부
	RadarLockonType type;		// 레이더 락온 타입(색상)
	GLuint arrowTextureID;		// 레이더 락온 화살표 텍스처 번호

public:
	RadarLockon();											// 생성자
	RadarLockon(JetFighter *caster, JetFighter *target);	// 생성자
	RadarLockon(RadarLockon& obj);							// 복사 생성자
	~RadarLockon();											// 소멸자

	void setLockedByWeapon(bool isLocked);	// 적에게 락온됨 여부를 설정
	JetFighter* getTargetFighterPtr();		// 레이더 락온이 된 타겟의 포인터 반환

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : SpeedAltMeter(v1)
*	- Last Update : 2016.07.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 전투기의 속도와 고도를 표시해주는 인터페이스.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h
*
*   - Version(v1): 속도 고도 미터계 기초 설계.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SpeedAltMeter : public GameWidget {
private:
	UImaster *pUIMaster;		// UI 마스터 포인터

	GLuint textureID;
	GLuint breakTextureID;
	GLuint boostingTextureID;
	JetFighter *pTargetJet;
	REAL speedMeter;
	REAL altMeter;

public:
	SpeedAltMeter(UImaster *pMaster);
	SpeedAltMeter(SpeedAltMeter& obj);
	~SpeedAltMeter();

	void setSpeed(REAL speed);
	void setAlt(REAL alt);
	void setTargetJet(JetFighter *jet);		// UI타겟이 되는 전투기 설정
	JetFighter* getTargetJetPtr();			// UI타겟이 되는 전투가 포인터 반환

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RadarMap(v1)
*	- Last Update : 2016.07.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 맵을 레이더 형식으로 출력. 자신과 적의 위치를 파악하기 쉽게 함.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h, Terrain.h, Player.h
*				   FlightObject.h, CombatFrame.h
*
*   - Version(v1): 레이더 맵 기초 설계.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Terrain;
class Player;
class FlightObject;

class RadarMap : public GameWidget {
private:
	UImaster *pUIMaster;					// UI 마스터 포인터

	GLuint mapGridTextureID;				// 미니맵 배경 격자 텍스처
	GLuint jetTextureID;					// 미니맵 전투기 텍스처
	GLuint missileTextureID;				// 미니맵 미사일 텍스처
	GLuint radarRadTextureID;				// 미니맵 레이더 반경 텍스처

	Terrain *pTerrain;						// 지형 포인터
	Player *pPlayer;						// 플레이어 포인터
	list<FlightObject*> *pFlightObjList;	// 비행체들 리스트

	REAL currentScanDist;					// 현재 스캔 범위
	REAL scanSpeed;							// 레이더 스캔 속도
public:
	RadarMap(UImaster *pMaster);			// 생성자
	RadarMap(RadarMap& obj);				// 복사 생성자
	~RadarMap();							// 소멸자

	void setTerrain(Terrain *terrain);		// 지형을 설정
	void setPlayer(Player *player);			// 플레이어를 설정
	void setScanSpeed(REAL speed);			// 스켄 속도를 설정

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트

private:
	void drawObjIcon(list<FlightObject*>* pObjList, REAL mapWidth, REAL mapHeight);	// 미니맵에 비행체, 미사일 아이콘을 그리는 함수
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LockonUI(v1)
*	- Last Update : 2016.07.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 락온 UI를 화면에 표시.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, Player.h, FlightObject.h
*
*   - Version(v1): 락온 클래스 기초 설계.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class LockonUI : public GameWidget {
private:
	UImaster *pUIMaster;		// UI 마스터 포인터

	REAL uiScale;				// UI 크기
	JetFighter *pTargetFighter;	// 무기에 의해 락온이 된 타겟
	JetFighter *pOwnerFighter;	// 무기로 락온을 시도 하는 타겟
	bool isLockonSuccess;		// 무기로 인해 락온이 완료된 여부

public:
	LockonUI(JetFighter *pOwnerJet, GLuint texID);	// 생성자
	LockonUI(LockonUI &obj);						// 복사 생성자
	~LockonUI();									// 소멸자

	void setLockonSuccess(bool success);		// 락온 성공 여부를 설정
	bool getLockonSuccess();					// 락온 성공 여부를 반환
	void setTargetFighter(JetFighter *fighter);	// 미사일 락온이 된 타겟 설정
	JetFighter* getTargetFighterPtr();			// 미사일 락온이 된 타겟의 포인터 반환
	
	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : StateSys(v2)
*	- Last Update : 2016.08.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 자신의 상태를 UI로 출력.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h, Player.h
*
*   - Version(v1): 상태 시스템 클래스 기초 설계. (2016.08.??)
*   - Version(v2): 엔진 출력 및 오버히트 추가. (2016.08.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class StateSys : public GameWidget {
private:
	UImaster *pUIMaster;			// UI 마스터 포인터

	Player *pOwnerPlayer;			// UI 소유주
	JetFighter *pOwnerJet;			// 소유주의 전투기
	GLuint jetTexture;				// 전투기 이미지 텍스처
	GLuint misTexture;				// 미사일 이미지 텍스처
	GLuint engTexture;				// 엔진 출력 텍스처

public:
	StateSys(UImaster *pMaster);		// 생성자
	StateSys::StateSys(StateSys &obj);	// 복사 생성자
	~StateSys();						// 소멸자

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : WarningSys(v3)
*	- Last Update : 2016.08.27
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 각종 경고를 UI로 출력
*
*	- Dependency : [StdHeader.h], [GameWidget.h], SoundManager.h, JetFighter.h, GLTextureFont.h, Player.h
*
*   - Version(v1): 경고 알림 시스템 클래스 기초 설계. (2016.08.11)
*   - Version(v2): 사운드 경보 추가. (2016.08.17)
*   - Version(v3): 코스 이탈 경고 추가. (2016.08.27)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Sound;

class WarningSys : public GameWidget {
private:
	UImaster *pUIMaster;		// UI 마스터 포인터

	Player *pOwnerPlayer;		// UI 소유주
	JetFighter *pOwnerJet;		// 소유주의 전투기

	// SYS 텍스처
	bool isMsgDestroy;				// sys UI가 파괴인지 히트인지를 구분하는 구분자
	REAL sysLeftDuration;			// sys UI 남은 지속시간
	GLuint sysTexture;				// 시스템 표현 텍스처 번호
	GLuint hitSysTexture;			// 미사일 적중 시 표현할 텍스처 번호
	GLuint destroySysTexture;		// 적 파괴 시 표현할 텍스처 번호

	// 경보 텍스처
	GLuint missileAlertTexture;		// 미사일 경보 시 표현할 텍스처 번호
	GLuint lockonAlertTexture;		// 미사일 락온 시 표현할 텍스처 번호
	GLuint pullUpTexture;			// 고도 상승 경보 텍스처 번호
	GLuint stallTexture;			// 스톨 경보 텍스처 번호
	GLuint offCourseTexture;		// 코스 이탈 텍스처 번호

	// 경보 사운드
	Sound *pSndMssileAlert;		// 미사일 경보 사운드
	Sound *pSndLockonWarning;	// 락온 경고 사운드

public:
	WarningSys(UImaster *pMaster);		// 생성자
	WarningSys(WarningSys &obj);		// 복사 생성자
	~WarningSys();						// 소멸자

	void setSystemAlarm(bool isDestory, REAL time); // 시스템 알람 추가

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RespawnUI(v1)
*	- Last Update : 2016.08.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 부활 시 출력할 상태 UI
*
*	- Dependency : [StdHeader.h], [GameWidget.h], GLTextureFont.h, Player.h
*
*   - Version(v1): 리스폰 UI 기초 설계. (2016.08.17)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class RespawnUI : public GameWidget {
private:
	UImaster *pUIMaster;		// UI 마스터 포인터

	REAL respawnElapsedTime;	// 리스폰 경과 시간
	Vector3 respawnCamPos;		// 리스폰에 임시로 사용될 카메라 위치
	Player *pOwnerPlayer;		// UI 소유주
	Player *pKillerPlayer;		// 처치한 전투기의 소유주

public:
	RespawnUI(UImaster *pMaster);	// 생성자
	RespawnUI(RespawnUI &obj);		// 복사 생성자
	~RespawnUI();					// 소멸자

	void setKillerPlayer(Player *pPlayer);	// 처치자 플레이어 설정
	Player* getKillerPlayerPtr();			// 처치자 플레이어 포인터 반환

	virtual bool draw();					// UI 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// UI 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// UI 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ScoreBoard(v1)
*	- Last Update : 2016.08.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 플레이어들의 점수를 나타내는 상황판
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): ScoreBoard 클래스 기초 설계. (2016.08.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class CombatFrame;

class ScoreBoard : public GameWidget {
private:
	UImaster *pUIMaster;			// UI 마스터 포인터
	CombatFrame *pFrame;			// 컴벳 프레임 포인터

	vector<Player*> scoreVector;	// 스코어 벡터 (점수와 킬이 높을은 플레이어가 front에 저장됨)

public:
	// 생성자 소멸자
	ScoreBoard(UImaster *pMaster);
	ScoreBoard(ScoreBoard &obj);
	~ScoreBoard();

	// 가상 함수
	virtual bool draw();					// UI 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// UI 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// UI 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : MissionMsg(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 미션 시작 및 종료를 표현하는 UI
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): MissionMsg 클래스 기초 설계. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class MissionMsg : public GameWidget {
private:
	UImaster *pUIMaster;			// UI 마스터 포인터

	REAL startMsgLeftTime;			// 미션 시작 남은 표현 시간
	REAL compeleteMsgLeftTime;		// 미션 종료 남은 표현 시간
	REAL sessionDestructLeftTime;	// 세션 파괴 남은 시간

	GLuint sysTexID;				// 시스템 텍스처 아이디
	GLuint missionStartTexID;		// 미션 시작 텍스처 아이디
	GLuint missionCompleteTexID;	// 미션 종료 텍스처 아이디
	GLuint serverDisconnectedTexID;	// 서버 접속 종료됨 텍스처 아이디

public:
	// 생성자 & 소멸자
	MissionMsg(UImaster *pMaster);
	MissionMsg(MissionMsg &obj);
	~MissionMsg();

	// 게터 & 세터
	REAL getSessionDestructLeftTime();		// 세션 파괴 남은 시간 반환

	// 가상 함수
	virtual bool draw();					// UI 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// UI 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// UI 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ScreenEffect(v1)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 화면에 특정 상태에 따른 효과를 출력하는 클래스.
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): ScreenEffect 클래스 기초 설계. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ScreenEffect : public GameWidget {
public:
	enum ScrEffectAction {				// 스크린에 적용할 이펙트 효과 열거형
		SCREFFECT_WARNGLOW, SCREFFECT_BOOST, SCREENEFFECT_DUST
	};

private:
	struct ScrEffect {					// 스크린 효과 구조체
		int iEffectID;					// 사용자가 정의한 이펙트 ID (이펙트마다 고유의 번호를 부여 가능하지만, 같은 번호라면 지속시간만 갱신됨: 부활:1,부스터:2,먼지:3)
		ScrEffectAction enAction;		// 적용할 이펙트 효과
		GLuint uiTexID;					// 이펙트 텍스처 ID
		REAL rLeftTime;					// 이펙트 지속 시간
		REAL rValue;					// 이펙트에서 사용할 측정 변수
	};

	UImaster *pUIMaster;				// UI 마스터 포인터
	vector<ScrEffect*> scrEffectVector;	// 스크린 효과를 담은 벡터

public:
	// 생성자 & 소멸자
	ScreenEffect(UImaster *pMaster);
	ScreenEffect(ScreenEffect& obj);
	~ScreenEffect();

	// 게터 & 세터
	void createEffect(REAL time, int effectID, ScrEffectAction action, GLuint texID = 0);	// 해당 번호의 이펙트를 동적 생성 (텍스처 아이디에 0 번을 넣으면 액션에 따른 디폴트 텍스처 적용)
	
	// 메소드
	void clearEffects();					// 이펙트 벡터의 데이터들을 모두 비움

	// 가상 함수
	virtual bool draw();					// UI 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// UI 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// UI 위젯을 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : UImaster(v5)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 플레이어의 모든 UI를 관장하는 클래스
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): UImaster 클래스 기초 설계. (2016.08.09)
*	- Version(v2): WarningSys 클래스 추가. (2016.08.11)
*	- Version(v3): RespawnUI 클래스 추가 및 update, draw 부분 처리를 Player 클래스로부터 이관. (2016.08.17)
*	- Version(v4): MissionMsg 클래스 추가. (2016.08.23)
*	- Version(v5): ScreenEffect 클래스 추가. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UImaster : public GameObject {
private:
	REAL scrWidth;							// 해상도 폭
	REAL scrHeight;							// 해상도 높이
	REAL dmgBlinkLeftTime;					// 피해 입을 시 색상 점등하는 남은 시간

	Player *pOwnPlayer;						// 소유주 플레이어 포인터
	list<GameWidget*> *pUIDrawList;			// 해당 플레이어의 UI 드로우 리스트 포인터

	StateSys *pStateSys;					// 해당 플레이어의 상태 시스템 UI
	SpeedAltMeter *pSpeedAltMeter;			// 해당 플레이어의 속도계 & 고도계
	list<RadarLockon*> radarLockonUIList;	// 해당 플레이어의 레이더 락온 UI리스트
	RadarMap *pRadarMap;					// 해당 플레이어의 레이더 맵
	WarningSys *pWarningSys;				// 해당 플레이어의 경고 시스템 UI
	RespawnUI *pRespawnUI;					// 해당 플레이어의 리스폰 UI
	ScoreBoard *pScoreBoard;				// 해당 플레이어의 스코어보드 UI
	MissionMsg *pMissionMsg;				// 해당 플레이어의 미션 메시지 UI
	ScreenEffect *pScreenEffect;			// 해당 플레이어의 스크린 이펙트 UI
	// 여기에 UI 계속 추가 . . .

public:
	UImaster(Player *_pOwnPlayer);			// 생성자
	UImaster(UImaster &obj);				// 복사 생성자
	~UImaster();							// 소멸

	// Getter & Setter
	REAL getScreenWidth();							// 해상도 폭 반환
	REAL getScreenHeight();							// 해상도 높이 반환
	void setDmgBlinkTime(REAL time);				// 피격 시 점멸하는 시간 설정
	REAL getDmgBlinkTime();							// 피격 시 점멸하는 시간 반환
	Player* getOwnPlayerPtr();						// UI 소유주 플레이어 포인터 반환
	StateSys* getStateSysPtr();						// 상태 시스템 UI 포인터 반환
	SpeedAltMeter* getSpeedAltMeterPtr();			// 속도 고도계 UI 포인터 반환
	list<RadarLockon*>* getRadarLockonUIListPtr();	// 레이더 락온 UI 리스트 포인터 반환
	RadarMap* getRadarMapPtr();						// 레이더 맵 UI 포인터 반환
	WarningSys* getWarningSysPtr();					// 경고 시스템 UI 포인터 반환
	RespawnUI* getRespawnUIPtr();					// 리스폰 UI 포인터 반환
	MissionMsg* getMissionMsgPtr();					// 미션 메시지 UI 포인터 반환
	ScreenEffect* getScreenEffectPtr();				// 스크린 이펙트 UI포인터 반환

	void updateUI(REAL timeElapsed);				// UI 업데이트
};


#endif