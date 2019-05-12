#include "Engine/Network/NetworkManager.h"
#include "Engine/GameFrame.h"
#include "Game/Object/Player.h"

NetworkManager::NetworkManager() {
	/* ���� ���� ���� */
	int err = -1;	// ���� ��ȣ�� �����ϱ� ���� ����

	/* ���� �ʱ�ȭ */
	memset(&wsaData, 0, sizeof(wsaData));					// WSADATA ����ü �ʱ�ȭ
	sprintf(cServerIP, "%s", "127.0.0.1");					// ���� IP ����
	usPortNumber = 30549;									// ����� ��Ʈ ��ȣ ����
	iClientSocket = -1;										// Ŭ���̾�Ʈ ���� ��ȣ �ʱ�ȭ
	memset(&sockServerAddr, 0, sizeof(sockServerAddr));		// ������ ���� ������ ���� ����ü �ʱ�ȭ


	/* WSADATA ������ ���� 2.0dll ��� �ʱ�ȭ �� ���� ó�� */
	if ((err = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) {
		// ���� ��� �ʱ�ȭ ����
		switch (err) {
		case WSASYSNOTREADY:
			throw "Server::Server() - ����: ��Ʈ��ũ ���� �غ� �����Ͽ����ϴ�.\n";
			break;
		case WSAVERNOTSUPPORTED:
			throw "Server::Server() - ����: ��û�� ������ ���� ������ �������� �ʽ��ϴ�.\n";
			break;
		case WSAEINPROGRESS:
			throw "Server::Server() - ����: ���ŷ ������ �������Դϴ�.\n";
			break;
		case WSAEPROCLIM:
			throw "Server::Server() - ����: ���� ������ ���� ������ �ʰ��Ǿ����ϴ�.\n";
			break;
		case WSAEFAULT:
			throw "Server::Server() - ����: �� ��° �μ��� �����Ͱ� ��ȿ�Ǿ����ϴ�.\n";
			break;
		default:
			throw "Server::Server() - ����: �� �� ���� ������ �߻��Ͽ����ϴ�.\n";
		}
	}

	/* ���� ���������� ���� ����ü �ʱ�ȭ */
	sockServerAddr.sin_family = AF_INET;					// IPv4 ȯ������ ���
	sockServerAddr.sin_addr.s_addr = inet_addr(cServerIP);	// ���ڿ� IP�ּҸ� ��ȯ�ؼ� ����
	sockServerAddr.sin_port = htons(usPortNumber);			// ���� ���Ͽ��� ����� ��Ʈ ��ȣ

	/* ���� �߽��� ���� UDP Ŭ���̾�Ʈ ���� ���� */
	if ((iClientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		// ���� ���� ����
		throw "Client::Client() - ����: ���� ������ �����Ͽ����ϴ�.\n";
	}

	/* ������ UDP ������ Non-Blocking �������� ���� */
	u_long sockMode = 1; // [ 0: Blocking, 1: Non-Blocking ]
	if ((err = ioctlsocket(iClientSocket, FIONBIO, &sockMode)) == SOCKET_ERROR) {
		throw "Client::Client() - ����: ���� �Ӽ� ������ �����Ͽ����ϴ�.\n";
	}
}

NetworkManager::NetworkManager(NetworkManager &obj) {
	/* ���� ������ ���� */
	memcpy_s(&wsaData, sizeof(WSADATA), &obj.wsaData, sizeof(WSADATA));
	memcpy_s(cServerIP, sizeof(cServerIP + 1), obj.cServerIP, sizeof(cServerIP + 1)); // NULL ���� ���� ����
	usPortNumber = obj.usPortNumber;
	iClientSocket = obj.iClientSocket;
	memcpy_s(&sockServerAddr, sizeof(sockaddr_in), &obj.sockServerAddr, sizeof(sockaddr_in));
}

NetworkManager::~NetworkManager() {
	/* ���� ���� ���� */
	int err = -1;	// ���� ��ȣ�� �����ϱ� ���� ����
	int serverAddrSize = sizeof(sockaddr_in);

	/* Ŭ���̾�Ʈ ���� ���� */
	if ((err = closesocket(iClientSocket)) == SOCKET_ERROR) {
		printf_s("Client::~Client() - ���: ���� ��ȣ %d�� �ݱ� �۾��� �����Ͽ����ϴ�. (���� ��ȣ: %d)\n", iClientSocket, WSAGetLastError());
	}
	iClientSocket = -1;

	/* WSADATA ���ҽ� ��ȯ ��û */
	if ((err = WSACleanup()) == SOCKET_ERROR) {
		printf_s("Client::~Client() - ���: WSADATA ���ҽ� ��ȯ�� �����Ͽ����ϴ�. (���� ��ȣ: %d)\n", WSAGetLastError());
	}

	/* ��Ÿ �ɹ� ���� �ʱ�ȭ*/
	memset(&wsaData, 0, sizeof(wsaData));
	memset(cServerIP, '\n', sizeof(cServerIP));
	usPortNumber = 0;
	memset(&sockServerAddr, 0, sizeof(sockServerAddr));
}

void NetworkManager::setServerIPv4(char *newIP) {
	strncpy(cServerIP, newIP, sizeof(cServerIP));
	sockServerAddr.sin_addr.s_addr = inet_addr(cServerIP);
}

char* NetworkManager::getServerIPv4PTr() {
	return cServerIP;
}

void NetworkManager::flushSocketBuffer() {
	sockaddr_in senderAddr;
	int senderAddrSize = sizeof(senderAddr);
	char tempBuf[4096];

	// ���Ͽ��� ū ������ŭ ����
	recvfrom(iClientSocket, (char*)tempBuf, sizeof(tempBuf), 0, (sockaddr*)&senderAddr, &senderAddrSize);
}

bool NetworkManager::sendMsgToServer(GamePacket *pSendPacket) {
	// ������ ���� ��Ŷ ����
	sendto(iClientSocket, (char*)pSendPacket, sizeof(GamePacket), 0, (sockaddr*)&sockServerAddr, sizeof(sockaddr_in));

	return true;
}

int NetworkManager::recvMsgFromServer(GamePacket *pRecvPacket) {
	sockaddr_in serverAddr;
	int serverAddrSize = sizeof(serverAddr);

	// ������ �����͸� ���� �� ������ �������� ũ�⸦ ��ȯ
	return recvfrom(iClientSocket, (char*)pRecvPacket, sizeof(GamePacket), 0, (sockaddr*)&serverAddr, &serverAddrSize);
}

char* NetworkManager::packToByteData(char *pDst, void *pSrc, int size) {
	char *pRt = pDst;

	memcpy_s(pDst, size, pSrc, size);
	pRt += size;

	return pRt;
}

char* NetworkManager::unpackFromByteData(void *pDst, char *pSrc, int size) {
	char *pRt = pSrc;

	memcpy_s(pDst, size, pSrc, size);
	pRt += size;

	return pRt;
}
