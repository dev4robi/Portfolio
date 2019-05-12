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
	// OpenGL ��� ���� ���� ����
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// ������ �ʱ�ȭ
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

	// ���� �ʿ��� �� �ε�
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 eye(0.0, 0.0, 0.0), at(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0), center(0.5, 0.5, 0.0);

	// ��� ���� ����
	pBackgroundSound = new Sound(eye, "res/Sounds/BGM_Skies_of_Deception.wav", false, true);

	// �÷��̾� ����
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

	// �÷��̾�� ���� �ּ� ���� �ʱ�ȭ
	strcpy(cPlayerName, pOwnerPlayer->getPlayerName().c_str());
	strcpy(cServerIP, pNetworkManager->getServerIPv4PTr());

	// RectViwer ��� ������ ����
	RectViewer *pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_backgrondID);
	addWidget(pViewer);
	pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_worldmapID);
	addWidget(pViewer);
	pWorldmapViewer = pViewer; // ���� ������Ʈ�� ���� ����� ��� �����͸� ����
	pViewer = new RectViewer(center, { scrWidth, scrHeight }, texture_gridID);
	addWidget(pViewer);

	// �»�� Player ID ��� ����
	pViewer = new RectViewer(Vector3(0.01, 0.96, 0.0), { 0, 0 }, 0);
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pViewer->setTextAttributes("PLAYER_ID :  " + string(cPlayerName), 8.0, true, 1);
	pPlayerNameViewer = pViewer;
	addWidget(pViewer);

	// ���ϴ� ���� ���� ��� ����
	pViewer = new RectViewer(Vector3(0.92, 0.02, 0.0), { 0, 0 }, 0);
	pViewer->setColor3(vecLobbyDefaultFontColor3);
	pViewer->setTextAttributes("VERSION: " + string(GAME_VERSION), 6.0, true, 1);
	pVersionViewer = pViewer;
	addWidget(pViewer);

	// LogoViewer �ΰ� ����
	pViewer = new LogoViewer(Vector3(0.8, 0.55, 0.0), { 512, 512 });
	addWidget(pViewer);

	// RectViwer ���� ���� ���� ����
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

	// ù ��° �ε��� ��� ������ ���� ������ ���̵� �ΰ� ����
	if (bFirstLoad == true) {
		pFadeinViewer = new RectViewer(Vector3(0.5, 0.5, 0.0), { 1920, 1080 }, 0); // Fade In
		pFadeinViewer->setColor3(0.0, 0.0, 0.0);
		pFadeinViewer->setColorAlpha(1.0);
		addWidget(pFadeinViewer);
	}

	// AceProject �ΰ� ����
	pViewer = new RectViewer(Vector3(0.8, 0.275, 0.0), { 640, 106 }, pResourceManager->getTextureID("res/Textures/logo/AceProjectLogo.png"));
	addWidget(pViewer);

	// Fade Out ���
	pViewer = new RectViewer(Vector3(0.5, 0.5, 0.0), { 1920, 1080 }, 0);
	pViewer->setColor3(Vector3(0.0, 0.0, 0.0));
	pViewer->setColorAlpha(0.0);
	pViewer->setVisible(false);
	pFadeoutViewer = pViewer;
	addWidget(pViewer);

	// PlayerSetting RectViewer ����
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
	// �¾� â ���̱�
	showSetup();
}

void LobbyFrame::doCredit() {
	enSelectedNextFrame = LobbySelect::SELECT_CREDIT;
}

void LobbyFrame::doExitGame() {
	enSelectedNextFrame = LobbySelect::SELECT_EXIT;
}

void LobbyFrame::changeSelect(bool moveUp, bool isSetup = false) {
	// ���� ����
	bool bPlaySound = false; // ���� ��� ����

	// �÷��̾� �¾��� �� ���� ���
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
	// �÷��̾� �¾��� ���� ���
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

	// ���� ���
	if (bPlaySound == true)
		new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/selectHorn.wav", false, false);
}

