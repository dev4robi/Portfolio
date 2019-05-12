#include "Game/Frame/MultiplayLobby.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/Frame/CombatFrame.h"
#include "Game/UI/Viewer/LogoViewer.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Object/Player.h"

MultiplayLobby::MultiplayLobby(Player *pPlayer) : GameFrame(TEXT("MultiplayLobby")) {
	/* ��Ƽ�÷��� �κ� ������ �ʱ�ȭ */
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

	/* �÷��̾� �ʱ�ȭ */
	pOwnerPlayer = pPlayer;
	GameCamera *pCam = pOwnerPlayer->getPlayerCamera();
	pCam->lookAt(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	playerVector.push_back(pOwnerPlayer);

	/* ���� �۾����� ���� �ػ� �޾ƿ��� */
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();

	/* ��Ƽ�÷��� �κ� ��ü ���� */
	// �ΰ� ���
	pViewerLogo = new LogoViewer(Vector3(0.5, 0.5, 0.0), { ((scrWidth * 256) / 1920.0), ((scrHeight * 256 / 1080.0)) });
	pViewerLogo->setLogoTurning(true);
	pViewerLogo->setColorAlpha(0.5);
	addWidget(pViewerLogo);

	// �޽��� ���
	pViewerMsg = new RectViewer(Vector3(0.15, 0.6, 0.0), { 0, 0 }, 0);
	pViewerMsg->setTextAttributes("CONNECTING TO SERVER...", 30.0, true, 1);
	addWidget(pViewerMsg);

	// ���ӵ� �÷��̾� �� ��� 
	pViewerPlayerCnt = new RectViewer(Vector3(0.15, 0.4, 0.0), { 0, 0 }, 0);
	pViewerPlayerCnt->setTextAttributes("--> CONNECTED PLAYER : 0", 18.0, true, 1);
	addWidget(pViewerPlayerCnt);

	// ���� ���ð� ���
	pViewerTimer = new RectViewer(Vector3(0.15, 0.25, 0.0), { 0, 0 }, 0);
	pViewerTimer->setTextAttributes("--> SESSION STARTS IN : ??", 16.0, true, 1);
	addWidget(pViewerTimer);

	// �÷��̾� �̸� ���
	Vector3 grayColor(0.8, 0.8, 0.8);
	for (int i = 0; i < MAX_PLAYER; ++i) {
		pViewerPlayerName[i] = new RectViewer(Vector3((i % 2 == 0) ? 0.7 : 0.85, 0.4 - ((i / 2) * 0.08), 0.0), { 0, 0 }, 0);
		pViewerPlayerName[i]->setTextAttributes("PLAYER", 10.0, true, 1);
		pViewerPlayerName[i]->setVisible(false);
		pViewerPlayerName[i]->setColor3(grayColor);
		addWidget(pViewerPlayerName[i]);
	}

	// ���� ���� �ؽ�Ʈ ���
	pViewerCancle = new RectViewer(Vector3(0.025, 0.05, 0.0), { 0, 0 }, 0);
	pViewerCancle->setTextAttributes("PRESS [ESC] TO RETURN TO LOBBY...", 8.0, true, 1);
	addWidget(pViewerCancle);

	// ���̵� ���
	pViewerFade = new RectViewer(Vector3(0.5, 0.5, 0.0), { scrWidth, scrHeight }, 0);
	pViewerFade->setVisible(false);
	addWidget(pViewerFade);

	// ������� ���
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_BattleField4_MainTheme.wav", false, true);
	pBackgroundSound->play();
}

MultiplayLobby::MultiplayLobby(MultiplayLobby &obj) : GameFrame(obj) {

}

MultiplayLobby::~MultiplayLobby() {
	/* ��Ʈ��ũ ���� ��� */
	pNetworkManager->flushSocketBuffer();

	/* �ɹ� ���� �ʱ�ȭ */
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

	// �÷��̾� ������ ������ ��Ƽ�÷��̰� ����Ǹ� CombatFrame�� ���۵ǰ�, ���� ���� �� ������Ų��
	// ��Ƽ�÷��̰� ������� ���� ����� �÷��̾� ���� ���� �� �����ȴ�
}

void MultiplayLobby::sendMsgToServer(REAL timeElapsed) {
	/* ���� ���� */
	static DWORD dwLastSendTime = 0;
	GamePacket toServerPacket;

	/* ���� ���ӽõ� ȸ�� ī���� */
	if (iServerConnectionTryCnt > 3) enClientState = ClientState::CLIENT_CONNECT_FAILED_SERVEROFF;

	/* �����κ��� ���� ���� �޽����� ���� �ʾҰų� �����κ��� 3�� �̻� �������� �ƴ� ��� 1�� ���� ����ȭ ���� */
	if (enClientState != ClientState::CLIENT_CONNECT_FAILED || enClientState != ClientState::CLIENT_CONNECT_FAILED_SERVEROFF) {
		// �� 1�ʸ��� Ŭ���̾�Ʈ ���¿� ���� ���� �޽��� ���� ����
		if (g_pGameManager->getElapsedTime() > dwLastSendTime + 1000) {
			// ������ ���� �õ� �ð� ����
			dwLastSendTime = g_pGameManager->getElapsedTime();
			switch (enClientState) {
			case ClientState::CLIENT_SEND_CONNECT: // Ŭ���̾�Ʈ ���� ���� ��û ��
				// ��Ŷ ���� �� ����
				toServerPacket.dwFrameTime = dwLastSendTime;
				toServerPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT;
				strncpy_s(toServerPacket.byteData, pOwnerPlayer->getPlayerName().c_str(), sizeof(char[9]));
				pNetworkManager->sendMsgToServer(&toServerPacket);
				++iServerConnectionTryCnt;
				break;
			case ClientState::CLIENT_WAIT_PLAYER: // Ŭ���̾�Ʈ �÷��̾� ��� ��

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
	/* ���� ���� */
	GamePacket recvPacket;		// �����κ��� ���� ��Ŷ
	char *pCstr = NULL;
	char cStrBuf[255];
	short sRecvShort = 0;
	int dwRecvInt = 0;
	DWORD dwRecvDword = 0;
	DWORD dwLeftTime = 0;
	Player *pPlayer = NULL;

	/* �����κ��� ��Ŷ�� �޾ƿ� */
	pNetworkManager->recvMsgFromServer(&recvPacket);

	/* ��Ŷ �м� */
	switch (recvPacket.enMsgType) {
	case GamePacket::MsgType::MSG_DEFAULT:
		// ... ���� �����κ��� �ƹ� �޽����� ���� ������ ���
		break;
	case GamePacket::MsgType::MSG_SESSION_START_LEFTTIME: // ���� ���۽ð� �� �ο� �� ����
		// ���� ���� ���ð�
		pCstr = recvPacket.byteData;
		memcpy_s(&dwRecvDword, sizeof(DWORD), recvPacket.byteData, sizeof(DWORD));
		sprintf_s(cStrBuf, "--> SESSION STARTS IN : %d", (int)(dwRecvDword / 1000));
		pCstr += sizeof(DWORD);
		pViewerTimer->setText(cStrBuf);
		// ���� �ð��� 5�� ���ϸ� �������� ǥ��
		if (dwRecvDword < 5000) {
			pViewerTimer->setColor3(1.00f, 0.00f, 0.00f);
		}

		// ������ �÷��̾� ��
		memcpy_s(&dwRecvInt, sizeof(int), pCstr, sizeof(int));
		sprintf_s(cStrBuf, "--> CONNECTED PLAYER : %d", (int)(dwRecvInt));
		pCstr += sizeof(int);
		// ������ �÷��̾��� ���� 8�� �������� �Ӱ� ǥ��
		pViewerPlayerCnt->setColor3(1.00f, 1.00f - dwRecvInt * 0.1f, 1.00f - dwRecvInt * 0.1f);
		pViewerPlayerCnt->setText(cStrBuf);
		break;
	case GamePacket::MsgType::MSG_CONNECT_FAIL_SAMEID: // ���̵� �ߺ����� ���� ����
		pViewerMsg->setText("ERROR : CHANGE PLAYER ID.");
		pViewerMsg->setPosX(0.15);
		pViewerMsg->setTagInt(0);
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MSG_CONNECT_FAIL_STARTED: // ���� �������� ���� ����
		pViewerMsg->setText("SESSION ALREADY STARTED.");
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MGS_CONNECT_FAIL_MAXPLAYER: // �ο� ���� ������ ���� ����
		pViewerMsg->setText("SESSION ALREADY FULL.");
		enClientState = ClientState::CLIENT_CONNECT_FAILED;
		break;
	case GamePacket::MsgType::MSG_CONNECT_SUCCESS: // ���� ���� �޽���
		enClientState = ClientState::CLIENT_WAIT_PLAYER;				// Ŭ���̾�Ʈ ���¸� �÷��̾� ��� ���·� ��ȯ
		pOwnerPlayer->setPlayerNumber((int)recvPacket.sPlayerNumber);	// �����κ��� �ο����� �÷��̾� ��ȣ ���
		pViewerMsg->setText("WAITING FOR PLAYERS...");
		pViewerMsg->setTagInt(10);										// 10�� �ױ׷� ���� (10��, 11�� �ױ׷� ���İ� ������ ����)
		pViewerMsg->setPosX(0.2);
		break;
	case GamePacket::MsgType::MSG_GIVE_CLIENTNAME: // Ŭ���̾�Ʈ ���� �޽���
		pCstr = recvPacket.byteData;
		memcpy_s(cStrBuf, sizeof(char[9]), pCstr, sizeof(char[9])); // �÷��̾� �̸� ����
		pCstr += sizeof(char[9]);
		memcpy_s(&sRecvShort, sizeof(short), pCstr, sizeof(short)); // �÷��̾� ��ȣ ����
		pCstr += sizeof(short);
		pPlayer = new Player((int)sRecvShort, cStrBuf); // �÷��̾� �����͸� �����Ͽ� ���Ϳ� ����
		playerVector.push_back(pPlayer);
		pPlayer = NULL;
		break;
	case GamePacket::MsgType::MSG_REMOVE_CLIENTNAME: // Ŭ���̾�Ʈ ���� ���� �޽���
		pCstr = recvPacket.byteData;
		memcpy_s(cStrBuf, sizeof(char[9]), pCstr, sizeof(char[9])); // �÷��̾� �̸� ����
		pCstr += sizeof(char[9]);
		memcpy_s(&sRecvShort, sizeof(short), pCstr, sizeof(short)); // �÷��̾� ��ȣ ����
		pCstr += sizeof(short);
		for (vector<Player*>::iterator iter = playerVector.begin(); iter != playerVector.end(); ++iter) { // ���� �÷��̾� ���̵� �˻� �� ����
			if (strcmp(cStrBuf, (*iter)->getPlayerName().c_str()) == 0) {
				delete (*iter);
			}
			playerVector.erase(iter);
			break;
		}
		break;
	case GamePacket::MsgType::MSG_SESSION_START: // ���� ���� �޽���
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
	/* ���� ���� */
	char cStrBuf[9]; // �÷��̾� �̸��� ���� ���� ����
	vector<Player*>::iterator iterPlayer = playerVector.begin(); // �÷��̾� ���͸� ������ �ݺ��� ����

	/* pViewerPlayerName : ��� ���� ���ӵ� �÷��̾ �ǵ��� �ǽð� ���� */
	for (int i = 0; i < MAX_PLAYER; ++i) {
		pViewerPlayerName[i]->setVisible(false); // �ϴ� ����
		if (iterPlayer != playerVector.end()) { // �÷��̾ �����ϸ�
			sprintf_s(cStrBuf, "%s", playerVector[i]->getPlayerName().c_str());
			pViewerPlayerName[i]->setTextAttributes(cStrBuf, 10.0, 0, 1); // ���̵� �޾ƿͼ� ��Ʈ �� �ְ�
			pViewerPlayerName[i]->setVisible(true); // ���� ����
			++iterPlayer; // �ݺ��ڰ� ������ ����Ű��
		}
	}

	/* pViewrMsg : ���� �������� ���� ���� */
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

	/* 10�� 11�� �ױ׸� ���� ����� ���İ� ���� */
	REAL textAlpha = pViewerMsg->getTextColorAlpha();
	if (pViewerMsg->getTagInt() == 10) {
		// ���� �ο�
		if (textAlpha > 0.25f) {
			pViewerMsg->setTextColorAlpha(textAlpha - (0.75f * timeElapsed));
		}
		else {
			pViewerMsg->setTagInt(11);
		}
	}
	else if (pViewerMsg->getTagInt() == 11) {
		// ���� ����
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
	/* Ű ������Ʈ */
	updateKey(timeElapsed);

	/* ���� ȿ�� ���� */
	sendMsgToServer(timeElapsed);		// �������� �޽��� ����
	recvMsgFromServer(timeElapsed);		// �����κ��� �޽��� ����
	updatePlayerViewer(timeElapsed);	// �÷��̾� �̸� ��� ȿ��

	/* ���� ������Ʈ �� ��ο� ����Ʈ�� Ǫ�� */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
		drawList.push_back(*iterWidget);
	}

	/* ������ ��ȯ Ȯ�� */
	if (bExitLobby == true) {
		// �������� ���� ���� �޽��� ����
		if (enClientState != ClientState::CLIENT_CONNECT_FAILED || enClientState != ClientState::CLIENT_CONNECT_FAILED_SERVEROFF) {
			GamePacket toServerPacket;
			toServerPacket.dwFrameTime = g_pGameManager->getElapsedTime();
			toServerPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
			strcpy(toServerPacket.byteData, pOwnerPlayer->getPlayerName().c_str());
			pNetworkManager->sendMsgToServer(&toServerPacket);
		}

		// �����κ��� ���� ���� �޽����� �޾� �����ϴ� ���
		if (enClientState == ClientState::CLIENT_START_SESSION) {
			// LPS ���� ����
			REAL savedLPS = rSessionLPSfromServer;

			// �÷��̾� ���� �� �÷��̾� ���� ����
			Player *pPlayer = pOwnerPlayer;
			vector<Player*> *pPlayerVecCopy = new vector<Player*>;
			for (int i = 0; i < playerVector.size(); ++i) {
				pPlayerVecCopy->push_back(playerVector[i]);
			}
			// �÷��̾� ������ �÷��̾� ��ȣ�� ���� �������� ����
			sort(pPlayerVecCopy->begin(), pPlayerVecCopy->end(), Player::cmpNumber);

			// ���� ������ �ı�
			delete this;

			// ���� ������ ����
			g_pGameManager->setGameFrame(new CombatFrame(pPlayer, pPlayerVecCopy, savedLPS));
			pPlayer = NULL;
			pPlayerVecCopy = NULL;

			// ������ ���� ��ȯ
			return true;
		}
		// �� �� �÷��̾ ��� �κ� ������ �ϴ� ���
		else {
			// �÷��̾� ���� ����
			Player *pPlayer = new Player(0, pOwnerPlayer->getPlayerName());

			// �÷��̾� ��ü ����
			for (vector<Player*>::iterator iter = playerVector.begin(); iter != playerVector.end(); ++iter)
				delete (*iter);

			// ���� ������ �ı�
			delete this;

			// ���� ������ ����
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer));

			// ������ ���� ��ȯ
			return true;
		}
	}

	return true;
}