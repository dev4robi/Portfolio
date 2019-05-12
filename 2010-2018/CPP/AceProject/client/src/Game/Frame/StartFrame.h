#ifndef __START_FRAME_H__
#define __START_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : StartFrame(v1)
*	- Last Update : 2016.10.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ������ �� ���ҽ��� �ε��ϱ� ���� �����ϴ� ������.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): StartFrame ���� ����. (2016.10.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;
class LogoViewer;

class StartFrame : public GameFrame {
private:
	RectViewer *pAceProjectLogo;		// ���� ������Ʈ�� �ΰ�
	RectViewer *pUnivMsgViewer;			// ������ǰ �޽��� ���
	RectViewer *pUnivLogoViewer;		// ���б� �ΰ� ���
	RectViewer *pTeamMsgViewer;			// �� �޽��� ���
	RectViewer *pTeamLogoViewer;		// �� �ΰ� ���
	LogoViewer *pLoadingLogo;			// �ε� �ΰ� ���
	RectViewer *pLoadingMsgViewer;		// �ε� % �޽��� ���
	RectViewer *pFadeViewer;			// ���̵�ƿ� ���

	bool bReqChangeFrame;				// ������ ���� ��û ���θ� ����
	bool bFastSpeedFade;				// ���� �ӵ��� ���̵�
	int iLogoState;						// �ΰ� ��� ����
	REAL rUnivLogoTime;					// ���� ��ǰ �ΰ� ��� �ð�
	REAL rTeamLogoTime;					// �� �ΰ� ��� �ð�
	REAL rMainLogoDelayTime;			// ���� �ΰ� ��±��� �߰����� ���� �ð�
	REAL rLobbyFrameDelayTime;			// �ε� �Ϸ� �� �κ� ���������� ��ȯ�ϱ� ���� ���� �ð�
	REAL rFadeoutTime;					// ���̵� �ƿ� �ð�
	REAL rFadeinTime;					// ���̵� �� �ð�
	int iLoadingPercent;				// �ε� �� ���ҽ� ����� �ۼ�Ʈ
	HANDLE hLoadingThread;				// �ε� ������ �ڵ�

public:
	// ������ & �Ҹ���
	StartFrame(Player *pPlayer = NULL);
	StartFrame(StartFrame &obj);
	~StartFrame();

	// ���� & ����
	// ...

	// �޼ҵ�
	void loadResource();										// �ʿ��� ���ҽ��� �ε�
	static DWORD WINAPI thread_loadResFromThread(void *p);		// ������ ���� �Լ�
	void updateState(REAL timeElapsedMulti);					// �ð��� ���� ���� ������Ʈ

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);	// Ű���� ������Ʈ
	virtual bool updateFrame(REAL timeElapsed); // ������ ������Ʈ
};



#endif