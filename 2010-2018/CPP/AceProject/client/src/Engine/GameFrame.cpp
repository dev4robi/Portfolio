#include "Engine/GameWidget.h"
#include "Engine/GameFrame.h"
#include "Game/Object/Skybox.h"
#include "Game/Object/Player.h"

GameFrame::GameFrame(wstring name) : GameObject() {
	// ���� �Ŵ��� ����
	g_pGameManager->setGameFrame(this); // ������ ������ ���ÿ� GameFrame�� ����Ʈ�� �߰��Ǵµ�, �Ϻ� Frame�� �����ڿ��� ������ �����ϹǷ� ���⼭ ���� �������� ������ ���־�� �Ѵ�.
	if ((pGameManager = g_pGameManager) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : g_pGameManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// ��ο� �Ŵ��� ����
	if ((pDrawManager = pGameManager->getDrawManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pDrawManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// ��ǲ �Ŵ��� ����
	if ((pInputManager = pGameManager->getInputManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pInputManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// ���ҽ� �Ŵ��� ����
	if ((pResourceManager = pGameManager->getResourceManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pResourceManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// ���� �Ŵ��� ����
	if ((pSoundManager = pGameManager->getSoundManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pSoundManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// ��Ʈ��ũ �Ŵ��� ����
	if ((pNetworkManager = pGameManager->getNetworkManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pNetworkManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// �÷��̾ NULL�� ����
	pOwnerPlayer = NULL;

	// ��� �ϴð� ������ NULL�� ����
	pSkybox = NULL;
	pTerrain = NULL;

	// ��� ������ NULL�� ����
	pBackgroundSound = NULL;

	// ������ �̸� ����
	wsFrameName = name;

	// ���� �ð�
	dwCreatedTime = g_pGameManager->getElapsedTime();
}

GameFrame::~GameFrame() {
	// ����Ʈ ����Ʈ �ı�
	GL_Effect *pEffect = NULL;
	for (list<GL_Effect*>::iterator iterEffect = effectList.begin(); iterEffect != effectList.end();) {
		pEffect = (*iterEffect);
		iterEffect = effectList.erase(iterEffect);

		if (pEffect != NULL) delete pEffect;
	}
	pEffect = NULL;

	// ���� ������ ���� �ı�
	GameWidget *pWidget = NULL;
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end();) {
		pWidget = (*iterWidget);
		iterWidget = widgetList.erase(iterWidget);
		
		if (pWidget != NULL) delete pWidget;
	}
	pWidget = NULL;

	// ���� ���� �ı�
	if (pSoundManager != NULL) pSoundManager->destroySoundAll();

	// �ϴû��� �� ���� �ı�
	if (pSkybox != NULL) delete pSkybox;
	if (pTerrain != NULL) delete pTerrain;

	// �����͸� NULL��
	pTerrain = NULL;
	pSkybox = NULL;
	pGameManager = NULL;
	pDrawManager = NULL;
	pInputManager = NULL;
	pResourceManager = NULL;
	pOwnerPlayer = NULL;
	pSoundManager = NULL;
	pNetworkManager = NULL;
}

bool GameFrame::getNetworkingUse() {
	return bUseNetworking;
}

void GameFrame::setNetworkingUse(bool use) {
	bUseNetworking = use;
}

DWORD GameFrame::getCreatedTime() {
	return dwCreatedTime;
}

wstring GameFrame::getFrameName() {
	return wsFrameName;
}

list<GameWidget*>* GameFrame::getDrawListPtr() {
	return &drawList;
}

list<GameWidget*>* GameFrame::getWidgetListPtr() {
	return &widgetList;
}

list<GL_Effect*>* GameFrame::getEffectListPtr() {
	return &effectList;
}

Skybox* GameFrame::getSkyboxPtr() {
	return pSkybox;
}

bool GameFrame::setSkybox(Skybox *skyboxPtr) {
	pSkybox = skyboxPtr;

	return true;
}

Terrain* GameFrame::getTerrainPtr() {
	return pTerrain;
}

bool GameFrame::setTerrain(Terrain *terrainPtr) {
	pTerrain = terrainPtr;

	return true;
}

Player* GameFrame::getOwnerPlayerPtr() {
	return pOwnerPlayer;
}

bool GameFrame::setBackgroundSound(Sound *pSound) {
	pBackgroundSound = pSound;

	return true;
}

Sound* GameFrame::getBackgroundSoundPtr() {
	return pBackgroundSound;
}

GameWidget* GameFrame::findWidget(const ID id) {
	list<GameWidget*>::iterator iter;
	for (iter = widgetList.begin(); iter != widgetList.end(); ++iter) {
		if ((*iter)->getID() == id) {
			return (*iter);
		}
	}

	return NULL;
}

bool GameFrame::destoryWidget(GameWidget *widgetPtr) {
	widgetPtr->destroy();

	delete widgetPtr;

	return true;
}

bool GameFrame::addWidget(GameWidget *widgetPtr) {
	widgetList.push_back(widgetPtr);

	return true;
}

bool GameFrame::addEffect(GL_Effect *pEffect) {
	effectList.push_back(pEffect);

	return true;
}

bool GameFrame::removeWidget(GameWidget *widgetPtr) {
	widgetList.remove(widgetPtr);

	return true;
}

void GameFrame::updateKey(REAL timeElapsed) {
	// ���� �Լ�
}

bool GameFrame::updateFrame(REAL timeElapsed) {
	// ���� �Լ�
	return true;
}