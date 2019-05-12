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

	// 게임 매니저가 생성되어 있는지 확인 (주의! : CreateWindowEx() 함수에서 WndProc을 수 차례 호출한다. 그 때는 pGameManager의 주소가 지정되있지 않기 때문에 이런 조취가 필요!)
	if (g_pGameManager == NULL) {
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	// 경과 시간을 저장
	float strt = g_pGameManager->getElapsedTime();

	switch (iMessage) {
	case WM_ACTIVATE: // 윈도우 활성 메시지
		if (!HIWORD(wParam)) {
			// 윈도우 최대화시 활성화
			g_pGameManager->getGameWndPtr()->setWndActiveState(true);
		}
		else {
			// 윈도우 최소화시 비활성화
			g_pGameManager->getGameWndPtr()->setWndActiveState(false);
		}
		return 0;
	case WM_SYSCOMMAND: // 시스템 메시지를 가로챔
		switch (wParam) {
			// 화면보호기와 모니터 절전모드 메시지를 가로채서 발생하지 않도록 함
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	case WM_SIZE: // OpenGL 윈도우 크기 변경 시
		if (g_pGameManager->getDrawManagerPtr() != NULL) {
			g_pGameManager->getGameWndPtr()->setWndSize(Size2D(LOWORD(lParam), HIWORD(lParam)));
		}
		return 0;
	case WM_MOUSEMOVE: // 마우스 이동
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_MBUTTONDOWN: // 휠 버튼 클릭
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_WHEELBTN, true);
		}
		return 0;
	case WM_MBUTTONUP: // 휠 버튼 뗌
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_WHEELBTN, false);
		}
		return 0;
	case WM_MOUSEWHEEL: // 마우스 휠
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			if ((SHORT)HIWORD(wParam) > 0){// 마우스휠을 올린 경우 
				pInputMgr->setMousemap(mouse_button::MOUSE_WHEELUPBTN, true);
			}
			else {// 마우스 휠을 내린 경우
				pInputMgr->setMousemap(mouse_button::MOUSE_WHEELDOWNBTN, true);
			}
		}
		return 0;
	case WM_LBUTTONDOWN: // 좌측 버튼 다운
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_LEFTBTN, true);
		}
		return 0;
	case WM_LBUTTONUP: // 좌측 버튼 업
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_LEFTBTN, false);
		}
		return 0;
	case WM_RBUTTONDOWN: // 우측 버튼 다운
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_RIGHTBTN, true);
		}
		return 0;
	case WM_RBUTTONUP: // 우측 버튼 업
		if (pInputMgr != NULL) {
			pInputMgr->setMousePos(LOWORD(lParam), HIWORD(lParam));
			pInputMgr->setMousemap(mouse_button::MOUSE_RIGHTBTN, false);
		}
		return 0;
	//case WM_SYSKEYDOWN: return 0; // ALT 키와 함께 키다운 (활성화 시 ALT+F4 같은 명령어 사용을 별도 처리 요함)
	case WM_KEYDOWN: // 키다운
		if (pInputMgr != NULL) {
			pInputMgr->setKeymap(wParam, true);
		}
		return 0;
	case WM_KEYUP: // 키업
		if (pInputMgr != NULL) {
			pInputMgr->setKeymap(wParam, false);
		}
		return 0;
	case WM_PAINT: // 테스트용 프레임 그리기
		hdc = BeginPaint(hWnd, &ps);
		#ifdef __DEBUG__
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(0, 0, 0));
			strt = g_pGameManager->getElapsedTime();
			swprintf_s(str, TEXT("TimeElapsed: %\.4lf초"), (strt / 1000.0));
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
	case WM_CHAR: // 문자 입력
		pInputMgr->setLastInputChar((TCHAR)wParam);
		return 0;
	case WM_CREATE: // 생성 시
		return 0;
	case WM_CLOSE: // 닫을 시
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

