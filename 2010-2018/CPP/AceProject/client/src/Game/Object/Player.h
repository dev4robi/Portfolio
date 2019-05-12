#ifndef __PLAYER_H__
#define __PLHAER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Player(v2)
*	- Last Update : 2016.09.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임에서 관리되는 유저(플레이어)의 개개인 정보를 담은 클래스.
*
*	- Dependency : [StdHeader.h], JetFighter.h, GameCamera.h, DrawManager.h, GameWidget.h, UI.h
*
*   - Version(v1): 플레이어 클래스 기초 설계 (2016.07.11)
*	- Version(v2): 플레이어 네트워크 업데이트됨 여부를 위한 맴버 추가. (2016.09.29)
*	
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GameCamera;
class GameWidget;
class JetFighter;
class Missile;

// For UI
class Test2DWidget;
class RadarLockon;
class SpeedAltMeter;
class RadarMap;
class Missile;
class StateSys;
class UImaster;

enum camera_option {
	CAM_DEFAULT, CAM_FIRST_PERSON, CAM_THIRD_PERSON
};

class Player : public GameObject {
private:
	string name;						// 플레이어 명
	int number;							// 플레이어 번호
	JetFighter *pJet;					// 플레이어 소유 전투기 포인터
	
	bool bNetworkDisconnected;			// 멀티플레이 네트워크 접속 종료됨 여부
	bool bNetworkUpdated;				// 멀티플레이 네트워크 업데이트 여부

	int playerKills;					// 플레이어 처형 횟수
	int playerScore;					// 플레이어 획득 점수
	int playerDeathCount;				// 플레이어 사망 횟수

	GameCamera *pCamera;				// 게임 카메라 포인터
	camera_option camOption;			// 카메라 옵션
	bool camChacingMissile;				// 카메라 미사일 추적
	bool camShowBehind;					// 카메라 후방 출력 옵션

	UImaster *pUImanager;					// 플레이어의 UI정보 포인터
	list<GameWidget*> playerUIDrawList;		// 해당 플레이어에게만 보이는 UI위젯 리스트 (플레이어의 모든 UI위젯이 최종적으로 이곳에 저장됨)

public:
	// Cons & Des
	Player(int _number, string _name);
	Player(Player& player);
	~Player();

	// Getter & Setter
	bool getNetworkDisconnected();						// 네트워크 연결 끊김 여부 반환
	void setNetworkDisconnected(bool disconnected);		// 네트워크 연결 끊김 여부 설정
	bool getNetworkUpdated();							// 네트워크 업데이트됨 여부 반환
	void setNetworkUpdated(bool updated);				// 네트워크 업데이트됨 여부 설정
	int getPlayerKills();								// 플레이어 처형 횟수를 반환
	void setPlayerKills(int val);						// 플레이어 처형 횟수를 설정
	int getPlayerScore();								// 플레이어 점수를 반환
	void setPlayeSocre(int val);						// 플레이어 점수를 설정
	int getPlayerDeathCount();							// 플레이어 사망 횟수를 반환
	void setPlayerDeathCount(int val);					// 플레이어 사망 횟수를 설정
	void setPlayerScore(int score);						// 플레이어 점수를 설정
	void setCamChacingMissile(bool chacing);			// 카메라 미사일 추적 옵션을 설정
	bool getCamShowBehind();							// 후방 카메라 사용 설정여부 반환
	void setCamShowBehind(bool useBehind);				// 후방 카메라 사용여부 설정 설정
	camera_option getCameraOption();					// 카메라 옵션 상태를 반환
	void setCameraOption(camera_option option);			// 카메라 옵션을 설정
	GameCamera* getPlayerCamera();						// 플레이어의 카메라 포인터 반환
	void setPlayerJet(JetFighter* jet);					// 플레이어의 전투기 설정
	JetFighter* getPlayerJet();							// 플레이어의 전투기 포인터 반환
	void setPlayerNumber(int playerNumber);				// 플레이어 번호 설정
	int getPlayerNumber();								// 플레이어 번호 반환
	void setPlayerName(char *newName);					// 플레이어 이름 설정
	string getPlayerName();								// 플레이어 이름 반환
	string* getPlayerNamePtr();							// 플레이어 이름 포인터 반환
	UImaster *getUImanagerPtr();						// 플레이어 UI 마스터의 포인터 반환
	list<RadarLockon*>* getRadarLockonUIListPtr();		// 레이더 락온 UI리스트 포인터 반환
	list<GameWidget*>* getPlayerUIDrawListPtr();		// 플레이어의 UI 드로우 리스트 포인터 반환
	SpeedAltMeter* getSpeedAltMeterPtr();				// 플레이어 전투기의 속도고도계 포인터 반환
	RadarMap* getRadarMapPtr();							// 플레이어 레이더 맵의 포인터 반환

	// Method
	static bool cmpScore(const Player *p1, const Player *p2);	// 플레이어의 점수 대소 비교
	static bool cmpNumber(const Player *p1, const Player *p2);	// 플레이어의 번호 비교

	void updateCamera(REAL timeElapsed);				// 플레이어의 카메라 업데이트
	void updatePlayer(REAL timeElapsed);				// 플레이어 업데이트
	void updatePlayerUI(REAL timeElapsed);				// 플레이어 UI 업데이트
};

#endif