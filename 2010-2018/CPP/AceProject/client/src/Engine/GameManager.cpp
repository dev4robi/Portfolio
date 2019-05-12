#include "Engine/GameWindow.h"
#include "Engine/GameFrame.h"
#include "Engine/DrawManager.h"
#include "Engine/InputManager.h"
#include "Engine/ResourceManager.h"
#include "Engine/SoundManager.h"
#include "Engine/GameManager.h"
#include "Engine/Network/NetworkManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[255];
	GameManager *pGameManager = g_pGameManager;
	InputManager *pInputMgr = g_pGameManager->getInputManagerPtr();

	// ���� �Ŵ����� �����Ǿ� �ִ��� Ȯ�� (����! : CreateWindowEx() �Լ����� WndProc�� �� ���� ȣ���Ѵ�. �� ���� pGameManager�� �ּҰ� ���������� �ʱ� ������ �̷� ���밡 �ʿ�!)
	if (g_pGameManager == NULL) {
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	// ��� �ð��� ����
	float strt = g_pGameManager->getElapsedTime();

	switch (iMessage) {
	case WM_ACTIVATE: // ������ Ȱ�� �޽���
		if (!HIWORD(wParam)) {
			// ������ �ִ�ȭ�� Ȱ��ȭ
			g_pGameManager->getGameWndPtr()->setWndActiveState(true);
		}
		else {
			// ������ �ּ�ȭ�� ��Ȱ��ȭ
			g_pGameManager->getGameWndPtr()->setWndActiveState(false);
		}
		return 0;
	case WM_SYSCOMMAND: // �ý��� �޽����� ����è
		switch (wParam) {
			// ȭ�麸ȣ��� ����� ������� �޽����� ����ä�� �߻����� �ʵ��� ��
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	case WM_SIZE: // OpenGL ������ ũ�� ���� ��
		if (g_pGameManager->getDrawManagerPtr() != NULL) {
			g_pGameManager->getGameWndPtr()->setWndSize(Size2D(LOWORD(lParam), HIWORD(lParam)));
		}
		return 0;
	case WM_MOUSEMOVE: // ���콺 �̵�
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_MBUTTONDOWN: // �� ��ư Ŭ��
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_WHEELBTN, true);
		}
		return 0;
	case WM_MBUTTONUP: // �� ��ư ��
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_WHEELBTN, false);
		}
		return 0;
	case WM_MOUSEWHEEL: // ���콺 ��
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			if ((SHORT)HIWORD(wParam) > 0){// ���콺���� �ø� ��� 
				pInputMgr->setMousemap(mouse_button::MOUSE_WHEELUPBTN, true);
			}
			else {// ���콺 ���� ���� ���
				pInputMgr->setMousemap(mouse_button::MOUSE_WHEELDOWNBTN, true);
			}
		}
		return 0;
	case WM_LBUTTONDOWN: // ���� ��ư �ٿ�
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_LEFTBTN, true);
		}
		return 0;
	case WM_LBUTTONUP: // ���� ��ư ��
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_LEFTBTN, false);
		}
		return 0;
	case WM_RBUTTONDOWN: // ���� ��ư �ٿ�
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_RIGHTBTN, true);
		}
		return 0;
	case WM_RBUTTONUP: // ���� ��ư ��
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_RIGHTBTN, false);
		}
		return 0;
	//case WM_SYSKEYDOWN: return 0; // ALT Ű�� �Բ� Ű�ٿ� (Ȱ��ȭ �� ALT+F4 ���� ��ɾ� ����� ���� ó�� ����)
	case WM_KEYDOWN: // Ű�ٿ�
		if (pInputMgr != NULL) {
			pInputMgr->setKeymap(wParam, true);
		}
		return 0;
	case WM_KEYUP: // Ű��
		if (pInputMgr != NULL) {
			pInputMgr->setKeymap(wParam, false);
		}
		return 0;
	case WM_PAINT: // �׽�Ʈ�� ������ �׸���
		hdc = BeginPaint(hWnd, &ps);
		#ifdef __DEBUG__
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(0, 0, 0));
			strt = g_pGameManager->getElapsedTime();
			swprintf_s(str, TEXT("TimeElapsed: %\.4lf��"), (strt / 1000.0));
			TextOut(hdc, 1650, 10, str, lstrlen(str));
			swprintf_s(str, TEXT("currentLPS: %.2f"), g_pGameManager->getCurrentLPS());
			TextOut(hdc, 1650, 30, str, lstrlen(str));
			swprintf_s(str, TEXT("currentFPS: %.2f"), g_pGameManager->getCurrentFPS());
			TextOut(hdc, 1650, 50, str, lstrlen(str));
			swprintf_s(str, TEXT("Mouse x: %4d, y: %4d"), g_pGameManager->getInputManagerPtr()->getMousePos().x, g_pGameManager->getInputManagerPtr()->getMousePos().y);
			TextOut(hdc, 1650, 70, str, lstrlen(str));
		#endif
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CHAR: // ���� �Է�
		pInputMgr->setLastInputChar((TCHAR)wParam);
		return 0;
	case WM_CREATE: // ���� ��
		return 0;
	case WM_CLOSE: // ���� ��
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

