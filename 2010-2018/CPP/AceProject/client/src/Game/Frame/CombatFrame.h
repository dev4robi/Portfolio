#ifndef __TEST_FRAME_H__
#define __TEST_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CombatFrame(v2)
*	- Last Update : 2016.09.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �����Ⳣ�� ������ ���õ� ������ Ŭ����.
*
*	- Dependency : [StdHeader.h], Player.h, Terrain.h, FlightObject.h
*
*   - Version(v1): �÷��̾� Ŭ���� ���� ����. (2016.07.11)
*	- Version(v2): ��Ƽ�÷��̾� ��� ����. (2016.09.16)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class Player;
class Terrain;
class FlightObject;
class GamePacket;
class RectViewer;

class CombatFrame : public GameFrame {
private:
	// ��Ƽ�÷��� ���� �ɹ� ����
	DWORD dwLastRecvTime;				// ���������� �޽����� ���Ź��� �ð�
	bool bMultiplayMode;				// ��Ƽ�÷��� ��� ����
	bool bServerDisconnected;			// ���� ���� ������ ����
	bool bRecvSendMsgFromServer;		// �������� �޽��� ���ۿϷ� �޽����� ���� ��� (��Ƽ�÷��� ����)
	bool bRecvUpdateMsgFromServer;		// �����κ��� ������Ʈ �޽����� ���� ���� (��Ƽ�÷��� ����)
	vector<int> playerControlVector;	// �÷��̾�κ��� ����� ������ ��� �� ��Ʈ��ũ ����ȭ�� �ؾ� �ϴ� ���ID�� ���� ����

	// ������ �ɹ� ����
	bool sessionExpired;				// ���� ���� ����
	REAL sessionLeftTime;				// ���� ���� �ð�
	list<Player*> playerList;			// ��� �÷��̾� �����͸� �����ϴ� ����Ʈ
	list<FlightObject*> flightObjList;	// ��� ����ü �����͸� �����ϴ� ����Ʈ
	bool bExitUIIsShown;				// ���� UI ��� ǥ�� ����
	RectViewer *pExitUIViewer;			// ���� UI ��� ������
	bool bHelpUIIsShown;				// ���� UI ��� ǥ�� ����
	RectViewer *pHelpUIViewer;			// ���� UI ��� ������
	RectViewer *pFadeoutViewer;			// ���̵� �ƿ� ���

public:
	// ������ & �Ҹ���
	CombatFrame(Player *pLobbyPlayer);													// �̱��÷��� ������
	CombatFrame(Player *pLobbyPlayer, vector<Player*> *pPlayerVector, REAL newTgtLPS);	// ��Ƽ�÷��� ������
	~CombatFrame();

	// ���� & ����
	bool isMultiplayMode();						// ��Ƽ�÷��� ��带 �������̸� true ��ȯ
	bool isServerDisconnected();				// ������ ������ ������ �����̸� true ��ȯ
	REAL getSessionLeftTime();					// ���� ���� �ð� ��ȯ
	vector<int>* getPlayerControlVectorPtr();	// �÷��̾�κ��� ����� ������ ��� �� ��Ʈ��ũ ����ȭ�� �ؾ� �ϴ� ���ID�� ���� ���� ������ ��ȯ

	list<Player*>* getPlayerListPtr();			// �÷��̾� ����Ʈ �����͸� ��ȯ
	list<FlightObject*>* getFlightObjListPtr();	// ����ü ����Ʈ �����͸� ��ȯ

	// �޼ҵ�
	void updateNetwork(REAL timeElapsed);							// ��Ʈ��ũ ������Ʈ
	void translatePacket(GamePacket *pPacket, REAL timeElapsed);	// ��Ŷ�� ������ �м��Ͽ� �׿� �´� Ŭ���̾�Ʈ ������Ʈ ����

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);	// Ű���� ������Ʈ
	virtual bool updateFrame(REAL timeElapsed); // ������ ������Ʈ

private:
	// ���� �Լ�
	void updateFlightObjList();					// ���� ����Ʈ�κ��� ����ü ����Ʈ�� ������ ����
};

#endif