void LobbyFrame::updateKey(REAL timeElapsed) {
	InputManager &inputMgr = (*pInputManager);

	// ��üȭ�� âȭ��
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

	/* �޴� �̵� (�÷��̾� ������ ���� ���� ���) */
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

		/* �޴� ���� */
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
	// �÷��̾� ������ ���� ���
	else {
		char ch = '\0';
		if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWN) {	// ����Ű �Է�
			changeSelect(false, true);
		}
		else if (inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWN) {
			changeSelect(true, true);
		}
		else if (inputMgr.getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) { // ESC �Է�
			// �¾� â �����
			hideSetup();
		}
		else if (inputMgr.getKeyState(VK_RETURN) == key_state::KEY_DOWN) {	// ENTER �Է�
			if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_OK) {
				if (bPlayerNameViewerErr == false && bServerIPViewerErr == false) {
					// �¾� â �����
					hideSetup();

					// �÷��̾� �̸� �� ���� IP ����
					pOwnerPlayer->setPlayerName(cPlayerName);
					pNetworkManager->setServerIPv4(cServerIP);
				}
				else {
					// ���� ���� ���
					// ...
				}
			}
		}
		else if ((ch = inputMgr.getLastInputChar()) != '\0') {	// ���� �Է��� ���
			// �ӽ� ���� ����
			char *pStr = NULL;	// ó���� ���ڿ��� �������� ����ų ������
			short strLen = 0;	// ó���� ���ڿ��� ���̸� ������ 2Byte����

			// ���ڿ� ������ ����
			if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_NAME) {
				pStr = cPlayerName;
				strLen = strlen(cPlayerName);
				pStr += strLen; // pStr�� ���ڿ��� ������ NULL���ڸ� ����Ű�� ��
			}
			else if (enCurrentSetupSelect == LobbySelect::SELECT_SETUP_IP) {
				pStr = cServerIP;
				strLen = strlen(cServerIP);
				pStr += strLen; // pStr�� ���ڿ��� ������ NULL���ڸ� ����Ű�� ��
			}

			// ���� �Է� �۾�
			if (ch > 32 && ch < 127) { // (Ư�� ���ڸ� ������ ���� ���� �ƽ�Ű �ڵ� ����)
				if (pStr != &cPlayerName[sizeof(cPlayerName) - 1] && pStr != &cServerIP[sizeof(cServerIP) - 1]) {
					*pStr = (char)ch;
				}
			}

			// ���� ����� �۾�
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
	/* �κ� �ð� ��� */
	rLobbyElapseTime += timeElapsed;

	/* Ű ������Ʈ */
	if (enSelectedNextFrame == LobbySelect::SELECT_NONE || enSelectedNextFrame == LobbySelect::SELECT_PLAYER) // ���̵�ƿ� �߿��� Ű ������Ʈ ��� �Ұ�
		updateKey(timeElapsed);

	/* ���� ������Ʈ */
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		// ������Ʈ
		(*iterWidget)->update(timeElapsed);
	}

	// ���̵� �� ���� ���� ������Ʈ
	if (pFadeinViewer != NULL && pFadeinViewer->getColorAlpha() > 0.00) {
		pFadeinViewer->setColorAlpha(pFadeinViewer->getColorAlpha() - (timeElapsed / 2.0)); // 2.0 �ʿ� ���� ���̵� ��
		if (pFadeinViewer->getColorAlpha() < 0.00) pFadeinViewer->setColorAlpha(0.00);
	}

	// �÷��̾� �̸� ���� ���� ������Ʈ
	pPlayerNameViewer->setText("PLAYER_ID :  " + pOwnerPlayer->getPlayerName());

	// ��� ����� ������ �߰����� ������Ʈ�� ����
	static REAL worldMapX = 0.0;
	pWorldmapViewer->setTextureXY((worldMapX += (0.02 * timeElapsed)) > 1.0 ? worldMapX = worldMapX - 1.0 : worldMapX, 0.00);

	// �޴� ���õ� ���ο� ���� ����, ��ġ, ũ�� ������Ʈ (�Ǽ�!: ������ ����Ͽ� ������ ������ ����ų�, �Լ� �����ͷ� ������ ������Ʈ ����� �����ϴ� ���� �� ȿ�����̿��� ��.)
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
				// ��ġ ���� �̵�
				offset = pViewer->getOffset();
				if ((offset.x += (0.5 * timeElapsed)) >= 0.05) offset.x = 0.05;
				pViewer->setOffset(offset);
				// ũ�� Ȯ��
				scale = pViewer->getScale3();
				if ((scale.x += (1.2 * timeElapsed)) >= 1.2) scale.x = 1.2;
				if ((scale.y += (1.2 * timeElapsed)) >= 1.2) scale.y = 1.2;
				pViewer->setScale3(scale);
				// ���� ����
				pViewer->setColor3(Vector3(0.125, 0.25, 0.5));
			}
		}
		else {
			if (pViewer != NULL) {
				// ��ġ ���� �̵�
				offset = pViewer->getOffset();
				if ((offset.x -= (0.5 * timeElapsed)) < 0.0) offset.x = 0.0;
				pViewer->setOffset(offset);
				// ũ�� ���
				scale = pViewer->getScale3();
				if ((scale.x -= (1.2 * timeElapsed)) < 1.0) scale.x = 1.0;
				if ((scale.y -= (1.2 * timeElapsed)) < 1.0) scale.y = 1.0;
				pViewer->setScale3(scale);
				// ���� ����
				pViewer->setColor3(LobbyFrame::vecLobbyDefaultFontColor3);
			}
		}
	}

	/* ��ο� ����Ʈ �ʱ�ȭ �� ���� ä������ */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		drawList.push_back(*iterWidget);
	}

	// �¾� �÷��̾� â�� ���̴� ���
	if (enSelectedNextFrame == LobbySelect::SELECT_PLAYER) {
		// ��� �ؽ�Ʈ ���� ������Ʈ
		pSetupPlayerNameViewer->setText("+ PLAYER ID(2-8) :  " + string(cPlayerName));
		pSetupServerIPViewer->setText("+ SERVER IP( 0.0.0.0 ) :  " + string(cServerIP));

		// ���� �˻�
		if (strlen(cPlayerName) < 2) bPlayerNameViewerErr = true;
		else bPlayerNameViewerErr = false;
		if (checkCollectIPv4Addr(cServerIP) == false) bServerIPViewerErr = true;
		else bServerIPViewerErr = false;

		// �¾� ������ ���� �� ũ�� ������Ʈ
		updateSetupViewerColorAndScale(enCurrentSetupSelect);
	}
	// ������ �ı� �� ��ȯ�� ��û�� ���
	else if (enSelectedNextFrame == LobbySelect::SELECT_SINGLEPLAY || enSelectedNextFrame == LobbySelect::SELECT_MULTIPLAY ||
			 enSelectedNextFrame == LobbySelect::SELECT_CREDIT || enSelectedNextFrame == LobbySelect::SELECT_EXIT) {
		// ���̵� �ƿ� �Ϸ� ��
		if (pFadeoutViewer->getColorAlpha() >= 1.00) {
			// ���� �����ӿ� ������ �÷��̾� ��ü�� ���� �� ���� �÷��̾��� ���� ����
			Player *pPlayer = new Player(pOwnerPlayer->getPlayerNumber(), pOwnerPlayer->getPlayerName());

			if (enCurrentSelect == LobbySelect::SELECT_SINGLEPLAY) {
				// �÷��̾� �ı� (����!: GameFrame �Ҹ��ڿ� �ְ� �Ǹ� ResultFrame �� ������ ó���� �� �־�� ��.)
				delete pOwnerPlayer;

				// �κ� �ı�
				delete this;

				// ���� ������ ����
				g_pGameManager->setGameFrame(new CombatFrame(pPlayer));

				// ������ �Ϸ� ��ȯ
				return true;
			}
			else if (enCurrentSelect == LobbySelect::SELECT_MULTIPLAY) {
				// �÷��̾� �ı� (����!: GameFrame �Ҹ��ڿ� �ְ� �Ǹ� ResultFrame �� ������ ó���� �� �־�� ��.)
				delete pOwnerPlayer;

				// �κ� �ı�
				delete this;

				// ���� ������ ����
				g_pGameManager->setGameFrame(new MultiplayLobby(pPlayer));
				//pPlayer = NULL;

				// ������ �Ϸ� ��ȯ
				return true;
			}
			else if (enCurrentSelect == LobbySelect::SELECT_CREDIT) {
				// �÷��̾� �ı� (����!: GameFrame �Ҹ��ڿ� �ְ� �Ǹ� ResultFrame �� ������ ó���� �� �־�� ��.)
				delete pOwnerPlayer;

				// �κ� �ı�
				delete this;

				// ���� ������ ����
				g_pGameManager->setGameFrame(new CreditsFrame(pPlayer));

				// ���α׷� ���� ��û
				return true;
			}
			else { // (enCurrentSelect == LobbySelect::SELECT_EXIT)
				// �κ� �ı�
				delete this;

				// ���α׷� ���� ��û
				return false;
			}
		}
		// ���̵� �ƿ� ���� ��
		else {
			pFadeoutViewer->setVisible(true);
			pFadeoutViewer->setColorAlpha(pFadeoutViewer->getColorAlpha() + (1.0 * timeElapsed));	// ȭ�� ���̵� �ƿ�
			pBackgroundSound->setGain(pBackgroundSound->getGain() - (1.0 * timeElapsed));			// ��� ���� ���̵� �ƿ�
		}
	}

	/* ���� ��ȯ */
	return true;
}

