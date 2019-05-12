#include "Engine/GameWindow.h"
#include "Engine/SoundManager.h"
#include "Game/Frame/CombatFrame.h"
#include "Game/Object/Player.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Terrain.h"
#include "Game/Object/Skybox.h"
#include "Game/Object/Weapon.h"
#include "Game/Object/Missile.h"
#include "Game/UI/UI.h"
#include "Game/UI/Viewer/RectViewer.h"
#include "Game/Effect/GL_Effect.h"
#include "Game/Frame/ResultFrame.h"

CombatFrame::CombatFrame(Player *pLobbyPlayer) : GameFrame(TEXT("CombatFrame_Single")) { // 싱글플레이 생성자
	// 멀티플레이 모드 설정
	dwLastRecvTime = 0;
	bMultiplayMode = false;
	bServerDisconnected = true;
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	playerControlVector.clear();

	// 세션 만료 및 도움말 표시 여부 초기화
	sessionExpired = false;
	bHelpUIIsShown = false;
	bExitUIIsShown = false;

	// 남은 세션 시간 설정
	sessionLeftTime = 300.0; // 디폴트 300초

	// 지형 및 하늘상자 추가 (주의!: 이 둘은 항상 WidgetList에 추가하지 않는다)
	pTerrain = new Terrain(35.0, "res/Textures/map/terrainTexture.png", "res/textures/map/heightmap.jpg", 0.0, 850.0, 32);
	pSkybox = new Skybox();

	// 배경 음악 생성
	setBackgroundSound(new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Alect_Squadron.wav", false, true));
	getBackgroundSoundPtr()->setGain(0.5);

	// 플레이어 설정 
	int playerNumber = 0;																				// 플레이어 번호
	Player *pPlayer = new Player(pLobbyPlayer->getPlayerNumber(), pLobbyPlayer->getPlayerName());		// 플레이어 생성
	Vector3 pos;
	pos.x = 5000.0;
	pos.y = (REAL)mymath::randomInt(2500, 4000);
	pos.z = 0.0;
	Vector3 centerDirection = Vector3(0.0, pos.y, 0.0) - pos;			// 중앙을 향하는 방향 벡터
	centerDirection.normalize();
	JetFighter *pJet = new JetFighter(pos, pPlayer, "F-22 Raptor");		// 전투기 객체 생성
	pPlayer->setPlayerJet(pJet);										// 플레이어 소유의 전투기로 설정
	pPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);      // 중앙을 바라보도록 초기화
	playerList.push_back(pPlayer);										// 게임 프레임에 플레이어 추가
	addWidget(pJet);													// 게임 프레임에 전투기 추가
	pOwnerPlayer = pPlayer;												// 프레임 오너 플레이어 설정

	// 전달받은 플레이어 객체 파괴
	delete pLobbyPlayer;
	pLobbyPlayer = NULL;

	// 테스트용 AI플레이어 생성
	char num;
	const int TEST_PLAYER = 7;
	for (int i = 0; i < TEST_PLAYER; ++i) {
		num = i + 48;
		pPlayer = new Player(playerNumber++, string("Tester") + num);	// 플레이어 생성
		pos.x = cosf(mymath::deg2rad((i + 1) * (360.0 / (TEST_PLAYER + 1)))) * 5000.0;
		pos.y = (REAL)mymath::randomInt(2500, 4000);
		pos.z = sinf(mymath::deg2rad((i + 1) * (360.0 / (TEST_PLAYER + 1)))) * 5000.0;
		centerDirection = Vector3(0.0, pos.y, 0.0) - pos;				// 중앙을 향하는 방향 벡터
		centerDirection.normalize();
		pJet = new JetFighter(pos, pPlayer, "F-22 Raptor");				// 전투기 객체 생성
		pPlayer->setPlayerJet(pJet);									// 플레이어 소유의 전투기로 설정
		pPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);	// 중앙을 바라보도록 초기화
		playerList.push_back(pPlayer);									// 게임 프레임에 플레이어 추가
		addWidget(pJet);												// 게임 프레임에 전투기 추가
	}

	// 추가 뷰어 생성 ( 이 뷰어들은 별도로 위젯 리스트에 넣지않고 관리 )
	Vector3 scrCenter(0.5, 0.5, 0.0);
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	pHelpUIViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/Help.png"));
	pExitUIViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/Exit.png"));
	pFadeoutViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, 0);
	pFadeoutViewer->setColor3(0.0, 0.0, 0.0);
	pFadeoutViewer->setColorAlpha(0.0);
	pHelpUIViewer->setVisible(false);
	pExitUIViewer->setVisible(false);
	pFadeoutViewer->setVisible(false);
}

