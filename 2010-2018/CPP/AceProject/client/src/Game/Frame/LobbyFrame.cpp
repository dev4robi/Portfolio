#include "Game/Frame/LobbyFrame.h"
#include "Engine/ResourceManager.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/UI/Viewer/LogoViewer.h"
#include "Game/Object/Player.h"
#include "Engine/GameCamera.h"
#include "Engine/GameWindow.h"
#include "Game/Frame/CombatFrame.h"
#include "Game/Frame/MultiplayLobby.h"
#include "Game/Frame/CreditsFrame.h"

LobbyFrame::LobbyFrame(Player *pPlayer, bool bFirstLoad) : GameFrame(TEXT("LobbyFrame")) {
	// OpenGL 배경 색상 버퍼 설정
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// 프레임 초기화
	vecLobbyDefaultFontColor3 = Vector3(0.00, 0.00, 0.25);
	rLobbyElapseTime = 0.00;
	enCurrentSelect = LobbySelect::SELECT_NONE;
	enSelectedNextFrame = LobbySelect::SELECT_NONE;
	enCurrentSetupSelect = LobbySelect::SELECT_SETUP_NAME;
	bShowSetupPlayer = false;
	bPlayerNameViewerErr = false;
	bServerIPViewerErr = false;
	pFadeinViewer = NULL;

	texture_backgrondID = pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/lobbyBackground.png");
	texture_worldmapID = pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/lobbyWorldmap.png");
	texture_gridID = pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/lobbyGrid.png");

	// 추후 필요한 값 로드
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 eye(0.0, 0.0, 0.0), at(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0), center(0.5, 0.5, 0.0);

	// 배경 음악 생성
	pBackgroundSound = new Sound(eye, "res/Sounds/BGM_Skies_of_Deception.wav", false, true);

	// 플레이어 생성
	if (pPlayer == NULL) {
		char buf[12];
		Vector3 eye(0.0, 0.0, 0.0), at(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0), center(0.5, 0.5, 0.0);
		sprintf_s(buf, "%d", timeGetTime());
		strncpy_s(buf, buf, 5);
		pOwnerPlayer = new Player(0, "ACE" + string(buf));
		GameCamera *pCam = pOwnerPlayer->getPlayerCamera();
		pCam->lookAt(eye, at, up);
	}
	else {
		pOwnerPlayer = pPlayer;
	}

	// 플레이어와 서버 주소 정보 초기화
	strcpy(cPlayerName, pOwnerPlayer->getPlayerName().c_str());
	strcpy(cServerIP, pNetworkManager->getServerIPv4PTr());

	// RectViwer 배경 위젯들 생성
	RectViewer *pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_backgrondID);
	addWidget(pViewer);
	pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_worldmapID);
	addWidget(pViewer);
	pWorldmapViewer = pViewer; // 쉬운 업데이트를 위해 월드맵 뷰어 포인터를 저장
	pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_gridID);
	addWidget(pViewer);

	// 좌상단 Player ID 뷰어 생성
	pViewer = new RectViewer(Vector3(0.01, 0.96, 0.0), { 0, 0 }, 0);
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pViewer->setTextAttributes("PLAYER_ID :  " + string(cPlayerName), 8.0, true, 1);
	pPlayerNameViewer = pViewer;
	addWidget(pViewer);

	// 우하단 게임 버전 뷰어 생성
	pViewer = new RectViewer(Vector3(0.92, 0.02, 0.0), { 0, 0 }, 0);
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pViewer->setTextAttributes("VERSION: " + string(GAME_VERSION), 6.0, true, 1);
	pVersionViewer = pViewer;
	addWidget(pViewer);

	// LogoViewer 로고 생성
	pViewer = new LogoViewer(Vector3(0.8, 0.55, 0.0), { 512, 512 });
	addWidget(pViewer);

	// RectViwer 현재 선택 위젯 생성
	pViewer = new RectViewer(Vector3(0.25, 0.83, 0.0), { 676, 94 }, pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/singlePractice.png")); // Single Practice
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pSinglePracticeViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.274, 0.6633, 0.0), { 768, 94 }, pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/multiplayCombat.png")); // Multiplay Combat
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pMultiplayCombatViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.235, 0.4966, 0.0), { 620, 94 }, pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/playerSetting.png")); // Player Setting
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pPlayerSettingViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.149, 0.3299, 0.0), { 288, 94 }, pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/credit.png")); // Credit
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pCreditViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.179, 0.1215, 0.0), { 406, 94 }, pResourceManager->getTextureID("res/Textures/frame/lobbyFrame/exitGame.png")); // Exit Game
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pExitGameViewer = pViewer;
	addWidget(pViewer);

	// 첫 번째 로드인 경우 연출을 위해 별도의 페이드 로고 생성
	if (bFirstLoad == true) {
		pFadeinViewer = new RectViewer(Vector3(0.5, 0.5, 0.0), { 1920, 1080 }, 0); // Fade In
		pFadeinViewer->setColor3(0.0, 0.0, 0.0);
		pFadeinViewer->setColorAlpha(1.0);
		addWidget(pFadeinViewer);
	}

	// AceProject 로고 생성
	pViewer = new RectViewer(Vector3(0.8, 0.275, 0.0), { 640, 106 }, pResourceManager->getTextureID("res/Textures/logo/AceProjectLogo.png"));
	addWidget(pViewer);

	// Fade Out 뷰어
	pViewer = new RectViewer(Vector3(0.5, 0.5, 0.0), { 1920, 1080 }, 0);
	pViewer->setColor3(Vector3(0.0, 0.0, 0.0));
	pViewer->setColorAlpha(0.0);
	pViewer->setVisible(false);
	pFadeoutViewer = pViewer;
	addWidget(pViewer);

	// PlayerSetting RectViewer 생성
	pViewer = new RectViewer(Vector3(0.15, 0.6, 0.0), { 0, 0 }, 0);		// Player Name
	pViewer->setTextAttributes("+ PLAYER ID(2-8) :  ", 20.0, true, 1);
	pViewer->setVisible(false);
	pSetupPlayerNameViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.15, 0.4, 0.0), { 0, 0 }, 0);		// Server IP
	pViewer->setTextAttributes("+ SERVER IP( 0.0.0.0 ) :  ", 20.0, true, 1);
	pViewer->setVisible(false);
	pSetupServerIPViewer = pViewer;
	addWidget(pViewer);

	pViewer = new RectViewer(Vector3(0.45, 0.2, 0.0), { 0, 0 }, 0);		// OK
	pViewer->setTextAttributes("[ O K ]", 18.0, true, 1);
	pViewer->setVisible(false);
	pSetupOkViewer = pViewer;
	addWidget(pViewer);

	pViewer = NULL;
}