void LobbyFrame::showSetup() {
	// ��� ���̱� �� �ʱ�ȭ
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
	// ��� ����� �� �ʱ�ȭ
	enSelectedNextFrame = LobbySelect::SELECT_NONE;
	enCurrentSelect = LobbySelect::SELECT_PLAYER;
	pFadeoutViewer->setColorAlpha(0.0);
	pFadeoutViewer->setVisible(false);
	pSetupPlayerNameViewer->setVisible(false);
	pSetupServerIPViewer->setVisible(false);
	pSetupOkViewer->setVisible(false);
}

void LobbyFrame::updateSetupViewerColorAndScale(LobbySelect enCurSelect) {
	// �ϴ� �⺻ ��Ʈ ũ��� ������� ���� �ʱ�ȭ
	pSetupPlayerNameViewer->setTextSize(20.0);
	pSetupServerIPViewer->setTextSize(20.0);
	pSetupOkViewer->setTextSize(18.0);
	pSetupPlayerNameViewer->setColor3(1.0, 1.0, 1.0);
	pSetupServerIPViewer->setColor3(1.0, 1.0, 1.0);
	pSetupOkViewer->setColor3(1.0, 1.0, 1.0);

	// ���� ���ο� ���� �ؽ�Ʈ ������ ���� �� ���� ���ο� ���� ���ο� ���� ���� ����
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

	// �����˻翡 ���� ��� ���� ����
	bool err = false;
	if (bPlayerNameViewerErr == true)	{ pSetupPlayerNameViewer->setColor3(0.9, 0.1, 0.1);	err = true; }
	if (bServerIPViewerErr == true)		{ pSetupServerIPViewer->setColor3(0.9, 0.1, 0.1);	err = true; }
	if (err == true) pSetupOkViewer->setColor3(0.9, 0.1, 0.1);
}

