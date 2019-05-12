#ifndef __STD_HEADER_SERVER_H__
#define __STD_HEADER_SERVER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : StdHeader_Server.h
*	- Last Update : 2016.09.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ���Ǵ� ������� ����. ���� ��� ������Ͽ��� ȣ���ϹǷ� ���� ���� ���� ����.
*
*	- Dependency : None
*
*   - Version(v1): ���� ���̺귯�� �߰���.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define __DEBUG__
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// ���Ĵٵ� ���̺귯��
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>
#include <conio.h>

// ���̺귯�� ��ũ
#pragma comment(lib, "ws2_32.lib")	// ������ ���� ���̺귯�� ��ũ
#pragma comment(lib, "winmm.lib")	// ������ ��Ƽ��Ƽ�� ���̺귯�� ��ũ

// ���ӽ����̽�
using namespace std;

// ���α׷��� ���� �ڷ���
typedef int INT;
typedef float REAL;

struct GamePacket {
	enum MsgType {
		// �⺻ ���� �� �޽��� Ÿ��
		MSG_DEFAULT,
		// Ŭ���̾�Ʈ ����ȭ �޽��� (���� �Լ��� ��ȣ�� �Ű����� ���� ���۵�)
		MSG_CLIENT_SYNC, 
		// Ŭ���̾�Ʈ ������Ʈ ��� �޽��� (�� �޽����� ���޹ޱ� �������� ������ ���Ÿ� �ݺ�)
		MSG_CLIENT_DOUPDATE,
		// Ŭ���̾�Ʈ ű �޽��� (�� �޽����� ���Ź��� Ŭ���̾�Ʈ�� ������ ������ ������)
		MSG_CLIENT_KICKED,
		// Ŭ���̾�Ʈ ��� �޽��� (�� �޽����� ������ Ŭ���̾�Ʈ�� ������Ʈ �޽��� ���ű��� ���� �۾��� ����)
		MSG_CLIENT_WAIT,
		// ������ ������ �� ���� ��û(���� ���� ��), Ż�� ��û(���� ���� ��, ���� ����) �޽���
		MSG_CONNECT, MSG_DISCONNECT,
		// ������ ���� ������ �� �����ϴ� �޽���
		MSG_CONNECT_SUCCESS,
		// ���� ���ӿ� ������ �� �����ϴ� �޽��� �� ����
		MSG_CONNECT_FAIL_SAMEID, MGS_CONNECT_FAIL_MAXPLAYER, MSG_CONNECT_FAIL_STARTED,
		// �����κ��� ���۹޴� ���� ���� �� �����ð� �޽���
		MSG_SESSION_START_LEFTTIME,
		// �����κ��� ���۹޴� ������ Ŭ���̾�Ʈ �̸�, Ż���� Ŭ���̾�Ʈ �̸� �޽���
		MSG_GIVE_CLIENTNAME, MSG_REMOVE_CLIENTNAME, 
		// �����κ��� ���۹޴� ���� ���� �޽���
		MSG_SESSION_START
	};

	// ��[256Byte] ũ���� ��Ŷ
	DWORD dwFrameTime;		// ��Ŷ�� ���۵� �ñ��� ������ Ÿ��
	MsgType enMsgType;		// ���� �޽��� ���� (����ȭ, ����, ����, �� �޽��� ����...)
	short sPlayerNumber;	// ���� �÷��̾� ��ȣ
	char byteData[246];		// 246����Ʈ ������

	// ������
	inline GamePacket() {
		dwFrameTime = 0;
		enMsgType = GamePacket::MsgType::MSG_DEFAULT;
		sPlayerNumber = 0;
		memset(byteData, '\n', sizeof(byteData));
	}

	// GamePacket ������ �����ε�
	inline void operator=(GamePacket &obj) {
		dwFrameTime = obj.dwFrameTime;
		enMsgType = obj.enMsgType;
		memcpy_s(byteData, sizeof(byteData), obj.byteData, sizeof(byteData));
		sPlayerNumber = obj.sPlayerNumber;
	}
};

#endif