LobbyFrame::LobbyFrame(LobbyFrame &obj) : GameFrame(obj) {

}

LobbyFrame::~LobbyFrame() {
	pWorldmapViewer = NULL;
	pSinglePracticeViewer = NULL;
	pMultiplayCombatViewer = NULL;
	pPlayerSettingViewer = NULL;
	pCreditViewer = NULL;
	pExitGameViewer = NULL;
	pFadeoutViewer = NULL;
	pSetupPlayerNameViewer = NULL;
	pSetupServerIPViewer = NULL;
	pSetupOkViewer = NULL;
}

void LobbyFrame::doSinglePractice() {
	enSelectedNextFrame = LobbySelect::SELECT_SINGLEPLAY;
}

void LobbyFrame::doMultiplayCombat() {
	enSelectedNextFrame = LobbySelect::SELECT_MULTIPLAY;
}

void LobbyFrame::doPlayerSetting() {
	// 셋업 창 보이기
	showSetup();
}

void LobbyFrame::doCredit() {
	enSelectedNextFrame = LobbySelect::SELECT_CREDIT;
}

void LobbyFrame::doExitGame() {
	enSelectedNextFrame = LobbySelect::SELECT_EXIT;
}

void LobbyFrame::changeSelect(bool moveUp, bool isSetup = false) {
	// 지역 변수
	bool bPlaySound = false; // 사운드 출력 여부

	// 플레이어 셋업이 안 켜진 경우
	if (isSetup == false) {
		bPlaySound = true;
		if (moveUp == false) {
			switch (enCurrentSelect) {
			case LobbySelect::SELECT_NONE:			enCurrentSelect = SELECT_SINGLEPLAY;	break;
			case LobbySelect::SELECT_SINGLEPLAY:	enCurrentSelect = SELECT_MULTIPLAY;		break;
			case LobbySelect::SELECT_MULTIPLAY:		enCurrentSelect = SELECT_PLAYER;		break;
			case LobbySelect::SELECT_PLAYER:		enCurrentSelect = SELECT_CREDIT;		break;
			case LobbySelect::SELECT_CREDIT:		enCurrentSelect = SELECT_EXIT;			break;
			case LobbySelect::SELECT_EXIT:			enCurrentSelect = SELECT_SINGLEPLAY;	break;
			default:								enCurrentSelect = SELECT_NONE;
			}
		}
		else {
			switch (enCurrentSelect) {
			case LobbySelect::SELECT_NONE:			enCurrentSelect = SELECT_EXIT;			break;
			case LobbySelect::SELECT_SINGLEPLAY:	enCurrentSelect = SELECT_EXIT;			break;
			case LobbySelect::SELECT_MULTIPLAY:		enCurrentSelect = SELECT_SINGLEPLAY;	break;
			case LobbySelect::SELECT_PLAYER:		enCurrentSelect = SELECT_MULTIPLAY;		break;
			case LobbySelect::SELECT_CREDIT:		enCurrentSelect = SELECT_PLAYER;		break;
			case LobbySelect::SELECT_EXIT:			enCurrentSelect = SELECT_CREDIT;		break;
			default:								enCurrentSelect = SELECT_NONE;
			}
		}
	}
	// 플레이어 셋업이 켜진 경우
	else {
		if (moveUp == false) {
			switch (enCurrentSetupSelect) {
			case LobbySelect::SELECT_SETUP_NAME:	enCurrentSetupSelect = SELECT_SETUP_IP;		bPlaySound = true;	break;
			case LobbySelect::SELECT_SETUP_IP:		enCurrentSetupSelect = SELECT_SETUP_OK;		bPlaySound = true;	break;
			}
		}
		else {
			switch (enCurrentSetupSelect) {
			case LobbySelect::SELECT_SETUP_IP:		enCurrentSetupSelect = SELECT_SETUP_NAME;	bPlaySound = true;	break;
			case LobbySelect::SELECT_SETUP_OK:		enCurrentSetupSelect = SELECT_SETUP_IP;		bPlaySound = true;	break;
			}
		}
	}

	// 사운드 재생
	if (bPlaySound == true)
		new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/selectHorn.wav", false, false);
}

