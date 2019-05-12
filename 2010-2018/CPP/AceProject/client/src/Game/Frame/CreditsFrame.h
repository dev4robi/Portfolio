#ifndef __CREDITS_FRAME_H__
#define __CREDITS_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CreditsFrame(v1)
*	- Last Update : 2016.09.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 개발자 및 도움을 준 고마운 분들, 사용된 툴에 대한 정보를 담은 프레임.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h, DataParser.h, RectViewer.h, LobbyFrame.h
*
*   - Version(v1): ResultFrame 기초 설계. (2016.09.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;

class CreditsFrame : public GameFrame {
private:
	// 컨트롤 멤버
	bool bReqExit;			// 크레딧 프레임 탈출 요청 여부 
	bool bPuase;			// 일시 정지 여부
	REAL rExitDelayTime;	// 탈출 요청 후 종료 까지 지연 시간
	REAL rSpeedMulti;		// 재생 배속

	// 뷰어 멤버
	list<RectViewer*> listScrollViewers;	// 위로 스크롤 업 될 뷰어들이 저장되는 리스트
	RectViewer* pHelpViewer;				// 도움말 뷰어 포인터
	RectViewer* pSpeedMultiViewer;			// 현재 재생 속도를 표현해주는 뷰어 포인터
	RectViewer* pCinematicFrameViewer;		// 테투리를 감싸고 있는 씨네마틱 뷰어 포인터
	RectViewer* pFadeoutViewer;				// 페이드아웃 뷰어 포인터

public:
	// 생성자 & 소멸자
	CreditsFrame(Player *pPlayer);
	CreditsFrame(CreditsFrame &obj);
	~CreditsFrame();

	// 게터 & 세터
	// ...

	// 메소드
	void loadViewerFromDatFile(char *fileName);	// Credit.dat 파일의 정보를 파싱하여 필요한 뷰어를 동적 할당
	void changeSpeedMulti(bool speedUp);		// 재생 배속 변경
	void updateLogic(REAL timeElapsed);			// 로직 업데이트 (소리 배속, 뷰어 동기화, 스크롤 업 등)

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);	// 키보드 업데이트
	virtual bool updateFrame(REAL timeElapsed); // 프레임 업데이트
};

#endif