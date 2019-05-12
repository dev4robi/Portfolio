#include "Game/Frame/StartFrame.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/UI/Viewer/LogoViewer.h"
#include "Engine/GameCamera.h"
#include "Engine/GameWindow.h"

StartFrame::StartFrame(Player *pPlayer) : GameFrame(TEXT("StartFrame")) {
	// OpenGL 배경 색상 버퍼 설정
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// 맴버 변수 초기화
	bReqChangeFrame = false;		// 프레임 변경 요청 여부를 저장
	bFastSpeedFade = false;			// 빠른 속도로 페이드
	iLogoState = 0;					// 로고 출력 상태
	rUnivLogoTime = 1.50;			// 졸업 작품 로고 출력 시간
	rTeamLogoTime = 1.50;			// 팀 로고 출력 시간
	rFadeoutTime = 1.00;			// 페이드 아웃 시간
	rFadeinTime = 1.00;				// 페이드 인 시간
	rMainLogoDelayTime = 1.00;		// 메인 로고 출력까지 추가적인 지연 시간
	rLobbyFrameDelayTime = 1.00;	// 로딩 완료 후 로비 프레임으로 전환하기 까지 지연 시간
	iLoadingPercent = 0;			// 로딩 된 리소스 백분율 퍼센트
	hLoadingThread = 0;				// 로딩 쓰레드 핸들

	// 마우스 숨기기
	pGameManager->getGameWndPtr()->setMouseVisible(false);

	// 리소스 로드 (여기서 최소한의 리소스를 선 로드)
	// StartFrame
	pResourceManager->loadTexture("res/Textures/logo/AceProjectLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 에이스 프로젝트 메인 로고
	pResourceManager->loadTexture("res/Textures/logo/aceProjectIcon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 에이스 프로젝트 아이콘 로고
	pResourceManager->loadTexture("res/Textures/logo/aceProjectIconBig.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 에이스 프로젝트 큰 아이콘 로고
	pResourceManager->loadTexture("res/Textures/logo/skunivLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 대학교 로고
	pResourceManager->loadTexture("res/Textures/logo/runestoneLogo.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 팀 로고
	pResourceManager->loadTexture("res/Textures/logo/logo_frame.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 로고 프레임 텍스처
	pResourceManager->loadTexture("res/Textures/logo/logo_ring_outter.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 로고 링 (외부) 텍스처
	pResourceManager->loadTexture("res/Textures/logo/logo_ring_inner.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 로고 링 (내부) 텍스처
	pResourceManager->loadTexture("res/Textures/font/basicfont.png", true, true, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// 기본 아스키코드 폰트 텍스처
	pResourceManager->loadTexture("res/Textures/font/koverwatchFont.png", true, true, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// 오버워치 아스키코드 폰트 텍스처
	pResourceManager->loadWavSound("res/Sounds/BGM_Overwatch_Main_Theme_Short.wav");										// 메인 프레임 사운드 로드

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

	// 지역 변수 선언
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 center(0.5, 0.5, 0.0), centerDown(0.5, 0.25, 0.0), rightDown(0.9125, 0.15, 0.0);

	// 뷰어 생성 및 초기화
	// 대학교 로고 뷰어
	pUnivLogoViewer = new RectViewer(center, { 384, 384 }, pResourceManager->getTextureID("res/Textures/logo/skunivLogo.png"));
	pUnivLogoViewer->setVisible(true);
	addWidget(pUnivLogoViewer);

	// 졸업 작품 메시지 뷰어
	pUnivMsgViewer = new RectViewer(centerDown, { 0, 0 }, 0);
	pUnivMsgViewer->setTextAttributes("2016 Seokyeong Univ Graduation works", 16.0, true, 1);
	pUnivMsgViewer->setVisible(true);
	pUnivMsgViewer->setTextAlignCenterX(true);
	addWidget(pUnivMsgViewer);

	// 팀 로고 뷰어
	pTeamLogoViewer = new RectViewer(center, { 320, 320 }, pResourceManager->getTextureID("res/Textures/logo/runestoneLogo.png"));
	pTeamLogoViewer->setVisible(false);
	addWidget(pTeamLogoViewer);

	// 팀 메시지 뷰어
	pTeamMsgViewer = new RectViewer(centerDown, { 0, 0 }, 0);
	pTeamMsgViewer->setTextAttributes("Team Runestone Presents", 16.0, true, 1);
	pTeamMsgViewer->setVisible(false);
	pTeamMsgViewer->setTextAlignCenterX(true);
	addWidget(pTeamMsgViewer);

	// 프로젝트 로고 뷰어
	pAceProjectLogo = new RectViewer(center, { 1228, 205 }, pResourceManager->getTextureID("res/Textures/logo/AceProjectLogo.png"));
	pAceProjectLogo->setVisible(false);
	addWidget(pAceProjectLogo);

	// 로딩 로고 뷰어
	pLoadingLogo = new LogoViewer(rightDown, { 128, 128 });
	pLoadingLogo->setVisible(false);
	addWidget(pLoadingLogo);

	// 로딩 메시지 뷰어
	pLoadingMsgViewer = new RectViewer(rightDown + Vector3(0.0, -0.1, 0.0), { 0, 0 }, 0);
	pLoadingMsgViewer->setTextAttributes("Loading : 0%", 8.0, true, 1);
	pLoadingMsgViewer->setVisible(false);
	pLoadingMsgViewer->setTextAlignCenterX(true);
	addWidget(pLoadingMsgViewer);

	// 페이드아웃 뷰어
	pFadeViewer = new RectViewer(center, { scrWidth, scrHeight }, 0);
	pFadeViewer->setColor3(0.0, 0.0, 0.0);
	pFadeViewer->setColorAlpha(1.0);
	pFadeViewer->setVisible(true);
	addWidget(pFadeViewer);

	// 배경 사운드
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Overwatch_Main_Theme_Short.wav", false, true);
	pBackgroundSound->pause();	// 배경 사운드 일시 정지
	pBackgroundSound->setGain(1.0);
}

StartFrame::StartFrame(StartFrame &obj) : GameFrame(obj) {

}

StartFrame::~StartFrame() {

}

void StartFrame::loadResource() {
	// 리소스 매니저의 %계산기에 StartFrame의 클래스를 연동
	pResourceManager->setResLoadedPercentPtr(&iLoadingPercent);

	// CombatFrame Resource
	Sleep(1500);	// 연출을 위한 강제 대기 (부하 시 제거하여도 상관 없음)
	pResourceManager->loadOBJ("res/Models/Fighter/FA-22_Raptor/FA-22_Raptor.obj");											// 전투기 모델 및 텍스처
	pResourceManager->loadOBJ("res/Models/Missile/AIM120D/AIM120D.obj");													// 미사일 모델 및 텍스처
	Sleep(1500);	// 연출을 위한 강제 대기 (부하 시 제거하여도 상관 없음)
	pResourceManager->loadTexture("res/Textures/map/terrainTexture.png", true, true, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 지형 텍스처
	pResourceManager->loadTexture("res/Textures/sky/skydome.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 하늘 돔 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkyTop.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 하늘 상자 상단 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkyBot.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 하늘 상자 하단 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkyEast.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 하늘 상자 동쪽 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkyWest.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 하늘 상자 서쪽 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkySouth.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// 하늘 상자 남쪽 텍스처
	pResourceManager->loadTexture("res/Textures/sky/SkyNorth.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// 하늘 상자 북쪽 텍스처
	pResourceManager->loadTexture("res/Textures/map/seaTexture.jpg", false, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 바다 텍스처
	pResourceManager->loadTexture("res/Textures/screen/dustGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 더스트 글로어 텍스처
	pResourceManager->loadTexture("res/Textures/screen/warningGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 경고 글로어 텍스처
	pResourceManager->loadTexture("res/Textures/screen/boostGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 부스터 글로어 텍스처
	pResourceManager->loadTexture("res/Textures/screen/Help.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 도움말 텍스처
	pResourceManager->loadTexture("res/Textures/screen/Exit.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 종료 확인 텍스처
	pResourceManager->loadTexture("res/Textures/ui/radarLockon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 레이더 락온 텍스처
	pResourceManager->loadTexture("res/Textures/ui/radarArrow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 레이더 에로우 텍스처
	pResourceManager->loadTexture("res/Textures/ui/uiRect.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// UI에 사용되는 네모 틀 텍스처
	pResourceManager->loadTexture("res/Textures/ui/uiRectWide.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// UI에 사용되는 가로로 넓은 네모 틀 텍스처
	pResourceManager->loadTexture("res/Textures/ui/mapGrid.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// 미니맵에 사용되는 격자 텍스처
	pResourceManager->loadTexture("res/Textures/ui/mapJet.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// 미니맵에 사용되는 전투기 텍스처
	pResourceManager->loadTexture("res/Textures/ui/mapMissile.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 미니맵에 사용되는 미사일 텍스처
	pResourceManager->loadTexture("res/Textures/ui/radarMapRad.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 미니맵에 사용되는 레이더 텍스처
	pResourceManager->loadTexture("res/Textures/ui/weaponLockon.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 무기 락온에 사용되는 텍스처
	pResourceManager->loadTexture("res/Textures/ui/gatlingAim.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 게틀링건 에임에 사용되는 텍스처
	pResourceManager->loadTexture("res/Textures/ui/jetUI.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// 상태 시스템 UI 에 사용되는 텍스처
	pResourceManager->loadTexture("res/Textures/ui/missileUI.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 상태 시스템 UI 에 사용되는 텍스처
	pResourceManager->loadTexture("res/Textures/ui/sys.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);					// 시스템 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/missionStartMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 미션 시작 시 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/missionCompleteMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 미션 종료 시 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/sysHit.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);				// 적 피격 시 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/sysDestroy.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 적 파괴 시 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/serverDisconnectedMsg.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);// 서버 디스커넥티드 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/msslAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 미사일 경보 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/offCourseAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 코스 이탈 경보 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/lockonAlert.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 락온 경보 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/pullUpWarning.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 고도 경보 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/stallWarning.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 스톨 경보 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/breakAlarm.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 브레이크 UI 텍스처
	pResourceManager->loadTexture("res/Textures/ui/boosting.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 부스팅 UI 텍스처
	pResourceManager->loadTexture("res/Textures/effect/basicSmoke.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 연기 효과에 사용되는 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/missileTrail.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 미사일 연기 효과에 사용되는 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/gunfire.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 게틀링건 총알 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/jetBlow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 전투기 애프터버너 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/glow.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 빛나는 효과의 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/explosionA.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 폭발 효과의 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/explosionB.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 폭발 효과의 이펙트 텍스처
	pResourceManager->loadTexture("res/Textures/effect/explosionCircleA.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);// 폭발 효과의 이펙트 텍스처	
	Sleep(1800);	// 연출을 위한 강제 대기 (부하 시 제거하여도 상관 없음)
	pResourceManager->loadWavSound("res/Sounds/beepWarning.wav");			// 알림 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/gunFire.wav");				// 기총 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/msslFire.wav");				// 미사일 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/msslExplosion.wav");			// 미사일 폭발 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/jetExplosion.wav");			// 전투기 폭발 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/msslWarning.wav");			// 미사일 경고 사운드
	pResourceManager->loadWavSound("res/Sounds/lockonWarning.wav");			// 락온 경고 사운드
	pResourceManager->loadWavSound("res/Sounds/jetEngine.wav");				// 전투기 엔진 사운드
	pResourceManager->loadWavSound("res/Sounds/missionAlarm.wav");			// 미션 알림 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/booster.wav");				// 부스터 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/BGM_Alect_Squadron.wav");	// 배경 음악
	// LobbyFrame Resource
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyBackground.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);	// 배경 그라디언트 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyWorldmap.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 배경 월드 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/lobbyGrid.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 배경 그리드 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/singlePractice.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// 배경 싱글 프렉티스 선택 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/multiplayCombat.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);	// 배경 멀티플레이 컴벳 선택 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/playerSetting.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);		// 배경 플레이어 셋팅 선택 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/credit.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);				// 배경 크레딧 선택 텍스처
	pResourceManager->loadTexture("res/Textures/frame/lobbyFrame/exitGame.png", true, false, GL_LINEAR, GL_LINEAR, GL_CLAMP);			// 배경 게임 나가기 선택 텍스처
	pResourceManager->loadWavSound("res/Sounds/selectHorn.wav");																		// 선택 사운드 로드
	pResourceManager->loadWavSound("res/Sounds/BGM_Skies_of_Deception.wav");															// 배경 음악 로드
	Sleep(1900);	// 연출을 위한 강제 대기 (부하 시 제거하여도 상관 없음)
	// Multiplay Lobby Resource
	pResourceManager->loadWavSound("res/Sounds/BGM_BattleField4_MainTheme.wav");	// 배경 음악
	// ResultFrame Resource
	pResourceManager->loadTexture("res/Textures/frame/resultFrame/result.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);			// 결과 문구 텍스처
	pResourceManager->loadTexture("res/Textures/frame/resultFrame/acePlayer.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 에이스 플레이어 텍스처
	pResourceManager->loadWavSound("res/Sounds/aced.wav");							// 에이스 사운드
	pResourceManager->loadWavSound("res/Sounds/BGM_Overwatch_Score_Screen.wav");	// 배경 음악
	Sleep(1000);	// 연출을 위한 강제 대기 (부하 시 제거하여도 상관 없음)
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
	pResourceManager->loadTexture("res/Textures/screen/cinematicGlower.png", true, false, GL_LINEAR, GL_LINEAR, GL_REPEAT);		// 씨네마틱 글로어
	pResourceManager->loadWavSound("res/Sounds/BGM_The_Wight_to_Remain.wav");	// 배경 음악

	// GameWindow 클래스의 쓰레드가 이미지 로딩이 완료된 HGLRC와 윈도우 HGLRC가 서로 같은 렌더링 콘텍스트를 공유하도록 함
	// 가정: 쓰레드의 HGLRC 에서 생성한 리스트들을 메인 윈도우의 HGLRC의 내용에 덮어씌우도록 내부적으로 구현된 것 같다.
	// 이 구문을 생략하면 윈도우 HGLRC에 텍스처들이 로드되어있지 않음을 확인할 수 있다.
	wglShareLists(*g_pGameManager->getGameWndPtr()->getHGLRC_ThreadPtr(), *g_pGameManager->getGameWndPtr()->getHGLRCPtr());

	// 리소스 로딩 완료로 설정 (로딩 완료를 1000% 로 설정하여 혹시 모를 연산 오차로 생기는 오류 방지)
	iLoadingPercent = 1000;
}

DWORD WINAPI StartFrame::thread_loadResFromThread(void *p) {
	StartFrame *pFrame = (StartFrame*)p;

	// HGLRC 를 두 개 만들고, 서로 공유시켜서 백그라운드 로딩을 시도하는 방법.
	// 참고: http://www.gpgstudy.com/forum/viewtopic.php?t=1165&highlight=%BE%B2%B7%B9%B5%E5

	// 쓰레드에서 사용할 HGLRC를 GameWindow에 저장된 쓰레드용 ThreadRC로 설정
	wglMakeCurrent(*g_pGameManager->getGameWndPtr()->getHDCPtr(), *g_pGameManager->getGameWndPtr()->getHGLRC_ThreadPtr());

	// 리소스 로드 멤버 함수 호출
	if (pFrame == NULL) throw ("Fail to start Thread.");
	else pFrame->loadResource();

	return 0;
}

void StartFrame::updateState(REAL timeElapsed) {
	// 대학교 로고 페이드 인
	if (iLogoState == 0) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() - (rFadeoutTime * timeElapsed)); // 페이드 인
		rUnivLogoTime -= timeElapsed;

		if (rUnivLogoTime < 0.00) ++iLogoState;
	}
	// 대학교 로고 페이드 아웃
	else if (iLogoState == 1) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() + (rFadeoutTime * timeElapsed)); // 페이드 아웃

		// 페이드 아웃 완료 시
		if (pFadeViewer->getColorAlpha() > 1.00) {
			++iLogoState;
			pUnivLogoViewer->setVisible(false);	// 로고 숨기기 및 보이기 전환
			pUnivMsgViewer->setVisible(false);
			pTeamLogoViewer->setVisible(true);
			pTeamMsgViewer->setVisible(true);
		}
	}
	// 팀 로고 페이드 인
	else if (iLogoState == 2) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() - (rFadeoutTime * timeElapsed)); // 페이드 인
		rTeamLogoTime -= timeElapsed;

		if (rTeamLogoTime < 0.00) ++iLogoState;
	}
	// 팀 로고 페이드 아웃
	else if (iLogoState == 3) {
		pFadeViewer->setColorAlpha(pFadeViewer->getColorAlpha() + (rFadeoutTime * timeElapsed)); // 페이드 아웃

		// 페이드 아웃 완료 시
		if (pFadeViewer->getColorAlpha() > 1.00) {
			rMainLogoDelayTime -= timeElapsed;	// 메인 로고 출력 지연 시간 감소
			
			// 메인 로고 출력 지연 시간이 다 경과
			if (rMainLogoDelayTime < 0.00) {
				++iLogoState;						// 다음 로고 상태로 전환
				pTeamLogoViewer->setVisible(false);	// 로고 숨기기 및 보이기 전환
				pTeamMsgViewer->setVisible(false);
				pAceProjectLogo->setVisible(true);
				pLoadingLogo->setVisible(true);
				pLoadingMsgViewer->setVisible(true);
				pFadeViewer->setColorAlpha(0.0);	// 페이드 아웃 뷰어를 즉시 페이드 인
				
				// 쓰레드 생성 후 리소스 로딩 시작
				LPDWORD threadID = 0;
				hLoadingThread = CreateThread(NULL, 0, &thread_loadResFromThread, this, 0, threadID);
				
				// 배경 음악 재생
				if (pBackgroundSound != NULL) pBackgroundSound->play();
			}
		}
	}
	// 메인 로고 출력 및 로딩 동기화
	else if (iLogoState == 4) {
		// 리소스 로딩 완료 시
		if (iLoadingPercent >= 1000) {
			++iLogoState;
		}
		else {
			// 로딩 % 동기화
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
	// 메인 로고 이동
	else if (iLogoState == 5) {
		// 상수
		Vector3 logoPos = pAceProjectLogo->getPos();										// 현재 메인 로고 위치
		SIZE logoSize = pAceProjectLogo->getSize();											// 현재 메인 로고 사이즈
		static const Vector3 newLogoPos(0.8, 0.275, 0.0);									// 새로 설정될 메인 로고 위치
		static const SIZE newLogoSize = { 640, 106 };										// 새로 설정될 메인 로고 사이즈
		static const SIZE logoSizeDec = { (logoSize.cx - newLogoSize.cx) * timeElapsed,
										  (logoSize.cy - newLogoSize.cy) * timeElapsed };	// 매 업데이트마다 메인 로고가 줄어들어야 하는 크기
		static const REAL logoDist = (logoPos - newLogoPos).magnitude();					// 초기 메인 로고 위치와 새 위치 사이의 거리	
		REAL moveDist = (logoDist * timeElapsed);											// 매 업데이트마다 메인 로고가 이동해야 하는 거리

		// 로딩 로고와 메시지 점차 페이드 아웃
		REAL logoAlpha = pLoadingLogo->getColorAlpha() - (1.0 * timeElapsed);
		if (logoAlpha < 0.00) logoAlpha = 0.00;
		pLoadingLogo->setColorAlpha(logoAlpha);
		pLoadingMsgViewer->setTextColorAlpha(logoAlpha);

		// 화면 전환을 위해 메인 로고 위치 이동
		bool bLogoPosComplete = false;				// 메인 로고가 이동해야 할 위치에 위치함
		Vector3 logoDir = (newLogoPos - logoPos);	// 메인 로고가 이동해야 할 방향벡터
		logoDir.normalize();
		if ((logoPos - newLogoPos).magnitude() > moveDist) {
			pAceProjectLogo->setPos(logoPos + (logoDir * moveDist));
		}
		else {
			bLogoPosComplete = true;
			pAceProjectLogo->setPos(newLogoPos);
		}

		// 메인 로고 크기 변경
		bool bLogoSizeComplete = false;				// 메인 로고가 변해야 할 크기에 도달함
		if (logoSize.cx > newLogoSize.cx && logoSize.cy > newLogoSize.cy) {
			pAceProjectLogo->setSize(logoSize.cx - logoSizeDec.cx, logoSize.cy - logoSizeDec.cy);
		}
		else {
			bLogoSizeComplete = true;
			pAceProjectLogo->setSize(newLogoSize.cx, newLogoSize.cy);
		}

		// 메인 로고 이동 및 크기 변경 완료 검사
		if (bLogoPosComplete == true && bLogoSizeComplete == true) {
			++iLogoState;
		}
	}
	// StartFrame 파괴 후 LobbyFrame로 전환할 수 있도록 스위치 멤버 변수 이용
	else if (iLogoState == 6) {
		// 지연 시간 감소시킴
		if ((rLobbyFrameDelayTime -= timeElapsed) < 0.00) {
			// 배경 음악 소거
			if (pBackgroundSound != NULL) {
				pBackgroundSound->setGain(0.00);
				pBackgroundSound->stop();
			}
			// 다음 프레임으로 넘어가도록 스위치 설정
			bReqChangeFrame = true;
		}
		else {
			// 배경 음악 점차 감소
			if (pBackgroundSound != NULL) {
				REAL newGain = (pBackgroundSound->getGain() - timeElapsed);
				if (newGain < 0.00) newGain = 0.00;
				pBackgroundSound->setGain(newGain);
			}
		}
	}
	// 그 외 예외처리
	else {
		// ...
	}
	
}

void StartFrame::updateKey(REAL timeElapsed) {
	// 로고 스테이트가 3 이하 시(로딩 전) 엔터를 누를 시 빠른 속도로 흘러감
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
	// 드로우 리스트 클리어
	drawList.clear();

	// 키 업데이트
	updateKey(timeElapsed);

	// 엔터 키 여부에 따른 페이드 속도 설정
	if (bFastSpeedFade == true) timeElapsed *= 3.0;

	// 스테이터스에 따른 업데이트
	updateState(timeElapsed);

	// 프레임 탈출 여부 조사
	if (bReqChangeFrame == true) {
		// 현재 프래임 파괴
		delete this;

		// 새 프레임 설정
		g_pGameManager->setGameFrame(new LobbyFrame(NULL, true)); // LobbyFrame 첫 번째 생성

		// 프레임 종료
		return true;
	}

	// 위젯 업데이트 후 드로우 리스트에 추가
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
		drawList.push_back(*iterWidget);
	}

	return true;
}

