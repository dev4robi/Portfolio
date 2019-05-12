#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameWindow (v6)
*	- Last Update : 2016.10.19
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 윈도우 운영체제에서 게임을 운용하기 위한 클래스. 운영체제에서 표시
*					 되는 윈도우를 조정할 수 있는 클래스이다.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 게임 윈도우 기초 설계
*   - Version(v2): OpenGL 윈도우로 호환
*   - Version(v3): 전체화면 모드 및 창 모드 작업 (2016.03.23)
*	- Version(v4): PIXELFORMATDESCRIPTOR 구조체에 올바른 자료형을 넣지 않아
*				   특정 환경에서 스텐실 버퍼 사용이 적용되지 않았던 것을 수정. (2016.09.17)
*	- Version(v5): 백그라운드 쓰레드를 통한 리소스 로드를 위해 hThreadRC 멤버 추가 및 초기화 루틴 생성 (2016.10.14)
*	- Version(v6): 마우스 숨기기/보이기 관련 멤버 함수 추가. (2016.10.19)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

enum DisplayMode {
	DM_WINDOWDED = 0, DM_FULLSCREEN
};

class GameWindow : public GameObject {
private:
	// 일반 윈도우 맴버 변수
	wstring		wsWndName;											// 윈도우 이름
	Point2D		wndPos;												// 윈도우 위치
	Size2D		wndSize;											// 윈도우 작업영역 크기
	bool		bActiveState;										// 윈도우 활성화 여부
	HWND		hWnd;												// 윈도우 핸들
	WNDCLASS	wndClass;											// 윈도우 클래스
	HINSTANCE	hInstance;											// 핸들 인스턴스
	HINSTANCE	hPrevInstance;										// 구 핸들 인스턴스
	LPSTR		lpszCmdParam;										// 입력 매개변수
	int			nCmdShow;											// 초기 표시 옵션
	LRESULT		(CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM);	// 콜백 윈프록 함수 포인터
	DWORD		dwStyle;											// 윈도우 스타일
	DWORD		dwExStyle;											// 윈도우 확장 스타일

	// GL Window를 위한 추가 맴버 변수
	HDC			hDC;												// 디바이스 컨텍스트 핸들 (이 곳에 윈도우가 그리기 작업 수행)
	HGLRC		hRC;												// GL 렌더링 컨텍스트 핸들 (이 곳에 GL이 그리기 작업 수행)
	HGLRC		hThreadRC;											// GL 쓰레드 렌더링 컨텍스트 핸들 (쓰레드를 사용한 백그라운드 리소스 로딩에 사용)
	DisplayMode	enDisplayMode;										// 디스플레이 모드

public:
	GameWindow(wstring wndName, LRESULT(CALLBACK *fpWndProc)(HWND, UINT, WPARAM, LPARAM),
			   HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdParam, int nCmdShow, Size2D size);
	~GameWindow();

	void setWndName(wstring wndName);										// 윈도우 이름을 설정
	wstring getWndName();													// 윈도우 이름을 복사
	void setWndProc(LRESULT (CALLBACK *fp)(HWND, UINT, WPARAM, LPARAM));	// 윈프록 함수를 설정
	bool createWnd();														// 윈도우 생성및 표시 후 성공여부 반환
	bool destroyWnd();														// 윈도우를 파괴 후 성공여부 반환
	void setWndActiveState(bool active);									// 윈도우 활성화 여부 설정
	bool getWndActiveState();												// 윈도우 활성화 여부 반환
	HWND getHWND();															// 윈도우 핸들을 반환
	HDC* getHDCPtr();														// 디바이스 콘텍스트 핸들 포인터를 반환
	HGLRC* getHGLRCPtr();													// GL 렌더링 콘텍스트 핸들 포인터를 반환
	HGLRC* getHGLRC_ThreadPtr();											// GL 렌더링 콘텍스트 (쓰레드 로딩용)핸들 포인터를 반환
	BOOL moveWnd();															// 윈도우를 좌표 맴버의 위치로 이동
	BOOL resizeWnd();														// 작업 영역의 크기를 해상도로 변경
	Point2D getWndPos();													// 윈도우 위치 반환(복사 생성자)
	Size2D getWndSize();													// 윈도우 크기 반환(복사 생성자)
	Size2D* getWndSizePtr();												// 윈도우 크기 포인터를 반환
	void setWndPos(Point2D point);											// 윈도우의 위치를 설정
	void setWndSize(Size2D size);											// 윈도우 작업 영역의 해상도를 설정
	bool setDisplayMode(DisplayMode DM_ENUM);								// 디스플레이 모드를 설정(DM_FULLSCREEN 사용 시 해상도 변경 필요)
	DisplayMode getDisplayMode();											// 디스플레이 모드를 반환
	void setMouseVisible(bool isShow);										// 마우스 보이기/숨기기 여부 설정
};

#endif