CombatFrame::CombatFrame(Player *pLobbyPlayer, vector<Player*> *pPlayerVector, REAL newTgtLPS) : GameFrame(TEXT("CombatFrame_Multi")) { // 멀티플레이 생성자
	/* 난수 시드 재설정 */
	srand(pLobbyPlayer->getPlayerNumber() + mymath::randomInt(1, 100000));

	/* 변수 초기화 */
	pGameManager->setTargetLPS(newTgtLPS);
	dwLastRecvTime = 0;
	bMultiplayMode = true;
	bServerDisconnected = false;
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	playerControlVector.reserve(32);
	playerControlVector.clear();
	sessionExpired = false;
	sessionLeftTime = 600.0;   // 디폴트 600초 (서버측에서 동기화 수행)
	bHelpUIIsShown = false;
	bExitUIIsShown = false;
	playerList.clear();

	/* 지형 및 하늘상자 추가 (주의!: 이 둘은 항상 WidgetList에 추가하지 않는다) */
	pTerrain = new Terrain(35.0, "res/Textures/map/terrainTexture.png", "res/textures/map/heightmap.jpg", 0.0, 850.0, 32);
	pSkybox = new Skybox();

	/* 배경 음악 생성 */
	setBackgroundSound(new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Alect_Squadron.wav", false, true));
	getBackgroundSoundPtr()->setGain(0.5);

	/* 플레이어 소유주의 플레이어 번호를 임시 저장 */
	int iOwnPlayerNum = pLobbyPlayer->getPlayerNumber();

	/* 다른 플레이어 정보를 재생성하여 리스트에 복사 */
	// (주의!: Player 클래스의 맴버인 UImaster 클래스는 생성자에서 현재 프레임이 어떤 프레임인지 저장하게 됨.)
	// (따라서 플레이어가 현재 프레임에서 생성되지 않는 경우 파괴된 프레임을 포인터로 가르키고 있게 됨.)
	Player *pNewPlayer = NULL;
	for (vector<Player*>::iterator iterVector = pPlayerVector->begin(); iterVector != pPlayerVector->end(); ++iterVector) {
		pNewPlayer = new Player((*iterVector)->getPlayerNumber(), (*iterVector)->getPlayerName());	// 플레이어 정보를 복사
		delete (*iterVector);																		// 다른 프레임에서 생성된 플레이어 정보 삭제
		playerList.push_back(pNewPlayer);															// 복사된 플레이어 정보 삽입

		if (pNewPlayer->getPlayerNumber() == iOwnPlayerNum) pOwnerPlayer = pNewPlayer;				// 플레이어 번호가 소유주 플레이어의 번호인 경우 생성된 플레이어를 소유주 플레이어로
	}
	delete pPlayerVector;   // 플레이어 정보가 복사 완료되었으므로 동적 공간의 플레이어 벡터 할당 해지
	pLobbyPlayer = NULL;
	pNewPlayer = NULL;

	/* 플레이어들의 전투기 생성 */
	Vector3 posCenter(0.0, 0.0, 0.0);
	Vector3 pos, centerDirection;
	JetFighter *pJet = NULL;
	Player *pTempPlayer = NULL;
	int i = 0;

	for (list<Player*>::iterator iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		pTempPlayer = (*iterPlayer);
		
		// 플레이어 순번에 따라 시계 방향으로 돌아가면서 전투기 좌표 설정
		pos.x = cosf(mymath::deg2rad((i + 1) * (360.0 / playerList.size()))) * 5000.0;
		pos.y = (REAL)mymath::randomInt(2500, 4000);
		pos.z = sinf(mymath::deg2rad((i + 1) * (360.0 / playerList.size()))) * 5000.0;
		centerDirection = Vector3(0.0, pos.y, 0.0) - pos;					// 중앙을 향하는 방향 벡터
		++i;

		// 전투기 생성
		centerDirection.normalize();
		pJet = new JetFighter(pos, pTempPlayer, "F-22 Raptor");				// 전투기 객체 생성
		pTempPlayer->setPlayerJet(pJet);									// 플레이어 소유의 전투기로 설정
		pTempPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);	// 중앙을 바라보도록 초기화
		addWidget(pJet);													// 게임 프레임에 전투기 추가

		// 테스트 메시지
	#ifdef __DEBUG__
		cout << "Version: " << GAME_VERSION << endl;
		cout << "PlayerList: " << (*iterPlayer)->getPlayerName() << "(" << (*iterPlayer)->getPlayerNumber() << ") / Jet_ID: " << (*iterPlayer)->getPlayerJet()->getID() << endl; // test
	#endif
	}

	// 추가 뷰어 생성 ( 이 뷰어들은 별도로 위젯 리스트에 넣지않고 관리 )
	Vector3 scrCenter(0.5, 0.5, 0.0);
	Size2D *pResolution = pDrawManager->getResolutionPtr();
	REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();
	pHelpUIViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/Help.png"));
	pExitUIViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, pResourceManager->getTextureID("res/Textures/screen/Exit.png"));
	pFadeoutViewer = new RectViewer(scrCenter, { scrWidth, scrHeight }, 0);
	pFadeoutViewer->setColor3(0.0, 0.0, 0.0);
	pFadeoutViewer->setColorAlpha(0.0);
	pHelpUIViewer->setVisible(false);
	pExitUIViewer->setVisible(false);
	pFadeoutViewer->setVisible(false);
}

