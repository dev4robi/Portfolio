#include "Engine/GameWidget.h"
#include "Engine/GameFrame.h"
#include "Game/Object/Skybox.h"
#include "Game/Object/Player.h"

GameFrame::GameFrame(wstring name) : GameObject() {
	// 게임 매니저 설정
	g_pGameManager->setGameFrame(this); // 위젯이 생성과 동시에 GameFrame의 리스트에 추가되는데, 일부 Frame의 생성자에서 위젯을 생성하므로 여기서 게임 프레임을 선지정 해주어야 한다.
	if ((pGameManager = g_pGameManager) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : g_pGameManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 드로우 매니저 설정
	if ((pDrawManager = pGameManager->getDrawManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pDrawManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 인풋 매니저 설정
	if ((pInputManager = pGameManager->getInputManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pInputManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 리소스 매니저 설정
	if ((pResourceManager = pGameManager->getResourceManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pResourceManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 사운드 매니저 설정
	if ((pSoundManager = pGameManager->getSoundManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pSoundManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 네트워크 매니저 설정
	if ((pNetworkManager = pGameManager->getNetworkManagerPtr()) == NULL) {
		MessageBox(NULL, TEXT("Fail to construct GameFrame"), TEXT("GameFrame::GameFrame(wstring name) : pNetworkManager is NULL."), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 플레이어를 NULL로 설정
	pOwnerPlayer = NULL;

	// 배경 하늘과 지형을 NULL로 설정
	pSkybox = NULL;
	pTerrain = NULL;

	// 배경 음악을 NULL로 설정
	pBackgroundSound = NULL;

	// 프레임 이름 설정
	wsFrameName = name;

	// 생성 시간
	dwCreatedTime = g_pGameManager->getElapsedTime();
}

GameFrame::~GameFrame() {
	// 이펙트 리스트 파괴
	GL_Effect *pEffect = NULL;
	for (list<GL_Effect*>::iterator iterEffect = effectList.begin(); iterEffect != effectList.end();) {
		pEffect = (*iterEffect);
		iterEffect = effectList.erase(iterEffect);

		if (pEffect != NULL) delete pEffect;
	}
	pEffect = NULL;

	// 동적 생성된 위젯 파괴
	GameWidget *pWidget = NULL;
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end();) {
		pWidget = (*iterWidget);
		iterWidget = widgetList.erase(iterWidget);
		
		if (pWidget != NULL) delete pWidget;
	}
	pWidget = NULL;

	// 남은 사운드 파괴
	if (pSoundManager != NULL) pSoundManager->destroySoundAll();

	// 하늘상자 및 지형 파괴
	if (pSkybox != NULL) delete pSkybox;
	if (pTerrain != NULL) delete pTerrain;

	// 포인터를 NULL로
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
	// 가상 함수
}

bool GameFrame::updateFrame(REAL timeElapsed) {
	// 가상 함수
	return true;
}