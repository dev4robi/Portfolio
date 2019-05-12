#include "Game/Frame/CreditsFrame.h"
#include "Game/Object/Player.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Frame/LobbyFrame.h"
#include "Library/DataParser.h"

CreditsFrame::CreditsFrame(Player *pPlayer) : GameFrame(TEXT("CreditsFrame")) {
	// OpenGL 배경 색상 버퍼 설정
	pDrawManager->setBufferClearColor(Vector3(0.00f, 0.00f, 0.00f));

	// 소유주 플레이어 설정
	pOwnerPlayer = pPlayer;

	// 멤버 변수 초기화
	bReqExit = false;
	bPuase = false;
	rExitDelayTime = 1.00;
	rSpeedMulti = 1.00;
	listScrollViewers.clear();

	// 지역 변수 선언
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	Vector3 scrCenter(0.5, 0.5, 0.0);

	// 뷰어 객체들 생성 후 벡터에 푸시
	loadViewerFromDatFile("dat/Credits.dat");

	// 씨네마틱 프레임
	pCinematicFrameViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/cinematicGlower.png"));
	pCinematicFrameViewer->setColor3(0.0, 0.0, 0.0);
	pCinematicFrameViewer->setColorAlpha(0.99);
	addWidget(pCinematicFrameViewer);

	// 프레임 제어 헬프 뷰어
	pHelpViewer = new RectViewer(Vector3(0.01, 0.03, 0.00), { 0, 0 }, 0);
	pHelpViewer->setTextAttributes("[ESC]: Back to Lobby   [<- / ->]: Speed   [Space]: Pause", 5.5, true, 1);
	addWidget(pHelpViewer);
	
	// 속도 배수 뷰어
	pSpeedMultiViewer = new RectViewer(Vector3(0.01, 0.06, 0.00), { 0, 0 }, 0);
	pSpeedMultiViewer->setTextAttributes("Speed: x1", 6.5, true, 1);
	addWidget(pSpeedMultiViewer);
	
	// 페이드아웃 뷰어
	pFadeoutViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, 0);
	pFadeoutViewer->setColor3(0.0, 0.0, 0.0);
	pFadeoutViewer->setColorAlpha(0.0);
	addWidget(pFadeoutViewer);

	// 사운드 재생
	pBackgroundSound = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_The_Wight_to_Remain.wav", false, true);
	pBackgroundSound->setGain(0.5);
	pBackgroundSound->play();
}

CreditsFrame::CreditsFrame(CreditsFrame &obj) : GameFrame(obj) {

}

CreditsFrame::~CreditsFrame() {

}