CombatFrame::~CombatFrame() {
	// 타겟 프레임 초기화
	pGameManager->setTargetLPS(50.0);

	// 뷰어 파괴
	if (pHelpUIViewer != NULL) delete pHelpUIViewer;
	if (pExitUIViewer != NULL) delete pExitUIViewer;
	if (pFadeoutViewer != NULL) delete pFadeoutViewer;
	pHelpUIViewer = NULL;
	pExitUIViewer = NULL;
	pFadeoutViewer = NULL;

	// 위 두개의 위젯을 제외한 위젯들은 GameFrame 에서 삭제한다.
	// 플레이어들은 ResultFrame 으로 전달, 필요한 정보를 사용 후 삭제한다.
}


bool CombatFrame::isMultiplayMode() {
	return bMultiplayMode;
}

bool CombatFrame::isServerDisconnected() {
	return bServerDisconnected;
}

REAL CombatFrame::getSessionLeftTime() {
	return sessionLeftTime;
}

vector<int>* CombatFrame::getPlayerControlVectorPtr() {
	return &playerControlVector;
}

list<Player*>* CombatFrame::getPlayerListPtr() {
	return &playerList;
}

void CombatFrame::updateNetwork(REAL timeElapsed) {
	// 지역 변수 선언
	char *pPtr = NULL;										// 서버로 전송할 동기화 패킷 생성중 사용될 포인터
	DWORD dwElapsedTime = g_pGameManager->getElapsedTime();	// 프로그램 경과 시간
	DWORD dwLastSendTime = 0;								// 마지막으로 서버에 전송한 시간
	bool bReadyForUpdate = false;							// 업데이트 수행 준비 여부
	list<Player*>::iterator iterPlayer;						// 플레이어 리스트를 조회하기 위한 반복자 변수
	GamePacket recvPacket;									// 서버로부터 받은 패킷 데이터를 저장할 스택 공간
	GamePacket sendPacket;									// 서버로 전송할 패킷
	static int iUpdateWaitLimitCnt = 0;						// 업데이트 최대 대기 시간 카운터

	// 변수 초기화
	iUpdateWaitLimitCnt = 0;
	dwLastRecvTime = (DWORD)timeGetTime();
	for (iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		if (pOwnerPlayer->getPlayerNumber() == (*iterPlayer)->getPlayerNumber()) {
			(*iterPlayer)->setNetworkUpdated(true); // 자기 자신으로부터는 동기화가 필요 없도록 함
		}
		else if ((*iterPlayer)->getNetworkDisconnected() == true) {
			(*iterPlayer)->setNetworkUpdated(true);	// 연결 끊긴 대상도 동과화가 필요 없도록 함
		}
		else {
			(*iterPlayer)->setNetworkUpdated(false); // 그 외 플레이어는 플레이어 업데이트됨 여부 초기화
		}
	}

	// 전송 패킷 초기화
	sendPacket.dwFrameTime = dwElapsedTime;
	sendPacket.sPlayerNumber = pOwnerPlayer->getPlayerNumber();
	sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_SYNC;
	
	// 플레이어 전투기 정보를 담음
	pPtr = pOwnerPlayer->getPlayerJet()->toGamePacket(&sendPacket);
	
	// 플레이어 명령 정보를 담음
	for (int i = 0; i < playerControlVector.size(); ++i) {
		memcpy_s(pPtr, sizeof(int), &playerControlVector[i], sizeof(int));
		pPtr += sizeof(int);
	}

	// 서버와 디스커넥트 여부 확인
	if (bServerDisconnected == false) {
		// 세션 업데이트 명령이 수신되지 않았거나, 모든 플레이어의 업데이트 패킷을 수신하지 않은 경우
		while (bReadyForUpdate == false) {
			// 서버로부터 5초 넘게 메시지 아무런 메시지 전송이 없는 경우 디스커넥트 루틴 수행
			// (서버측에서는 다른 플레이어의 MSG_CLIENT_SYNC 메시지가 모두 전송되길 대기할 때 MSG_CLIENT_WAIT 메시지를 전송해준다)
			if ((DWORD)timeGetTime() > dwLastRecvTime + 5000) {
				// 클라이언트 디스커넥션 상태로 전환
				bServerDisconnected = true;

				// 남은 세션 시간을 0으로 설정하여 세션 종료 유도
				sessionLeftTime = 0.00;

				// 나머지 구문을 수행하지 않고 종료, 나머지 업데이트를 수행하면서 프레임이 종료됨
				return;
			}
			// 서버로부터 메시지를 지속적으로 수신하고 있는 경우
			else {
				// 서버로부터 아직 클라이언트 데이터 수신 여부를 확인받지 못한 경우 매 0.05~0.1초 마다 서버로 전송
				if (bRecvSendMsgFromServer == false && (DWORD)timeGetTime() > (dwLastSendTime + mymath::randomInt(50, 100))) {
					// 저장된 동기화 메시지 패킷을 서버로 전송
					pNetworkManager->sendMsgToServer(&sendPacket);
					dwLastSendTime = (DWORD)timeGetTime();
					cout << "[LeftTime:" << this->sessionLeftTime << "] 서버로부터 동기화완료 신호를 받지 못해 0.1초 재전송 수행." << endl; // test 
				}
				// 서버로부터 수신 여부를 확인받은 이후는 1.0 초마다 동기화 메시지만 전송 (클라이언트 데이터를 수신 받았던 서버가 이 메시지를 전송받으면 MSG_CLIENT_WAIT 메시지를 전송해 준다)
				else if ((DWORD)timeGetTime() > dwLastSendTime + 1000) {
					pNetworkManager->sendMsgToServer(&sendPacket);
					dwLastSendTime = (DWORD)timeGetTime();
					cout << "[LeftTime:" << this->sessionLeftTime << "] 서버에 1.0초 주기 동기화 메시지 전송." << endl; // test
				}
				// 서버로부터 보낸 메시지가 없을 때 까지 수신 수행
				while (pNetworkManager->recvMsgFromServer(&recvPacket) > 0) {
					// 수신받은 메시지를 분석하여 클라이언트에 적용
					translatePacket(&recvPacket, timeElapsed);
					dwLastRecvTime = (DWORD)timeGetTime();
				}
				// 모든 플레이어로부터 동기화 업데이트 되었는지 확인
				bool bAllPlayerUpdated = true;
				for (iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
					if ((*iterPlayer)->getNetworkUpdated() == false) {
						bAllPlayerUpdated = false;
						printf("[LeftTime:%f] 플레이어 %s(Num:%d)로 부터의 업데이트 패킷이 아직 도착하지 않았습니다.\n", this->sessionLeftTime, (*iterPlayer)->getPlayerName(), (*iterPlayer)->getPlayerNumber()); // test
						break;
					}
				}
				// 업데이트 가능 여부 검사
				if (bAllPlayerUpdated == true && bRecvUpdateMsgFromServer == true) {
					// 서버로부터 업데이트 허가를 받고, 모든 플레이어의 동기화 업데이트가 완료된 경우 while문을 탈출하며 업데이트 수행
					bReadyForUpdate = true;
				}
				// 서버로부터 업데이트 명령은 받았지만 모든 플레이어의 업데이트가 되지 않고 업데이트가 0.1초 이상 지연되는 경우 강제로 업데이트 구문 수행
				else if (bRecvUpdateMsgFromServer == true && bAllPlayerUpdated == false) {
					if (iUpdateWaitLimitCnt > 5) {
						bReadyForUpdate = true;
						printf("[LeftTime:%f] 플레이어 동기화 명령 대기 초과로 강제 업데이트 수행.", this->sessionLeftTime);
					}
				}
				else {
					cout << "[LeftTime:" << this->sessionLeftTime << "] 업데이트 불가."; // test
					if (bAllPlayerUpdated == false) cout << " - 모든 플레이어의 업데이트가 수행되지 않음."; // test 
					if (bRecvUpdateMsgFromServer == false) cout << " - 서버로부터 업데이트 메시지 수진되지 않음.\n"; // test 
				}
			}

			++iUpdateWaitLimitCnt; // 이 루프를 몇 회 도는지 확인
		}
	}

	// 여기에 도달했다면 함수를 탈출하면서 클라이언트 업데이트가 수행됨.
	// 다음 수신을 위해 두 맴버 변수를 초기화.
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	// 동기화가 수행되야 하는 명령을 담은 벡터를 비움
	playerControlVector.clear();
	cout << "[LeftTime:" << this->sessionLeftTime << "] 반복 회차(" << iUpdateWaitLimitCnt - 1 << "업데이트 수행. \n"; // test
}

