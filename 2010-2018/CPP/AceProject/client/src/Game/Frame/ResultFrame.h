#ifndef __RESULT_FRAME_H__
#define __RESULT_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ResultFrame(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 전투 후 결과를 출력시켜주는 프레임.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): ResultFrame 기초 설계. (2016.08.23)
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

	ResultScene enScene;			// 프레임 씬 경과 카운터
	REAL rFrameElapseTime;			// 프레임 경과 시간
	REAL rScoreShowTerm;			// 설정된 스코어 표시 간격
	int iPlayerCount;				// 플레이어 카운터
	vector<Player*> playerVector;	// 플레이어 벡터
	bool bShowScore;				// 점수 표시 시작 여부
	bool bQuitable;					// 프레임 종료 허가됨 여부
	Vector3 vecScorePos;			// 점수 표시 시작 위치

	RectViewer *pAnykeyViewer;		// 아무 키나 누르세요 뷰어
	RectViewer *pAceViewer;			// 에이스 마크 뷰어
	RectViewer *pFadeoutViewer;		// 페이드 아웃 뷰어

	GLuint texture_missionResult;	// 미션 결과 텍스처
	GLuint texture_acePlayer;		// 에이스 플레이어 텍스처

public:
	// 생성자, 소멸자
	ResultFrame(vector<Player*> playerList, Player *pOwnPlayer);
	ResultFrame(ResultFrame &obj);
	~ResultFrame();

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
};

#endif