GameManager::GameManager(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	// 1. 전역 변수 게임 매니저의 값을 해당 클래스로 지정, 게임 매니저 생성 시간과 경과 시간을 저장
	g_pGameManager = this;
	dwCreatedTime = timeGetTime();
	dwElapsedTime = 0;

	// 2. 매니저와 프레임 포인터를 NULL로 초기화
	pGameWnd = NULL;
	pGameFrame = NULL;
	pDrawManager = NULL;
	pInputManager = NULL;
	pResourceManager = NULL;
	pSoundManager = NULL;

	// 3. 윈도우 생성
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

	// 4. 매니저 생성
	// 4-1. 드로우 매니저
	windowSize.set(pGameWnd->getWndSize()); // 주의! : 창 모드로 실행 시 해상도가 변경되기 때문에 다시 받아온다
	DrawManager *pTempDrawManager = new DrawManager(pGameWnd->getHDCPtr(), pGameWnd->getHGLRCPtr(), pGameWnd->getWndSizePtr());
	if (pTempDrawManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : DrawManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setDrawManager(pTempDrawManager);

	// 4-2. 인풋 매니저
	InputManager *pTempInputManager = new InputManager();
	if (pTempInputManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : InputManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setInputManager(pTempInputManager);

	// 4-3. 리소스 매니저
	ResourceManager *pTempResourceManager = new ResourceManager();
	if (pTempResourceManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : ResourceManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setResourceManager(pTempResourceManager);

	// 4-4. 사운드 매니저
	SoundManager *pTempSoundManager = new SoundManager();
	if (pTempSoundManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : SoundManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setSoundManager(pTempSoundManager);

	// 4-5. 네트워크 매니저
	NetworkManager *pTempNetworkManager = new NetworkManager();
	if (pTempNetworkManager == NULL) {
		MessageBox(NULL, TEXT("GameManager constructor error."), TEXT("GameManager::GameManager : NetworkManager creation error"), (MB_OK | MB_ICONINFORMATION));
		g_pGameManager = NULL;
		return;
	}
	setNetworkManager(pTempNetworkManager);

	// 그 외 매니저 추가시 여기에 추가
	// ...
}

GameManager::~GameManager() {
	// 1. 윈도우 설정값 초기화
	if (pGameWnd != NULL) pGameWnd->setMouseVisible(true);

	// 2. 네트워크 매니저 파괴
	if (pNetworkManager != NULL) {
		delete pNetworkManager;
	}
	pNetworkManager = NULL;
	
	// 3. 드로우 매니저 파괴
	if (pDrawManager != NULL)
		delete pDrawManager;
	pDrawManager = NULL;

	// 4. 인풋 매니저 파괴
	if (pInputManager != NULL) {
		delete pInputManager;
	}
	pInputManager = NULL;

	// 5. 리소스 매니저 파괴
	if (pResourceManager != NULL) {
		delete pResourceManager;
	}
	pResourceManager = NULL;

	// 6. 사운드 매니저 파괴
	if (pSoundManager != NULL) {
		delete pSoundManager;
	}
	pSoundManager = NULL;

	// 7. 게임 윈도우 파괴
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
	MSG msg;								// 윈도우 이벤트 메시지
	bool bIsQuit = false;					// 메인 루프 종료 확인 논리값
	int iLogicCounter = 0;					// 로직 카운터
	int iFrameCounter = 0;					// 프레임 카운터
	double dbLastEngineTime;				// 마지막으로 엔진 연산을 끝낸 시간
	double dbNextLogicTime;					// 다음번에 로직 처리를 시작해야 하는 시간 (정밀도를 위해 double형 사용 요함)
	double dbNextFrameTime;					// 다음번에 렌더링을 시작해야 하는 시간
	double dbNextCounterTime;				// 다음번에 LPS/FPS 카운팅을 수행해야 하는 시간
	const float fCounterInterval = 1000.0;	// 500 = 0.5 초에 한 번 평균 LPS/FPS 계산

	timeBeginPeriod(1);							// timeGetTime() 함수의 해상도를 1ms(0.001초)로 변경
	dbNextLogicTime = (double)timeGetTime();	// 윈도우즈 구동 이후 흐른 시간을 가져와서 다음 로직 처리시간에 대입
	dbNextFrameTime = dbNextLogicTime;
	dbNextCounterTime = dbNextLogicTime;

	// 1. 게임 메인 루프
	while (bIsQuit != true) {
		// 2. 윈도우 메시지 우선 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// 2-1. 윈도우 메시지가 있을 시 메시지를 가져옴
			GetMessage(&msg, NULL, 0, 0);

			// 2-2. 종료 메시지의 경우 메인 루프 탈출을 유도
			if (msg.message == WM_QUIT) {
				bIsQuit = true;
			}

			// 2-3. 윈도우 메시지 번역 및 전달
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 3. 윈도우 메시지가 없을 시 게임 엔진 처리
		else {
			// 3-1. 엔진 실행 후 경과시간 저장			
			dwElapsedTime = (timeGetTime() - dwCreatedTime);

			// 3-2. 입출력 및 메인 로직 처리
			if (((double)timeGetTime() > dbNextLogicTime)) {
				if (fCurrentLPS > fTargetLPS + 10) { // 창모드 드래그 등으로 LPS가 치솟는 경우를 방지
					fCurrentLPS = fTargetLPS;
					dbNextLogicTime = (double)timeGetTime() + (1000.0 / fTargetLPS);
				}
				else { // 여기서 입출력과 게임 메인 로직 처리 (로직 처리 시간은 무조건 0.02초에 한 번으로 고정 프레임 사용 - 한 루프 당 900만번 정도의 연산까지 성능 감소 없음 확인)
					REAL timeElapsed = (1.0f / fTargetLPS);					// 고정된 업데이트 시간 (0.02)
					pSoundManager->update(timeElapsed);						// 사운드 업데이트
					pInputManager->update(timeElapsed);						// 입력 처리
					if (pGameFrame->updateFrame(timeElapsed) == false) {	// 프레임 업데이트 (false 반환 시 루프 탈출)
						bIsQuit = true;
						continue;
					}

					iLogicCounter++;
					dbNextLogicTime += (1000.0 / fTargetLPS);
				}
			}

			// 3-3. 윈도우 활성화 상태일 때 그리기 및 LPS/FPS연산 처리
			if (pGameWnd->getWndActiveState() == true) {
				if (((double)timeGetTime() > dbNextFrameTime)) {
					if (fCurrentFPS > fTargetFPS + 10) { // 창모드 드래그 등으로 FPS가 치솟는 경우를 방지
						fCurrentFPS = fTargetFPS;
						dbNextFrameTime = (double)timeGetTime() + (1000.0 / fTargetFPS);
					}
					else {
						// 3-4. 화면 렌더링
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

				// 3-5. LPS/FPS 연산 후 저장
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
			// 3-6. 윈도우 비활성화 상태일 때는 다음 그리기 시간을 재설정해 준다
			else {
				iLogicCounter = 0;
				iFrameCounter = 0;
				dbNextFrameTime = ((double)timeGetTime() + (1000.0 / fTargetFPS));
				dbNextCounterTime = ((double)timeGetTime() + fCounterInterval);
			}
		}
	}

	timeEndPeriod(1);	// timeGetTime()의 해상도를 복구

	// 4. 메인 루프 종료
	return;
}
