#ifndef __MULTIPLAY_LOBBY_H__
#define __MULTIPLAY_LOBBY_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : MultiplayLobby(v1)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ��Ƽ�÷��� �κ� ������.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): MultiplayLobby ������ ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;
class LogoViewer;
class Player;

class MultiplayLobby : public GameFrame {
private:
	static const int MAX_PLAYER = 8;

	enum ClientState {
		CLIENT_CONNECT_FAILED, CLIENT_CONNECT_FAILED_SERVEROFF,
		CLIENT_SEND_CONNECT, CLIENT_WAIT_PLAYER,
		CLIENT_START_SESSION
	};

	// Ŭ���̾�Ʈ ����
	REAL		rSessionLPSfromServer;			// �����κ��� ���۹��� ���� �������� LPS
	bool		bExitLobby;						// �κ� ������ ���� Ȯ��
	ClientState enClientState;					// Ŭ���̾�Ʈ�� ���� ������
	int			iServerConnectionTryCnt;		// ���� ���� �õ� Ƚ��

	// ���
	LogoViewer *pViewerLogo;					// ��� �ΰ� ���
	RectViewer *pViewerMsg;						// ���� ���� �޽��� ���
	RectViewer *pViewerPlayerCnt;				// ���ӵ� �÷��̾��� �� ���
	RectViewer *pViewerTimer;					// ���� ���ð� ���
	RectViewer *pViewerPlayerName[MAX_PLAYER];	// ���ӵ� �÷��̾� �̸� ���
	RectViewer *pViewerCancle;					// ���� ���� �ؽ�Ʈ ���
	RectViewer *pViewerFade;					// ��� ���̵� ȿ�� ���
	vector<Player*> playerVector;				// ���ӵ� �÷��̾� ����

public:
	// ������ & �Ҹ���
	MultiplayLobby(Player *pPlayer);
	MultiplayLobby(MultiplayLobby &obj);
	~MultiplayLobby();

	// ���� & ����
	// ...

	// �޼ҵ�
	void sendMsgToServer(REAL timeElapsed);		// �������� ���¿� ���� �޽����� ����
	void recvMsgFromServer(REAL timeElapsed);	// �����κ��� ���� ���� �޽����� ���� ������Ʈ
	void updatePlayerViewer(REAL timeElapsed);	// ���ӵ� �÷��̾� ��� ��� ������Ʈ

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
};

#endif