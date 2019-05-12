#include "Game/Frame/MultiplayLobby.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/Frame/CombatFrame.h"
#include "Game/UI/Viewer/LogoViewer.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Object/Player.h"

MultiplayLobby::MultiplayLobby(Player *pPlayer) : GameFrame(TEXT("MultiplayLobby")) {
	/* 멀티플레이 로비 프레임 초기화 */
	bExitLobby = false;
	enClientState = CLIENT_SEND_CONNECT;
	pViewerLogo = NULL;
	pViewerTimer = NULL;
	pViewerFade = NULL;
	pViewerPlayerCnt = NULL;
	playerVector.reserve(16);
	playerVector.clear();
	iServerConnectionTryCnt = 0;
	rSessionLPSfromServer = 50.0;

	/* 플레이어 초기화 */
	pOwnerPlayer = pPlayer;
	GameCamera *pCam = pOwnerPlayer->getPlayerCamera();
	pCam->lookAt(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	playerVector.push_back(pOwnerPlayer);

	/* 다음 작업들을 위해 해상도 받아오기 */
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();

	/* 멀티플레이 로비 뷰어객체 생성 */
	// 로고 뷰어
	pViewerLogo = new LogoViewer(Vector3(0.5, 0.5, 0.0), { ((scrWidth * 256) / 1920.0), ((scrHeight * 256 / 1080.0)) });
	pViewerLogo->setLogoTurning(true);
	pViewerLogo->setColorAlpha(0.5);
	addWidget(pViewerLogo);

	// 메시지 뷰어
	pViewerMsg = new RectViewer(Vector3(0.15, 0.6, 0.0), { 0, 0 }, 0);
	pViewerMsg->setTextAttributes("CONNECTING TO SERVER...", 30.0, true, 1);
	addWidget(pViewerMsg);

	// 접속된 플레이어 수 뷰어 
	pViewerPlayerCnt = new RectViewer(Vector3(0.15, 0.4, 0.0), { 0, 0 }, 0);
	pViewerPlayerCnt->setTextAttributes("--> CONNECTED PLAYER : 0", 18.0, true, 1);
	addWidget(pViewerPlayerCnt);

	// 남은 대기시간 뷰어
	pViewerTimer = new RectViewer(Vector3(0.15, 0.25, 0.0), { 0, 0 }, 0);
	pViewerTimer->setTextAttributes("--> SESSION STARTS IN : ??", 16.0, true, 1);
	addWidget(pViewerTimer);

	// 플레이어 이름 뷰어
	Vector3 grayColor(0.8, 0.8, 0.8);
	for (int i = 0; i < MAX_PLAYER; ++i) {
		pViewerPlayerName[i] = new RectViewer(Vector3((i % 2 == 0) ? 0.7 : 0.85, 0.4 - ((i / 2) * 0.08), 0.0), { 0, 0 }, 0);
		pViewerPlayerName[i]->setTextAttributes("PLAYER", 10.0, true, 1);
		pViewerPlayerName[i]->setVisible(false);
		pViewerPlayerName[i]->setColor3(grayColor);
		addWidget(pViewerPlayerName[i]);
	}

	// 연결 종료 텍스트 뷰어
	pViewerCancle = new RectViewer(Vector3(0.025, 0.05, 0.0), { 0, 0 }, 0);
	pViewerCancle->setTextAttributes("PRESS [ESC] TO RETURN TO LOBBY...", 8.0, true, 1);
	addWidget(pViewerCancle);

	// 페이드 뷰어
	pViewerFade = new RectViewer(Vector3(0.5, 0.5, 0.0), { scrWidth, scrHeight }, 0);
	pViewerFade->setVisible(false);
	addWidget(pViewerFade);

	// 배경음악 재생
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_BattleField4_MainTheme.wav", false, true);
	pBackgroundSound->play();
}

MultiplayLobby::MultiplayLobby(MultiplayLobby &obj) : GameFrame(obj) {

}

MultiplayLobby::~MultiplayLobby() {
	/* 네트워크 소켓 비움 */
	pNetworkManager->flushSocketBuffer();

	/* 맴버 변수 초기화 */
	bExitLobby = false;
	enClientState = CLIENT_SEND_CONNECT;
	pViewerLogo = NULL;
	pViewerMsg = NULL;
	pViewerPlayerCnt = NULL;
	pViewerTimer = NULL;
	for (int i = 0; i < MAX_PLAYER; ++i)
		pViewerPlayerName[i] = NULL;
	pViewerCancle = NULL;
	pViewerFade = NULL;
	playerVector.clear();

	// 플레이어 벡터의 내용은 멀티플레이가 수행되면 CombatFrame에 전송되고, 게임 종료 시 삭제시킨다
	// 멀티플레이가 수행되지 않은 경우라면 플레이어 접속 종료 시 삭제된다
}

void MultiplayLobby::sendMsgToServer(REAL timeElapsed) {
	/* 변수 선언 */
	static DWORD dwLastSendTime = 0;
	GamePacket toServerPacket;

	/* 서버 접속시도 회수 카운팅 */
	if (iServerConnectionTryCnt > 3) enClientState = ClientState::CLIENT_CONNECT_FAILED_SERVEROFF;

	/* 서버로부터 접속 실패 메시지를 받지 않았거나 서버로부터 3초 이상 무응답이 아닌 경우 1초 마다 동기화 수행 */
	if (enClientState != ClientState::CLIENT_CONNECT_FAILED || enClientState != ClientState::CLIENT_CONNECT_FAILED_SERVEROFF) {
		// 매 1초마다 클라이언트 상태에 따른 서버 메시지 전송 수행
		if (g_pGameManager->getElapsedTime() > dwLastSendTime + 1000) {
			// 마지막 전송 시도 시간 저장
			dwLastSendTime = g_pGameManager->getElapsedTime();
			switch (enClientState) {
			case ClientState::CLIENT_SEND_CONNECT: // 클라이언트 서버 접속 요청 시
				// 패킷 생성 및 전송
				toServerPacket.dwFrameTime = dwLastSendTime;
				toServerPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT;
				strncpy_s(toServerPacket.byteData, pOwnerPlayer->getPlayerName().c_str(), sizeof(char[9]));
				pNetworkManager->sendMsgToServer(&toServerPacket);
				++iServerConnectionTryCnt;
				break;
			case ClientState::CLIENT_WAIT_PLAYER: // 클라이언트 플레이어 대기 시

				break;
			}
		}
		else {
			// IDLE time...
		}
	}
	else {
		// ...
	}
}

void MultiplayLobby::recvMsgFromServer(REAL timeElapsed) {
	/* 지역 변수 */
	GamePacket recvPacket;		// 서버로부터 받은 패킷
	char *pCstr = NULL;
	char cStrBuf[255];
	short sRecvShort = 0;
	int dwRecvInt = 0;
	DWORD dwRecvDword = 0;
	DWORD dwLeftTime = 0;
	Player *pPlayer = NULL;

	/* 서버로부터 패킷을 받아옴 */
	pNetworkManager->recvMsgFromServer(&recvPacket);

	/* 패킷 분석 */
	switch (recvPacket.enMsgType) {
	case GamePacket::MsgType::MSG_DEFAULT:
		// ... 게임 서버로부터 아무 메시지를 받지 못했을 경우
		break;
	case GamePacket::MsgType::MSG_SESSION_START_LEFTTIME: // 남은 시작시간 및 인원 수 수신
		// 남은 시작 대기시간
		pCstr = recvPacket.byteData;
		memcpy_s(&dwRecvDword, sizeof(DWORD), recvPacket.byteData, sizeof(DWORD));
		sprintf_s(cStrBuf, "--> SESSION STARTS IN : %d", (int)(dwRecvDword / 1000));
		pCstr += sizeof(DWORD);
		pViewerTimer->setText(cStrBuf);
		// 남은 시간이 5초 이하면 적색으로 표시
		if (dwRecvDword < 5000) {
			pViewerTimer->setColor3(1.00f, 0.00f, 0.00f);
		}

		// 접속한 플레이어 수
		memcpy_s(&dwRecvInt, sizeof(int), pCstr, sizeof(int));
		sprintf_s(cStrBuf, "--> CONNECTED PLAYER : %d", (int)(dwRecvInt));
		pCstr += sizeof(int);
		// 접속한 플레이어의 수가 8명에 가까울수록 붉게 표현
		pViewerPlayerCnt->setColor3(1.00f, 1.00f - dwRecvInt * 0.1f, 1.00f - dwRecvInt * 0.1f);
		pViewerPlayerCnt->setText(cStrBuf);
		break;
	case GamePacket::MsgType::MSG_CONNECT_FAIL_SAMEID: // 아이디 중복으로 접속 실패
		pViewerMsg->setText("ERROR : CHANGE PLAYER ID.");
		pViewerMsg->setPosX(0.15);
		pViewerMsg->setTagInt(0);
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MSG_CONNECT_FAIL_STARTED: // 세션 시작으로 접속 실패
		pViewerMsg->setText("SESSION ALREADY STARTED.");
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MGS_CONNECT_FAIL_MAXPLAYER: // 인원 가득 참으로 접속 실패
		pViewerMsg->setText("SESSION ALREADY FULL.");
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MSG_CONNECT_SUCCESS: // 접속 성공 메시지
		enClientState = ClientState::CLIENT_WAIT_PLAYER;				// 클라이언트 상태를 플레이어 대기 상태로 전환
		pOwnerPlayer->setPlayerNumber((int)recvPacket.sPlayerNumber);	// 서버로부터 부여받은 플레이어 번호 사용
		pViewerMsg->setText("WAITING FOR PLAYERS...");
		pViewerMsg->setTagInt(10);										// 10번 테그로 설정 (10번, 11번 테그로 알파값 변경을 구현)
		pViewerMsg->setPosX(0.2);
		break;
	case GamePacket::MsgType::MSG_GIVE_CLIENTNAME: // 클라이언트 접속 메시지
		pCstr = recvPacket.byteData;
		memcpy_s(cStrBuf, sizeof(char[9]), pCstr, sizeof(char[9])); // 플레이어 이름 정보
		pCstr += sizeof(char[9]);
		memcpy_s(&sRecvShort, sizeof(short), pCstr, sizeof(short)); // 플레이어 번호 정보
		pCstr += sizeof(short);
		pPlayer = new Player((int)sRecvShort, cStrBuf); // 플레이어 데이터를 생성하여 벡터에 넣음
		playerVector.push_back(pPlayer);
		pPlayer = NULL;
		break;
	case GamePacket::MsgType::MSG_REMOVE_CLIENTNAME: // 클라이언트 접속 종료 메시지
		pCstr = recvPacket.byteData;
		memcpy_s(cStrBuf, sizeof(char[9]), pCstr, sizeof(char[9])); // 플레이어 이름 정보
		pCstr += sizeof(char[9]);
		memcpy_s(&sRecvShort, sizeof(short), pCstr, sizeof(short)); // 플레이어 번호 정보
		pCstr += sizeof(short);
		for (vector<Player*>::iterator iter = playerVector.begin(); iter != playerVector.end(); ++iter) { // 같은 플레이어 아이디 검사 후 삭제
			if (strcmp(cStrBuf, (*iter)->getPlayerName().c_str()) == 0) {
				delete (*iter);
			}
			playerVector.erase(iter);
			break;
		}
		break;
	case GamePacket::MsgType::MSG_SESSION_START: // 세션 시작 메시지
		pCstr = recvPacket.byteData;
		memcpy_s(&rSessionLPSfromServer, sizeof(REAL), recvPacket.byteData, sizeof(REAL));
		pCstr += sizeof(REAL);
		enClientState = ClientState::CLIENT_START_SESSION;
		bExitLobby = true;
		break;
	default:;
		// ...
	}
}

void MultiplayLobby::updatePlayerViewer(REAL timeElapsed) {
	/* 지역 변수 */
	char cStrBuf[9]; // 플레이어 이름을 담을 지역 변수
	vector<Player*>::iterator iterPlayer = playerVector.begin(); // 플레이어 벡터를 조사할 반복자 변수

	/* pViewerPlayerName : 출력 값을 접속된 플레이어가 되도록 실시간 갱신 */
	for (int i = 0; i < MAX_PLAYER; ++i) {
		pViewerPlayerName[i]->setVisible(false); // 일단 숨김
		if (iterPlayer != playerVector.end()) { // 플레이어가 존재하면
			sprintf_s(cStrBuf, "%s", playerVector[i]->getPlayerName().c_str());
			pViewerPlayerName[i]->setTextAttributes(cStrBuf, 10.0, 0, 1); // 아이디를 받아와서 렉트 뷰어에 넣고
			pViewerPlayerName[i]->setVisible(true); // 숨김 해제
			++iterPlayer; // 반복자가 다음을 가르키게
		}
	}

	/* pViewrMsg : 서버 오프라인 상태 오류 */
	if (enClientState == ClientState::CLIENT_CONNECT_FAILED_SERVEROFF) {
		pViewerMsg->setText("ERROR : SERVER OFFLINE.");
		pViewerMsg->setPos(0.175, 0.5, 0.0);
		pViewerMsg->setColor3(Vector3(0.9, 0.1, 0.1));
		pViewerMsg->setColorAlpha(1.0);
		pViewerPlayerCnt->setVisible(false);
		pViewerTimer->setVisible(false);
		for (int i = 0; i < MAX_PLAYER; ++i) {
			pViewerPlayerName[i]->setVisible(false);
		}
	}

	/* 10번 11번 테그를 가진 뷰어의 알파값 적용 */
	REAL textAlpha = pViewerMsg->getTextColorAlpha();
	if (pViewerMsg->getTagInt() == 10) {
		// 투명도 부여
		if (textAlpha > 0.25f) {
			pViewerMsg->setTextColorAlpha(textAlpha - (0.75f * timeElapsed));
		}
		else {
			pViewerMsg->setTagInt(11);
		}
	}
	else if (pViewerMsg->getTagInt() == 11) {
		// 투명도 제거
		if (textAlpha < 1.0f) {
			pViewerMsg->setTextColorAlpha(textAlpha + (0.75f * timeElapsed));
		}
		else {
			pViewerMsg->setTagInt(10);
		}
	}
}

void MultiplayLobby::updateKey(REAL timeElapsed) {
	if (pInputManager->getKeyState(VK_ESCAPE) == key_state::KEY_DOWNHOLDING) {
		bExitLobby = true;
	}
}

bool MultiplayLobby::updateFrame(REAL timeElapsed) {
	/* 키 업데이트 */
	updateKey(timeElapsed);

	/* 위젯 효과 적용 */
	sendMsgToServer(timeElapsed);		// 서버에게 메시지 전송
	recvMsgFromServer(timeElapsed);		// 서버로부터 메시지 수신
	updatePlayerViewer(timeElapsed);	// 플레이어 이름 뷰어 효과

	/* 위젯 업데이트 및 드로우 리스트에 푸시 */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
		drawList.push_back(*iterWidget);
	}

	/* 프레임 전환 확인 */
	if (bExitLobby == true) {
		// 서버에게 접속 종료 메시지 전송
		if (enClientState != ClientState::CLIENT_CONNECT_FAILED || enClientState != ClientState::CLIENT_CONNECT_FAILED_SERVEROFF) {
			GamePacket toServerPacket;
			toServerPacket.dwFrameTime = g_pGameManager->getElapsedTime();
			toServerPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
			strcpy(toServerPacket.byteData, pOwnerPlayer->getPlayerName().c_str());
			pNetworkManager->sendMsgToServer(&toServerPacket);
		}

		// 서버로부터 세션 시작 메시지를 받아 종료하는 경우
		if (enClientState == ClientState::CLIENT_START_SESSION) {
			// LPS 정보 복사
			REAL savedLPS = rSessionLPSfromServer;

			// 플레이어 정보 및 플레이어 벡터 복사
			Player *pPlayer = pOwnerPlayer;
			vector<Player*> *pPlayerVecCopy = new vector<Player*>;
			for (int i = 0; i < playerVector.size(); ++i) {
				pPlayerVecCopy->push_back(playerVector[i]);
			}
			// 플레이어 정보를 플레이어 번호에 따라 오른차순 정렬
			sort(pPlayerVecCopy->begin(), pPlayerVecCopy->end(), Player::cmpNumber);

			// 현재 프레임 파괴
			delete this;

			// 다음 프레임 설정
			g_pGameManager->setGameFrame(new CombatFrame(pPlayer, pPlayerVecCopy, savedLPS));
			pPlayer = NULL;
			pPlayerVecCopy = NULL;

			// 프레임 종료 반환
			return true;
		}
		// 그 외 플레이어가 대기 로비를 나가려 하는 경우
		else {
			// 플레이어 정보 복사
			Player *pPlayer = new Player(0, pOwnerPlayer->getPlayerName());

			// 플레이어 객체 삭제
			for (vector<Player*>::iterator iter = playerVector.begin(); iter != playerVector.end(); ++iter)
				delete (*iter);

			// 현재 프레임 파괴
			delete this;

			// 다음 프레임 설정
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer));

			// 프레임 종료 반환
			return true;
		}
	}

	return true;
}