GameManager::GameManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	// 1. ���� ���� ���� �Ŵ����� ���� �ش� Ŭ������ ����, ���� �Ŵ��� ���� �ð��� ��� �ð��� ����
	g_pGameManager = this;
	dwCreatedTime = timeGetTime();
	dwElapsedTime = 0;

	// 2. �Ŵ����� ������ �����͸� NULL�� �ʱ�ȭ
	pGameWnd = NULL;
	pGameFrame = NULL;
	pDrawManager = NULL;
	pInputManager = NULL;
	pResourceManager = NULL;
	pSoundManager = NULL;

	// 3. ������ ����
	RECT resolution;
	GetWindowRect(GetDesktopWindow(), &resolution);
	Size2D windowSize(resolution.right, resolution.bottom);
	GameWindow *pTempGameWnd = new GameWindow(g_wsGameTitle, WndProc, hInstance, hPrevInstance, lpszCmdParam, nCmdShow, windowSize);
	setGameWnd(pTempGameWnd);
	if ((pGameWnd->createWnd()) == false) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : GameWindow creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}

	// 4. �Ŵ��� ����
	// 4-1. ��ο� �Ŵ���
	windowSize.set(pGameWnd->getWndSize()); // ����! : â ���� ���� �� �ػ󵵰� ����Ǳ� ������ �ٽ� �޾ƿ´�
	DrawManager *pTempDrawManager = new DrawManager(pGameWnd->getHDCPtr(), pGameWnd->getHGLRCPtr(), pGameWnd->getWndSizePtr());
	if (pTempDrawManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : DrawManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setDrawManager(pTempDrawManager);

	// 4-2. ��ǲ �Ŵ���
	InputManager *pTempInputManager = new InputManager();
	if (pTempInputManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : InputManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setInputManager(pTempInputManager);

	// 4-3. ���ҽ� �Ŵ���
	ResourceManager *pTempResourceManager = new ResourceManager();
	if (pTempResourceManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : ResourceManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setResourceManager(pTempResourceManager);

	// 4-4. ���� �Ŵ���
	SoundManager *pTempSoundManager = new SoundManager();
	if (pTempSoundManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : SoundManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setSoundManager(pTempSoundManager);

	// 4-5. ��Ʈ��ũ �Ŵ���
	NetworkManager *pTempNetworkManager = new NetworkManager();
	if (pTempNetworkManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : NetworkManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setNetworkManager(pTempNetworkManager);

	// �� �� �Ŵ��� �߰��� ���⿡ �߰�
	// ...
}

GameManager::~GameManager() {
	// 1. ������ ������ �ʱ�ȭ
	if (pGameWnd != NULL) pGameWnd->setMouseVisible(true);

	// 2. ��Ʈ��ũ �Ŵ��� �ı�
	if (pNetworkManager != NULL) {
		delete pNetworkManager;
	}
	pNetworkManager = NULL;
	
	// 3. ��ο� �Ŵ��� �ı�
	if (pDrawManager != NULL)
		delete pDrawManager;
	pDrawManager = NULL;

	// 4. ��ǲ �Ŵ��� �ı�
	if (pInputManager != NULL) {
		delete pInputManager;
	}
	pInputManager = NULL;

	// 5. ���ҽ� �Ŵ��� �ı�
	if (pResourceManager != NULL) {
		delete pResourceManager;
	}
	pResourceManager = NULL;

	// 6. ���� �Ŵ��� �ı�
	if (pSoundManager != NULL) {
		delete pSoundManager;
	}
	pSoundManager = NULL;

	// 7. ���� ������ �ı�
	if (pGameWnd != NULL)
		pGameWnd->destroyWnd();
		delete pGameWnd;
	pGameWnd = NULL;
}

void GameManager::setGameWnd(GameWindow* gameWnd) {
	GameManager::pGameWnd = gameWnd;
}

GameWindow* GameManager::getGameWndPtr() {
	return GameManager::pGameWnd;
}

void GameManager::setGameFrame(GameFrame* gameFrame) {
	GameManager::pGameFrame = gameFrame;
}

GameFrame* GameManager::getGameFramePtr() {
	return GameManager::pGameFrame;
}

void GameManager::setDrawManager(DrawManager* drawManager) {
	GameManager::pDrawManager = drawManager;
}

DrawManager* GameManager::getDrawManagerPtr() {
	return pDrawManager;
}

void GameManager::setInputManager(InputManager* inputManager) {
	pInputManager = inputManager;
}

InputManager* GameManager::getInputManagerPtr() {
	return pInputManager;
}

void GameManager::setResourceManager(ResourceManager *resourceManager) {
	pResourceManager = resourceManager;
}

ResourceManager* GameManager::getResourceManagerPtr() {
	return pResourceManager;
}

void GameManager::setSoundManager(SoundManager *soundManager) {
	pSoundManager = soundManager;
}

SoundManager* GameManager::getSoundManagerPtr() {
	return pSoundManager;
}

void GameManager::setNetworkManager(NetworkManager *networkManager) {
	pNetworkManager = networkManager;
}

NetworkManager* GameManager::getNetworkManagerPtr() {
	return pNetworkManager;
}

DWORD GameManager::getCreatedTime() {
	return dwCreatedTime;
}

DWORD GameManager::getElapsedTime() {
	return dwElapsedTime;
}

float GameManager::getCurrentLPS() {
	return fCurrentLPS;
}

void GameManager::setTargetLPS(float lps) {
	fTargetLPS = lps;
}

float GameManager::getCurrentFPS() {
	return fCurrentFPS;
}

void GameManager::setTargetFPS(float fps) {
	fTargetFPS = fps;
}

void GameManager::startMainLoop() {
	MSG msg;								// ������ �̺�Ʈ �޽���
	bool bIsQuit = false;					// ���� ���� ���� Ȯ�� ����
	int iLogicCounter = 0;					// ���� ī����
	int iFrameCounter = 0;					// ������ ī����
	double dbLastEngineTime;				// ���������� ���� ������ ���� �ð�
	double dbNextLogicTime;					// �������� ���� ó���� �����ؾ� �ϴ� �ð� (���е��� ���� double�� ��� ����)
	double dbNextFrameTime;					// �������� �������� �����ؾ� �ϴ� �ð�
	double dbNextCounterTime;				// �������� LPS/FPS ī������ �����ؾ� �ϴ� �ð�
	const float fCounterInterval = 1000.0;	// 500 = 0.5 �ʿ� �� �� ��� LPS/FPS ���

	timeBeginPeriod(1);							// timeGetTime() �Լ��� �ػ󵵸� 1ms(0.001��)�� ����
	dbNextLogicTime = (double)timeGetTime();	// �������� ���� ���� �帥 �ð��� �����ͼ� ���� ���� ó���ð��� ����
	dbNextFrameTime = dbNextLogicTime;
	dbNextCounterTime = dbNextLogicTime;

	// 1. ���� ���� ����
	while (bIsQuit != true) {
		// 2. ������ �޽��� �켱 ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// 2-1. ������ �޽����� ���� �� �޽����� ������
			GetMessage(&msg, NULL, 0, 0);

			// 2-2. ���� �޽����� ��� ���� ���� Ż���� ����
			if (msg.message == WM_QUIT) {
				bIsQuit = true;
			}

			// 2-3. ������ �޽��� ���� �� ����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 3. ������ �޽����� ���� �� ���� ���� ó��
		else {
			// 3-1. ���� ���� �� ����ð� ����			
			dwElapsedTime = (timeGetTime() - dwCreatedTime);

			// 3-2. ����� �� ���� ���� ó��
			if (((double)timeGetTime() > dbNextLogicTime)) {
				if (fCurrentLPS > fTargetLPS + 10) { // â��� �巡�� ������ LPS�� ġ�ڴ� ��츦 ����
					fCurrentLPS = fTargetLPS;
					dbNextLogicTime = (double)timeGetTime() + (1000.0 / fTargetLPS);
				}
				else { // ���⼭ ����°� ���� ���� ���� ó�� (���� ó�� �ð��� ������ 0.02�ʿ� �� ������ ���� ������ ��� - �� ���� �� 900���� ������ ������� ���� ���� ���� Ȯ��)
					REAL timeElapsed = (1.0f / fTargetLPS);					// ������ ������Ʈ �ð� (0.02)
					pSoundManager->update(timeElapsed);						// ���� ������Ʈ
					pInputManager->update(timeElapsed);						// �Է� ó��
					if (pGameFrame->updateFrame(timeElapsed) == false) {	// ������ ������Ʈ (false ��ȯ �� ���� Ż��)
						bIsQuit = true;
						continue;
					}

					iLogicCounter++;
					dbNextLogicTime += (1000.0 / fTargetLPS);
				}
			}

			// 3-3. ������ Ȱ��ȭ ������ �� �׸��� �� LPS/FPS���� ó��
			if (pGameWnd->getWndActiveState() == true) {
				if (((double)timeGetTime() > dbNextFrameTime)) {
					if (fCurrentFPS > fTargetFPS + 10) { // â��� �巡�� ������ FPS�� ġ�ڴ� ��츦 ����
						fCurrentFPS = fTargetFPS;
						dbNextFrameTime = (double)timeGetTime() + (1000.0 / fTargetFPS);
					}
					else {
						// 3-4. ȭ�� ������
  						pDrawManager->draw();
						pDrawManager->swap();

						#ifdef __DEBUG__
							RECT tempRect;
							tempRect.top = 10;
							tempRect.right = 1920;
							tempRect.left = 1650;
							tempRect.bottom = 90;
							InvalidateRect(getGameWndPtr()->getHWND(), &tempRect, TRUE);
						#endif

						iFrameCounter++;
						dbNextFrameTime += (1000.0 / fTargetFPS);
					}
				}

				// 3-5. LPS/FPS ���� �� ����
				if ((double)timeGetTime() > dbNextCounterTime) {
					fCurrentLPS = iLogicCounter;
					fCurrentFPS = iFrameCounter;
					fCurrentLPS = ((float)iLogicCounter * (1000.0 / fCounterInterval));
					fCurrentFPS = ((float)iFrameCounter * (1000.0 / fCounterInterval));
					iLogicCounter = 0;
					iFrameCounter = 0;
					dbNextCounterTime += fCounterInterval;
				}
			}
			// 3-6. ������ ��Ȱ��ȭ ������ ���� ���� �׸��� �ð��� �缳���� �ش�
			else {
				iLogicCounter = 0;
				iFrameCounter = 0;
				dbNextFrameTime = ((double)timeGetTime() + (1000.0 / fTargetFPS));
				dbNextCounterTime = ((double)timeGetTime() + fCounterInterval);
			}
		}
	}

	timeEndPeriod(1);	// timeGetTime()�� �ػ󵵸� ����

	// 4. ���� ���� ����
	return;
}
