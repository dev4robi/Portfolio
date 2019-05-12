#include "Engine/GameManager.h"
#include "Engine/GameWindow.h"
#include "Engine/DrawManager.h"
#include "Engine/InputManager.h"
#include "Game/GameMain.h"
#include "Game/Frame/StartFrame.h"
#include <time.h>

#ifdef __DEBUG__
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" ) // WinAPI 프로젝트에 디버깅용 콘솔창을 띄우기 위함
#endif

wstring g_wsGameTitle = wstring(TEXT("Test Window - 실험용 윈도우"));	// 게임 윈도우 타이틀(StdHeader.h 에 선언된 전역 변수)
GameManager *g_pGameManager = NULL;										// 게임 매니저 포인터(StdHeader.h 에 선언된 전역 변수)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	// 난수 시드 설정
	srand(time(NULL));

	try {
		// 1. 게임 매니저 생성
		GameManager *pMainManager = new GameManager(hInstance, hPrevInstance, lpszCmdParam, nCmdShow);
		g_pGameManager = pMainManager;

		if (pMainManager == NULL) {
			MessageBox(NULL, TEXT("WinMain error."), TEXT("Fail to construct GameManager"), (MB_OK | MB_ICONINFORMATION));
			exit(EXIT_FAILURE);
		}

		// 2. 목표 LPS/FPS설정
		g_pGameManager->setTargetLPS(50.0);		// 최대   0.02 초에 한 번 게임 로직을 수행 (게임 안정성을 위한 고정 프레임 레이트)
		g_pGameManager->setTargetFPS(60.0);		// 최대 0.0167 초에 한 번 화면 그리기 작업 및 갱신

		// 3. 시작 프레임 설정 (메인 프레임)
		g_pGameManager->setGameFrame(new StartFrame());
		//g_pGameManager->setGameFrame(new LobbyFrame(NULL));

		// 4. 메인 루프 시작
		g_pGameManager->startMainLoop();
	}
	catch (exception e) { cout << "오류 발생! 원인: " << e.what() <<  endl << "종료하려면 엔터를 입력하십시오.\a" << endl;
						  getchar(); exit(EXIT_FAILURE); }

	// 5. 매니저 해제 후 프로그램 정상 종료
	if (g_pGameManager != NULL) delete g_pGameManager;
	g_pGameManager = NULL;
	return 0;
}
