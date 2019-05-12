#include "Game/Frame/CreditsFrame.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Frame/LobbyFrame.h"
#include "Library/DataParser.h"

CreditsFrame::CreditsFrame(Player *pPlayer) : GameFrame(TEXT("CreditsFrame")) {
	// OpenGL ��� ���� ���� ����
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// ������ �÷��̾� ����
	pOwnerPlayer = pPlayer;

	// ��� ���� �ʱ�ȭ
	bReqExit = false;
	bPuase = false;
	rExitDelayTime = 1.00;
	rSpeedMulti = 1.00;
	listScrollViewers.clear();

	// ���� ���� ����
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 scrCenter(0.5, 0.5, 0.0);

	// ��� ��ü�� ���� �� ���Ϳ� Ǫ��
	loadViewerFromDatFile("dat/Credits.dat");

	// ���׸�ƽ ������
	pCinematicFrameViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/cinematicGlower.png"));
	pCinematicFrameViewer->setColor3(0.0, 0.0, 0.0);
	pCinematicFrameViewer->setColorAlpha(0.99);
	addWidget(pCinematicFrameViewer);

	// ������ ���� ���� ���
	pHelpViewer = new RectViewer(Vector3(0.01, 0.03, 0.00), { 0, 0 }, 0);
	pHelpViewer->setTextAttributes("[ESC]: Back to Lobby   [<- / ->]: Speed   [Space]: Pause", 5.5, true, 1);
	addWidget(pHelpViewer);
	
	// �ӵ� ��� ���
	pSpeedMultiViewer = new RectViewer(Vector3(0.01, 0.06, 0.00), { 0, 0 }, 0);
	pSpeedMultiViewer->setTextAttributes("Speed: x1", 6.5, true, 1);
	addWidget(pSpeedMultiViewer);
	
	// ���̵�ƿ� ���
	pFadeoutViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, 0);
	pFadeoutViewer->setColor3(0.0, 0.0, 0.0);
	pFadeoutViewer->setColorAlpha(0.0);
	addWidget(pFadeoutViewer);

	// ���� ���
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_The_Wight_to_Remain.wav", false, true);
	pBackgroundSound->setGain(0.5);
	pBackgroundSound->play();
}

CreditsFrame::CreditsFrame(CreditsFrame &obj) : GameFrame(obj) {

}

CreditsFrame::~CreditsFrame() {

}

