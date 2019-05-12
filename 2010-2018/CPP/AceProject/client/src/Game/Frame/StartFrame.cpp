#include "Game/Frame/StartFrame.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/UI/Viewer/LogoViewer.h"
#include "Engine/GameCamera.h"
#include "Engine/GameWindow.h"

StartFrame::StartFrame(Player *pPlayer) : GameFrame(TEXT("StartFrame")) {
	// OpenGL ��� ���� ���� ����
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// �ɹ� ���� �ʱ�ȭ
	bReqChangeFrame = false;		// ������ ���� ��û ���θ� ����
	bFastSpeedFade = false;			// ���� �ӵ��� ���̵�
	iLogoState = 0;					// �ΰ� ��� ����
	rUnivLogoTime = 1.50;			// ���� ��ǰ �ΰ� ��� �ð�
	rTeamLogoTime = 1.50;			// �� �ΰ� ��� �ð�
	rFadeoutTime = 1.00;			// ���̵� �ƿ� �ð�
	rFadeinTime = 1.00;				// ���̵� �� �ð�
	rMainLogoDelayTime = 1.00;		// ���� �ΰ� ��±��� �߰����� ���� �ð�
	rLobbyFrameDelayTime = 1.00;	// �ε� �Ϸ� �� �κ� ���������� ��ȯ�ϱ� ���� ���� �ð�
	iLoadingPercent = 0;			// �ε� �� ���ҽ� ����� �ۼ�Ʈ
	hLoadingThread = 0;				// �ε� ������ �ڵ�

	// ���콺 �����
	pGameManager->getGameWndPtr()->setMouseVisible(false);

	// ���ҽ� �ε� (���⼭ �ּ����� ���ҽ��� �� �ε�)
	// StartFrame
	pResourceManager->loadTexture("res/Textures/logo/AceProjectLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// ���̽� ������Ʈ ���� �ΰ�
	pResourceManager->loadTexture("res/Textures/logo/aceProjectIcon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// ���̽� ������Ʈ ������ �ΰ�
	pResourceManager->loadTexture("res/Textures/logo/aceProjectIconBig.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// ���̽� ������Ʈ ū ������ �ΰ�
	pResourceManager->loadTexture("res/Textures/logo/skunivLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���б� �ΰ�
	pResourceManager->loadTexture("res/Textures/logo/runestoneLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �� �ΰ�
	pResourceManager->loadTexture("res/Textures/logo/logo_frame.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �ΰ� ������ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/logo/logo_ring_outter.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// �ΰ� �� (�ܺ�) �ؽ�ó
	pResourceManager->loadTexture("res/Textures/logo/logo_ring_inner.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// �ΰ� �� (����) �ؽ�ó
	pResourceManager->loadTexture("res/Textures/font/basicfont.png", true, true, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// �⺻ �ƽ�Ű�ڵ� ��Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/font/koverwatchFont.png", true, true, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// ������ġ �ƽ�Ű�ڵ� ��Ʈ �ؽ�ó
	pResourceManager->loadWavSound("res/Sounds/BGM_Overwatch_Main_Theme_Short.wav");										// ���� ������ ���� �ε�

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

	// ���� ���� ����
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 center(0.5, 0.5, 0.0), centerDown(0.5, 0.25, 0.0), rightDown(0.9125, 0.15, 0.0);

	// ��� ���� �� �ʱ�ȭ
	// ���б� �ΰ� ���
	pUnivLogoViewer = new RectViewer(center, { 384, 384 }, pResourceManager->getTextureID("res/Textures/logo/skunivLogo.png"));
	pUnivLogoViewer->setVisible(true);
	addWidget(pUnivLogoViewer);

	// ���� ��ǰ �޽��� ���
	pUnivMsgViewer = new RectViewer(centerDown, { 0, 0 }, 0);
	pUnivMsgViewer->setTextAttributes("2016 Seokyeong Univ Graduation works", 16.0, true, 1);
	pUnivMsgViewer->setVisible(true);
	pUnivMsgViewer->setTextAlignCenterX(true);
	addWidget(pUnivMsgViewer);

	// �� �ΰ� ���
	pTeamLogoViewer = new RectViewer(center, { 320, 320 }, pResourceManager->getTextureID("res/Textures/logo/runestoneLogo.png"));
	pTeamLogoViewer->setVisible(false);
	addWidget(pTeamLogoViewer);

	// �� �޽��� ���
	pTeamMsgViewer = new RectViewer(centerDown, { 0, 0 }, 0);
	pTeamMsgViewer->setTextAttributes("Team Runestone Presents", 16.0, true, 1);
	pTeamMsgViewer->setVisible(false);
	pTeamMsgViewer->setTextAlignCenterX(true);
	addWidget(pTeamMsgViewer);

	// ������Ʈ �ΰ� ���
	pAceProjectLogo = new RectViewer(center, { 1228, 205 }, pResourceManager->getTextureID("res/Textures/logo/AceProjectLogo.png"));
	pAceProjectLogo->setVisible(false);
	addWidget(pAceProjectLogo);

	// �ε� �ΰ� ���
	pLoadingLogo = new LogoViewer(rightDown, { 128, 128 });
	pLoadingLogo->setVisible(false);
	addWidget(pLoadingLogo);

	// �ε� �޽��� ���
	pLoadingMsgViewer = new RectViewer(rightDown + Vector3(0.0, -0.1, 0.0), { 0, 0 }, 0);
	pLoadingMsgViewer->setTextAttributes("Loading : 0%", 8.0, true, 1);
	pLoadingMsgViewer->setVisible(false);
	pLoadingMsgViewer->setTextAlignCenterX(true);
	addWidget(pLoadingMsgViewer);

	// ���̵�ƿ� ���
	pFadeViewer = new RectViewer(center, { scrWidth, scrHeight }, 0);
	pFadeViewer->setColor3(0.0, 0.0, 0.0);
	pFadeViewer->setColorAlpha(1.0);
	pFadeViewer->setVisible(true);
	addWidget(pFadeViewer);

	// ��� ����
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Overwatch_Main_Theme_Short.wav", false, true);
	pBackgroundSound->pause();	// ��� ���� �Ͻ� ����
	pBackgroundSound->setGain(1.0);
}

StartFrame::StartFrame(StartFrame &obj) : GameFrame(obj) {

}

StartFrame::~StartFrame() {

}

void StartFrame::loadResource() {
	// ���ҽ� �Ŵ����� %���⿡ StartFrame�� Ŭ������ ����
	pResourceManager->setResLoadedPercentPtr(&iLoadingPercent);

	// CombatFrame Resource
	Sleep(1500);	// ������ ���� ���� ��� (���� �� �����Ͽ��� ��� ����)
	pResourceManager->loadOBJ("res/Models/Fighter/FA-22_Raptor/FA-22_Raptor.obj");											// ������ �� �� �ؽ�ó
	pResourceManager->loadOBJ("res/Models/Missile/AIM120D/AIM120D.obj");													// �̻��� �� �� �ؽ�ó
	Sleep(1500);	// ������ ���� ���� ��� (���� �� �����Ͽ��� ��� ����)
	pResourceManager->loadTexture("res/Textures/map/terrainTexture.png", true, true, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/skydome.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// �ϴ� �� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkyTop.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// �ϴ� ���� ��� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkyBot.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// �ϴ� ���� �ϴ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkyEast.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// �ϴ� ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkyWest.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// �ϴ� ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkySouth.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// �ϴ� ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/sky/SkyNorth.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// �ϴ� ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/map/seaTexture.jpg", false, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �ٴ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/screen/dustGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ����Ʈ �۷ξ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/screen/warningGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// ��� �۷ξ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/screen/boostGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �ν��� �۷ξ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/screen/Help.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/screen/Exit.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���� Ȯ�� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/radarLockon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���̴� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/radarArrow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���̴� ���ο� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/uiRect.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// UI�� ���Ǵ� �׸� Ʋ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/uiRectWide.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// UI�� ���Ǵ� ���η� ���� �׸� Ʋ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/mapGrid.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// �̴ϸʿ� ���Ǵ� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/mapJet.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// �̴ϸʿ� ���Ǵ� ������ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/mapMissile.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �̴ϸʿ� ���Ǵ� �̻��� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/radarMapRad.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �̴ϸʿ� ���Ǵ� ���̴� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/weaponLockon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� ���¿� ���Ǵ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/gatlingAim.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ��Ʋ���� ���ӿ� ���Ǵ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/jetUI.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// ���� �ý��� UI �� ���Ǵ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/missileUI.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���� �ý��� UI �� ���Ǵ� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/sys.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);					// �ý��� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/missionStartMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �̼� ���� �� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/missionCompleteMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// �̼� ���� �� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/sysHit.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// �� �ǰ� �� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/sysDestroy.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �� �ı� �� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/serverDisconnectedMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);// ���� ��Ŀ��Ƽ�� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/msslAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �̻��� �溸 UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/offCourseAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �ڽ� ��Ż �溸 UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/lockonAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ���� �溸 UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/pullUpWarning.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// �� �溸 UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/stallWarning.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� �溸 UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/breakAlarm.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �극��ũ UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/ui/boosting.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// �ν��� UI �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/basicSmoke.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� ȿ���� ���Ǵ� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/missileTrail.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// �̻��� ���� ȿ���� ���Ǵ� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/gunfire.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ��Ʋ���� �Ѿ� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/jetBlow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ������ �����͹��� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/glow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ������ ȿ���� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/explosionA.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� ȿ���� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/explosionB.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���� ȿ���� ����Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/effect/explosionCircleA.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);// ���� ȿ���� ����Ʈ �ؽ�ó	
	Sleep(1800);	// ������ ���� ���� ��� (���� �� �����Ͽ��� ��� ����)
	pResourceManager->loadWavSound("res/Sounds/beepWarning.wav");			// �˸� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/gunFire.wav");				// ���� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/msslFire.wav");				// �̻��� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/msslExplosion.wav");			// �̻��� ���� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/jetExplosion.wav");			// ������ ���� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/msslWarning.wav");			// �̻��� ��� ����
	pResourceManager->loadWavSound("res/Sounds/lockonWarning.wav");			// ���� ��� ����
	pResourceManager->loadWavSound("res/Sounds/jetEngine.wav");				// ������ ���� ����
	pResourceManager->loadWavSound("res/Sounds/missionAlarm.wav");			// �̼� �˸� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/booster.wav");				// �ν��� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/BGM_Alect_Squadron.wav");	// ��� ����
	// LobbyFrame Resource
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyBackground.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// ��� �׶���Ʈ �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyWorldmap.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ��� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyGrid.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ��� �׸��� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/singlePractice.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// ��� �̱� ����Ƽ�� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/multiplayCombat.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);	// ��� ��Ƽ�÷��� �ĺ� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/playerSetting.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// ��� �÷��̾� ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/credit.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// ��� ũ���� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/exitGame.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// ��� ���� ������ ���� �ؽ�ó
	pResourceManager->loadWavSound("res/Sounds/selectHorn.wav");																		// ���� ���� �ε�
	pResourceManager->loadWavSound("res/Sounds/BGM_Skies_of_Deception.wav");															// ��� ���� �ε�
	Sleep(1900);	// ������ ���� ���� ��� (���� �� �����Ͽ��� ��� ����)
	// Multiplay Lobby Resource
	pResourceManager->loadWavSound("res/Sounds/BGM_BattleField4_MainTheme.wav");	// ��� ����
	// ResultFrame Resource
	pResourceManager->loadTexture("res/Textures/frame/resultFrame/result.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// ��� ���� �ؽ�ó
	pResourceManager->loadTexture("res/Textures/frame/resultFrame/acePlayer.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���̽� �÷��̾� �ؽ�ó
	pResourceManager->loadWavSound("res/Sounds/aced.wav");							// ���̽� ����
	pResourceManager->loadWavSound("res/Sounds/BGM_Overwatch_Score_Screen.wav");	// ��� ����
	Sleep(1000);	// ������ ���� ���� ��� (���� �� �����Ͽ��� ��� ����)
	// CreditFrame Resource
	pResourceManager->loadTexture("res/Textures/logo/3dsmaxLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 3dsMAX Logo
	pResourceManager->loadTexture("res/Textures/logo/gamedevLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// gamedev.net Logo
	pResourceManager->loadTexture("res/Textures/logo/neheLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// nehe.gamedev.net Logo
	pResourceManager->loadTexture("res/Textures/logo/stackoverLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// stackoverflow.com Logo
	pResourceManager->loadTexture("res/Textures/logo/gimpLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// Gimp Logo
	pResourceManager->loadTexture("res/Textures/logo/openalLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// OpenAL Logo
	pResourceManager->loadTexture("res/Textures/logo/opencvLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// OpenCV Logo
	pResourceManager->loadTexture("res/Textures/logo/openglLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// OpenGL Logo
	pResourceManager->loadTexture("res/Textures/logo/visualstudio2013Logo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// VisualStudio 2013 Logo
	pResourceManager->loadTexture("res/Textures/logo/wavosaurLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// WavosaurLogo
	pResourceManager->loadTexture("res/Textures/logo/dphowareLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// dphoware Logo
	pResourceManager->loadTexture("res/Textures/logo/devilLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// DevIL Logo
	pResourceManager->loadTexture("res/Textures/logo/glmLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// glm Logo
	pResourceManager->loadTexture("res/Textures/screen/cinematicGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// ���׸�ƽ �۷ξ�
	pResourceManager->loadWavSound("res/Sounds/BGM_The_Wight_to_Remain.wav");	// ��� ����

	// GameWindow Ŭ������ �����尡 �̹��� �ε��� �Ϸ�� HGLRC�� ������ HGLRC�� ���� ���� ������ ���ؽ�Ʈ�� �����ϵ��� ��
	// ����: �������� HGLRC ���� ������ ����Ʈ���� ���� �������� HGLRC�� ���뿡 ����쵵�� ���������� ������ �� ����.
	// �� ������ �����ϸ� ������ HGLRC�� �ؽ�ó���� �ε�Ǿ����� ������ Ȯ���� �� �ִ�.
	wglShareLists(*g_pGameManager->getGameWndPtr()->getHGLRC_ThreadPtr(), *g_pGameManager->getGameWndPtr()->getHGLRCPtr());

	// ���ҽ� �ε� �Ϸ�� ���� (�ε� �ϷḦ 1000% �� �����Ͽ� Ȥ�� �� ���� ������ ����� ���� ����)
	iLoadingPercent = 1000;
}

DWORD WINAPI StartFrame::thread_loadResFromThread(void *p) {
	StartFrame *pFrame = (StartFrame*)p;

	// HGLRC �� �� �� �����, ���� �������Ѽ� ��׶��� �ε��� �õ��ϴ� ���.
	// ����: http://www.gpgstudy.com/forum/viewtopic.php?t=1165&highlight=%BE%B2%B7%B9%B5%E5

	// �����忡�� ����� HGLRC�� GameWindow�� ����� ������� ThreadRC�� ����
	wglMakeCurrent(*g_pGameManager->getGameWndPtr()->getHDCPtr(), *g_pGameManager->getGameWndPtr()->getHGLRC_ThreadPtr());

	// ���ҽ� �ε� ��� �Լ� ȣ��
	if (pFrame == NULL) throw ("Fail to start Thread.");
	else pFrame->loadResource();

	return 0;
}

void StartFrame::updateState(REAL timeElapsed) {
	// ���б� �ΰ� ���̵� ��
	if (iLogoState == 0) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() - (rFadeoutTime * timeElapsed)); // ���̵� ��
		rUnivLogoTime -= timeElapsed;

		if (rUnivLogoTime < 0.00) ++iLogoState;
	}
	// ���б� �ΰ� ���̵� �ƿ�
	else if (iLogoState == 1) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() + (rFadeoutTime * timeElapsed)); // ���̵� �ƿ�

		// ���̵� �ƿ� �Ϸ� ��
		if (pFadeViewer->getColorAlpha() > 1.00) {
			++iLogoState;
			pUnivLogoViewer->setVisible(false);	// �ΰ� ����� �� ���̱� ��ȯ
			pUnivMsgViewer->setVisible(false);
			pTeamLogoViewer->setVisible(true);
			pTeamMsgViewer->setVisible(true);
		}
	}
	// �� �ΰ� ���̵� ��
	else if (iLogoState == 2) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() - (rFadeoutTime * timeElapsed)); // ���̵� ��
		rTeamLogoTime -= timeElapsed;

		if (rTeamLogoTime < 0.00) ++iLogoState;
	}
	// �� �ΰ� ���̵� �ƿ�
	else if (iLogoState == 3) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() + (rFadeoutTime * timeElapsed)); // ���̵� �ƿ�

		// ���̵� �ƿ� �Ϸ� ��
		if (pFadeViewer->getColorAlpha() > 1.00) {
			rMainLogoDelayTime -= timeElapsed;	// ���� �ΰ� ��� ���� �ð� ����
			
			// ���� �ΰ� ��� ���� �ð��� �� ���
			if (rMainLogoDelayTime < 0.00) {
				++iLogoState;						// ���� �ΰ� ���·� ��ȯ
				pTeamLogoViewer->setVisible(false);	// �ΰ� ����� �� ���̱� ��ȯ
				pTeamMsgViewer->setVisible(false);
				pAceProjectLogo->setVisible(true);
				pLoadingLogo->setVisible(true);
				pLoadingMsgViewer->setVisible(true);
				pFadeViewer->setColorAlpha(0.0);	// ���̵� �ƿ� �� ��� ���̵� ��
				
				// ������ ���� �� ���ҽ� �ε� ����
				LPDWORD threadID = 0;
				hLoadingThread = CreateThread(NULL, 0, &thread_loadResFromThread, this, 0, threadID);
				
				// ��� ���� ���
				if (pBackgroundSound != NULL) pBackgroundSound->play();
			}
		}
	}
	// ���� �ΰ� ��� �� �ε� ����ȭ
	else if (iLogoState == 4) {
		// ���ҽ� �ε� �Ϸ� ��
		if (iLoadingPercent >= 1000) {
			++iLogoState;
		}
		else {
			// �ε� % ����ȭ
			static char strBuf[16];
			if (iLoadingPercent >= 100) {
				pLoadingMsgViewer->setText("Loading Done");
			}
			else {
				sprintf_s(strBuf, "Loading : %d%%", iLoadingPercent);
				pLoadingMsgViewer->setText(strBuf);
			}
		}
	}
	// ���� �ΰ� �̵�
	else if (iLogoState == 5) {
		// ���
		Vector3 logoPos = pAceProjectLogo->getPos();										// ���� ���� �ΰ� ��ġ
		SIZE logoSize = pAceProjectLogo->getSize();											// ���� ���� �ΰ� ������
		static const Vector3 newLogoPos(0.8, 0.275, 0.0);									// ���� ������ ���� �ΰ� ��ġ
		static const SIZE newLogoSize = { 640, 106 };										// ���� ������ ���� �ΰ� ������
		static const SIZE logoSizeDec = { (logoSize.cx - newLogoSize.cx) * timeElapsed,
										  (logoSize.cy - newLogoSize.cy) * timeElapsed };	// �� ������Ʈ���� ���� �ΰ� �پ���� �ϴ� ũ��
		static const REAL logoDist = (logoPos - newLogoPos).magnitude();					// �ʱ� ���� �ΰ� ��ġ�� �� ��ġ ������ �Ÿ�	
		REAL moveDist = (logoDist * timeElapsed);											// �� ������Ʈ���� ���� �ΰ� �̵��ؾ� �ϴ� �Ÿ�

		// �ε� �ΰ�� �޽��� ���� ���̵� �ƿ�
		REAL logoAlpha = pLoadingLogo->getColorAlpha() - (1.0 * timeElapsed);
		if (logoAlpha < 0.00) logoAlpha = 0.00;
		pLoadingLogo->setColorAlpha(logoAlpha);
		pLoadingMsgViewer->setTextColorAlpha(logoAlpha);

		// ȭ�� ��ȯ�� ���� ���� �ΰ� ��ġ �̵�
		bool bLogoPosComplete = false;				// ���� �ΰ� �̵��ؾ� �� ��ġ�� ��ġ��
		Vector3 logoDir = (newLogoPos - logoPos);	// ���� �ΰ� �̵��ؾ� �� ���⺤��
		logoDir.normalize();
		if ((logoPos - newLogoPos).magnitude() > moveDist) {
			pAceProjectLogo->setPos(logoPos + (logoDir * moveDist));
		}
		else {
			bLogoPosComplete = true;
			pAceProjectLogo->setPos(newLogoPos);
		}

		// ���� �ΰ� ũ�� ����
		bool bLogoSizeComplete = false;				// ���� �ΰ� ���ؾ� �� ũ�⿡ ������
		if (logoSize.cx > newLogoSize.cx && logoSize.cy > newLogoSize.cy) {
			pAceProjectLogo->setSize(logoSize.cx - logoSizeDec.cx, logoSize.cy - logoSizeDec.cy);
		}
		else {
			bLogoSizeComplete = true;
			pAceProjectLogo->setSize(newLogoSize.cx, newLogoSize.cy);
		}

		// ���� �ΰ� �̵� �� ũ�� ���� �Ϸ� �˻�
		if (bLogoPosComplete == true && bLogoSizeComplete == true) {
			++iLogoState;
		}
	}
	// StartFrame �ı� �� LobbyFrame�� ��ȯ�� �� �ֵ��� ����ġ ��� ���� �̿�
	else if (iLogoState == 6) {
		// ���� �ð� ���ҽ�Ŵ
		if ((rLobbyFrameDelayTime -= timeElapsed) < 0.00) {
			// ��� ���� �Ұ�
			if (pBackgroundSound != NULL) {
				pBackgroundSound->setGain(0.00);
				pBackgroundSound->stop();
			}
			// ���� ���������� �Ѿ���� ����ġ ����
			bReqChangeFrame = true;
		}
		else {
			// ��� ���� ���� ����
			if (pBackgroundSound != NULL) {
				REAL newGain = (pBackgroundSound->getGain() - timeElapsed);
				if (newGain < 0.00) newGain = 0.00;
				pBackgroundSound->setGain(newGain);
			}
		}
	}
	// �� �� ����ó��
	else {
		// ...
	}
	
}

void StartFrame::updateKey(REAL timeElapsed) {
	// �ΰ� ������Ʈ�� 3 ���� ��(�ε� ��) ���͸� ���� �� ���� �ӵ��� �귯��
	if (iLogoState < 4) {
		if (pInputManager->getKeyState(VK_RETURN) == key_state::KEY_DOWNHOLDING) {
			bFastSpeedFade = true;
		}
		else {
			bFastSpeedFade = false;
		}
	}
	else {
		bFastSpeedFade = false;
	}
}

bool StartFrame::updateFrame(REAL timeElapsed) {
	// ��ο� ����Ʈ Ŭ����
	drawList.clear();

	// Ű ������Ʈ
	updateKey(timeElapsed);

	// ���� Ű ���ο� ���� ���̵� �ӵ� ����
	if (bFastSpeedFade == true) timeElapsed *= 3.0;

	// �������ͽ��� ���� ������Ʈ
	updateState(timeElapsed);

	// ������ Ż�� ���� ����
	if (bReqChangeFrame == true) {
		// ���� ������ �ı�
		delete this;

		// �� ������ ����
		g_pGameManager->setGameFrame(new LobbyFrame(NULL, true)); // LobbyFrame ù ��° ����

		// ������ ����
		return true;
	}

	// ���� ������Ʈ �� ��ο� ����Ʈ�� �߰�
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
		drawList.push_back(*iterWidget);
	}

	return true;
}

