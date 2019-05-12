#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameWindow (v6)
*	- Last Update : 2016.10.19
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ������ �ü������ ������ ����ϱ� ���� Ŭ����. �ü������ ǥ��
*					 �Ǵ� �����츦 ������ �� �ִ� Ŭ�����̴�.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): ���� ������ ���� ����
*   - Version(v2): OpenGL ������� ȣȯ
*   - Version(v3): ��üȭ�� ��� �� â ��� �۾� (2016.03.23)
*	- Version(v4): PIXELFORMATDESCRIPTOR ����ü�� �ùٸ� �ڷ����� ���� �ʾ�
*				   Ư�� ȯ�濡�� ���ٽ� ���� ����� ������� �ʾҴ� ���� ����. (2016.09.17)
*	- Version(v5): ��׶��� �����带 ���� ���ҽ� �ε带 ���� hThreadRC ��� �߰� �� �ʱ�ȭ ��ƾ ���� (2016.10.14)
*	- Version(v6): ���콺 �����/���̱� ���� ��� �Լ� �߰�. (2016.10.19)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

enum DisplayMode {
	DM_WINDOWDED = 0, DM_FULLSCREEN
};

class GameWindow : public GameObject {
private:
	// �Ϲ� ������ �ɹ� ����
	wstring		wsWndName;											// ������ �̸�
	Point2D		wndPos;												// ������ ��ġ
	Size2D		wndSize;											// ������ �۾����� ũ��
	bool		bActiveState;										// ������ Ȱ��ȭ ����
	HWND		hWnd;												// ������ �ڵ�
	WNDCLASS	wndClass;											// ������ Ŭ����
	HINSTANCE	hInstance;											// �ڵ� �ν��Ͻ�
	HINSTANCE	hPrevInstance;										// �� �ڵ� �ν��Ͻ�
	LPSTR		lpszCmdParam;										// �Է� �Ű�����
	int			nCmdShow;											// �ʱ� ǥ�� �ɼ�
	LRESULT		(CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM);	// �ݹ� ������ �Լ� ������
	DWORD		dwStyle;											// ������ ��Ÿ��
	DWORD		dwExStyle;											// ������ Ȯ�� ��Ÿ��

	// GL Window�� ���� �߰� �ɹ� ����
	HDC			hDC;												// ����̽� ���ؽ�Ʈ �ڵ� (�� ���� �����찡 �׸��� �۾� ����)
	HGLRC		hRC;												// GL ������ ���ؽ�Ʈ �ڵ� (�� ���� GL�� �׸��� �۾� ����)
	HGLRC		hThreadRC;											// GL ������ ������ ���ؽ�Ʈ �ڵ� (�����带 ����� ��׶��� ���ҽ� �ε��� ���)
	DisplayMode	enDisplayMode;										// ���÷��� ���

public:
	GameWindow(wstring wndName, LRESULT(CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM),
			   HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdParam, int nCmdShow, Size2D size);
	~GameWindow();

	void setWndName(wstring wndName);										// ������ �̸��� ����
	wstring getWndName();													// ������ �̸��� ����
	void setWndProc(LRESULT (CALLBACK *fp)(HWND, UINT, WPARAM, LPARAM));	// ������ �Լ��� ����
	bool createWnd();														// ������ ������ ǥ�� �� �������� ��ȯ
	bool destroyWnd();														// �����츦 �ı� �� �������� ��ȯ
	void setWndActiveState(bool active);									// ������ Ȱ��ȭ ���� ����
	bool getWndActiveState();												// ������ Ȱ��ȭ ���� ��ȯ
	HWND getHWND();															// ������ �ڵ��� ��ȯ
	HDC* getHDCPtr();														// ����̽� ���ؽ�Ʈ �ڵ� �����͸� ��ȯ
	HGLRC* getHGLRCPtr();													// GL ������ ���ؽ�Ʈ �ڵ� �����͸� ��ȯ
	HGLRC* getHGLRC_ThreadPtr();											// GL ������ ���ؽ�Ʈ (������ �ε���)�ڵ� �����͸� ��ȯ
	BOOL moveWnd();															// �����츦 ��ǥ �ɹ��� ��ġ�� �̵�
	BOOL resizeWnd();														// �۾� ������ ũ�⸦ �ػ󵵷� ����
	Point2D getWndPos();													// ������ ��ġ ��ȯ(���� ������)
	Size2D getWndSize();													// ������ ũ�� ��ȯ(���� ������)
	Size2D* getWndSizePtr();												// ������ ũ�� �����͸� ��ȯ
	void setWndPos(Point2D point);											// �������� ��ġ�� ����
	void setWndSize(Size2D size);											// ������ �۾� ������ �ػ󵵸� ����
	bool setDisplayMode(DisplayMode DM_ENUM);								// ���÷��� ��带 ����(DM_FULLSCREEN ��� �� �ػ� ���� �ʿ�)
	DisplayMode getDisplayMode();											// ���÷��� ��带 ��ȯ
	void setMouseVisible(bool isShow);										// ���콺 ���̱�/����� ���� ����
};

#endif