#ifndef __DRAW_MANAGER_H__
#define __DRAW_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : DrawManager (v2)
*	- Last Update : 2016.06.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 화면을 백 버퍼에 그려주고 필요한 영상처리 등을 수행한 뒤,
*					 윈도우 화면의 프론트 버퍼로 전송해 주는 클래스.
*
*	- Dependency : [StdHeader.h], GameManager.h, GameWindow.h, GameFrame.h, GameWidget.h, GameCamera.h
*				   , Game/Skybox.h, GL_Effect.h
*
*	- Version(v1 - 16.05.11) : 클래스 기초 정의 및 그리기 정의.
*	- Version(v2 - 16.06.11) : 하늘 상자 그리기를 위한 부분 추가.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GameWidget;
class GameCamera;
class GL_Effect;

class DrawManager : public GameObject {
private:
	HDC *pHDC;						// GameWindow의 HDC 포인터
	HGLRC *pHGLRC;					// GameWindow의 HGLRC 포인터
	Size2D *pResolution;			// GameWindow의 작업 영역 해상도 포인터
	GameCamera *pCamera;			// GameFrame의 카메라
	list<GameWidget*> *pDrawList;	// GameFrame의 그리기 작업을 해야 할 GameWidget 리스트 포인터
	list<GL_Effect*> *pEffectList;	// GameFrame의 그리기 작업을 해야 할 GL_Effect 리스트 포인터

public:
	DrawManager(HDC* hdc, HGLRC* hglrc, Size2D* pResolutio);
	~DrawManager();

	void setBufferClearColor(Vector3 color);		// 배경 초기화 버퍼 색상을 설정
	Size2D *getResolutionPtr();						// 드로우 매니저의 해상도 포인터를 반환
	GameCamera *getCameraFromFrame();				// 프레임 소유주 플레이어의 카메라 정보를 가져온다
	Vector3 get2dPostionFromVector3(Vector3 vec);	// Vector3로부터 카메라가 적용된 후의 화면상 2차원 좌표를 반환

	bool initGL();		// OpenGL 설정 초기화
	bool draw();		// 리스트를 순회하며 그리기 작업을 수행
	bool swap();		// GL의 렌더링 버퍼인 HGLRC와 윈도우의 HDC를 스왑하여 화면 출력
};

#endif