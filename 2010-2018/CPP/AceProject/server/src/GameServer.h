#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

extern float SESSION_LPS;
extern int SESSION_START_DELAY;
extern int SESSION_FINISH_TIME;

#include "StdHeader_Server.h"

class GameServer {
public:
	struct ClientInfo {
		sockaddr_in clientAddr;						// Ŭ���̾�Ʈ ���� �ּ� ����
		char cClientName[9];						// Ŭ���̾�Ʈ �̸�
		short sClientNumber;						// Ŭ���̾�Ʈ ��ȣ (�÷��̾� ��ȣ)
		bool bDataReceived;							// �̹� ������Ʈ�� �����͸� �޾Ҵ��� ����
		DWORD dwLastRecvedTime;						// ���������� �����͸� ������ �ð�

		// ClientInfo ������ �����ε�
		inline void operator=(ClientInfo &obj) {
			memcpy_s(&clientAddr, sizeof(clientAddr), &obj.clientAddr, sizeof(clientAddr));
			memcpy_s(cClientName, sizeof(cClientName), obj.cClientName, sizeof(cClientName));
			sClientNumber = obj.sClientNumber;
			bDataReceived = obj.bDataReceived;
			dwLastRecvedTime = obj.dwLastRecvedTime;
		}
	};
	static const int MAX_CLIENT = 8;				// �ִ� Ŭ���̾�Ʈ ��

private:
	bool bSessionStarted;							// ���� ���۵� ����
	bool bSessionStarting;							// ���� ������ ����
	DWORD dwSessionStartLeftTime;					// ���� ���۱��� ���� �ð�
	DWORD dwSessionFinsihLeftTime;					// ���� ������� ���� �ð�
	DWORD dwServerElapsedTime;						// ���� ���� �� ��� �ð�
	REAL rSessionLPSbyFixedTime;					// ���� �������� ����Ͽ� ������ ������Ʈ �� �ֱ⸦ ����
	WSADATA wsaData;								// ������ ���� �ʱ�ȭ ����ü
	unsigned short usPortNumber;					// ���� ��Ʈ ��ȣ
	int iServerSocket;								// ���� ���� �ڵ�
	sockaddr_in serverAddr;							// ���� ���� ������ ���� ����ü
	vector<struct GamePacket> clientPacketVector;	// Ŭ���̾�Ʈ�κ��� ���� ��Ŷ�� ������ ��Ŷ�� �ӽ� �����ϴ� ����
	vector<ClientInfo*> clientInfoVector;			// Ŭ���̾�Ʈ ������ ���� ����ü ������ ����
	int iLastAssignedClientNumber;					// ���������� �ο��� Ŭ���̾�Ʈ ��ȣ

public:
	// ������ & �Ҹ���
	GameServer();
	GameServer(GameServer &obj);
	~GameServer();

	// ���� & ����
	// ...

	// �޼ҵ�
	void initalizeForLobby();						// �κ� �������� ������ ������ �� �ְ� �ɹ� �ʱ�ȭ
	bool startMainLoop();							// ���� ���� ���� ����
	bool clientConnection(DWORD timeElapsed);		// Ŭ���̾�Ʈ Ŀ�ؼ� ����
	bool checkSessionStart(DWORD timeElapsed);		// ���� ���� �˻�
	bool updateSession(DWORD timeElapsed);			// �� Ŭ���̾�Ʈ���� ���� ������Ʈ �۾� ����

private:
	// ���� �޼ҵ�
	void removeClientInfo(short number);	// �ش� ��ȣ�� Ŭ���̾�Ʈ ������ ���� �Ҵ� ���� �� clientInfoVector���� ����
};

#endif