#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameManager (v3)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ������ ���� ��� �κ��� �Ѱ��ϴ� Ŭ����. ���� ���� ������
*					 ���� �� ȭ�� �������� ���� �⺻ ������ ��ϴ� ������ �ش��� �ȴ�.
*
*	- Dependency : [GameObject.h, <Windows.h>], GameWindow.h, GameFrame.h, DrawManager.h, InputManager.h
*				   ResourceManager.h, SoundManager.h
*
*	- Version(v1): ���� �Ŵ��� Ŭ���� ���� ����. (2016.03.13)
*	- Version(v2): ���� �Ŵ��� �߰�. (2016.08.16)
*	- Version(v3): ��Ʈ��ũ �Ŵ��� �߰�. (2016.09.12)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ����! : StdHeader.h �� GameManager.h ������ ���ԵǹǷ� �ʿ��� #include�� �������� �����ؾ� ��. */
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
	GameWindow		*pGameWnd;			// ������ ������
	GameFrame		*pGameFrame;		// ������ ������
	DrawManager		*pDrawManager;		// ��ο� �Ŵ��� ������
	InputManager	*pInputManager;		// ��ǲ �Ŵ��� ������
	ResourceManager *pResourceManager;	// ���ҽ� �Ŵ��� ������
	SoundManager	*pSoundManager;		// ���� �Ŵ��� ������
	NetworkManager	*pNetworkManager;	// ��Ʈ��ũ �Ŵ��� ������

	DWORD dwCreatedTime;				// �Ŵ��� ���� �ð�
	DWORD dwElapsedTime;				// �Ŵ��� ���� �� ����ð�
	float fTargetLPS;					// ��ǥ LPS(Logic Per Second : �ʴ� ���� ���� ó�� Ƚ��)
	float fCurrentLPS;					// ���� LPS
	float fTargetFPS;					// ��ǥ FPS
	float fCurrentFPS;					// ���� FPS

public:
	GameManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow); // ������
	~GameManager();	// �Ҹ���

	void setGameWnd(GameWindow* gameWnd);						// ���� �����츦 ����
	GameWindow* getGameWndPtr();								// ���� ������ �����͸� ��ȯ
	void setGameFrame(GameFrame* gameFrame);					// ���� �������� ����
	GameFrame* getGameFramePtr();								// ���� ������ �����͸� ��ȯ
	void setDrawManager(DrawManager* drawManager);				// ��ο� �Ŵ����� ����
	DrawManager* getDrawManagerPtr();							// ��ο� �Ŵ��� �����͸� ��ȯ
	void setInputManager(InputManager* inputManager);			// ��ǲ �Ŵ����� ����
	InputManager* getInputManagerPtr();							// ��ǲ �Ŵ��� �����͸� ��ȯ
	void setResourceManager(ResourceManager *resourceManager);	// ���ҽ� �Ŵ����� ����
	ResourceManager* getResourceManagerPtr();					// ���ҽ� �Ŵ��� �����͸� ��ȯ
	void setSoundManager(SoundManager *soundManager);			// ���� �Ŵ����� ����
	SoundManager* getSoundManagerPtr();							// ���� �Ŵ��� �����͸� ��ȯ
	void setNetworkManager(NetworkManager *networkManager);		// ��Ʈ��ũ �Ŵ����� ����
	NetworkManager* getNetworkManagerPtr();						// ��Ʈ��ũ �Ŵ��� �����͸� ��ȯ

	DWORD getCreatedTime();						// �Ŵ��� ���� �ð� ��ȯ
	DWORD getElapsedTime();						// �Ŵ��� ���� �� ��� �ð� �����͸� ��ȯ
	float getCurrentLPS();						// ���� LPS�� ��ȯ
	void setTargetLPS(float lps);				// ��ǥ LPS�� ����
	float getCurrentFPS();						// ���� FPS�� ��ȯ
	void setTargetFPS(float fps);				// ��ǥ FPS�� ����

	void startMainLoop();						// ���� ������ ����
};

#endif