void LobbyFrame::updateKey(REAL timeElapsed) {
	InputManager &inputMgr = (*pInputManager);

	// 전체화면 창화면
	static bool useFullScreen = true;
	if (inputMgr.getKeyState(VK_F5) == key_state::KEY_UP) {
		if (useFullScreen == false) {
			g_pGameManager->getGameWndPtr()->setDisplayMode(DM_WINDOWDED);
			useFullScreen = true;
		}
		else {
			g_pGameManager->getGameWndPtr()->setWndSize(Size2D(1920, 1080));
			g_pGameManager->getGameWndPtr()->setDisplayMode(DM_FULLSCREEN);
			useFullScreen = false;
		}
	}

	/* 메뉴 이동 (플레이어 셋팅을 켜지 않은 경우) */
	if (enSelectedNextFrame != LobbySelect::SELECT_PLAYER) {
		if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWN) {
			changeSelect(false);
		}
		if (inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWN) {
			changeSelect(true);
		}
		static REAL downHoldTime = 0.00;
		if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWNHOLDING) {
			if ((downHoldTime += timeElapsed) > 0.2) {
				downHoldTime = 0.00;
				changeSelect(false);
			}
		}
		if (inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWNHOLDING) {
			if ((downHoldTime += timeElapsed) > 0.2) {
				downHoldTime = 0.00;
				changeSelect(true);
			}
		}
		if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_UP || inputMgr.getKeyState(VK_UP) == key_state::KEY_UP) {
			downHoldTime = 0.00;
		}

		/* 메뉴 선택 */
		if (inputMgr.getKeyState(VK_RETURN) == key_state::KEY_DOWN) {
			switch (enCurrentSelect) {
			default:
			case LobbySelect::SELECT_NONE: break;
			case LobbySelect::SELECT_SINGLEPLAY:
				doSinglePractice();
				return;
			case LobbySelect::SELECT_MULTIPLAY:
				doMultiplayCombat();
				return;
			case LobbySelect::SELECT_PLAYER:
				doPlayerSetting();
				return;
			case LobbySelect::SELECT_CREDIT:
				doCredit();
				return;
			case LobbySelect::SELECT_EXIT:
				doExitGame();
				return;
			}
		}
	}
	// 플레이어 셋팅이 켜진 경우
	else {
		char ch = '\0';
		if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWN) {	// 방향키 입력
			changeSelect(false, true);
		}
		else if (inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWN) {
			changeSelect(true, true);
		}
		else if (inputMgr.getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) { // ESC 입력
			// 셋업 창 숨기기
			hideSetup();
		}
		else if (inputMgr.getKeyState(VK_RETURN) == key_state::KEY_DOWN) {	// ENTER 입력
			if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_OK) {
				if (bPlayerNameViewerErr == false && bServerIPViewerErr == false) {
					// 셋업 창 숨기기
					hideSetup();

					// 플레이어 이름 및 서버 IP 변경
					pOwnerPlayer->setPlayerName(cPlayerName);
					pNetworkManager->setServerIPv4(cServerIP);
				}
				else {
					// 오류 사운드 재생
					// ...
				}
			}
		}
		else if ((ch = inputMgr.getLastInputChar()) != '\0') {	// 문자 입력의 경우
			// 임시 지역 변수
			char *pStr = NULL;	// 처리할 문자열의 마지막을 가리킬 포인터
			short strLen = 0;	// 처리할 문자열의 길이를 저장할 2Byte정수

			// 문자열 마지막 선택
			if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_NAME) {
				pStr = cPlayerName;
				strLen = strlen(cPlayerName);
				pStr += strLen; // pStr이 문자열의 마지막 NULL문자를 가르키게 함
			}
			else if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_IP) {
				pStr = cServerIP;
				strLen = strlen(cServerIP);
				pStr += strLen; // pStr이 문자열의 마지막 NULL문자를 가르키게 함
			}

			// 문자 입력 작업
			if (ch > 32 && ch < 127) { // (특수 문자를 포함한 영문 숫자 아스키 코드 범위)
				if (pStr != &cPlayerName[sizeof(cPlayerName) - 1] && pStr != &cServerIP[sizeof(cServerIP) - 1]) {
					*pStr = (char)ch;
				}
			}

			// 문자 지우기 작업
			else if (ch == VK_BACK || ch == VK_DELETE) {
				if (pStr != &cPlayerName[0] && pStr != &cServerIP[0]) {
					--pStr;
					*pStr = '\0';
				}
			}
			pStr = NULL;
		}
	}
}