void CombatFrame::translatePacket(GamePacket *pPacket, REAL timeElapsed) {
	switch (pPacket->enMsgType) {
	// 서버로부터 강퇴됨(세션 도중) 메시지
	case GamePacket::MsgType::MSG_CLIENT_KICKED:
		bRecvUpdateMsgFromServer = true;	// 업데이트를 수행할 수 있도록 해줌
		bServerDisconnected = true;			// 하지만 서버 디스커넥티드 상태로 전환됨
		sessionLeftTime = 0.00;				// 세션 시간이 만료되어 서버 디스커넥티드 메시지 출력 후 프레임 종료 유도
		break;

	// 다른 클라이언트 탈퇴함(세션 도중) 메시지
	case GamePacket::MsgType::MSG_DISCONNECT:
		for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {	// 플레이어 리스트 중 해당 번호의 플레이어가 있는지 확인
			if ((*iter)->getPlayerNumber() == (int)pPacket->sPlayerNumber) {
				(*iter)->getPlayerJet()->setHiding(true);	// 해당 플레이어의 전투기를 비활성화 처리
				(*iter)->setNetworkDisconnected(true);		// 해당 플레이어의 네트워크 연결 상태를 디스커넥션으로 설정
			}
		}
		break;

	// 클라이언트 대기 메시지 (이 메시지를 수신한 클라이언트는 업데이트 메시지 수신까지 수신 작업만 수행)
	case GamePacket::MsgType::MSG_CLIENT_WAIT:
		bRecvSendMsgFromServer = true;
		break;
	
	// 클라이언트 동기화 메시지 (각종 함수들 번호와 매개변수 값이 전송됨)
	case GamePacket::MsgType::MSG_CLIENT_SYNC:
		// 클라이언트 -> 서버 : 1. 동기화 메시지 처음 전송 시 : 플레이어의 좌표, 저장된 명령들을 전송.
		//						2. 동기화 메시지 두번 째 이후 : 서버에게 아직 클라이언트가 연결되어 있음을 전송.
		// 서버 -> 클라이언트 : 다른 클라이언트의 동기화 해야 할 명령들을 전송.
		for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
			if ((*iter)->getPlayerNumber() == pPacket->sPlayerNumber) {
				char *pBytePtr = NULL;
				// 전투기 동기화
				pBytePtr = (*iter)->getPlayerJet()->fromGamePacket(pPacket);

				// 명령 동기화
				while (*pBytePtr != '\0' && pBytePtr != NULL) {
					int funcID = 0;		int argi[2];
					REAL argf = 0.0f;
					JetFighter *pJet = (*iter)->getPlayerJet();
					pBytePtr = NetworkManager::unpackFromByteData(&funcID, pBytePtr, sizeof(funcID));

					switch (funcID) {
					default:	case 0:		break;
					case 1:		pJet->getFirstWeaponPtr()->wpnTgtChangeRequest();	break;
					case 2:		pJet->getFirstWeaponPtr()->fireWeapon();			break;
					case 3:		pJet->getBaseWeaponPtr()->fireWeapon();				break;
					case 4:		pJet->getBaseWeaponPtr()->stopFireWeapon();			break;
					case 5:		
						// 패킷에서 데이터 추출
						pBytePtr = NetworkManager::unpackFromByteData(&argi[0], pBytePtr, sizeof(int));	// 매개변수 1 : 피해 입힌 플레이어 번호 추출
						pBytePtr = NetworkManager::unpackFromByteData(&argf, pBytePtr, sizeof(REAL));	// 매개변수 2 : 피해량 추출
						pBytePtr = NetworkManager::unpackFromByteData(&argi[1], pBytePtr, sizeof(int));	// 매개변수 3 : 자살 여부 추출
						// 플레이어 리스트에서 데미지 타겟이 되는 플레이어의 전투기를 찾음
						for (list<Player*>::iterator iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
							if ((*iterPlayer)->getPlayerNumber() == argi[0]) {
								(*iter)->getPlayerJet()->dealtDamage((*iterPlayer)->getPlayerJet(), argf, (bool)argi[1], false);
								break;
							}
						}
					break;
					// Add case6: break; here...
					}
				}
				// 해당 플레이어 업데이트 여부 설정
				(*iter)->setNetworkUpdated(true);
				
				// 다른 플레이어 검사 생략
				break;
			}
		}
		break;
	
	// 클라이언트 업데이트 허락 메시지 (이 메시지를 전달받기 전까지는 데이터 수신만 반복)
	case GamePacket::MsgType::MSG_CLIENT_DOUPDATE:
		sessionLeftTime = (REAL)(pPacket->dwFrameTime / 1000.0);	// 업데이트 명령에서는 서버 시간이 아닌 남은 세션 시간을 dwFrameTime에 담아둠
		bRecvUpdateMsgFromServer = true;
		break;
	
	// 그 외 패킷 손상 등으로 이상값이 전송될 때 예외 처리 구문
	default:
		// ...
		;
	}
}

