#ifndef __GAME_FRAME_H__
#define __GAME_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameFrame (v5)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 시퀀스(Sequence) 혹은 씬(Scene)이라고도 할 수 있는 클래스로
*					 현재 화면에 보이는 모든 정보와 기능들을 함축하고 있는 클래스이다.
*					 이 클래스를 상속하여 메인 메뉴, 옵션, 게임, 크레딧 등을 표현.
*
*	- Dependency : [StdHeader.h, DrawManager.h, InputManager.h, GameCamera.h, GameWidget.h
*				   ResourceManager.h, Skybox.h, SoundManager.h, NetworkManager.h]
*
*	- Version(v1) : 클래스 기초 정의. (16.03.13)
*	- Version(v2) : 편의를 위해 클래스 내부에 다른 매니저들의 포인터 정의. (16.05.16)
*	- Version(v3) : 별도의 렌더링을 위한 스카이박스를 가리키는 맴버 변수 추가. (16.06.11)
*	- Version(v4) : 프레임 별도의 배경 음악을 설정을 위한 포인터 멤버 추가. (16.08.17)
*	- Version(v5) : 네트워크 매니저 관련 추가. (2016.09.12)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/DrawManager.h"				// class DrawManager;
#include "Engine/InputManager.h"			// class InputManager;
#include "Engine/GameCamera.h"				// class GameCamera;
#include "Engine/GameWidget.h"				// class GameWidget;
#include "Engine/ResourceManager.h"			// class ResourceManager;
#include "Engine/SoundManager.h"			// class SoundManager;
#include "Engine/Network/NetworkManager.h"	// class NetworkManager;

class Skybox;
class Terrain;
class Player;
class GL_Effect;

class GameFrame : public GameObject {
protected:
	GameManager		*pGameManager;				// 게임 매니저 포인터
	DrawManager		*pDrawManager;				// 드로우 매니저 포인터
	InputManager	*pInputManager;				// 인풋 매니저 포인터
	ResourceManager *pResourceManager;			// 리소스 매니저 포인터
	SoundManager	*pSoundManager;				// 사운드 매니저 포인터
	NetworkManager	*pNetworkManager;			// 네트워크 매니저 포인터
	Player			*pOwnerPlayer;				// 프레임 소유주 플레이어

	bool bUseNetworking;						// 네트워킹 사용 여부
	DWORD dwCreatedTime;						// 프레임이 생성된 시간
	wstring wsFrameName;						// 프레임 이름
	Skybox *pSkybox;							// 하늘 상자 위젯 포인터
	Terrain *pTerrain;							// 지형 위젯 포인터
	Sound *pBackgroundSound;					// 프레임 배경 음악
	list<GameWidget*> drawList;					// 드로우 리스트 (모든 그려질 위젯을 여기에 보관)
	list<GameWidget*> widgetList;				// 위젯 리스트 (그려지던 안그려지던 모든 위젯을 보관)
	list<GL_Effect*> effectList;				// 파티클 이펙트 리스트

public:
	GameFrame(wstring name);					// 생성자
	virtual ~GameFrame();						// 소멸자

	bool getNetworkingUse();					// 네트워킹 사용 여부 반환
	void setNetworkingUse(bool use);			// 네트워킹 사용 여부 설정
	DWORD getCreatedTime();						// 프레임 생성 시간을 반환
	wstring getFrameName();						// 프레임 이름을 반환
	Player* getOwnerPlayerPtr();				// 프레임 소유주 플레이어 포인터 반환
	list<GameWidget*>* getDrawListPtr();		// 드로우 리스트의 포인터를 반환
	list<GameWidget*>* getWidgetListPtr();		// 위젯 리스트의 포인터를 반환
	list<GL_Effect*>* getEffectListPtr();		// 이펙트 리스트의 포인터를 반환
	
	bool setSkybox(Skybox *skyboxPtr);			// 스카이박스 설정
	Skybox *getSkyboxPtr();						// 하늘상자 포인터 반환
	bool setTerrain(Terrain *terrainPtr);		// 지형 설정
	Terrain *getTerrainPtr();					// 지형 포인터 반환
	bool setBackgroundSound(Sound *pSound);		// 배경 음악 설정
	Sound* getBackgroundSoundPtr();				// 배경 음악 포인터 반환
	GameWidget* findWidget(const ID id);		// ID로 위젯을 찾아 포인터 반환
	bool destoryWidget(GameWidget* widgetPtr);	// 위젯을 리스트에서 제거
	
	bool addWidget(GameWidget *widgetPtr);			// 위젯을 리스트에 추가
	bool addEffect(GL_Effect *pEffect);				// 이펙트를 리스트에 추가
	bool removeWidget(GameWidget *widgetPtr);		// 위젯을 리스트에서 제거

	virtual bool updateFrame(REAL timeElapsed);		// 프레임을 업데이트(가상 함수)
	virtual void updateKey(REAL timeElapsed);		// 키보드 업데이트(가상 함수)
};

#endif