bool LobbyFrame::updateFrame(REAL timeElapsed) {
	/* 로비 시간 경과 */
	rLobbyElapseTime += timeElapsed;

	/* 키 업데이트 */
	if (enSelectedNextFrame == LobbySelect::SELECT_NONE || enSelectedNextFrame == LobbySelect::SELECT_PLAYER) // 페이드아웃 중에는 키 업데이트 사용 불가
		updateKey(timeElapsed);

	/* 위젯 업데이트 */
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		// 업데이트
		(*iterWidget)->update(timeElapsed);
	}

	// 페이드 인 위젯 별도 업데이트
	if (pFadeinViewer != NULL && pFadeinViewer->getColorAlpha() > 0.00) {
		pFadeinViewer->setColorAlpha(pFadeinViewer->getColorAlpha() - (timeElapsed / 2.0)); // 2.0 초에 걸쳐 페이드 인
		if (pFadeinViewer->getColorAlpha() < 0.00) pFadeinViewer->setColorAlpha(0.00);
	}

	// 플레이어 이름 위젯 별도 업데이트
	pPlayerNameViewer->setText("PLAYER_ID :  " + pOwnerPlayer->getPlayerName());

	// 배경 월드맵 위젯은 추가적인 업데이트를 가함
	static REAL worldMapX = 0.0;
	pWorldmapViewer->setTextureXY((worldMapX += (0.02 * timeElapsed)) > 1.0 ? worldMapX = worldMapX - 1.0 : worldMapX, 0.00);

	// 메뉴 선택됨 여부에 따른 색상, 위치, 크기 업데이트 (실수!: 위젯을 상속하여 별도의 위젯을 만들거나, 함수 포인터로 위젯의 업데이트 기능을 정의하는 것이 더 효율적이였을 것.)
	Vector3 offset, scale;
	RectViewer *pSelectedViwer = NULL;
	switch (enCurrentSelect) {
	case LobbySelect::SELECT_SINGLEPLAY:	pSelectedViwer = pSinglePracticeViewer;		break;
	case LobbySelect::SELECT_MULTIPLAY:		pSelectedViwer = pMultiplayCombatViewer;	break;
	case LobbySelect::SELECT_PLAYER:		pSelectedViwer = pPlayerSettingViewer;		break;
	case LobbySelect::SELECT_CREDIT:		pSelectedViwer = pCreditViewer;				break;
	case LobbySelect::SELECT_EXIT:			pSelectedViwer = pExitGameViewer;			break;
	default:								pSelectedViwer = NULL;
	}
	for (int i = 0; i < 5; ++i) {
		RectViewer *pViewer = NULL;
		switch (i) {
		case 0:		pViewer = pSinglePracticeViewer;	break;
		case 1:		pViewer = pMultiplayCombatViewer;	break;
		case 2:		pViewer = pPlayerSettingViewer;		break;
		case 3:		pViewer = pCreditViewer;			break;
		case 4:		pViewer = pExitGameViewer;			break;
		default:	pViewer = NULL;
		}

		if (pSelectedViwer == pViewer) {
			if (pViewer != NULL) {
				// 위치 우측 이동
				offset = pViewer->getOffset();
				if ((offset.x += (0.5 * timeElapsed)) >= 0.05) offset.x = 0.05;
				pViewer->setOffset(offset);
				// 크기 확장
				scale = pViewer->getScale3();
				if ((scale.x += (1.2 * timeElapsed)) >= 1.2) scale.x = 1.2;
				if ((scale.y += (1.2 * timeElapsed)) >= 1.2) scale.y = 1.2;
				pViewer->setScale3(scale);
				// 색상 변경
				pViewer->setColor3(Vector3(0.125, 0.25, 0.5));
			}
		}
		else {
			if (pViewer != NULL) {
				// 위치 좌측 이동
				offset = pViewer->getOffset();
				if ((offset.x -= (0.5 * timeElapsed)) < 0.0) offset.x = 0.0;
				pViewer->setOffset(offset);
				// 크기 축소
				scale = pViewer->getScale3();
				if ((scale.x -= (1.2 * timeElapsed)) < 1.0) scale.x = 1.0;
				if ((scale.y -= (1.2 * timeElapsed)) < 1.0) scale.y = 1.0;
				pViewer->setScale3(scale);
				// 색상 복원
				pViewer->setColor3(LobbyFrame::vecLobbyDefaultFontColor3);
			}
		}
	}

	/* 드로우 리스트 초기화 및 위젯 채워넣음 */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		drawList.push_back(*iterWidget);
	}

	// 셋업 플레이어 창이 보이는 경우
	if (enSelectedNextFrame == LobbySelect::SELECT_PLAYER) {
		// 뷰어 텍스트 정보 업데이트
		pSetupPlayerNameViewer->setText("+ PLAYER ID(2-8) :  " + string(cPlayerName));
		pSetupServerIPViewer->setText("+ SERVER IP( 0.0.0.0 ) :  " + string(cServerIP));

		// 오류 검사
		if (strlen(cPlayerName) < 2) bPlayerNameViewerErr = true;
		else bPlayerNameViewerErr = false;
		if (checkCollectIPv4Addr(cServerIP) == false) bServerIPViewerErr = true;
		else bServerIPViewerErr = false;

		// 셋업 뷰어들의 색상 및 크기 업데이트
		updateSetupViewerColorAndScale(enCurrentSetupSelect);
	}
	// 프레임 파괴 및 전환이 요청된 경우
	else if (enSelectedNextFrame == LobbySelect::SELECT_SINGLEPLAY || enSelectedNextFrame == LobbySelect::SELECT_MULTIPLAY ||
			 enSelectedNextFrame == LobbySelect::SELECT_CREDIT || enSelectedNextFrame == LobbySelect::SELECT_EXIT) {
		// 페이드 아웃 완료 시
		if (pFadeoutViewer->getColorAlpha() >= 1.00) {
			// 다음 프레임에 전달할 플레이어 객체를 생성 후 현재 플레이어의 내용 복사
			Player *pPlayer = new Player(pOwnerPlayer->getPlayerNumber(), pOwnerPlayer->getPlayerName());

			if (enCurrentSelect == LobbySelect::SELECT_SINGLEPLAY) {
				// 플레이어 파괴 (주의!: GameFrame 소멸자에 넣게 되면 ResultFrame 에 별도의 처리를 해 주어야 함.)
				delete pOwnerPlayer;

				// 로비 파괴
				delete this;

				// 다음 프레임 설정
				g_pGameManager->setGameFrame(new CombatFrame(pPlayer));

				// 프레임 완료 반환
				return true;
			}
			else if (enCurrentSelect == LobbySelect::SELECT_MULTIPLAY) {
				// 플레이어 파괴 (주의!: GameFrame 소멸자에 넣게 되면 ResultFrame 에 별도의 처리를 해 주어야 함.)
				delete pOwnerPlayer;

				// 로비 파괴
				delete this;

				// 다음 프레임 설정
				g_pGameManager->setGameFrame(new MultiplayLobby(pPlayer));
				//pPlayer = NULL;

				// 프레임 완료 반환
				return true;
			}
			else if (enCurrentSelect == LobbySelect::SELECT_CREDIT) {
				// 플레이어 파괴 (주의!: GameFrame 소멸자에 넣게 되면 ResultFrame 에 별도의 처리를 해 주어야 함.)
				delete pOwnerPlayer;

				// 로비 파괴
				delete this;

				// 다음 프레임 설정
				g_pGameManager->setGameFrame(new CreditsFrame(pPlayer));

				// 프로그램 종료 요청
				return true;
			}
			else { // (enCurrentSelect == LobbySelect::SELECT_EXIT)
				// 로비 파괴
				delete this;

				// 프로그램 종료 요청
				return false;
			}
		}
		// 페이드 아웃 중일 시
		else {
			pFadeoutViewer->setVisible(true);
			pFadeoutViewer->setColorAlpha(pFadeoutViewer->getColorAlpha() + (1.0 * timeElapsed));	// 화면 페이드 아웃
			pBackgroundSound->setGain(pBackgroundSound->getGain() - (1.0 * timeElapsed));			// 배경 음악 페이드 아웃
		}
	}

	/* 성공 반환 */
	return true;
}

