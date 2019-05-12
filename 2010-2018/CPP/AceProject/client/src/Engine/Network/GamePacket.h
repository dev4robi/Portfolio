#ifndef __GAME_PACKET_H__
#define __GAME_PACKET_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : GamePacket.h
*	- Last Update : 2016.09.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ��Ŷ ����ü ����. �������� GamePacket.h �� �׻� ����ȭ �� ��.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): ���� ��Ŷ ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

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
		memset(byteData, '\0', sizeof(byteData));
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