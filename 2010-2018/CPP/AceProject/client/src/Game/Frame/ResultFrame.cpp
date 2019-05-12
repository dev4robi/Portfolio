#include "Game/Frame/ResultFrame.h"
#include "Engine/GameCamera.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Frame/LobbyFrame.h"
#include "Game/UI/Viewer/LogoViewer.h"

ResultFrame::ResultFrame(vector<Player*> playerList, Player *pOwnPlayer) : GameFrame(TEXT("ResultFrame")) {
	// OpenGL 배경 색상 버퍼 설정
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// 클래스 초기화
	enScene = ResultScene::SCENE_BEGIN;
	rFrameElapseTime = 0.0;
	pOwnerPlayer = pOwnPlayer;
	rScoreShowTerm = 0.5;
	playerVector = playerList;
	bQuitable = false;
	bShowScore = false;
	vecScorePos = Vector3(0.225, 0.6, 0.0);
	iPlayerCount = 0;

	// 플레이어를 점수에 따라 정렬
	sort(playerVector.begin(), playerVector.end(), Player::cmpScore);

	// 텍스처 번호 설정
	texture_missionResult = pResourceManager->getTextureID("res/Textures/frame/resultFrame/result.png");
	texture_acePlayer = pResourceManager->getTextureID("res/Textures/frame/resultFrame/acePlayer.png");

	// 플레이어 카메라 설정
	GameCamera &cam = *(pOwnerPlayer->getPlayerCamera());
	Vector3 eye(0.0, 0.0, 0.0), at(1.0, 0.0, 0.0), up(0.0, 1.0, 0.0);
	cam.lookAt(eye, at, up);

	// 사운드 재생
	pBackgroundSound = new Sound(eye, "res/Sounds/BGM_Overwatch_Score_Screen.wav", false, true);
	
	// 검은 배경 추가
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	RectViewer *pBackground = new RectViewer(Vector3(0.5, 0.5, 0.0), { pResolution->getWidth(), pResolution->getHeight() }, 0);
	pBackground->setColor3(0.0, 0.0, 0.0);
	addWidget(pBackground);

	// 배경 로고 추가
	REAL logoWidth = (pResolution->getWidth() * 1024.0 / 1920.0), logoHeight = (pResolution->getHeight() * 1024.0 / 1080.0);
	LogoViewer *pLogoViewer = new LogoViewer(Vector3(0.5, 0.5, 0.0), { logoWidth, logoHeight });
	pLogoViewer->setColorAlpha(0.2);
	addWidget(pLogoViewer);

	// 뷰어 초기화
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
	// 플레이어 삭제
	for (int i = 0; i < playerVector.size(); ++i) {
		delete playerVector[i];
	}
	playerVector.clear();

	// 포인터 초기화
	pAnykeyViewer = NULL;
	pAceViewer = NULL;
	pFadeoutViewer = NULL;
}

void ResultFrame::updateKey(REAL timeElapsed) {
	// . . .
	// ResultFrame 은 구현할 기능이 간단하여 별도의 updateKey 함수를 사용하지 않고, updateFrame 내에 모든 기능을 정의.
}

