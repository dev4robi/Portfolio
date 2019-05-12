#ifndef __CREDITS_FRAME_H__
#define __CREDITS_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CreditsFrame(v1)
*	- Last Update : 2016.09.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ������ �� ������ �� ���� �е�, ���� ���� ���� ������ ���� ������.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h, DataParser.h, RectViewer.h, LobbyFrame.h
*
*   - Version(v1): ResultFrame ���� ����. (2016.09.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;

class CreditsFrame : public GameFrame {
private:
	// ��Ʈ�� ���
	bool bReqExit;			// ũ���� ������ Ż�� ��û ���� 
	bool bPuase;			// �Ͻ� ���� ����
	REAL rExitDelayTime;	// Ż�� ��û �� ���� ���� ���� �ð�
	REAL rSpeedMulti;		// ��� ���

	// ��� ���
	list<RectViewer*> listScrollViewers;	// ���� ��ũ�� �� �� ������ ����Ǵ� ����Ʈ
	RectViewer* pHelpViewer;				// ���� ��� ������
	RectViewer* pSpeedMultiViewer;			// ���� ��� �ӵ��� ǥ�����ִ� ��� ������
	RectViewer* pCinematicFrameViewer;		// �������� ���ΰ� �ִ� ���׸�ƽ ��� ������
	RectViewer* pFadeoutViewer;				// ���̵�ƿ� ��� ������

public:
	// ������ & �Ҹ���
	CreditsFrame(Player *pPlayer);
	CreditsFrame(CreditsFrame &obj);
	~CreditsFrame();

	// ���� & ����
	// ...

	// �޼ҵ�
	void loadViewerFromDatFile(char *fileName);	// Credit.dat ������ ������ �Ľ��Ͽ� �ʿ��� �� ���� �Ҵ�
	void changeSpeedMulti(bool speedUp);		// ��� ��� ����
	void updateLogic(REAL timeElapsed);			// ���� ������Ʈ (�Ҹ� ���, ��� ����ȭ, ��ũ�� �� ��)

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);	// Ű���� ������Ʈ
	virtual bool updateFrame(REAL timeElapsed); // ������ ������Ʈ
};

#endif