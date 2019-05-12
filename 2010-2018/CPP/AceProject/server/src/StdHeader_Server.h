#ifndef __STD_HEADER_SERVER_H__
#define __STD_HEADER_SERVER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : StdHeader_Server.h
*	- Last Update : 2016.09.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 자주 사용되는 헤더들의 집합. 거의 모든 헤더파일에서 호출하므로 잦은 수정 금지 요함.
*
*	- Dependency : None
*
*   - Version(v1): 기초 라이브러리 추가됨.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define __DEBUG__
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// 스탠다드 라이브러리
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>
#include <conio.h>

// 라이브러리 링크
#pragma comment(lib, "ws2_32.lib")	// 윈도우 소켓 라이브러리 링크
#pragma comment(lib, "winmm.lib")	// 윈도우 멀티미티어 라이브러리 링크

// 네임스페이스
using namespace std;

// 프로그래머 정의 자료형
typedef int INT;
typedef float REAL;

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
		memset(byteData, '\n', sizeof(byteData));
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