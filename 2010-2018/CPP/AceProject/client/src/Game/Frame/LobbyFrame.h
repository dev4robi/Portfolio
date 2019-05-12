#ifndef __LOBBY_FRAME_H__
#define __LOBBY_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LobbyFrame(v1)
*	- Last Update : 2016.08.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 메인 로비 프레임.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): LobbyFrame 기초 설계. (2016.08.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;
class LogoViewer;

class LobbyFrame : public GameFrame {
private:
	enum LobbySelect {
		SELECT_NONE, SELECT_SINGLEPLAY, SELECT_MULTIPLAY, SELECT_PLAYER, SELECT_CREDIT, SELECT_EXIT,
		SELECT_SETUP_NAME, SELECT_SETUP_IP, SELECT_SETUP_OK
	};
	
	Vector3 vecLobbyDefaultFontColor3;	// 로비프레임의 폰트 디폴트 색상

	char cPlayerName[9];				// 플레이어 이름을 임시 저장하기 위한 공간
	char cServerIP[16];					// 서버 IP주소를 임시 저장하기 위한 공간
	REAL rLobbyElapseTime;				// 로비 경과 시간
	LobbySelect enCurrentSelect;		// 현재 선택 메뉴
	LobbySelect enSelectedNextFrame;	// 선택된 다음 프레임
	GLuint texture_backgrondID;			// 배경 텍스처 아이디
	GLuint texture_worldmapID;			// 배경 월드맵 텍스처 아이디
	GLuint texture_gridID;				// 배경 그리드 텍스처 아이디

	RectViewer *pWorldmapViewer;		// 배경 월드맵 뷰어 위젯 포인터
	RectViewer *pPlayerNameViewer;		// 플레이어 이름 뷰어 위젯 포인터 
	RectViewer *pVersionViewer;			// 버전 뷰어 위젯 포인터
	RectViewer *pSinglePracticeViewer;	// 싱글 프랙티스 뷰어 위젯 포인터
	RectViewer *pMultiplayCombatViewer;	// 멀티플레이 뷰어 위젯 포인터
	RectViewer *pPlayerSettingViewer;	// 플레이어 셋팅 뷰어 위젯 포인터
	RectViewer *pCreditViewer;			// 크레딧 뷰어 위젯 포인터
	RectViewer *pExitGameViewer;		// 게임 종료 뷰어 위젯 포인터
	RectViewer *pFadeoutViewer;			// 페이드 아웃 뷰어 위젯 포인터
	RectViewer *pFadeinViewer;			// 페이드 인 뷰어 위젯 포인터 (처음 로비를 로드 시 연출을 위해 사용하는 뷰어)
	
	bool bShowSetupPlayer;				// 셋업 플레이어 보이기 여부
	LobbySelect enCurrentSetupSelect;	// 셋업 플레이어에서 현재 선택한 값
	bool bPlayerNameViewerErr;			// 셋업 플레이어에서 플레이어 이름 뷰어에 올바른 값이 아닐 때 true
	bool bServerIPViewerErr;			// 셋업 플레이어에서 서버 IP 뷰어에 올바른 값이 아닐 때 true
	RectViewer *pSetupPlayerNameViewer;	// 셋업 플레이어에서 플레이어 아이디를 변경할 때 사용하는 뷰어 포인터
	RectViewer *pSetupServerIPViewer;	// 셋업 플레이어에서 서버 아이피를 변경할 때 사용하는 뷰어 포인터
	RectViewer *pSetupOkViewer;			// 셋업 플레이어에서 설정 확인 문구를 표시하기 위한 뷰어 포인터

public:
	// 생성자 소멸자
	LobbyFrame(Player *pPlayer = NULL, bool bFirstLoad = false);	// 전달받은 플레이어(디폴트 NULL), 첫 번째로 호출됨 여부(디폴트 false)
	LobbyFrame(LobbyFrame &obj);
	~LobbyFrame();

	// Getter & Setter
	LobbySelect getLobbySelect();				// 현재 로비 선택값 반환
	void setLobbySelect(LobbySelect select);	// 현재 로비 선택값 설정

	// 메소드
	void doSinglePractice();						// 싱글플레이 루틴 수행
	void doMultiplayCombat();						// 멀티플레이 루틴 수행
	void doPlayerSetting();							// 플레이어 설정 루틴 수행
	void doCredit();								// 크레딧 루틴 수행
	void doExitGame();								// 게임 종료 수행
	void changeSelect(bool moveUp, bool isSetup);	// 선택 변경

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
	
	// 전용 함수
private:
	void showSetup();												// 셋업 창 보이기
	void hideSetup();												// 셋업 창 숨기기
	void updateSetupViewerColorAndScale(LobbySelect enCurSelect);	// 셋업 뷰어 색상을 지정
	bool checkCollectIPv4Addr(const char *pStr);					// 올바른 IPv4 문자열인지를 확인				
};

#endif