#ifndef __RESULT_FRAME_H__
#define __RESULT_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ResultFrame(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �� ����� ��½����ִ� ������.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): ResultFrame ���� ����. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;

class ResultFrame : public GameFrame {
private:
	enum ResultScene {
		SCENE_BEGIN, SCENE_PRINT_RESULT, SCENE_PRINT_SCROE, SCENE_PRINT_ANYKEY, SCENE_EXITING
	};

	ResultScene enScene;			// ������ �� ��� ī����
	REAL rFrameElapseTime;			// ������ ��� �ð�
	REAL rScoreShowTerm;			// ������ ���ھ� ǥ�� ����
	int iPlayerCount;				// �÷��̾� ī����
	vector<Player*> playerVector;	// �÷��̾� ����
	bool bShowScore;				// ���� ǥ�� ���� ����
	bool bQuitable;					// ������ ���� �㰡�� ����
	Vector3 vecScorePos;			// ���� ǥ�� ���� ��ġ

	RectViewer *pAnykeyViewer;		// �ƹ� Ű�� �������� ���
	RectViewer *pAceViewer;			// ���̽� ��ũ ���
	RectViewer *pFadeoutViewer;		// ���̵� �ƿ� ���

	GLuint texture_missionResult;	// �̼� ��� �ؽ�ó
	GLuint texture_acePlayer;		// ���̽� �÷��̾� �ؽ�ó

public:
	// ������, �Ҹ���
	ResultFrame(vector<Player*> playerList, Player *pOwnPlayer);
	ResultFrame(ResultFrame &obj);
	~ResultFrame();

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
};

#endif