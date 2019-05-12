#ifndef __START_FRAME_H__
#define __START_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : StartFrame(v1)
*	- Last Update : 2016.10.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 시작할 때 리소스를 로드하기 위해 실행하는 프레임.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): StartFrame 기초 설계. (2016.10.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;
class LogoViewer;

class StartFrame : public GameFrame {
private:
	RectViewer *pAceProjectLogo;		// 메인 프로젝트명 로고
	RectViewer *pUnivMsgViewer;			// 졸업작품 메시지 뷰어
	RectViewer *pUnivLogoViewer;		// 대학교 로고 뷰어
	RectViewer *pTeamMsgViewer;			// 팀 메시지 뷰어
	RectViewer *pTeamLogoViewer;		// 팀 로고 뷰어
	LogoViewer *pLoadingLogo;			// 로딩 로고 뷰어
	RectViewer *pLoadingMsgViewer;		// 로딩 % 메시지 뷰어
	RectViewer *pFadeViewer;			// 페이드아웃 뷰어

	bool bReqChangeFrame;				// 프레임 변경 요청 여부를 저장
	bool bFastSpeedFade;				// 빠른 속도로 페이드
	int iLogoState;						// 로고 출력 상태
	REAL rUnivLogoTime;					// 졸업 작품 로고 출력 시간
	REAL rTeamLogoTime;					// 팀 로고 출력 시간
	REAL rMainLogoDelayTime;			// 메인 로고 출력까지 추가적인 지연 시간
	REAL rLobbyFrameDelayTime;			// 로딩 완료 후 로비 프레임으로 전환하기 까지 지연 시간
	REAL rFadeoutTime;					// 페이드 아웃 시간
	REAL rFadeinTime;					// 페이드 인 시간
	int iLoadingPercent;				// 로딩 된 리소스 백분율 퍼센트
	HANDLE hLoadingThread;				// 로딩 쓰레드 핸들

public:
	// 생성자 & 소멸자
	StartFrame(Player *pPlayer = NULL);
	StartFrame(StartFrame &obj);
	~StartFrame();

	// 게터 & 세터
	// ...

	// 메소드
	void loadResource();										// 필요한 리소스를 로드
	static DWORD WINAPI thread_loadResFromThread(void *p);		// 스레드 메인 함수
	void updateState(REAL timeElapsedMulti);					// 시간에 따른 상태 업데이트

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);	// 키보드 업데이트
	virtual bool updateFrame(REAL timeElapsed); // 프레임 업데이트
};



#endif