void CombatFrame::updateKey(REAL timeElapsed) {
	InputManager &inputMgr = *pInputManager;
	JetFighter *pJet = pOwnerPlayer->getPlayerJet();

#ifdef __DEBUG__
	// 오너 스왑
	if (bMultiplayMode == false) {
		if (inputMgr.getKeyState('1') == key_state::KEY_DOWN) {
			for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
				cout << "Player" << (*iter)->getPlayerNumber() << ": " << (*iter)->getPlayerName() << endl;
			}
			list<Player*>::iterator iterPlayer = find(playerList.begin(), playerList.end(), pOwnerPlayer); // 플레이어 자신을 찾고
			if (++iterPlayer != playerList.end()) { // 다음 플레이어가 존재하면 해당 플레이어를 주인 플레이어로 전환
				pOwnerPlayer = (*iterPlayer);
			}
			else {
				iterPlayer = playerList.begin(); // 다음 플레이어가 없으면 가장 첫번째 플레이어를 주인 플레이어로 전환
				pOwnerPlayer = (*iterPlayer);
			}
		}
	}
	// 지형 와이어프레임 모드
	if (inputMgr.getKeyState('T') == key_state::KEY_DOWN) {
		if (pTerrain->getTextureID() != 0) pTerrain->setTextureID(0);
		else pTerrain->setTextureID(pResourceManager->getTextureID("res/Textures/map/terrainTexture.png"));
	}
	// 전투기 초기화
	if (inputMgr.getKeyState('0') == key_state::KEY_DOWN) {
		pJet->setPos(0.0, 2500.0, 0.0);
		pJet->directVector = Vector3(1.0, 0.0, 0.0);
		pJet->upVector = Vector3(0.0, 1.0, 0.0);
		pJet->rightVector = Vector3(0.0, 0.0, 1.0);
		pJet->transform.identity();
		pJet->HP = pJet->maxHP;
		Quaternion temp;
		temp.fromAxisAngle(Vector3(1.0, 0.0, 0.0), pJet->rotateOffset[0]);
		pJet->transform *= temp;
		temp.fromAxisAngle(Vector3(0.0, 1.0, 0.0), pJet->rotateOffset[1]);
		pJet->transform *= temp;
		temp.fromAxisAngle(Vector3(0.0, 0.0, 1.0), pJet->rotateOffset[2]);
		pJet->transform *= temp;
		pJet->pitch = pJet->yaw = pJet->roll = 0.0;
		pJet->setState(FlightObject::object_state::STATE_ALIVE);
		pJet->getBaseWeaponPtr()->setQuantity(800);
		pJet->currentEngPower = 0.0;
		pJet->currentSpeed = 500.0;
	}
