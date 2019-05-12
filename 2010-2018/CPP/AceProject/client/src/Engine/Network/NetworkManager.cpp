#include "Engine/Network/NetworkManager.h"
#include "Engine/GameFrame.h"
#include "Game/Object/Player.h"

NetworkManager::NetworkManager() {
	/* 지역 변수 선언 */
	int err = -1;	// 에러 번호를 저장하기 위한 변수

	/* 변수 초기화 */
	memset(&wsaData, 0, sizeof(wsaData));					// WSADATA 구조체 초기화
	sprintf(cServerIP, "%s", "127.0.0.1");					// 서버 IP 설정
	usPortNumber = 30549;									// 사용할 포트 번호 설정
	iClientSocket = -1;										// 클라이언트 소켓 번호 초기화
	memset(&sockServerAddr, 0, sizeof(sockServerAddr));		// 서버측 소켓 정보를 담을 구조체 초기화


	/* WSADATA 윈도우 소켓 2.0dll 사용 초기화 및 오류 처리 */
	if ((err = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) {
		// 소켓 사용 초기화 실패
		switch (err) {
		case WSASYSNOTREADY:
			throw "Server::Server() - 실패: 네트워크 접속 준비를 실패하였습니다.\n";
			break;
		case WSAVERNOTSUPPORTED:
			throw "Server::Server() - 실패: 요청된 윈도우 소켓 버전이 지원되지 않습니다.\n";
			break;
		case WSAEINPROGRESS:
			throw "Server::Server() - 실패: 블로킹 조작이 수행중입니다.\n";
			break;
		case WSAEPROCLIM:
			throw "Server::Server() - 실패: 지원 가능한 소켓 개수가 초과되었습니다.\n";
			break;
		case WSAEFAULT:
			throw "Server::Server() - 실패: 두 번째 인수의 포인터가 무효되었습니다.\n";
			break;
		default:
			throw "Server::Server() - 실패: 알 수 없는 오류가 발생하였습니다.\n";
		}
	}

	/* 서버 소켓정보를 담은 구조체 초기화 */
	sockServerAddr.sin_family = AF_INET;					// IPv4 환경으로 통신
	sockServerAddr.sin_addr.s_addr = inet_addr(cServerIP);	// 문자열 IP주소를 변환해서 넣음
	sockServerAddr.sin_port = htons(usPortNumber);			// 서버 소켓에서 사용할 포트 번호

	/* 수신 발신을 위한 UDP 클라이언트 소켓 생성 */
	if ((iClientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		// 소켓 생성 실패
		throw "Client::Client() - 실패: 소켓 생성을 실패하였습니다.\n";
	}

	/* 생성된 UDP 소켓을 Non-Blocking 소켓으로 설정 */
	u_long sockMode = 1; // [ 0: Blocking, 1: Non-Blocking ]
	if ((err = ioctlsocket(iClientSocket, FIONBIO, &sockMode)) == SOCKET_ERROR) {
		throw "Client::Client() - 실패: 소켓 속성 설정에 실패하였습니다.\n";
	}
}

NetworkManager::NetworkManager(NetworkManager &obj) {
	/* 복사 생성자 수행 */
	memcpy_s(&wsaData, sizeof(WSADATA), &obj.wsaData, sizeof(WSADATA));
	memcpy_s(cServerIP, sizeof(cServerIP + 1), obj.cServerIP, sizeof(cServerIP + 1)); // NULL 문자 또한 복사
	usPortNumber = obj.usPortNumber;
	iClientSocket = obj.iClientSocket;
	memcpy_s(&sockServerAddr, sizeof(sockaddr_in), &obj.sockServerAddr, sizeof(sockaddr_in));
}

NetworkManager::~NetworkManager() {
	/* 지역 변수 선언 */
	int err = -1;	// 에러 번호를 저장하기 위한 변수
	int serverAddrSize = sizeof(sockaddr_in);

	/* 클라이언트 소켓 닫음 */
	if ((err = closesocket(iClientSocket)) == SOCKET_ERROR) {
		printf_s("Client::~Client() - 경고: 소켓 번호 %d의 닫기 작업이 실패하였습니다. (오류 번호: %d)\n", iClientSocket, WSAGetLastError());
	}
	iClientSocket = -1;

	/* WSADATA 리소스 반환 요청 */
	if ((err = WSACleanup()) == SOCKET_ERROR) {
		printf_s("Client::~Client() - 경고: WSADATA 리소스 반환에 실패하였습니다. (오류 번호: %d)\n", WSAGetLastError());
	}

	/* 기타 맴버 변수 초기화*/
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

	// 소켓에서 큰 공간만큼 수신
	recvfrom(iClientSocket, (char*)tempBuf, sizeof(tempBuf), 0, (sockaddr*)&senderAddr, &senderAddrSize);
}

bool NetworkManager::sendMsgToServer(GamePacket *pSendPacket) {
	// 서버로 게임 패킷 전송
	sendto(iClientSocket, (char*)pSendPacket, sizeof(GamePacket), 0, (sockaddr*)&sockServerAddr, sizeof(sockaddr_in));

	return true;
}

int NetworkManager::recvMsgFromServer(GamePacket *pRecvPacket) {
	sockaddr_in serverAddr;
	int serverAddrSize = sizeof(serverAddr);

	// 소켓의 데이터를 수신 후 수신한 데이터의 크기를 반환
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