void LobbyFrame::showSetup() {
	// 뷰어 보이기 및 초기화
	enSelectedNextFrame = LobbySelect::SELECT_PLAYER;
	enCurrentSetupSelect = LobbySelect::SELECT_SETUP_NAME;
	pFadeoutViewer->setColorAlpha(0.75);
	pFadeoutViewer->setVisible(true);
	pSetupPlayerNameViewer->setVisible(true);
	pSetupServerIPViewer->setVisible(true);
	pSetupOkViewer->setVisible(true);
	strncpy(cPlayerName, pOwnerPlayer->getPlayerName().c_str(), sizeof(cPlayerName));
	strncpy(cServerIP, pNetworkManager->getServerIPv4PTr(), sizeof(cServerIP));
}

void LobbyFrame::hideSetup() {
	// 뷰어 숨기기 및 초기화
	enSelectedNextFrame = LobbySelect::SELECT_NONE;
	enCurrentSelect = LobbySelect::SELECT_PLAYER;
	pFadeoutViewer->setColorAlpha(0.0);
	pFadeoutViewer->setVisible(false);
	pSetupPlayerNameViewer->setVisible(false);
	pSetupServerIPViewer->setVisible(false);
	pSetupOkViewer->setVisible(false);
}

void LobbyFrame::updateSetupViewerColorAndScale(LobbySelect enCurSelect) {
	// 일단 기본 폰트 크기와 흰색으로 색상 초기화
	pSetupPlayerNameViewer->setTextSize(20.0);
	pSetupServerIPViewer->setTextSize(20.0);
	pSetupOkViewer->setTextSize(18.0);
	pSetupPlayerNameViewer->setColor3(1.0, 1.0, 1.0);
	pSetupServerIPViewer->setColor3(1.0, 1.0, 1.0);
	pSetupOkViewer->setColor3(1.0, 1.0, 1.0);

	// 선택 여부에 따라 텍스트 사이즈 조절 및 선택 여부와 오류 여부에 따라 색상 지정
	if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_NAME) {
		pSetupPlayerNameViewer->setTextSize(22.0);
		pSetupPlayerNameViewer->setColor3(0.0, 0.635, 0.909);
	}
	else if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_IP) {
		pSetupServerIPViewer->setTextSize(22.0);
		pSetupServerIPViewer->setColor3(0.0, 0.635, 0.909);
	}
	else if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_OK) {
		pSetupOkViewer->setTextSize(25.0);
		pSetupOkViewer->setColor3(0.0, 0.635, 0.909);
	}

	// 오류검사에 따른 경고 색상 지정
	bool err = false;
	if (bPlayerNameViewerErr == true)	{ pSetupPlayerNameViewer->setColor3(0.9, 0.1, 0.1);	err = true; }
	if (bServerIPViewerErr == true)		{ pSetupServerIPViewer->setColor3(0.9, 0.1, 0.1);	err = true; }
	if (err == true) pSetupOkViewer->setColor3(0.9, 0.1, 0.1);
}

