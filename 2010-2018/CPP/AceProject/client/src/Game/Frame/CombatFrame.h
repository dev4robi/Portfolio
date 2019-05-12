#ifndef __TEST_FRAME_H__
#define __TEST_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : CombatFrame(v2)
*	- Last Update : 2016.09.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 전투기끼리 전투에 관련된 프레임 클래스.
*
*	- Dependency : [StdHeader.h], Player.h, Terrain.h, FlightObject.h
*
*   - Version(v1): 플레이어 클래스 기초 설계. (2016.07.11)
*	- Version(v2): 멀티플레이어 모드 설계. (2016.09.16)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class Player;
class Terrain;
class FlightObject;
class GamePacket;
class RectViewer;

class CombatFrame : public GameFrame {
private:
	// 멀티플레이 관련 맴버 변수
	DWORD dwLastRecvTime;				// 마지막으로 메시지를 수신받은 시간
	bool bMultiplayMode;				// 멀티플레이 모드 여부
	bool bServerDisconnected;			// 서버 연결 해제됨 여부
	bool bRecvSendMsgFromServer;		// 서버에게 메시지 전송완료 메시지를 받은 경우 (멀티플레이 전용)
	bool bRecvUpdateMsgFromServer;		// 서버로부터 업데이트 메시지를 받은 여부 (멀티플레이 전용)
	vector<int> playerControlVector;	// 플레이어로부터 수행된 전투기 명령 중 네트워크 동기화를 해야 하는 명령ID를 담은 벡터

	// 프레임 맴버 변수
	bool sessionExpired;				// 세션 만료 여부
	REAL sessionLeftTime;				// 세션 남은 시간
	list<Player*> playerList;			// 모든 플레이어 포인터를 저장하는 리스트
	list<FlightObject*> flightObjList;	// 모든 비행체 포인터를 저장하는 리스트
	bool bExitUIIsShown;				// 종료 UI 뷰어 표시 여부
	RectViewer *pExitUIViewer;			// 종료 UI 뷰어 포인터
	bool bHelpUIIsShown;				// 도움말 UI 뷰어 표시 여부
	RectViewer *pHelpUIViewer;			// 도움말 UI 뷰어 포인터
	RectViewer *pFadeoutViewer;			// 페이드 아웃 뷰어

public:
	// 생성자 & 소멸자
	CombatFrame(Player *pLobbyPlayer);													// 싱글플레이 생성자
	CombatFrame(Player *pLobbyPlayer, vector<Player*> *pPlayerVector, REAL newTgtLPS);	// 멀티플레이 생성자
	~CombatFrame();

	// 게터 & 세터
	bool isMultiplayMode();						// 멀티플레이 모드를 수행중이면 true 반환
	bool isServerDisconnected();				// 서버와 연결이 단절된 상태이면 true 반환
	REAL getSessionLeftTime();					// 세션 만료 시간 반환
	vector<int>* getPlayerControlVectorPtr();	// 플레이어로부터 수행된 전투기 명령 중 네트워크 동기화를 해야 하는 명령ID를 담은 벡터 참조자 반환

	list<Player*>* getPlayerListPtr();			// 플레이어 리스트 포인터를 반환
	list<FlightObject*>* getFlightObjListPtr();	// 비행체 리스트 포인터를 반환

	// 메소드
	void updateNetwork(REAL timeElapsed);							// 네트워크 업데이트
	void translatePacket(GamePacket *pPacket, REAL timeElapsed);	// 패킷의 내용을 분석하여 그에 맞는 클라이언트 업데이트 수행

	// 가상 함수
	virtual void updateKey(REAL timeElapsed);	// 키보드 업데이트
	virtual bool updateFrame(REAL timeElapsed); // 프레임 업데이트

private:
	// 전용 함수
	void updateFlightObjList();					// 위젯 리스트로부터 비행체 리스트를 별도로 추출
};

#endif