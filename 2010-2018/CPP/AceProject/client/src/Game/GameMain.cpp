#include "Engine/GameManager.h"
#include "Engine/GameWindow.h"
#include "Engine/DrawManager.h"
#include "Engine/InputManager.h"
#include "Game/GameMain.h"
#include "Game/Frame/StartFrame.h"
#include <time.h>

#ifdef __DEBUG__
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" ) // WinAPI ������Ʈ�� ������ �ܼ�â�� ���� ����
#endif

wstring g_wsGameTitle = wstring(TEXT("Test Window - ����� ������"));	// ���� ������ Ÿ��Ʋ(StdHeader.h �� ����� ���� ����)
GameManager *g_pGameManager = NULL;										// ���� �Ŵ��� ������(StdHeader.h �� ����� ���� ����)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	// ���� �õ� ����
	srand(time(NULL));

	try {
		// 1. ���� �Ŵ��� ����
		GameManager *pMainManager = new GameManager(hInstance, hPrevInstance, lpszCmdParam, nCmdShow);
		g_pGameManager = pMainManager;

		if (pMainManager == NULL) {
			MessageBox(NULL, TEXT("WinMain error."), TEXT("Fail to construct GameManager"), (MB_OK | MB_ICONINFORMATION));
			exit(EXIT_FAILURE);
		}

		// 2. ��ǥ LPS/FPS����
		g_pGameManager->setTargetLPS(50.0);		// �ִ�   0.02 �ʿ� �� �� ���� ������ ���� (���� �������� ���� ���� ������ ����Ʈ)
		g_pGameManager->setTargetFPS(60.0);		// �ִ� 0.0167 �ʿ� �� �� ȭ�� �׸��� �۾� �� ����

		// 3. ���� ������ ���� (���� ������)
		g_pGameManager->setGameFrame(new StartFrame());
		//g_pGameManager->setGameFrame(new LobbyFrame(NULL));

		// 4. ���� ���� ����
		g_pGameManager->startMainLoop();
	}
	catch (exception e) { cout << "���� �߻�! ����: " << e.what() <<  endl << "�����Ϸ��� ���͸� �Է��Ͻʽÿ�.\a" << endl;
						  getchar(); exit(EXIT_FAILURE); }

	// 5. �Ŵ��� ���� �� ���α׷� ���� ����
	if (g_pGameManager != NULL) delete g_pGameManager;
	g_pGameManager = NULL;
	return 0;
}