bool ResultFrame::updateFrame(REAL timeElapsed) {
	/* 프레임 시간 경과 */
	rFrameElapseTime += timeElapsed;

	/* 위젯 업데이트 */
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsed);
	}

	/* RESULT 문구 출력 */
	if (enScene == ResultScene::SCENE_BEGIN && rFrameElapseTime > 1.00) {
		enScene = ResultScene::SCENE_PRINT_RESULT;
		Vector3 vec(0.5, 0.8, 0.0);
		SIZE size = { 1024, 128 };
		addWidget(new RectViewer(vec, size, texture_missionResult));
	}

	/* 점수 출력 시작 */
	static REAL scoreShowTerm = rScoreShowTerm;
	if (enScene == ResultScene::SCENE_PRINT_RESULT && rFrameElapseTime > 2.50) {
		// 처음 진입 시 씬 상태 변경
		if (bShowScore == false) {
			bShowScore = true;

			RectViewer *pScoreTitle = new RectViewer(Vector3(0.22, 0.67, 0.0), { 0, 0 }, 0);
			pScoreTitle->setTextAttributes("R A N K                I D               K I L L S               S C O R E", 10.0, true, 1);
			pScoreTitle->setColor3(Vector3(0.33, 1.00, 1.00));
			addWidget(pScoreTitle);
		}

		// 다음 출력 시간 갱신 및 출력 시간 경과 시 출력
		if ((scoreShowTerm -= timeElapsed) <= 0.0) {
			// 다음 출력 시간 갱신
			scoreShowTerm = (rScoreShowTerm - scoreShowTerm);

			// 스코어 출력 ( i == 0 : RANK / 1 :ID / 2 : KILLS / 3 : SCORE ) 
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

				switch (iPlayerCount) { // 금, 은, 동, 흰색 색상 지정
				case 0:		textColor = Vector3(1.0, 1.0, 0.0);		break;
				case 1:		textColor = Vector3(0.77, 0.77, 0.77);	break;
				case 2:		textColor = Vector3(0.73, 0.48, 0.34);	break;
				default:	textColor = Vector3(1.0, 1.0, 1.0);		break;

				}
				// 자신은 붉은색톤을 더하고 크기를 조금 키워 출력
				REAL textSize = 9.0;
				if (pOwnerPlayer->getPlayerName() == pPlayer->getPlayerName()){
					textColor = Vector3(1.0f, 0.0f, 0.5f);
					textSize += 2.0f;
				}

				pScoreViwer->setColor3(textColor);
				pScoreViwer->setTextAttributes(string(scoreStrBuf), textSize, true, 1); // 스코어 정보 및 크기 지정
				addWidget(pScoreViwer);
			}
			vecScorePos.y -= 0.065; // 한 행 아래로 이동
			++iPlayerCount;

			if (iPlayerCount == playerVector.size()) {
				enScene = ResultScene::SCENE_PRINT_ANYKEY;
				scoreShowTerm = rScoreShowTerm;				// static이므로 한 번만 초기화가 된다 -> 다음번 출력을 위해 초기화
			}
		}
	}

	// 종료 문구 점멸
	if (pAnykeyViewer != NULL)
		pAnykeyViewer->setTextColorAlpha(abs(sinf(rFrameElapseTime * 3.0)));

	/* 점수 출력 끝, 아무 키나 누르세요 문구 출력 */
	if (enScene == ResultScene::SCENE_PRINT_ANYKEY) {
		// 종료 문구와 에이스 메달 생성
		if (bQuitable == false) {
			bQuitable = true;
			// 에이스 문구
			pAceViewer = new RectViewer(Vector3(0.23, 0.60, 0.0), { 80, 80 }, texture_acePlayer);
			pAceViewer->setVisible(false);
			addWidget(pAceViewer);
			// 종료 문구
			pAnykeyViewer = new RectViewer(Vector3(0.03, 0.04, 0.0), {0, 0}, 0);
			pAnykeyViewer->setTextAttributes("PRESS [ENTER] TO EXIT... ", 9.0, true, 1);
			pAnykeyViewer->setColor3(Vector3(1.0, 1.0, 1.0));
			pAnykeyViewer->setVisible(false);
			addWidget(pAnykeyViewer);
		}

		// 에이스 훈장 출력 및 사운드 출력
		if (pAceViewer != NULL) {
			// 훈장 숨기기 해제
			if (pAceViewer->getElapsedTime() > 0.50) {
				pAceViewer->setVisible(true);
				// 사운드 출력
				new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/aced.wav", false, false);
				// 종료 문구 숨기기 해제
				pAnykeyViewer->setVisible(true);
				pAceViewer = NULL;
			}
		}

		// 엔터 입력 시 페이드 아웃 뷰어 추가
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
	

	/* 프레임 종료 시작 */
	if (enScene == ResultScene::SCENE_EXITING) {
		// 페이드 아웃 수행
		if (pFadeoutViewer != NULL) {
			pFadeoutViewer->setColorAlpha(pFadeoutViewer->getColorAlpha() + (1.0 * timeElapsed));
			pBackgroundSound->setGain(pBackgroundSound->getGain() - 0.5 * timeElapsed);
		}

		// 완전히 페이드 아웃이 완료된 경우
		if (pFadeoutViewer->getColorAlpha() >= 1.0) {
			// 플레이어 일부 정보 복사
			Player *pPlayer = new Player(0, pOwnerPlayer->getPlayerName());

			// 프레임 파괴
			delete this;

			// 다음 프레임 설정 (주의!: 항상 본인 파괴 후 다음 프레임을 지정해 주어야 한다.)
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer));

			// 결과 프레임 만료
			return true;
		}
	}

	/* 드로우 리스트 초기화 및 위젯 추가 */
	drawList.clear();
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		drawList.push_back(*iterWidget);
	}

	/* 성공 반환 */
	return true;
}