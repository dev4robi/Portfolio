#include "Engine/GameWindow.h"

GameWindow::GameWindow(wstring wndName, LRESULT (CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM),
					   HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdParam, int nCmdShow, Size2D size) {
	// 윈도우 이름 설정
	setWndName(wndName);

	// 윈도우 클래스 정의
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = (hInst);
	wndClass.lpfnWndProc = (fpWndProc);
	wndClass.lpszClassName = (wsWndName.c_str());
	wndClass.lpszMenuName = (NULL);
	wndClass.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);

	// 윈프록 설정
	setWndProc(fpWndProc);

	// 윈메인 매개변수 설정
	GameWindow::wndSize = size;
	GameWindow::bActiveState = true;
	GameWindow::hInstance = hInst;
	GameWindow::hPrevInstance = hPrevInst;
	GameWindow::lpszCmdParam = lpszCmdParam;
	GameWindow::nCmdShow = nCmdShow;
	GameWindow::hDC = NULL;
	GameWindow::hRC = NULL;
	GameWindow::enDisplayMode = (DM_FULLSCREEN);
								//(DM_WINDOWDED); // 초기 화면 모드(전체화면 / 창모드)
}

GameWindow::~GameWindow() {

}

void GameWindow::setWndName(wstring wndName) {
	wsWndName = wndName;
}

wstring GameWindow::getWndName() {
	return wsWndName;
}

void GameWindow::setWndProc(LRESULT (CALLBACK *fp)(HWND, UINT, WPARAM, LPARAM)) {
	fpWndProc = fp;
}