void CreditsFrame::loadViewerFromDatFile(char *fileName) {
	// ���� ���� �ʱ�ȭ
	int i = 0;							// for ������ ���� ���� ����
	const Vector3 pos(0.5, 0.0, 0.0);	// ������ ����� �⺻ ��ġ
	REAL rLastYpos = 0.0f;				// ���������� ������ ����� Y��ġ (���� ��ġ �������� ����ϱ� ����)
	REAL rColor4[4] = { 1.0f, };		// RGBA ������ �ӽ� �����ϱ� ���� ����
	char *pStr = NULL;					// �ļ����� ��ȯ�� ���ڿ� �����͸� �ӽ÷� ����ų ������ (���� �Ҵ� ������ ����)
	DataParser parser;					// *.dat ���� �ļ�
	
	// �ļ� ũ���� ������ ���� ����
	if (parser.openFile(fileName) == false) {
		MessageBox(NULL, TEXT("CreditsFrame: Fail to open file 'dat/Credits.dat'!"), TEXT("CreditsFrame::loadViewerFromDatFile() File open error"), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// �����͸� �Ľ��Ͽ� ���� ���� ����
	while (parser.findHeader("Viewer") == true) {
		// �ʱ�ȭ
		for (i = 0; i < 4; ++i) rColor4[i] = 1.0f;

		// ��� ����� ã�� ��� ���� ����
		RectViewer *pViewer = new RectViewer(pos, { 0, 0 }, 0);
		
		// ���� ���� ���� �˻�
		if (pViewer != NULL) {
			// �Ӽ��� �Ľ�
			if (parser.findAttribute("PosX") == true) {
				pViewer->setPosX(parser.parseFloatFromAttribute());
			}
			if (parser.findAttribute("OffsetY") == true) {
				rLastYpos -= parser.parseFloatFromAttribute();
				pViewer->setPosY(rLastYpos);
			}
			if (parser.findAttribute("ImgName") == true) {
				pStr = parser.parseStringFromAttribute();
				if (pStr != NULL) {
					pViewer->setTextureID(pResourceManager->getTextureID(pStr));
					delete pStr;
				}
				pStr = NULL;
			}
			if (parser.findAttribute("ImgX") == true) {
				pViewer->setSize(parser.parseIntFromAttribute(), pViewer->getSize().cy);
			}
			if (parser.findAttribute("ImgY") == true) {
				pViewer->setSize(pViewer->getSize().cx, parser.parseIntFromAttribute());
			}
			if (parser.findAttribute("Text") == true) {
				pStr = parser.parseStringFromAttribute();
				if (pStr != NULL) {
					pViewer->setText(pStr);
					delete pStr;
				}
				pStr = NULL;
			}
			if (parser.findAttribute("FontID") == true) {
				pViewer->setFontID(parser.parseIntFromAttribute());
			}
			if (parser.findAttribute("FontSz") == true) {
				pViewer->setTextSize(parser.parseFloatFromAttribute());
			}
			if (parser.findAttribute("ColorR") == true) {
				rColor4[0] = parser.parseFloatFromAttribute();
			}
			if (parser.findAttribute("ColorG") == true) {
				rColor4[1] = parser.parseFloatFromAttribute();
			}
			if (parser.findAttribute("ColorB") == true) {
				rColor4[2] = parser.parseFloatFromAttribute();
			}
			if (parser.findAttribute("ColorA") == true) {
				rColor4[3] = parser.parseFloatFromAttribute();
			}
			pViewer->setColor3(rColor4[0], rColor4[1], rColor4[2]);	// ���� �����ʹ� �ӽ� ������ �ξ��ٰ� �ϰ� ����
			pViewer->setColorAlpha(rColor4[3]);
			pViewer->setTextColorAlpha(rColor4[3]);
			if (parser.findAttribute("ACenter") == true) {
				pViewer->setTextAlignCenterX((parser.parseIntFromAttribute() == 1 ? true : false));
			}
			if (parser.findAttribute("FuncID") == true) {
				pViewer->setTagInt(parser.parseIntFromAttribute());
			}

			// ���� �߰�
			addWidget(pViewer);						// ������ ���� ����Ʈ�� �߰�
			listScrollViewers.push_back(pViewer);	// ������ ��ũ�� ����Ʈ�� �߰�
			pViewer = NULL;
		}
		// ���� ���� ����
		else {
			// ������ �Ľ� �� �ʱ�ȭ ����
		}
	}
}

void CreditsFrame::changeSpeedMulti(bool speedUp) {
	if (speedUp == true) {
		if (rSpeedMulti == 1.00f) {
			rSpeedMulti = 2.0f;
		}
		else {
			rSpeedMulti = 4.00f;
		}
	}
	else {
		if (rSpeedMulti == 4.00f) {
			rSpeedMulti = 2.00f;
		}
		else {
			rSpeedMulti = 1.00f;
		}
	}
}

void CreditsFrame::updateLogic(REAL timeElapsedMulti) {
	// �Ͻ����� ���°� �ƴ� ���
	if (bPuase == false) {
		// ��� ��ũ�Ѹ� �ӵ�
		Vector3 scrollSpd(0.0, 0.15, 0.0);
		scrollSpd *= timeElapsedMulti;

		// ��� ��� ǥ�� ������Ʈ
		char strBuf[11];
		sprintf_s(strBuf, "Speed: x%d", (int)rSpeedMulti);
		pSpeedMultiViewer->setText(strBuf);

		// ��� ���� �ӵ� ������Ʈ
		pBackgroundSound->setPitch(this->rSpeedMulti);

		// ������ ���� ��ũ���ϰ� Ư�� �±׹�ȣ�� �� ������Ʈ
		for (list<RectViewer*>::iterator iter = listScrollViewers.begin(); iter != listScrollViewers.end();) {
			RectViewer *pViewer = (*iter);
			
			if (pViewer != NULL) {
				// Tag: 1 ���� ����
				if (pViewer->getTagInt() == 1) {
					DWORD dwVar = ((DWORD)timeGetTime() * timeElapsedMulti);
					pViewer->setColor3(abs(sinf(dwVar * 1.33)), abs(cos(dwVar)), abs(sinf(dwVar)));
				}

				// ��� ��ũ��
				pViewer->setPos(pViewer->getPos() + scrollSpd);

				// �ʹ� �Ʒ� ��ġ�ϸ� ������ �׸��� �۾� ����
				if (pViewer->getPos().y < -1.5) {
					pViewer->setVisible(false);
					++iter;
				}
				// �ʹ� ���� ��ġ�ϸ� ���� �׸��� �۾� �����ϰ�, ��ũ�� ����Ʈ���� ����
				else if (pViewer->getPos().y > 1.5) {
					pViewer->setVisible(false);
					iter = listScrollViewers.erase(iter);
				}
				// �� ���� ��� ������ �׸���, ���� ������ ���� �� �ֵ��� ��
				else {
					pViewer->setVisible(true);
					++iter;
				}
			}
		}
	}
	// �Ͻ����� ������ ���
	else {
		pSpeedMultiViewer->setText("Paused");
	}

	// ��ũ�� ���Ϳ� ���� ������ ������ ũ���� ���� ����
	if (listScrollViewers.size() <= 0) {
		bReqExit = true;
	}
}

void CreditsFrame::updateKey(REAL timeElapsed) {
	if (pInputManager->getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) {
		bReqExit = true;
	}
	if (pInputManager->getKeyState(VK_SPACE) == key_state::KEY_DOWN) {
		// �Ͻ����� ����ġ ��ȯ
		bPuase = !bPuase;

		// ��� ���� �Ͻ����� �� �ٽ� ���
		if (bPuase == false) pBackgroundSound->play();
		else pBackgroundSound->pause();
	}
	if (pInputManager->getKeyState(VK_RIGHT) == key_state::KEY_DOWN) {
		changeSpeedMulti(true);
	}
	if (pInputManager->getKeyState(VK_LEFT) == key_state::KEY_DOWN) {
		changeSpeedMulti(false);
	}
}

bool CreditsFrame::updateFrame(REAL timeElapsed) {
	// �ð� ��� ����
	REAL timeElapsedMulti = (timeElapsed * rSpeedMulti);

	// ��ο� ����Ʈ �ʱ�ȭ
	drawList.clear();

	// Ű ������Ʈ
	updateKey(timeElapsed);

	// ���� ������Ʈ
	updateLogic(timeElapsedMulti);

	// ������ ���� ���� Ȯ��
	if (bReqExit == true) {
		// ������ ���� ���� �ð��� ������ ���
		if ((rExitDelayTime -= timeElapsed) < 0.00) {
			// ���� �Ҹ� ��
			pBackgroundSound->setGain(0.0);

			// �÷��̾� ���� ī�� �� ���� �÷��̾� ���� �������� ����
			Player *pPlayer = new Player(pOwnerPlayer->getPlayerNumber(), pOwnerPlayer->getPlayerName());
			delete pOwnerPlayer;

			// ���� ������ �ı�
			delete this;

			// �� ������ ����
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer)); // LobbyFrame �� ���� ���������� ����

			// ������ ����
			return true;
		}
		// ������ ������� ���� �ð��� ���� ���
		else {
			REAL newGain = pBackgroundSound->getGain() - timeElapsed;
			REAL newAlpha = pFadeoutViewer->getColorAlpha() + timeElapsed;
			
			// ���� ����
			if (newGain < 0.00) newGain = 0.00;
			if (newAlpha > 1.00) newAlpha = 1.00;

			// ���� ����
			pBackgroundSound->setGain(newGain);

			// ȭ�� ���̵� �ƿ�
			pFadeoutViewer->setColorAlpha(newAlpha);
		}
	}

	// ���� ������Ʈ �� ��ο� ����Ʈ�� �߰�
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsedMulti);
		drawList.push_back(*iterWidget);
	}

	return true;
}