void CreditsFrame::loadViewerFromDatFile(char *fileName) {
	// 지역 변수 초기화
	int i = 0;							// for 루프를 위한 지역 변수
	const Vector3 pos(0.5, 0.0, 0.0);	// 생성된 뷰어의 기본 위치
	REAL rLastYpos = 0.0f;				// 마지막으로 생성된 뷰어의 Y위치 (다음 위치 오프셋을 계산하기 위함)
	REAL rColor4[4] = { 1.0f, };		// RGBA 색상값을 임시 저장하기 위한 공간
	char *pStr = NULL;					// 파서에서 반환된 문자열 포인터를 임시로 가르킬 포인터 (동적 할당 해제를 위함)
	DataParser parser;					// *.dat 파일 파서
	
	// 파서 크레딧 데이터 파일 오픈
	if (parser.openFile(fileName) == false) {
		MessageBox(NULL, TEXT("CreditsFrame: Fail to open file 'dat/Credits.dat'!"), TEXT("CreditsFrame::loadViewerFromDatFile() File open error"), (MB_OK | MB_ICONINFORMATION));
		return;
	}

	// 데이터를 파싱하여 동적 생성 시작
	while (parser.findHeader("Viewer") == true) {
		// 초기화
		for (i = 0; i < 4; ++i) rColor4[i] = 1.0f;

		// 뷰어 헤더를 찾은 경우 동적 생성
		RectViewer *pViewer = new RectViewer(pos, { 0, 0 }, 0);
		
		// 동적 생성 여부 검사
		if (pViewer != NULL) {
			// 속성값 파싱
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
			pViewer->setColor3(rColor4[0], rColor4[1], rColor4[2]);	// 색상 데이터는 임시 저장해 두었다가 일괄 적용
			pViewer->setColorAlpha(rColor4[3]);
			pViewer->setTextColorAlpha(rColor4[3]);
			if (parser.findAttribute("ACenter") == true) {
				pViewer->setTextAlignCenterX((parser.parseIntFromAttribute() == 1 ? true : false));
			}
			if (parser.findAttribute("FuncID") == true) {
				pViewer->setTagInt(parser.parseIntFromAttribute());
			}

			// 위젯 추가
			addWidget(pViewer);						// 위젯을 위젯 리스트에 추가
			listScrollViewers.push_back(pViewer);	// 위젯을 스크롤 리스트에 추가
			pViewer = NULL;
		}
		// 동적 생성 실패
		else {
			// 나머지 파싱 및 초기화 제외
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
	// 일시정지 상태가 아닌 경우
	if (bPuase == false) {
		// 뷰어 스크롤링 속도
		Vector3 scrollSpd(0.0, 0.15, 0.0);
		scrollSpd *= timeElapsedMulti;

		// 배속 뷰어 표시 업데이트
		char strBuf[11];
		sprintf_s(strBuf, "Speed: x%d", (int)rSpeedMulti);
		pSpeedMultiViewer->setText(strBuf);

		// 배경 음악 속도 업데이트
		pBackgroundSound->setPitch(this->rSpeedMulti);

		// 뷰어들을 위로 스크롤하고 특정 태그번호의 뷰어를 업데이트
		for (list<RectViewer*>::iterator iter = listScrollViewers.begin(); iter != listScrollViewers.end();) {
			RectViewer *pViewer = (*iter);
			
			if (pViewer != NULL) {
				// Tag: 1 색상 변경
				if (pViewer->getTagInt() == 1) {
					DWORD dwVar = ((DWORD)timeGetTime() * timeElapsedMulti);
					pViewer->setColor3(abs(sinf(dwVar * 1.33)), abs(cos(dwVar)), abs(sinf(dwVar)));
				}

				// 뷰어 스크롤
				pViewer->setPos(pViewer->getPos() + scrollSpd);

				// 너무 아래 위치하면 위젯을 그리기 작업 제외
				if (pViewer->getPos().y < -1.5) {
					pViewer->setVisible(false);
					++iter;
				}
				// 너무 위에 위치하면 위젯 그리기 작업 제외하고, 스크롤 리스트에서 제거
				else if (pViewer->getPos().y > 1.5) {
					pViewer->setVisible(false);
					iter = listScrollViewers.erase(iter);
				}
				// 그 외의 경우 위젯을 그리고, 다음 위젯을 비교할 수 있도록 함
				else {
					pViewer->setVisible(true);
					++iter;
				}
			}
		}
	}
	// 일시정지 상태인 경우
	else {
		pSpeedMultiViewer->setText("Paused");
	}

	// 스크롤 벡터에 남은 위젯이 없으면 크레딧 종료 유도
	if (listScrollViewers.size() <= 0) {
		bReqExit = true;
	}
}

void CreditsFrame::updateKey(REAL timeElapsed) {
	if (pInputManager->getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) {
		bReqExit = true;
	}
	if (pInputManager->getKeyState(VK_SPACE) == key_state::KEY_DOWN) {
		// 일시정지 스위치 전환
		bPuase = !bPuase;

		// 배경 음악 일시정지 및 다시 재생
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
	// 시간 배속 설정
	REAL timeElapsedMulti = (timeElapsed * rSpeedMulti);

	// 드로우 리스트 초기화
	drawList.clear();

	// 키 업데이트
	updateKey(timeElapsed);

	// 로직 업데이트
	updateLogic(timeElapsedMulti);

	// 프레임 종료 여부 확인
	if (bReqExit == true) {
		// 프레임 종료 지연 시간이 소진된 경우
		if ((rExitDelayTime -= timeElapsed) < 0.00) {
			// 사운드 소리 끔
			pBackgroundSound->setGain(0.0);

			// 플레이어 정보 카피 후 기존 플레이어 정보 수동으로 삭제
			Player *pPlayer = new Player(pOwnerPlayer->getPlayerNumber(), pOwnerPlayer->getPlayerName());
			delete pOwnerPlayer;

			// 현재 프래임 파괴
			delete this;

			// 새 프레임 설정
			g_pGameManager->setGameFrame(new LobbyFrame(pPlayer)); // LobbyFrame 을 다음 프레임으로 설정

			// 프레임 종료
			return true;
		}
		// 프레임 종료까지 지연 시간이 남은 경우
		else {
			REAL newGain = pBackgroundSound->getGain() - timeElapsed;
			REAL newAlpha = pFadeoutViewer->getColorAlpha() + timeElapsed;
			
			// 범위 제어
			if (newGain < 0.00) newGain = 0.00;
			if (newAlpha > 1.00) newAlpha = 1.00;

			// 사운드 감소
			pBackgroundSound->setGain(newGain);

			// 화면 페이드 아웃
			pFadeoutViewer->setColorAlpha(newAlpha);
		}
	}

	// 위젯 업데이트 및 드로우 리스트에 추가
	for (list<GameWidget*>::iterator iterWidget = widgetList.begin(); iterWidget != widgetList.end(); ++iterWidget) {
		(*iterWidget)->update(timeElapsedMulti);
		drawList.push_back(*iterWidget);
	}

	return true;
}