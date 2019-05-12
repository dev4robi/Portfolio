#include "Engine/GameWindow.h"

GameWindow::GameWindow(wstring wndName, LRESULT (CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM),
					   HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdParam, int nCmdShow, Size2D size) {
	// ������ �̸� ����
	setWndName(wndName);

	// ������ Ŭ���� ����
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

	// ������ ����
	setWndProc(fpWndProc);

	// ������ �Ű����� ����
	GameWindow::wndSize = size;
	GameWindow::bActiveState = true;
	GameWindow::hInstance = hInst;
	GameWindow::hPrevInstance = hPrevInst;
	GameWindow::lpszCmdParam = lpszCmdParam;
	GameWindow::nCmdShow = nCmdShow;
	GameWindow::hDC = NULL;
	GameWindow::hRC = NULL;
	GameWindow::enDisplayMode = (DM_FULLSCREEN);
								//(DM_WINDOWDED); // �ʱ� ȭ�� ���(��üȭ�� / â���)
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
	// ������ Ŭ���� ��� �õ�
	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, TEXT("Fail to register WNDCLASS."), TEXT("GameWindow::createWnd() Register Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// ������ ����
	LPCTSTR lpszClass = wndClass.lpszClassName;
	GameWindow::hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
									  0, 0, wndSize.getWidth(), wndSize.getHeight(),
									  NULL, (HMENU)NULL, hInstance, NULL);

	// ������ ���� ���� ��
	if (hWnd == NULL) {
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to create window."), TEXT("GameWindow::createWnd() CreateWindowEx Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// ��üȭ�� ��� �õ�
	if (!setDisplayMode(enDisplayMode)) {
		MessageBox(NULL, TEXT("Fail to select display mode."), TEXT("GameWindow::createWnd() Display Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// �ȼ� ���� ��ȣ�� ������
	DWORD dwBits = 24;
	static PIXELFORMATDESCRIPTOR pfd = {	// ��������� �ʿ�� �ϴ� �ȼ����� ������ ����
		sizeof(PIXELFORMATDESCRIPTOR),		// �ȼ����� ��ũ������ ũ��
		1,									// ����
		PFD_DRAW_TO_WINDOW |				// ������ �����츦 �����ؾ� ��
		PFD_SUPPORT_OPENGL |				// ������ OpenGL�� �����ؾ� ��
		PFD_DOUBLEBUFFER,					// ���� ���۸��� �����ؾ� ��
		PFD_TYPE_RGBA,						// RGBA ������ ��û
		dwBits,								// ���� ���̸� ����
		0, 0, 0, 0, 0, 0,					// �����Ʈ ����
		0,									// ���� ���� ����
		0,									// ����Ʈ ��Ʈ ����
		0,									// Accumulation ���� ����
		0, 0, 0, 0,							// Accumulation ��Ʈ ����
		24,									// 24��Ʈ ���� ����(Z-Buffer)
		8,									// 8��Ʈ ���ٽ� ����
		0,									// Auxiliary ���� ����
		PFD_MAIN_PLANE,						// ���� ����� ���̾�
		0,									// ����� ����
		0, 0, 0								// ���̾� ����ũ ����
	};

	// HDC ȹ��
	if (!(hDC = GetDC(hWnd))) {
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get HDC from window."), TEXT("GameWindow::createWnd() HDC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// �ȼ����� ȹ�� �� ����
	int iPixelFormat;
	if (!(iPixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		// �ȼ����� ȹ�� ����
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to set Pixelformat to HDC."), TEXT("GameWindow::createWnd() Pixelformat get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}
	else {
		// �ȼ����� ȹ�� ���� �� ����
		if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) {
			// �ȼ� ������ ���� ����
			destroyWnd();
			MessageBox(NULL, TEXT("Fail to set Pixelformat to HDC."), TEXT("GameWindow::createWnd() Pixelformat set Error"), (MB_OK | MB_ICONINFORMATION));
			return false;
		}
	}

	// HGLRC ȹ��
	if (!(hRC = wglCreateContext(hDC))) {
		// HGLRC ȹ�� ����
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get HGLRC from OpenGL."), TEXT("GameWindow::createWnd() HGLRC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// HGLRC Ȱ��ȭ
	if (!wglMakeCurrent(hDC, hRC)) {
		// Ȱ��ȭ ���� �� ���÷��� ������ ���� �� �޽��� �ڽ� ���
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to active HGLRC."), TEXT("GameWindow::createWnd() HGLRC activation Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// ������� HGLRC ȹ��
	if (!(hThreadRC = wglCreateContext(hDC))) {
		// ������� HGLRC ȹ�� ����
		destroyWnd();
		MessageBox(NULL, TEXT("Fail to get Thread_HGLRC from OpenGL."), TEXT("GameWindow::createWnd() HGLRC get Error"), (MB_OK | MB_ICONINFORMATION));
		return false;
	}

	// GameWindow Ŭ������ ������ HGLRC�� ������ HGLRC�� ���� ���� ������ ���ؽ�Ʈ�� �����ϵ��� ��
	wglShareLists(hRC, hThreadRC);

	// ������ ǥ��
	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);	// ���� �켱��
	SetFocus(hWnd);				// Ű���� ��Ŀ���� ���� �����쿡 ��

	// ���� ��ȯ
	return true;
}

bool GameWindow::destroyWnd() {
	// ��üȭ�� ����� ��� ������ ���Ḧ ���� ��üȭ���� ����
	if (enDisplayMode == DM_FULLSCREEN) {
		ChangeDisplaySettings(NULL, CDS_RESET);	// �������� ������Ʈ���� ���� ����� �⺻ �ػ�, ���̹���, ���ļ� ���� ���
	}

	// ������ ���ؽ�Ʈ�� ������ �ִ��� �˻��Ͽ� HDC���� HGLRC�� �и�(������ ���ؽ�Ʈ ����)
	if (hRC) {
		// DC�� RC���ؽ�Ʈ�� ���� ���� ���� Ȯ��
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, TEXT("Fail to release DC and RC."), TEXT("GameWindow::destroyWnd() Release Error"), (MB_OK | MB_ICONINFORMATION));
		}

		// RC�� ���� �������� Ȯ��
		if (!wglDeleteContext(hRC)) {
			MessageBox(NULL, TEXT("Fail to delete RC."), TEXT("GameWindow::destroyWnd() Delete Error"), (MB_OK | MB_ICONINFORMATION));
		}
		hRC = NULL;	// ���� ���� ���þ��� RC�ڵ��� NULL�� ����		
	}
	if (hThreadRC) {
		// ThreadRC�� ���� �������� Ȯ��
		if (!wglDeleteContext(hThreadRC)) {
			MessageBox(NULL, TEXT("Fail to delete ThreadRC."), TEXT("GameWindow::destroyWnd() Delete Error"), (MB_OK | MB_ICONINFORMATION));
		}
		hThreadRC = NULL;	// ���� ���� ���þ��� ThreadRC�ڵ��� NULL�� ����		
	}

	// DC�� ���� �������� Ȯ��
	if ((hDC) && (!ReleaseDC(hWnd, hDC))) {
		MessageBox(NULL, TEXT("Fail to release DC."), TEXT("GameWindow::destroyWnd() Release Error"), (MB_OK | MB_ICONINFORMATION));
		hDC = NULL;	// ���� �Ұ��� �� DC�ڵ��� NULL�� ����
	}

	// ������ �ڵ� ���������� â �ı� �� �������θ� Ȯ��
	if ((hWnd) && (!DestroyWindow(hWnd))) {
		MessageBox(NULL, TEXT("Fail to destroy Window."), TEXT("GameWindow::destroyWnd() Destroy Error"), (MB_OK | MB_ICONINFORMATION));
		hWnd = NULL;	// ������ �ڵ��� NULL�� ����
	}

	// ���α׷� ���� �� ����ߴ� ������ Ŭ���� �ı�
	if (!UnregisterClass(wsWndName.c_str(), hInstance)) {
		MessageBox(NULL, TEXT("Fail to unregister WNDCLASS."), TEXT("GameWindow::destroyWnd() Unregister Error"), (MB_OK | MB_ICONINFORMATION));
		hInstance = NULL; // ������ Ŭ���� �ڵ��� NULL�� ����	
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
	// ȭ�� ��� ����
	enDisplayMode = DM_ENUM;

	// ��û�� ��尡 ��üȭ���� ��� ��üȭ���� �õ�
	DWORD dwBits = 32;
	if (enDisplayMode == DM_FULLSCREEN) {
		DEVMODE dmScreenSettings;													// ���÷��� ��ġ ���
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));						// ��ġ ��� 0���� �ʱ�ȭ
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);							// DEVMODE ����ü�� ũ��
		dmScreenSettings.dmPelsWidth = wndSize.getWidth();							// �� ����
		dmScreenSettings.dmPelsHeight = wndSize.getHeight();						// ���� ����
		dmScreenSettings.dmBitsPerPel = dwBits;										// �ȼ��� ��Ʈ ���� (Win8 �̻󿡼� 32�̻��� ���� ���� ����)
		dmScreenSettings.dmFields = (DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT);	// �ʵ�

		// ��带 �����ϰ� ����� �޾ƿ´�
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, TEXT("Your VGA is not support fullscreen mode. Use windowed mode instead?"), TEXT("GameWindow::createWnd() Fullscreen Error"), (MB_YESNO | MB_ICONINFORMATION) == IDYES)) {
				enDisplayMode = DM_WINDOWDED;	// â ���� ���� ��ȯ
			}
			else {
				return false; // �ػ� ���� ����
			}
		}
	}

	// ��üȭ���� �����ƴ��� �ٽ� Ȯ��
	if (enDisplayMode == DM_FULLSCREEN) {
		dwExStyle = (WS_EX_APPWINDOW | WS_EX_TOPMOST);	// ������ Ȯ�� ��Ÿ��
		dwStyle = (WS_POPUP | WS_VISIBLE);				// �˾� ������ ��Ÿ��
	}
	// â ��� �������� ��ȯ
	else {
		ChangeDisplaySettings(NULL, CDS_RESET);
		wndSize.setWidth(wndSize.getWidth() * 0.90);		// ��üȭ���� �ƴϸ� ������ ũ�⸦ �ػ��� 90%ũ��� ����
		wndSize.setHeight(wndSize.getHeight() * 0.90);
		dwExStyle = (WS_EX_LEFT);							// �������� Ȯ�� ��Ÿ��
		dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE);		// ������ ��Ÿ��
	}

	// ��û�� ���÷��� ũ��� ������ ũ�� �� ��Ÿ�� ����
	RECT wndRect;
	wndRect.top = 0;
	wndRect.right = wndSize.getWidth();
	wndRect.left = 0;
	wndRect.bottom = wndSize.getHeight();
	SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, wndSize.getWidth(), wndSize.getHeight(), SWP_SHOWWINDOW);

	// ��üȭ���� ��� �ִ�ȭ�� ��Ŵ
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
