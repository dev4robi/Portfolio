#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : NetworkManager.h
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 서버와 UDP 통신을 하는 네트워크 매니저 클래스.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 클라이언트 기초 설계.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/Network/GamePacket.h"

class NetworkManager : public GameObject {
private:
	WSADATA wsaData;				// 윈도우 소켓 사용 구조체
	char cServerIP[16];				// 서버 IP
	unsigned short usPortNumber;	// 클라이언트에서 사용할 포트 번호
	int iClientSocket;				// 클라이언트 소켓 번호
	sockaddr_in sockServerAddr;		// 서버측 소켓 정보

public:
	// 생성자 & 소멸자
	NetworkManager();
	NetworkManager(NetworkManager &obj);
	~NetworkManager();

	// 게터 & 세터
	void setServerIPv4(char *newIP);					// 서버 IP주소를 설정
	char* getServerIPv4PTr();							// 서버 IP주소 포인터를 반환

	// 메소드
	void flushSocketBuffer();							// 소켓 버퍼의 내용을 모두 비움
	bool sendMsgToServer(GamePacket *pSendPacket);		// 서버로 메시지 패킷을 전송
	int recvMsgFromServer(GamePacket *pRecvPacket);		// 서버로부터 수신받은 메시지 패킷을 수신 후 그 크기 반환

	// 스테틱 메소드
	static char* packToByteData(char *pDst, void *pSrc, int size);		// 데이터를 게임 패킷의 바이트 데이터로 패킹하고 그 다음 공간의 포인터를 반환하는 함수
	static char* unpackFromByteData(void *pDst, char *pSrc, int size);	// 게임 패킷의 바이트 데이터를 데이터로 언팩하고 그 다음 공간의 포인터를 반환하는 함수
};

#endif