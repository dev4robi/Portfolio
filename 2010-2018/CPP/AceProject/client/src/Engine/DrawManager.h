#ifndef __DRAW_MANAGER_H__
#define __DRAW_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : DrawManager (v2)
*	- Last Update : 2016.06.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ȭ���� �� ���ۿ� �׷��ְ� �ʿ��� ����ó�� ���� ������ ��,
*					 ������ ȭ���� ����Ʈ ���۷� ������ �ִ� Ŭ����.
*
*	- Dependency : [StdHeader.h], GameManager.h, GameWindow.h, GameFrame.h, GameWidget.h, GameCamera.h
*				   , Game/Skybox.h, GL_Effect.h
*
*	- Version(v1 - 16.05.11) : Ŭ���� ���� ���� �� �׸��� ����.
*	- Version(v2 - 16.06.11) : �ϴ� ���� �׸��⸦ ���� �κ� �߰�.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GameWidget;
class GameCamera;
class GL_Effect;

class DrawManager : public GameObject {
private:
	HDC *pHDC;						// GameWindow�� HDC ������
	HGLRC *pHGLRC;					// GameWindow�� HGLRC ������
	Size2D *pResolution;			// GameWindow�� �۾� ���� �ػ� ������
	GameCamera *pCamera;			// GameFrame�� ī�޶�
	list<GameWidget*> *pDrawList;	// GameFrame�� �׸��� �۾��� �ؾ� �� GameWidget ����Ʈ ������
	list<GL_Effect*> *pEffectList;	// GameFrame�� �׸��� �۾��� �ؾ� �� GL_Effect ����Ʈ ������

public:
	DrawManager(HDC* hdc, HGLRC* hglrc, Size2D* pResolutio);
	~DrawManager();

	void setBufferClearColor(Vector3 color);		// ��� �ʱ�ȭ ���� ������ ����
	Size2D *getResolutionPtr();						// ��ο� �Ŵ����� �ػ� �����͸� ��ȯ
	GameCamera *getCameraFromFrame();				// ������ ������ �÷��̾��� ī�޶� ������ �����´�
	Vector3 get2dPostionFromVector3(Vector3 vec);	// Vector3�κ��� ī�޶� ����� ���� ȭ��� 2���� ��ǥ�� ��ȯ

	bool initGL();		// OpenGL ���� �ʱ�ȭ
	bool draw();		// ����Ʈ�� ��ȸ�ϸ� �׸��� �۾��� ����
	bool swap();		// GL�� ������ ������ HGLRC�� �������� HDC�� �����Ͽ� ȭ�� ���
};

#endif