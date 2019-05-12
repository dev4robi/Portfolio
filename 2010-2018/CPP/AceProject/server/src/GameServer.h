#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

extern float SESSION_LPS;
extern int SESSION_START_DELAY;
extern int SESSION_FINISH_TIME;

#include "StdHeader_Server.h"

class GameServer {
public:
	struct ClientInfo {
		sockaddr_in clientAddr;						// 클라이언트 소켓 주소 정보
		char cClientName[9];						// 클라이언트 이름
		short sClientNumber;						// 클라이언트 번호 (플레이어 번호)
		bool bDataReceived;							// 이번 업데이트에 데이터를 받았는지 여부
		DWORD dwLastRecvedTime;						// 마지막으로 데이터를 수신한 시간

		// ClientInfo 연산자 오버로딩
		inline void operator=(ClientInfo &obj) {
			memcpy_s(&clientAddr, sizeof(clientAddr), &obj.clientAddr, sizeof(clientAddr));
			memcpy_s(cClientName, sizeof(cClientName), obj.cClientName, sizeof(cClientName));
			sClientNumber = obj.sClientNumber;
			bDataReceived = obj.bDataReceived;
			dwLastRecvedTime = obj.dwLastRecvedTime;
		}
	};
	static const int MAX_CLIENT = 8;				// 최대 클라이언트 수

private:
	bool bSessionStarted;							// 세션 시작됨 여부
	bool bSessionStarting;							// 세션 시작중 여부
	DWORD dwSessionStartLeftTime;					// 세션 시작까지 남은 시간
	DWORD dwSessionFinsihLeftTime;					// 세션 종료까지 남은 시간
	DWORD dwServerElapsedTime;						// 서버 구동 후 경과 시간
	REAL rSessionLPSbyFixedTime;					// 고정 프레임을 사용하여 세션이 업데이트 될 주기를 저장
	WSADATA wsaData;								// 윈도우 소켓 초기화 구조체
	unsigned short usPortNumber;					// 서버 포트 번호
	int iServerSocket;								// 서버 소켓 핸들
	sockaddr_in serverAddr;							// 서버 소켓 정보를 담은 구조체
	vector<struct GamePacket> clientPacketVector;	// 클라이언트로부터 받은 패킷중 전달할 패킷을 임시 보관하는 벡터
	vector<ClientInfo*> clientInfoVector;			// 클라이언트 정보를 담은 구조체 포인터 벡터
	int iLastAssignedClientNumber;					// 마지막으로 부여된 클라이언트 번호

public:
	// 생성자 & 소멸자
	GameServer();
	GameServer(GameServer &obj);
	~GameServer();

	// 게터 & 세터
	// ...

	// 메소드
	void initalizeForLobby();						// 로비 프레임의 역할을 수행할 수 있게 맴버 초기화
	bool startMainLoop();							// 서버 메인 로직 시작
	bool clientConnection(DWORD timeElapsed);		// 클라이언트 커넥션 수행
	bool checkSessionStart(DWORD timeElapsed);		// 세션 시작 검사
	bool updateSession(DWORD timeElapsed);			// 각 클라이언트들의 세션 업데이트 작업 수행

private:
	// 전용 메소드
	void removeClientInfo(short number);	// 해당 번호의 클라이언트 정보의 동적 할당 해제 및 clientInfoVector에서 제거
};

#endif