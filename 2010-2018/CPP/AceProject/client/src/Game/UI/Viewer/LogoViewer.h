#ifndef __LOGO_VIEWER_H__
#define __LOGO_VIEWER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LogoViewer(v1)
*	- Last Update : 2016.08.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �ΰ� ����ϴ� ����.
*
*	- Dependency : [StdHeader.h, RectViewer.h], DrawManager.h, 
*
*   - Version(v1): RectViewer Ŭ���� ���� ����. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Game/UI/Viewer/RectViewer.h"

class LogoViewer : public RectViewer {
private:
	bool bUseTurning;					// �ΰ� ȸ�� ��� ����
	RectViewer *pRingOutter;			// �ΰ� �ܺ� ��
	RectViewer *pRingInner;				// �ΰ� ���� ��
	RectViewer *pFrameViwer;			// �ΰ� ������

	GLuint texture_ring_outter;			// �ܺ� �� �ؽ�ó
	GLuint texture_ring_inner;			// ���� �� �ؽ�ó
	GLuint texture_frame;				// ���� ������ �ؽ�ó

public:
	// ������ �Ҹ���
	LogoViewer(Vector3 pos, SIZE size);
	LogoViewer(LogoViewer &obj);
	~LogoViewer();

	// �޼ҵ�
	bool getLogoTurning();				// �ΰ� ȸ�� ���θ� ��ȯ
	void setLogoTurning(bool useTurn);	// �ΰ� ȸ�� ���θ� ����

	// ���� �Լ�
	virtual bool draw();
	virtual bool destroy();
	virtual bool update(REAL timeElapsed);
};

#endif