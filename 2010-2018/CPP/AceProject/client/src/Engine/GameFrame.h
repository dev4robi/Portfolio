#ifndef __GAME_FRAME_H__
#define __GAME_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameFrame (v5)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ������(Sequence) Ȥ�� ��(Scene)�̶�� �� �� �ִ� Ŭ������
*					 ���� ȭ�鿡 ���̴� ��� ������ ��ɵ��� �����ϰ� �ִ� Ŭ�����̴�.
*					 �� Ŭ������ ����Ͽ� ���� �޴�, �ɼ�, ����, ũ���� ���� ǥ��.
*
*	- Dependency : [StdHeader.h, DrawManager.h, InputManager.h, GameCamera.h, GameWidget.h
*				   ResourceManager.h, Skybox.h, SoundManager.h, NetworkManager.h]
*
*	- Version(v1) : Ŭ���� ���� ����. (16.03.13)
*	- Version(v2) : ���Ǹ� ���� Ŭ���� ���ο� �ٸ� �Ŵ������� ������ ����. (16.05.16)
*	- Version(v3) : ������ �������� ���� ��ī�̹ڽ��� ����Ű�� �ɹ� ���� �߰�. (16.06.11)
*	- Version(v4) : ������ ������ ��� ������ ������ ���� ������ ��� �߰�. (16.08.17)
*	- Version(v5) : ��Ʈ��ũ �Ŵ��� ���� �߰�. (2016.09.12)
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
	GameManager		*pGameManager;				// ���� �Ŵ��� ������
	DrawManager		*pDrawManager;				// ��ο� �Ŵ��� ������
	InputManager	*pInputManager;				// ��ǲ �Ŵ��� ������
	ResourceManager *pResourceManager;			// ���ҽ� �Ŵ��� ������
	SoundManager	*pSoundManager;				// ���� �Ŵ��� ������
	NetworkManager	*pNetworkManager;			// ��Ʈ��ũ �Ŵ��� ������
	Player			*pOwnerPlayer;				// ������ ������ �÷��̾�

	bool bUseNetworking;						// ��Ʈ��ŷ ��� ����
	DWORD dwCreatedTime;						// �������� ������ �ð�
	wstring wsFrameName;						// ������ �̸�
	Skybox *pSkybox;							// �ϴ� ���� ���� ������
	Terrain *pTerrain;							// ���� ���� ������
	Sound *pBackgroundSound;					// ������ ��� ����
	list<GameWidget*> drawList;					// ��ο� ����Ʈ (��� �׷��� ������ ���⿡ ����)
	list<GameWidget*> widgetList;				// ���� ����Ʈ (�׷����� �ȱ׷����� ��� ������ ����)
	list<GL_Effect*> effectList;				// ��ƼŬ ����Ʈ ����Ʈ

public:
	GameFrame(wstring name);					// ������
	virtual ~GameFrame();						// �Ҹ���

	bool getNetworkingUse();					// ��Ʈ��ŷ ��� ���� ��ȯ
	void setNetworkingUse(bool use);			// ��Ʈ��ŷ ��� ���� ����
	DWORD getCreatedTime();						// ������ ���� �ð��� ��ȯ
	wstring getFrameName();						// ������ �̸��� ��ȯ
	Player* getOwnerPlayerPtr();				// ������ ������ �÷��̾� ������ ��ȯ
	list<GameWidget*>* getDrawListPtr();		// ��ο� ����Ʈ�� �����͸� ��ȯ
	list<GameWidget*>* getWidgetListPtr();		// ���� ����Ʈ�� �����͸� ��ȯ
	list<GL_Effect*>* getEffectListPtr();		// ����Ʈ ����Ʈ�� �����͸� ��ȯ
	
	bool setSkybox(Skybox *skyboxPtr);			// ��ī�̹ڽ� ����
	Skybox *getSkyboxPtr();						// �ϴû��� ������ ��ȯ
	bool setTerrain(Terrain *terrainPtr);		// ���� ����
	Terrain *getTerrainPtr();					// ���� ������ ��ȯ
	bool setBackgroundSound(Sound *pSound);		// ��� ���� ����
	Sound* getBackgroundSoundPtr();				// ��� ���� ������ ��ȯ
	GameWidget* findWidget(const ID id);		// ID�� ������ ã�� ������ ��ȯ
	bool destoryWidget(GameWidget* widgetPtr);	// ������ ����Ʈ���� ����
	
	bool addWidget(GameWidget *widgetPtr);			// ������ ����Ʈ�� �߰�
	bool addEffect(GL_Effect *pEffect);				// ����Ʈ�� ����Ʈ�� �߰�
	bool removeWidget(GameWidget *widgetPtr);		// ������ ����Ʈ���� ����

	virtual bool updateFrame(REAL timeElapsed);		// �������� ������Ʈ(���� �Լ�)
	virtual void updateKey(REAL timeElapsed);		// Ű���� ������Ʈ(���� �Լ�)
};

#endif