#include "Game/Frame/ResultFrame.h"
#include "Engine/GameCamera.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/UI/Viewer/LogoViewer.h"

ResultFrame::ResultFrame(vector<Player*> playerList, Player *pOwnPlayer) : GameFrame(TEXT("ResultFrame")) {
	// OpenGL ��� ���� ���� ����
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// Ŭ���� �ʱ�ȭ
	enScene = ResultScene::SCENE_BEGIN;
	rFrameElapseTime = 0.0;
	pOwnerPlayer = pOwnPlayer;
	rScoreShowTerm = 0.5;
	playerVector = playerList;
	bQuitable = false;
	bShowScore = false;
	vecScorePos = Vector3(0.225, 0.6, 0.0);
	iPlayerCount = 0;

	// �÷��̾ ������ ���� ����
	sort(playerVector.begin(), playerVector.end(), Player::cmpScore);

	// �ؽ�ó ��ȣ ����
	texture_missionResult = pResourceManager->getTextureID("res/Textures/frame/resultFrame/result.png");
	texture_acePlayer = pResourceManager->getTextureID("res/Textures/frame/resultFrame/acePlayer.png");

	// �÷��̾� ī�޶� ����
	GameCamera &cam = *(pOwnerPlayer->getPlayerCamera());
	Vector3 eye(0.0, 0.0, 0.0), at(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0);
	cam.lookAt(eye, at, up);

	// ���� ���
	pBackgroundSound = new Sound(eye, "res/Sounds/BGM_Overwatch_Score_Screen.wav", false, true);
	
	// ���� ��� �߰�
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	RectViewer *pBackground = new RectViewer(Vector3(0.5, 0.5, 0.0), { pResolution->getWidth(), pResolution->getHeight() }, 0);
	pBackground->setColor3(0.0, 0.0, 0.0);
	addWidget(pBackground);

	// ��� �ΰ� �߰�
	REAL logoWidth = (pResolution->getWidth() * 1024.0 / 1920.0), logoHeight = (pResolution->getHeight() * 1024.0 / 1080.0);
	LogoViewer *pLogoViewer = new LogoViewer(Vector3(0.5, 0.5, 0.0), { logoWidth, logoHeight });
	pLogoViewer->setColorAlpha(0.2);
	addWidget(pLogoViewer);

	// ��� �ʱ�ȭ
	pAnykeyViewer = NULL;
	pAceViewer = NULL;
	pFadeoutViewer = NULL;
}

ResultFrame::ResultFrame(ResultFrame &obj) : GameFrame(obj) {
	rFrameElapseTime = obj.rFrameElapseTime;
	rScoreShowTerm = obj.rScoreShowTerm;
	playerVector = obj.playerVector;
	bQuitable = obj.bQuitable;
	texture_missionResult = obj.texture_missionResult;
	texture_acePlayer = obj.texture_acePlayer;
}

ResultFrame::~ResultFrame() {
	// �÷��̾� ����
	for (int i = 0; i < playerVector.size(); ++i) {
		delete playerVector[i];
	}
	playerVector.clear();

	// ������ �ʱ�ȭ
	pAnykeyViewer = NULL;
	pAceViewer = NULL;
	pFadeoutViewer = NULL;
}

void ResultFrame::updateKey(REAL timeElapsed) {
	// . . .
	// ResultFrame �� ������ ����� �����Ͽ� ������ updateKey �Լ��� ������� �ʰ�, updateFrame ���� ��� ����� ����.
}

