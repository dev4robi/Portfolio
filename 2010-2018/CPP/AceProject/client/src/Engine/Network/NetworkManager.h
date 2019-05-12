#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : NetworkManager.h
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ������ UDP ����� �ϴ� ��Ʈ��ũ �Ŵ��� Ŭ����.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): Ŭ���̾�Ʈ ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/Network/GamePacket.h"

class NetworkManager : public GameObject {
private:
	WSADATA wsaData;				// ������ ���� ��� ����ü
	char cServerIP[16];				// ���� IP
	unsigned short usPortNumber;	// Ŭ���̾�Ʈ���� ����� ��Ʈ ��ȣ
	int iClientSocket;				// Ŭ���̾�Ʈ ���� ��ȣ
	sockaddr_in sockServerAddr;		// ������ ���� ����

public:
	// ������ & �Ҹ���
	NetworkManager();
	NetworkManager(NetworkManager &obj);
	~NetworkManager();

	// ���� & ����
	void setServerIPv4(char *newIP);					// ���� IP�ּҸ� ����
	char* getServerIPv4PTr();							// ���� IP�ּ� �����͸� ��ȯ

	// �޼ҵ�
	void flushSocketBuffer();							// ���� ������ ������ ��� ���
	bool sendMsgToServer(GamePacket *pSendPacket);		// ������ �޽��� ��Ŷ�� ����
	int recvMsgFromServer(GamePacket *pRecvPacket);		// �����κ��� ���Ź��� �޽��� ��Ŷ�� ���� �� �� ũ�� ��ȯ

	// ����ƽ �޼ҵ�
	static char* packToByteData(char *pDst, void *pSrc, int size);		// �����͸� ���� ��Ŷ�� ����Ʈ �����ͷ� ��ŷ�ϰ� �� ���� ������ �����͸� ��ȯ�ϴ� �Լ�
	static char* unpackFromByteData(void *pDst, char *pSrc, int size);	// ���� ��Ŷ�� ����Ʈ �����͸� �����ͷ� �����ϰ� �� ���� ������ �����͸� ��ȯ�ϴ� �Լ�
};

#endif