#endif
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
	// 도움말
	if (inputMgr.getKeyState(VK_F1) == key_state::KEY_DOWN) {
		if (sessionExpired == false) bHelpUIIsShown = !(bHelpUIIsShown);
	}
	// 종료
	if (inputMgr.getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) {
		// 종료 닫기
		if (bExitUIIsShown == true) bExitUIIsShown = false;

		// 종료 열기
		else if (sessionExpired == false) bExitUIIsShown = !(bExitUIIsShown);
	}
	// 비행기 조종
#ifdef __DEBUG__
	if (inputMgr.getKeyHoldTime('R') <= -1.0) {
		pJet->setPosY(10000.0);
	}
	if (inputMgr.getKeyHoldTime('T') <= -1.0) {
		pJet->setPosY(1500.0);
	}
	if (inputMgr.getKeyState('Q') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('Q') == key_state::KEY_DOWN) {
		pJet->setPosY(pJet->getPosY() - 1.0); // test
	}
	if (inputMgr.getKeyState('E') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('E') == key_state::KEY_DOWN) {
		pJet->setPosY(pJet->getPosY() + 1.0); // test
	}
#endif
	if (inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState(VK_UP) == key_state::KEY_DOWN) {
		pJet->pitching(-pJet->default_pitchTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState(VK_DOWN) == key_state::KEY_DOWN) {
		pJet->pitching(pJet->default_pitchTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState('A') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('A') == key_state::KEY_DOWN) {
		pJet->yawing(pJet->default_yawTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState('D') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('D') == key_state::KEY_DOWN) {
		pJet->yawing(-pJet->default_yawTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState(VK_LEFT) == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState(VK_LEFT) == key_state::KEY_DOWN) {
		pJet->rolling(pJet->default_rollTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState(VK_RIGHT) == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState(VK_RIGHT) == key_state::KEY_DOWN) {
		pJet->rolling(-pJet->default_rollTurn * timeElapsed, true);
	}
	if (inputMgr.getKeyState('W') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('W') == key_state::KEY_DOWN) { // 비행기 가속
		pJet->acceleration(timeElapsed);
	}
	if (inputMgr.getKeyState('S') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('S') == key_state::KEY_DOWN) { // 비행기 감속
		pJet->deacceleration(timeElapsed);
	}
	else {
		pJet->isDeaccelerating = false;
	}
	if (inputMgr.getKeyState('X') == key_state::KEY_DOWN) {
		pJet->boosting(timeElapsed);
	}
	if (inputMgr.getKeyState('Y') == key_state::KEY_DOWN) {
		if (bExitUIIsShown == true) {
			bExitUIIsShown = false;
			sessionLeftTime = 0.00f;

			// 멀티플레이 모드였다면 서버 디스커넥션 메시지 전송 후 싱글플레이 모드로 전환
			if (bMultiplayMode == true) {
				GamePacket toServerPacket;
				toServerPacket.dwFrameTime = g_pGameManager->getElapsedTime();
				toServerPacket.sPlayerNumber = pOwnerPlayer->getPlayerNumber();
				toServerPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
				pNetworkManager->sendMsgToServer(&toServerPacket);
				bMultiplayMode = false;
			}
		}
	}
	if (inputMgr.getKeyState('N') == key_state::KEY_DOWN) {
		if (bExitUIIsShown == true) {
			bExitUIIsShown = false;
		}
	}

	// 무기 사용 (Use Network)
	if (inputMgr.getKeyState(' ') == key_state::KEY_DOWN) {
		if (bMultiplayMode == true) { // 멀티플레이의 경우
			playerControlVector.push_back(2);
		}
		pJet->getFirstWeaponPtr()->fireWeapon();
	}
	if (inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_DOWN || inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_DOWNHOLDING) {
		if (bMultiplayMode == true) { // 멀티플레이의 경우
			playerControlVector.push_back(3);
		}
		pJet->getBaseWeaponPtr()->fireWeapon();
	}
	if (inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_UP) {
		if (bMultiplayMode == true) { // 멀티플레이의 경우
			playerControlVector.push_back(4);
		}
		pJet->getBaseWeaponPtr()->stopFireWeapon();
	}
	// 미사일 추적 카메라
	if (inputMgr.getKeyHoldTime(' ') <= -0.5) {
		pOwnerPlayer->setCamChacingMissile(true);
	}
	if (inputMgr.getKeyState(' ') == key_state::KEY_UP) {
		pOwnerPlayer->setCamChacingMissile(false);
	}
	// 후방 카메라
	if (inputMgr.getKeyState('C') == key_state::KEY_DOWN) {
		pOwnerPlayer->setCamShowBehind(true);
	}
	if (inputMgr.getKeyState('C') == key_state::KEY_UP) {
		pOwnerPlayer->setCamShowBehind(false);
	}
	// 락온 타겟 변경 (Use Network)
	if (inputMgr.getKeyState('\t') == key_state::KEY_DOWN) {
		if (pJet->getFirstWeaponPtr() != NULL) {
			if (bMultiplayMode == true) { // 멀티플레이의 경우
				playerControlVector.push_back(1);
			}
			pJet->getFirstWeaponPtr()->wpnTgtChangeRequest();
		}
	}
	// 카메라 시점 변경
	if (inputMgr.getKeyState('P') == key_state::KEY_DOWN) {
		pOwnerPlayer->getCameraOption() != camera_option::CAM_FIRST_PERSON ? pOwnerPlayer->setCameraOption(camera_option::CAM_FIRST_PERSON) :
			pOwnerPlayer->setCameraOption(camera_option::CAM_THIRD_PERSON);
	}
}

bool CombatFrame::updateFrame(REAL timeElapsed) {
	POINT mousePos = pInputManager->getMousePos();
	InputManager &inputMgr = *pInputManager;
	GameCamera *pCam = pOwnerPlayer->getPlayerCamera();
	JetFighter *pJet = pOwnerPlayer->getPlayerJet();
	SoundManager &soundMgr = *(g_pGameManager->getSoundManagerPtr());

	/* 프레임 초기화 */
	drawList.clear();	// 드로우 리스트 초기화 => 위젯들 업데이트 후 프레임 업데이트 마지막 부분에서 위젯들을 다시 추가

	/* 키 입력 업데이트 */
	updateKey(timeElapsed);

	/* 멀티플레이시 네트워크 업데이트 */
	if (bMultiplayMode == true) {
		updateNetwork(timeElapsed);
	}

	/* 모든 위젯 업데이트 */
	for (list<GameWidget*>::iterator iter = widgetList.begin(); iter != widgetList.end();) {
		// 업데이트에서 false인 경우는 위젯 삭제를 요청하는 경우 뿐임
		if ((*iter)->update(timeElapsed) == false) {
			// 전투기나 미사일의 경우에는 FlightObjectList에서도 지워주도록 함
			int widgetType = (*iter)->getWidgetType();
			if (widgetType == GameWidget::WIDGET_JET || widgetType == GameWidget::WIDGET_MISSILE) { // 비행체의 경우 비행체 리스트에서도 제거
				flightObjList.remove(static_cast<FlightObject*>(*iter));
			}

			delete (*iter);

			iter = widgetList.erase(iter);
		}
		else {
			++iter;
		}
	}
	/* 도움말 위젯 업데이트 */
	if (bHelpUIIsShown == true) {
		pHelpUIViewer->setVisible(true);
		pFadeoutViewer->setVisible(true);
		pFadeoutViewer->setColorAlpha(0.75);
	}
	else {
		pHelpUIViewer->setVisible(false);
		pFadeoutViewer->setVisible(false);
		pFadeoutViewer->setColorAlpha(0.0);
	}
	/* 종료 위젯 업데이트 */
	if (bExitUIIsShown == true) {
		pExitUIViewer->setVisible(true);
		pFadeoutViewer->setVisible(true);
		pFadeoutViewer->setColorAlpha(0.75);
	}
	else {
		pExitUIViewer->setVisible(false);
		pFadeoutViewer->setVisible(false);
		pFadeoutViewer->setColorAlpha(0.0);
	}

	/* 플레이어 업데이트 */
	for (list<Player*>::iterator iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		(*iterPlayer)->updatePlayer(timeElapsed);

		// 프레임 오너 플레이어의 경우 UI위젯을 프레임 위젯 리스트에 추가해 주면서 업데이트
		if ((*iterPlayer) == pOwnerPlayer) {
			// 오너 플레이어 카메라 업데이트
			(*iterPlayer)->updateCamera(timeElapsed);

			// 오너 플레이어 UI 업데이트
			(*iterPlayer)->updatePlayerUI(timeElapsed);

			// 드로우 리스트에 오너 플레이어 UI 추가
			list<GameWidget*> *uiList = (*iterPlayer)->getPlayerUIDrawListPtr();
			for (list<GameWidget*>::iterator iterUI = uiList->begin(); iterUI != uiList->end(); ++iterUI) {
				drawList.push_back((*iterUI));
			}
		}
		else {
			// AI 테스트
			if (bMultiplayMode == false) {
				if ((*iterPlayer)->getPlayerJet()->getPos().x > pTerrain->getScaledSize().cx / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().x < -pTerrain->getScaledSize().cx / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().z > pTerrain->getScaledSize().cy / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().z < -pTerrain->getScaledSize().cy / 4.0) // 맵 구역에서 많이 벗어나면 턴
					(*iterPlayer)->getPlayerJet()->yawing(8.0 * timeElapsed);
				else {
					if ((*iterPlayer)->getPlayerJet()->currentSpeed < 1000.0) {
						(*iterPlayer)->getPlayerJet()->acceleration(timeElapsed);
					}
				}
			}
		}
	}

	/* 전투기 무기 업데이트 (다른 전투기 위치등의 업데이트가 선 요구되어 여기에 위치) */
	for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
		if ((*iter)->getPlayerJet() != NULL) {
			if ((*iter)->getPlayerJet()->getBaseWeaponPtr() != NULL)
				(*iter)->getPlayerJet()->getBaseWeaponPtr()->update(timeElapsed);
			if ((*iter)->getPlayerJet()->getFirstWeaponPtr() != NULL)
				(*iter)->getPlayerJet()->getFirstWeaponPtr()->update(timeElapsed);
		}
	}

	/* 이펙트 업데이트 */
	for (list<GL_Effect*>::iterator iterEffect = effectList.begin(); iterEffect != effectList.end();) {
		if ((*iterEffect)->update(timeElapsed) == false) {
			delete (*iterEffect);
			iterEffect = effectList.erase(iterEffect);
		}
		else {
			++iterEffect;
		}
	}
	
	/* 남은 세션 시간 감소 */
	if (bMultiplayMode == false) { // 싱글플레이의 경우 경과 시간에 따라 감소, 멀티플레이의 경우 서버 업데이트 명령에 동기화
		sessionLeftTime -= timeElapsed;
	}
	if (sessionLeftTime <= 0.0) {
		sessionLeftTime = 0.0;

		// 처음 세션 만료 시
		if (sessionExpired == false) {
			sessionExpired = true;
			bHelpUIIsShown = false;
			bExitUIIsShown = false;
			new Sound(pCam->getPosition(), "res/Sounds/missionAlarm.wav", false, false);

			// 멀티플레이 모드였다면 서버 디스커넥션 메시지 전송 후 싱글플레이 모드로 전환
			if (bMultiplayMode == true) {
				GamePacket toServerPacket;
				toServerPacket.dwFrameTime = g_pGameManager->getElapsedTime();
				toServerPacket.sPlayerNumber = pOwnerPlayer->getPlayerNumber();
				toServerPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
				pNetworkManager->sendMsgToServer(&toServerPacket);
				bMultiplayMode = false;
			}
		}

		// 배경 음악 사운드를 줄임
		pBackgroundSound->setGain(pBackgroundSound->getGain() - (0.25 * timeElapsed));

		// 세션(프레임) 파괴
		if (pOwnerPlayer->getUImanagerPtr()->getMissionMsgPtr()->getSessionDestructLeftTime() <= 0.00) { // UI로부터 세션 만료까지 남은 시간을 받아옴
			// 다음 프레임에 플레이어를 전달하기 위해 벡터에 복사
			Player *pOwner = pOwnerPlayer;
			vector<Player*> playerVec;
			for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
				playerVec.push_back(*iter); // 플레이어 포인터를 벡터에 담아 전달
			}

			// 본인 파괴
			delete this;

			// 다음 프레임 설정 (주의!: 항상 본인 파괴 후 다음 프레임을 지정해 주어야 한다.)
			g_pGameManager->setGameFrame(new ResultFrame(playerVec, pOwner));

			// 세션 만료
			return true;
		}
	}

	/* 전투기와 미사일들을 그리기 리스트에 추가 (프레임 소유주 플레이어의 비행체는 항상 마지막에 추가) */
	list<FlightObject*> playerObjList;
	for (list<FlightObject*>::iterator iterFlight = flightObjList.begin(); iterFlight != flightObjList.end(); ++iterFlight) {
		if ((*iterFlight)->pOwningPlayer != this->pOwnerPlayer) {
			drawList.push_back(*iterFlight);
		}
		else {
			playerObjList.push_back(*iterFlight);
		}
	}
	for (list<FlightObject*>::iterator iterPlayerObj = playerObjList.begin(); iterPlayerObj != playerObjList.end(); ++iterPlayerObj) {
		drawList.push_back(*iterPlayerObj);
	}
	/* 도움말 UI와 페이드아웃 뷰어를 그리기 리스트에 추가 */
	if (pFadeoutViewer != NULL) drawList.push_back(pFadeoutViewer);	// 페이드아웃 뷰어는 마지막쯤에 
	if (pHelpUIViewer != NULL) drawList.push_back(pHelpUIViewer);	// 도움말 뷰어는 그 다음에
	if (pExitUIViewer != NULL) drawList.push_back(pExitUIViewer);	// 종료 뷰어는 가장 마지막에

	return true;
}

list<FlightObject*>* CombatFrame::getFlightObjListPtr() {
	return &flightObjList;
}
