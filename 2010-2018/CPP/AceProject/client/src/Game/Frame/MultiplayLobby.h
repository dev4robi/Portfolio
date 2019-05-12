#ifndef __MULTIPLAY_LOBBY_H__
#define __MULTIPLAY_LOBBY_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : MultiplayLobby(v1)
*	- Last Update : 2016.09.12
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 멀티플레이 로비 프레임.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): MultiplayLobby 프레임 기초 설계.
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

	// 클라이언트 상태
	REAL		rSessionLPSfromServer;			// 서버로부터 전송받은 세션 프레임의 LPS
	bool		bExitLobby;						// 로비를 나가는 여부 확인
	ClientState enClientState;					// 클라이언트의 상태 열거형
	int			iServerConnectionTryCnt;		// 서버 접속 시도 횟수

	// 뷰어
	LogoViewer *pViewerLogo;					// 배경 로고 뷰어
	RectViewer *pViewerMsg;						// 접속 상태 메시지 뷰어
	RectViewer *pViewerPlayerCnt;				// 접속된 플레이어의 수 뷰어
	RectViewer *pViewerTimer;					// 남은 대기시간 뷰어
	RectViewer *pViewerPlayerName[MAX_PLAYER];	// 접속된 플레이어 이름 뷰어
	RectViewer *pViewerCancle;					// 연결 종료 텍스트 뷰어
	RectViewer *pViewerFade;					// 배경 페이드 효과 뷰어
	vector<Player*> playerVector;				// 접속된 플레이어 정보

public:
	// 생성자 & 소멸자
	MultiplayLobby(Player *pPlayer);
	MultiplayLobby(MultiplayLobby &obj);
	~MultiplayLobby();

	// 게터 & 세터
	// ...

	// 메소드
	void sendMsgToServer(REAL timeElapsed);		// 서버에게 상태에 따른 메시지를 전송
	void recvMsgFromServer(REAL timeElapsed);	// 서버로부터 응답 받은 메시지로 상태 업데이트
	void updatePlayerViewer(REAL timeElapsed);	// 접속된 플레이어 명단 뷰어 업데이트

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
};

#endif