bool ResultFrame::updateFrame(REAL timeElapsed) {
	/* ������ �ð� ��� */
	rFrameElapseTime += timeElapsed;

	/* ���� ������Ʈ */
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
	}

	/* RESULT ���� ��� */
	if (enScene == ResultScene::SCENE_BEGIN && rFrameElapseTime > 1.00) {
		enScene = ResultScene::SCENE_PRINT_RESULT;
		Vector3 vec(0.5, 0.8, 0.0);
		SIZE size = { 1024, 128 };
		addWidget(new RectViewer(vec, size, texture_missionResult));
	}

	/* ���� ��� ���� */
	static REAL scoreShowTerm = rScoreShowTerm;
	if (enScene == ResultScene::SCENE_PRINT_RESULT && rFrameElapseTime > 2.50) {
		// ó�� ���� �� �� ���� ����
		if (bShowScore == false) {
			bShowScore = true;

			RectViewer *pScoreTitle = new RectViewer(Vector3(0.22, 0.67, 0.0), { 0, 0 }, 0);
			pScoreTitle->setTextAttributes("R A N K                I D               K I L L S               S C O R E", 10.0, true, 1);
			pScoreTitle->setColor3(Vector3(0.33, 1.00, 1.00));
			addWidget(pScoreTitle);
		}

		// ���� ��� �ð� ���� �� ��� �ð� ��� �� ���
		if ((scoreShowTerm -= timeElapsed) <= 0.0) {
			// ���� ��� �ð� ����
			scoreShowTerm = (rScoreShowTerm - scoreShowTerm);

			// ���ھ� ��� ( i == 0 : RANK / 1 :ID / 2 : KILLS / 3 : SCORE ) 
			Player *pPlayer = playerVector[iPlayerCount];
			Vector3 textColor;

			for (int i = 0; i < 4; ++i) {
				RectViewer *pScoreViwer = new RectViewer(vecScorePos, { 0, 0 }, 0);
				char scoreStrBuf[32];
				
				switch (i) {
				case 0: vecScorePos.x = 0.26; sprintf_s(scoreStrBuf, "%d", iPlayerCount + 1);					break;
				case 1: vecScorePos.x = 0.36; sprintf_s(scoreStrBuf, "%8s", pPlayer->getPlayerName().c_str());	break;
				case 2: vecScorePos.x = 0.51; sprintf_s(scoreStrBuf, "%3d", pPlayer->getPlayerKills());			break;
				case 3: vecScorePos.x = 0.68; sprintf_s(scoreStrBuf, "%5d", pPlayer->getPlayerScore());			break;
				}
				pScoreViwer->setPos(vecScorePos);

				switch (iPlayerCount) { // ��, ��, ��, ��� ���� ����
				case 0:		textColor = Vector3(1.0, 1.0, 0.0);		break;
				case 1:		textColor = Vector3(0.77, 0.77, 0.77);	break;
				case 2:		textColor = Vector3(0.73, 0.48, 0.34);	break;
				default:	textColor = Vector3(1.0, 1.0, 1.0);		break;

				}
				// �ڽ��� ���������� ���ϰ� ũ�⸦ ���� Ű�� ���
				REAL textSize = 9.0;
				if (pOwnerPlayer->getPlayerName() == pPlayer->getPlayerName()){
					textColor = Vector3(1.0f, 0.0f, 0.5f);
					textSize += 2.0f;
				}

				pScoreViwer->setColor3(textColor);
				pScoreViwer->setTextAttributes(string(scoreStrBuf), textSize, true, 1); // ���ھ� ���� �� ũ�� ����
				addWidget(pScoreViwer);
			}
			vecScorePos.y -= 0.065; // �� �� �Ʒ��� �̵�
			++iPlayerCount;

			if (iPlayerCount == playerVector.size()) {
				enScene = ResultScene::SCENE_PRINT_ANYKEY;
				scoreShowTerm = rScoreShowTerm;				// static�̹Ƿ� �� ���� �ʱ�ȭ�� �ȴ� -> ������ ����� ���� �ʱ�ȭ
			}
		}
	}

	// ���� ���� ����
	if (pAnykeyViewer != NULL)
		pAnykeyViewer->setTextColorAlpha(abs(sinf(rFrameElapseTime * 3.0)));

	/* ���� ��� ��, �ƹ� Ű�� �������� ���� ��� */
	if (enScene == ResultScene::SCENE_PRINT_ANYKEY) {
		// ���� ������ ���̽� �޴� ����
		if (bQuitable == false) {
			bQuitable = true;
			// ���̽� ����
			pAceViewer = new RectViewer(Vector3(0.23, 0.60, 0.0), { 80, 80 }, texture_acePlayer);
			pAceViewer->setVisible(false);
			addWidget(pAceViewer);
			// ���� ����
			pAnykeyViewer = new RectViewer(Vector3(0.03, 0.04, 0.0), {0, 0}, 0);
			pAnykeyViewer->setTextAttributes("PRESS [ENTER] TO EXIT... ", 9.0, true, 1);
			pAnykeyViewer->setColor3(Vector3(1.0, 1.0, 1.0));
			pAnykeyViewer->setVisible(false);
			addWidget(pAnykeyViewer);
		}

		// ���̽� ���� ��� �� ���� ���
		if (pAceViewer != NULL) {
			// ���� ����� ����
			if (pAceViewer->getElapsedTime() > 0.50) {
				pAceViewer->setVisible(true);
				// ���� ���
				new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/aced.wav", false, false);
				// ���� ���� ����� ����
				pAnykeyViewer->setVisible(true);
				pAceViewer = NULL;
			}
		}

		// ���� �Է� �� ���̵� �ƿ� ��� �߰�
		if (pInputManager->getKeyState(VK_RETURN) == key_state::KEY_DOWN) {
			enScene = ResultScene::SCENE_EXITING;

			Size2D *pResolution = pDrawManager->getResolutionPtr();
			SIZE size;
			size.cx = pResolution->getWidth();
			size.cy = pResolution->getHeight();
			pFadeoutViewer = new RectViewer(Vector3(0.5, 0.5, 0.0), size, 0);
			pFadeoutViewer->setColor3(Vector3(0.0, 0.0, 0.0));
			pFadeoutViewer->setColorAlpha(0.0);
			addWidget(pFadeoutViewer);
		}
	}
	

	/* ������ ���� ���� */
	if (enScene == ResultScene::SCENE_EXITING) {
		// ���̵� �ƿ� ����
		if (pFadeoutViewer != NULL) {
			pFadeoutViewer->setColorAlpha(pFadeoutViewer->getColorAlpha() + (1.0 * timeElapsed));
			pBackgroundSound->setGain(pBackgroundSound->getGain() - 0.5 * timeElapsed);
		}

		// ������ ���̵� �ƿ��� �Ϸ�� ���
		if (pFadeoutViewer->getColorAlpha() >= 1.0) {
			// �÷��̾� �Ϻ� ���� ����
			Player *pPlayer = new Player(0, pOwnerPlayer->getPlayerName());

			// ������ �ı�
			delete this;

			// ���� ������ ���� (����!: �׻� ���� �ı� �� ���� �������� ������ �־�� �Ѵ�.)
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer));

			// ��� ������ ����
			return true;
		}
	}

	/* ��ο� ����Ʈ �ʱ�ȭ �� ���� �߰� */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		drawList.push_back(*iterWidget);
	}

	/* ���� ��ȯ */
	return true;
}