bool LobbyFrame::checkCollectIPv4Addr(const char *pStr) {
	// 지역 변수
	char ipAddr[16];				// 전달받은 ip주소를 복사하기 위한 공간
	char *pTokStr = NULL;			// 전달받은 문자열을 "." 문자를 기준으로 자른 문자의 시작 지점 포인터
	short strLen = strlen(pStr);	// 전달받은 문자열의 길이
	short strDotCnt = 0;			// 전달받은 문자열에서 '.' 문자의 개수
	short strNumConCnt = 0;			// 전달받은 문자열에서 '숫자' 가 몇번 연속으로 나타나는지 카운팅하는 정수
	unsigned short ipNumber = 0;	// 전달받은 문자열을 숫자로 구성할 시 0~255 범위 내인지를 확인
	short i = 0;					// 루프에 사용하기 위한 정수

	// 전달받은 IP주소를 복사
	strcpy_s(ipAddr, pStr);

	// 전달받은 문자열의 길이가 IPv4 의 길이를 초과하는지 확인
	if (strLen > 15) { // 255.255.255.255 = char[15]
		return false;
	}

	// 전달받은 문자열에 '숫자'와 '.' 외의 문자가 포함되는지 확인
	for (i = 0; i < strLen; ++i) {
		if (mymath::charIsNum(ipAddr[i]) == false) { // '숫자'가 아니고
			if (ipAddr[i] != '.') { // '.' 문자가 아니면
				return false;
			}
		}
	}

	// 전달받은 문자열에 '.' 문자가 3개인지, '.'문자 앞 뒤로 '숫자' 가 존재하는지 확인
	for (i = 0; i < strLen; ++i) {
		// 맨 앞이나 맨 뒤에 '.' 이 존재한 경우 false 반환
		if (i == 0 || i == 15) {
			if (ipAddr[i] == '.') return false;
		}

		// '.'의 개수를 카운팅하고 앞 뒤로 '숫자' 가 존재하는지 확인
		if (ipAddr[i] == '.') {
			++strDotCnt;
			
			// 맨 앞과 맨 뒤가 아닌 경우에만 조사
			if (i != 0 && i != 15) {
				// '.' 문자 앞 뒤로 '숫자'가 존재하는지 검사
				if (mymath::charIsNum(ipAddr[i - 1]) == false || mymath::charIsNum(ipAddr[i + 1]) == false) {
					return false;
				}
			}
		}
	}
	if (strDotCnt != 3) {
		return false;
	}

	// 전달받은 문자열에 숫자가 4번이상 연속으로 나타나는지 확인
	for (i = 0; i < strLen; ++i) {
		if (mymath::charIsNum(ipAddr[i]) == true) ++strNumConCnt;
		else strNumConCnt = 0;
	}
	if (strNumConCnt > 3) {
		return false;
	}

	// 전달받은 문자열 문자에 "001", "00" 등의 '0'으로 인한 오류가 있는지 확인
	for (i = 0; i < strLen; ++i) {
		if (ipAddr[i] == '0' && mymath::charIsNum(ipAddr[i + 1]) == true) { // '0' 뒤에 숫자가 존재할 경우
			if (mymath::charIsNum(ipAddr[i - 1]) == true && ipAddr[i - 1] == '0') { // '0' 앞에 '0'이 아닌 '숫자'가 존재해야 함
				return false;
			}
			if (ipAddr[i - 1] == '.') { // '0' 앞에 '.'이 있고, 뒤에 숫자가 있는 경우는 IP주소가 아님
				return false;
			}
		}
	}

	// 전달받은 문자열의 숫자가 0~255 범위 내인지 확인 (복사된 문자열에 수정 작업이 가해지므로 가장 마지막에 수행)
	pTokStr = strtok(ipAddr, ".");
	do {
		ipNumber = (unsigned short)atoi(pTokStr);
		if (ipNumber < 0 || ipNumber > 255) {
			return false;
		}
	} while ((pTokStr = strtok(NULL, ".")) != NULL);

	// 위 조건 검사들을 모두 통과하면 전달받은 문자열은 IPv4 주소임을 반환
	return true;
}