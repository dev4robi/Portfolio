#ifndef __GAME_PACKET_H__
#define __GAME_PACKET_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : GamePacket.h
*	- Last Update : 2016.09.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 패킷 구조체 정보. 서버측의 GamePacket.h 와 항상 동기화 할 것.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 게임 패킷 기초 설계.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

struct GamePacket {
	enum MsgType {
		// 기본 생성 시 메시지 타입
		MSG_DEFAULT,
		// 클라이언트 동기화 메시지 (각종 함수들 번호와 매개변수 값이 전송됨)
		MSG_CLIENT_SYNC,
		// 클라이언트 업데이트 허락 메시지 (이 메시지를 전달받기 전까지는 데이터 수신만 반복)
		MSG_CLIENT_DOUPDATE,
		// 클라이언트 킥 메시지 (이 메시지를 수신받은 클라이언트는 서버와 접속이 단절됨)
		MSG_CLIENT_KICKED,
		// 클라이언트 대기 메시지 (이 메시지를 수신한 클라이언트는 업데이트 메시지 수신까지 수신 작업만 수행)
		MSG_CLIENT_WAIT,
		// 서버에 접속할 때 접속 요청(세션 시작 전), 탈퇴 요청(세션 시작 전, 세션 도중) 메시지
		MSG_CONNECT, MSG_DISCONNECT,
		// 서버에 접속 성공할 때 수신하는 메시지
		MSG_CONNECT_SUCCESS,
		// 서버 접속에 실패할 때 수신하는 메시지 세 종류
		MSG_CONNECT_FAIL_SAMEID, MGS_CONNECT_FAIL_MAXPLAYER, MSG_CONNECT_FAIL_STARTED,
		// 서버로부터 전송받는 세션 시작 전 남은시간 메시지
		MSG_SESSION_START_LEFTTIME,
		// 서버로부터 전송받는 접속한 클라이언트 이름, 탈퇴한 클라이언트 이름 메시지
		MSG_GIVE_CLIENTNAME, MSG_REMOVE_CLIENTNAME,
		// 서버로부터 전송받는 세션 시작 메시지
		MSG_SESSION_START
	};

	// 총[256Byte] 크기의 패킷
	DWORD dwFrameTime;		// 패킷이 전송된 시기의 프레임 타임
	MsgType enMsgType;		// 보낸 메시지 종류 (동기화, 접속, 종료, 등 메시지 구분...)
	short sPlayerNumber;	// 보낸 플레이어 번호
	char byteData[246];		// 246바이트 데이터

	// 생성자
	inline GamePacket() {
		dwFrameTime = 0;
		enMsgType = GamePacket::MsgType::MSG_DEFAULT;
		sPlayerNumber = 0;
		memset(byteData, '\0', sizeof(byteData));
	}

	// GamePacket 연산자 오버로딩
	inline void operator=(GamePacket &obj) {
		dwFrameTime = obj.dwFrameTime;
		enMsgType = obj.enMsgType;
		memcpy_s(byteData, sizeof(byteData), obj.byteData, sizeof(byteData));
		sPlayerNumber = obj.sPlayerNumber;
	}
};

#endif