bool GameWindow::createWnd() {
	// 윈도우 클래스 등록 시도
	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, TEXT("Fail to register WNDCLASS."), TEXT("GameWindow::createWnd() Register Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// 윈도우 생성
	LPCTSTR lpszClass = wndClass.lpszClassName;
	GameWindow::hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
									  0, 0, wndSize.getWidth(), wndSize.getHeight(),
									  NULL, (HMENU)NULL, hInstance, NULL);

	// 윈도우 생성 실패 시
	if (hWnd == NULL) {
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to create window."), TEXT("GameWindow::createWnd() CreateWindowEx Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// 전체화면 모드 시도
	if (!setDisplayMode(enDisplayMode)) {
		MessageBox(NULL, TEXT("Fail to select display mode."), TEXT("GameWindow::createWnd() Display Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// 픽셀 포맷 번호를 가져옴
	DWORD dwBits = 24;
	static PIXELFORMATDESCRIPTOR pfd = {	// 윈도우즈에게 필요로 하는 픽셀포맷 정보를 전달
		sizeof(PIXELFORMATDESCRIPTOR),		// 픽셀포맷 디스크립터의 크기
		1,									// 버전
		PFD_DRAW_TO_WINDOW |				// 포맷이 윈도우를 지원해야 함
		PFD_SUPPORT_OPENGL |				// 포맷이 OpenGL을 지원해야 함
		PFD_DOUBLEBUFFER,					// 더블 버퍼링을 지원해야 함
		PFD_TYPE_RGBA,						// RGBA 포맷을 요청
		dwBits,								// 색상 깊이를 선택
		0, 0, 0, 0, 0, 0,					// 색상비트 무시
		0,									// 알파 버퍼 없음
		0,									// 쉬프트 비트 무시
		0,									// Accumulation 버퍼 없음
		0, 0, 0, 0,							// Accumulation 비트 무시
		24,									// 24비트 깊이 버퍼(Z-Buffer)
		8,									// 8비트 스텐실 버퍼
		0,									// Auxiliary 버퍼 없음
		PFD_MAIN_PLANE,						// 메인 드로잉 레이어
		0,									// 예약된 공간
		0, 0, 0								// 레이어 마스크 무시
	};

	// HDC 획득
	if (!(hDC = GetDC(hWnd))) {
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get HDC from window."), TEXT("GameWindow::createWnd() HDC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// 픽셀포맷 획득 및 설정
	int iPixelFormat;
	if (!(iPixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		// 픽셀포맷 획득 실패
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to set Pixelformat to HDC."), TEXT("GameWindow::createWnd() Pixelformat get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}
	else {
		// 픽셀포맷 획득 성공 후 설정
		if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) {
			// 픽셀 포맷을 설정 실패
			destroyWnd();
			MessageBox(NULL, TEXT("Fail to set Pixelformat to HDC."), TEXT("GameWindow::createWnd() Pixelformat set Error"), (MB_OK | MB_ICONINFORMATION));
			return false;
		}
	}

	// HGLRC 획득
	if (!(hRC = wglCreateContext(hDC))) {
		// HGLRC 획득 실패
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get HGLRC from OpenGL."), TEXT("GameWindow::createWnd() HGLRC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// HGLRC 활성화
	if (!wglMakeCurrent(hDC, hRC)) {
		// 활성화 실패 시 디스플레이 정보를 리셋 후 메시지 박스 출력
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to active HGLRC."), TEXT("GameWindow::createWnd() HGLRC activation Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// 쓰레드용 HGLRC 획득
	if (!(hThreadRC = wglCreateContext(hDC))) {
		// 쓰레드용 HGLRC 획득 실패
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get Thread_HGLRC from OpenGL."), TEXT("GameWindow::createWnd() HGLRC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// GameWindow 클래스의 쓰레드 HGLRC와 윈도우 HGLRC가 서로 같은 렌더링 콘텍스트를 공유하도록 함
	wglShareLists(hRC, hThreadRC);

	// 윈도우 표시
	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);	// 높은 우선권
	SetFocus(hWnd);				// 키보드 포커스를 현재 윈도우에 줌

	// 성공 반환
	return true;
}

bool GameWindow::destroyWnd() {
	// 전체화면 모드일 경우 안전한 종료를 위해 전체화면을 해제
	if (enDisplayMode == DM_FULLSCREEN) {
		ChangeDisplaySettings(NULL, CDS_RESET);	// 윈도우즈 레지스트리에 현재 저장된 기본 해상도, 깊이버퍼, 주파수 등을 사용
	}

	// 렌더링 컨텍스트를 가지고 있는지 검사하여 HDC에서 HGLRC를 분리(렌더링 컨텍스트 해제)
	if (hRC) {
		// DC와 RC컨텍스트의 해제 가능 여부 확인
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, TEXT("Fail to release DC and RC."), TEXT("GameWindow::destroyWnd() Release Error"), (MB_OK | MB_ICONINFORMATION));
		}

		// RC를 삭제 가능한지 확인
		if (!wglDeleteContext(hRC)) {
			MessageBox(NULL, TEXT("Fail to delete RC."), TEXT("GameWindow::destroyWnd() Delete Error"), (MB_OK | MB_ICONINFORMATION));
		}
		hRC = NULL;	// 삭제 여부 관련없이 RC핸들을 NULL로 변경		
	}
	if (hThreadRC) {
		// ThreadRC를 삭제 가능한지 확인
		if (!wglDeleteContext(hThreadRC)) {
			MessageBox(NULL, TEXT("Fail to delete ThreadRC."), TEXT("GameWindow::destroyWnd() Delete Error"), (MB_OK | MB_ICONINFORMATION));
		}
		hThreadRC = NULL;	// 삭제 여부 관련없이 ThreadRC핸들을 NULL로 변경		
	}

	// DC를 해제 가능한지 확인
	if ((hDC) && (!ReleaseDC(hWnd, hDC))) {
		MessageBox(NULL, TEXT("Fail to release DC."), TEXT("GameWindow::destroyWnd() Release Error"), (MB_OK | MB_ICONINFORMATION));
		hDC = NULL;	// 해제 불가할 시 DC핸들을 NULL로 변경
	}

	// 윈도우 핸들 존재유무와 창 파괴 후 성공여부를 확인
	if ((hWnd) && (!DestroyWindow(hWnd))) {
		MessageBox(NULL, TEXT("Fail to destroy Window."), TEXT("GameWindow::destroyWnd() Destroy Error"), (MB_OK | MB_ICONINFORMATION));
		hWnd = NULL;	// 윈도우 핸들을 NULL로 변경
	}

	// 프로그램 시작 시 등록했던 윈도우 클래스 파괴
	if (!UnregisterClass(wsWndName.c_str(), hInstance)) {
		MessageBox(NULL, TEXT("Fail to unregister WNDCLASS."), TEXT("GameWindow::destroyWnd() Unregister Error"), (MB_OK | MB_ICONINFORMATION));
		hInstance = NULL; // 윈도우 클래스 핸들을 NULL로 변경	
	}

	return true;
}

void GameWindow::setWndActiveState(bool active) {
	bActiveState = active;
}

bool GameWindow::getWndActiveState() {
	return bActiveState;
}

HWND GameWindow::getHWND() {
	return hWnd;
}

HDC* GameWindow::getHDCPtr() {
	return &hDC;
}

HGLRC* GameWindow::getHGLRCPtr() {
	return &hRC;
}

HGLRC* GameWindow::getHGLRC_ThreadPtr() {
	return &hThreadRC;
}

BOOL GameWindow::moveWnd() {
	return MoveWindow(hWnd, (int)wndPos.getX(), (int)wndPos.getY(), wndSize.getWidth(), wndSize.getHeight(), false);
}

BOOL GameWindow::resizeWnd() {
	RECT rect;
	rect.bottom = 0;
	rect.left = 0;
	rect.right = wndSize.getWidth();
	rect.top = wndSize.getHeight();
	AdjustWindowRectEx(&rect, dwStyle, NULL, dwExStyle);
	return MoveWindow(hWnd, (int)wndPos.getX(), (int)wndPos.getY(), wndSize.getWidth(), wndSize.getHeight(), false);
}

Point2D GameWindow::getWndPos() {
	return wndPos;
}

Size2D GameWindow::getWndSize() {
	return wndSize;
}

Size2D* GameWindow::getWndSizePtr() {
	return &wndSize;
}

void GameWindow::setWndSize(Size2D size) {
	wndSize = size;
}

void GameWindow::setWndPos(Point2D point) {
	wndPos = point;
}

bool GameWindow::setDisplayMode(DisplayMode DM_ENUM) {
	// 화면 모드 변경
	enDisplayMode = DM_ENUM;

	// 요청된 모드가 전체화면의 경우 전체화면을 시도
	DWORD dwBits = 32;
	if (enDisplayMode == DM_FULLSCREEN) {
		DEVMODE dmScreenSettings;													// 디스플레이 장치 모드
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));						// 장치 모드 0으로 초기화
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);							// DEVMODE 구조체의 크기
		dmScreenSettings.dmPelsWidth = wndSize.getWidth();							// 폭 전달
		dmScreenSettings.dmPelsHeight = wndSize.getHeight();						// 높이 전달
		dmScreenSettings.dmBitsPerPel = dwBits;										// 픽셀당 비트 전달 (Win8 이상에선 32이상의 값만 전달 가능)
		dmScreenSettings.dmFields = (DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT);	// 필드

		// 모드를 선택하고 결과를 받아온다
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, TEXT("Your VGA is not support fullscreen mode. Use windowed mode instead?"), TEXT("GameWindow::createWnd() Fullscreen Error"), (MB_YESNO | MB_ICONINFORMATION) == IDYES)) {
				enDisplayMode = DM_WINDOWDED;	// 창 모드로 강제 변환
			}
			else {
				return false; // 해상도 변경 실패
			}
		}
	}

	// 전체화면이 지원됐는지 다시 확인
	if (enDisplayMode == DM_FULLSCREEN) {
		dwExStyle = (WS_EX_APPWINDOW | WS_EX_TOPMOST);	// 윈도우 확장 스타일
		dwStyle = (WS_POPUP | WS_VISIBLE);				// 팝업 윈도우 스타일
	}
	// 창 모드 설정으로 변환
	else {
		ChangeDisplaySettings(NULL, CDS_RESET);
		wndSize.setWidth(wndSize.getWidth() * 0.90);		// 전체화면이 아니면 윈도우 크기를 해상도의 90%크기로 생성
		wndSize.setHeight(wndSize.getHeight() * 0.90);
		dwExStyle = (WS_EX_LEFT);							// 윈도우즈 확장 스타일
		dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE);		// 윈도우 스타일
	}

	// 요청된 디스플레이 크기로 윈도우 크기 및 스타일 변경
	RECT wndRect;
	wndRect.top = 0;
	wndRect.right = wndSize.getWidth();
	wndRect.left = 0;
	wndRect.bottom = wndSize.getHeight();
	SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, wndSize.getWidth(), wndSize.getHeight(), SWP_SHOWWINDOW);

	// 전체화면일 경우 최대화를 시킴
	if (enDisplayMode == DM_FULLSCREEN) {
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	return true;
}

DisplayMode GameWindow::getDisplayMode() {
	return enDisplayMode;
}

void GameWindow::setMouseVisible(bool isShow) {
	if (isShow == true) {
		ShowCursor(true);
	}
	else {
		ShowCursor(false);
	}
}
