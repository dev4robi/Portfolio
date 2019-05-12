#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameManager (v3)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 엔진에 대한 모든 부분을 총괄하는 클래스. 게임 메인 루프와
*					 로직 및 화면 렌더링에 대한 기본 정보를 운영하는 엔진의 밑단이 된다.
*
*	- Dependency : [GameObject.h, <Windows.h>], GameWindow.h, GameFrame.h, DrawManager.h, InputManager.h
*				   ResourceManager.h, SoundManager.h
*
*	- Version(v1): 게임 매니저 클래스 기초 설계. (2016.03.13)
*	- Version(v2): 사운드 매니저 추가. (2016.08.16)
*	- Version(v3): 네트워크 매니저 추가. (2016.09.12)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* 주의! : StdHeader.h 에 GameManager.h 파일이 포함되므로 필요한 #include를 수동으로 기입해야 함. */
#include "Engine/GameObject.h"
#include <Windows.h>

class GameWindow;
class GameFrame;
class DrawManager;
class InputManager;
class ResourceManager;
class SoundManager;
class NetworkManager;

class GameManager : public GameObject {
private:
	GameWindow		*pGameWnd;			// 윈도우 포인터
	GameFrame		*pGameFrame;		// 프레임 포인터
	DrawManager		*pDrawManager;		// 드로우 매니저 포인터
	InputManager	*pInputManager;		// 인풋 매니저 포인터
	ResourceManager *pResourceManager;	// 리소스 매니저 포인터
	SoundManager	*pSoundManager;		// 사운드 매니저 포인터
	NetworkManager	*pNetworkManager;	// 네트워크 매니저 포인터

	DWORD dwCreatedTime;				// 매니저 생성 시간
	DWORD dwElapsedTime;				// 매니저 생성 후 경과시간
	float fTargetLPS;					// 목표 LPS(Logic Per Second : 초당 게임 로직 처리 횟수)
	float fCurrentLPS;					// 현재 LPS
	float fTargetFPS;					// 목표 FPS
	float fCurrentFPS;					// 현재 FPS

public:
	GameManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow); // 생성자
	~GameManager();	// 소멸자

	void setGameWnd(GameWindow* gameWnd);						// 게임 윈도우를 설정
	GameWindow* getGameWndPtr();								// 게임 윈도우 포인터를 반환
	void setGameFrame(GameFrame* gameFrame);					// 게임 프레임을 설정
	GameFrame* getGameFramePtr();								// 게임 프레임 포인터를 반환
	void setDrawManager(DrawManager* drawManager);				// 드로우 매니저를 설정
	DrawManager* getDrawManagerPtr();							// 드로우 매니저 포인터를 반환
	void setInputManager(InputManager* inputManager);			// 인풋 매니저를 설정
	InputManager* getInputManagerPtr();							// 인풋 매니저 포인터를 반환
	void setResourceManager(ResourceManager *resourceManager);	// 리소스 매니저를 설정
	ResourceManager* getResourceManagerPtr();					// 리소스 매니저 포인터를 반환
	void setSoundManager(SoundManager *soundManager);			// 사운드 매니저를 설정
	SoundManager* getSoundManagerPtr();							// 사운드 매니저 포인터를 반환
	void setNetworkManager(NetworkManager *networkManager);		// 네트워크 매니저를 설정
	NetworkManager* getNetworkManagerPtr();						// 네트워크 매니저 포인터를 반환

	DWORD getCreatedTime();						// 매니저 생성 시간 반환
	DWORD getElapsedTime();						// 매니저 생성 후 경과 시간 포인터를 반환
	float getCurrentLPS();						// 현재 LPS를 반환
	void setTargetLPS(float lps);				// 목표 LPS를 설정
	float getCurrentFPS();						// 현재 FPS를 반환
	void setTargetFPS(float fps);				// 목표 FPS를 설정

	void startMainLoop();						// 메인 루프를 시작
};

#endif