bool LobbyFrame::checkCollectIPv4Addr(const char *pStr) {
	// ���� ����
	char ipAddr[16];				// ���޹��� ip�ּҸ� �����ϱ� ���� ����
	char *pTokStr = NULL;			// ���޹��� ���ڿ��� "." ���ڸ� �������� �ڸ� ������ ���� ���� ������
	short strLen = strlen(pStr);	// ���޹��� ���ڿ��� ����
	short strDotCnt = 0;			// ���޹��� ���ڿ����� '.' ������ ����
	short strNumConCnt = 0;			// ���޹��� ���ڿ����� '����' �� ��� �������� ��Ÿ������ ī�����ϴ� ����
	unsigned short ipNumber = 0;	// ���޹��� ���ڿ��� ���ڷ� ������ �� 0~255 ���� �������� Ȯ��
	short i = 0;					// ������ ����ϱ� ���� ����

	// ���޹��� IP�ּҸ� ����
	strcpy_s(ipAddr, pStr);

	// ���޹��� ���ڿ��� ���̰� IPv4 �� ���̸� �ʰ��ϴ��� Ȯ��
	if (strLen > 15) { // 255.255.255.255 = char[15]
		return false;
	}

	// ���޹��� ���ڿ��� '����'�� '.' ���� ���ڰ� ���ԵǴ��� Ȯ��
	for (i = 0; i < strLen; ++i) {
		if (mymath::charIsNum(ipAddr[i]) == false) { // '����'�� �ƴϰ�
			if (ipAddr[i] != '.') { // '.' ���ڰ� �ƴϸ�
				return false;
			}
		}
	}

	// ���޹��� ���ڿ��� '.' ���ڰ� 3������, '.'���� �� �ڷ� '����' �� �����ϴ��� Ȯ��
	for (i = 0; i < strLen; ++i) {
		// �� ���̳� �� �ڿ� '.' �� ������ ��� false ��ȯ
		if (i == 0 || i == 15) {
			if (ipAddr[i] == '.') return false;
		}

		// '.'�� ������ ī�����ϰ� �� �ڷ� '����' �� �����ϴ��� Ȯ��
		if (ipAddr[i] == '.') {
			++strDotCnt;
			
			// �� �հ� �� �ڰ� �ƴ� ��쿡�� ����
			if (i != 0 && i != 15) {
				// '.' ���� �� �ڷ� '����'�� �����ϴ��� �˻�
				if (mymath::charIsNum(ipAddr[i - 1]) == false || mymath::charIsNum(ipAddr[i + 1]) == false) {
					return false;
				}
			}
		}
	}
	if (strDotCnt != 3) {
		return false;
	}

	// ���޹��� ���ڿ��� ���ڰ� 4���̻� �������� ��Ÿ������ Ȯ��
	for (i = 0; i < strLen; ++i) {
		if (mymath::charIsNum(ipAddr[i]) == true) ++strNumConCnt;
		else strNumConCnt = 0;
	}
	if (strNumConCnt > 3) {
		return false;
	}

	// ���޹��� ���ڿ� ���ڿ� "001", "00" ���� '0'���� ���� ������ �ִ��� Ȯ��
	for (i = 0; i < strLen; ++i) {
		if (ipAddr[i] == '0' && mymath::charIsNum(ipAddr[i + 1]) == true) { // '0' �ڿ� ���ڰ� ������ ���
			if (mymath::charIsNum(ipAddr[i - 1]) == true && ipAddr[i - 1] == '0') { // '0' �տ� '0'�� �ƴ� '����'�� �����ؾ� ��
				return false;
			}
			if (ipAddr[i - 1] == '.') { // '0' �տ� '.'�� �ְ�, �ڿ� ���ڰ� �ִ� ���� IP�ּҰ� �ƴ�
				return false;
			}
		}
	}

	// ���޹��� ���ڿ��� ���ڰ� 0~255 ���� ������ Ȯ�� (����� ���ڿ��� ���� �۾��� �������Ƿ� ���� �������� ����)
	pTokStr = strtok(ipAddr, ".");
	do {
		ipNumber = (unsigned short)atoi(pTokStr);
		if (ipNumber < 0 || ipNumber > 255) {
			return false;
		}
	} while ((pTokStr = strtok(NULL, ".")) != NULL);

	// �� ���� �˻���� ��� ����ϸ� ���޹��� ���ڿ��� IPv4 �ּ����� ��ȯ
	return true;
}