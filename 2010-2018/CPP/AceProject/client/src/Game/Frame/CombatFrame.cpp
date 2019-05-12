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

CombatFrame::CombatFrame(Player *pLobbyPlayer) : GameFrame(TEXT("CombatFrame_Single")) { // �̱��÷��� ������
	// ��Ƽ�÷��� ��� ����
	dwLastRecvTime = 0;
	bMultiplayMode = false;
	bServerDisconnected = true;
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	playerControlVector.clear();

	// ���� ���� �� ���� ǥ�� ���� �ʱ�ȭ
	sessionExpired = false;
	bHelpUIIsShown = false;
	bExitUIIsShown = false;

	// ���� ���� �ð� ����
	sessionLeftTime = 300.0; // ����Ʈ 300��

	// ���� �� �ϴû��� �߰� (����!: �� ���� �׻� WidgetList�� �߰����� �ʴ´�)
	pTerrain = new Terrain(35.0, "res/Textures/map/terrainTexture.png", "res/textures/map/heightmap.jpg", 0.0, 850.0, 32);
	pSkybox = new Skybox();

	// ��� ���� ����
	setBackgroundSound(new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Alect_Squadron.wav", false, true));
	getBackgroundSoundPtr()->setGain(0.5);

	// �÷��̾� ���� 
	int playerNumber = 0;																				// �÷��̾� ��ȣ
	Player *pPlayer = new Player(pLobbyPlayer->getPlayerNumber(), pLobbyPlayer->getPlayerName());		// �÷��̾� ����
	Vector3 pos;
	pos.x = 5000.0;
	pos.y = (REAL)mymath::randomInt(2500, 4000);
	pos.z = 0.0;
	Vector3 centerDirection = Vector3(0.0, pos.y, 0.0) - pos;			// �߾��� ���ϴ� ���� ����
	centerDirection.normalize();
	JetFighter *pJet = new JetFighter(pos, pPlayer, "F-22 Raptor");		// ������ ��ü ����
	pPlayer->setPlayerJet(pJet);										// �÷��̾� ������ ������� ����
	pPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);      // �߾��� �ٶ󺸵��� �ʱ�ȭ
	playerList.push_back(pPlayer);										// ���� �����ӿ� �÷��̾� �߰�
	addWidget(pJet);													// ���� �����ӿ� ������ �߰�
	pOwnerPlayer = pPlayer;												// ������ ���� �÷��̾� ����

	// ���޹��� �÷��̾� ��ü �ı�
	delete pLobbyPlayer;
	pLobbyPlayer = NULL;

	// �׽�Ʈ�� AI�÷��̾� ����
	char num;
	const int TEST_PLAYER = 7;
	for (int i = 0; i < TEST_PLAYER; ++i) {
		num = i + 48;
		pPlayer = new Player(playerNumber++, string("Tester") + num);	// �÷��̾� ����
		pos.x = cosf(mymath::deg2rad((i + 1) * (360.0 / (TEST_PLAYER + 1)))) * 5000.0;
		pos.y = (REAL)mymath::randomInt(2500, 4000);
		pos.z = sinf(mymath::deg2rad((i + 1) * (360.0 / (TEST_PLAYER + 1)))) * 5000.0;
		centerDirection = Vector3(0.0, pos.y, 0.0) - pos;				// �߾��� ���ϴ� ���� ����
		centerDirection.normalize();
		pJet = new JetFighter(pos, pPlayer, "F-22 Raptor");				// ������ ��ü ����
		pPlayer->setPlayerJet(pJet);									// �÷��̾� ������ ������� ����
		pPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);	// �߾��� �ٶ󺸵��� �ʱ�ȭ
		playerList.push_back(pPlayer);									// ���� �����ӿ� �÷��̾� �߰�
		addWidget(pJet);												// ���� �����ӿ� ������ �߰�
	}

	// �߰� ��� ���� ( �� ������ ������ ���� ����Ʈ�� �����ʰ� ���� )
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

CombatFrame::CombatFrame(Player *pLobbyPlayer, vector<Player*> *pPlayerVector, REAL newTgtLPS) : GameFrame(TEXT("CombatFrame_Multi")) { // ��Ƽ�÷��� ������
	/* ���� �õ� �缳�� */
	srand(pLobbyPlayer->getPlayerNumber() + mymath::randomInt(1, 100000));

	/* ���� �ʱ�ȭ */
	pGameManager->setTargetLPS(newTgtLPS);
	dwLastRecvTime = 0;
	bMultiplayMode = true;
	bServerDisconnected = false;
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	playerControlVector.reserve(32);
	playerControlVector.clear();
	sessionExpired = false;
	sessionLeftTime = 600.0;   // ����Ʈ 600�� (���������� ����ȭ ����)
	bHelpUIIsShown = false;
	bExitUIIsShown = false;
	playerList.clear();

	/* ���� �� �ϴû��� �߰� (����!: �� ���� �׻� WidgetList�� �߰����� �ʴ´�) */
	pTerrain = new Terrain(35.0, "res/Textures/map/terrainTexture.png", "res/textures/map/heightmap.jpg", 0.0, 850.0, 32);
	pSkybox = new Skybox();

	/* ��� ���� ���� */
	setBackgroundSound(new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/BGM_Alect_Squadron.wav", false, true));
	getBackgroundSoundPtr()->setGain(0.5);

	/* �÷��̾� �������� �÷��̾� ��ȣ�� �ӽ� ���� */
	int iOwnPlayerNum = pLobbyPlayer->getPlayerNumber();

	/* �ٸ� �÷��̾� ������ ������Ͽ� ����Ʈ�� ���� */
	// (����!: Player Ŭ������ �ɹ��� UImaster Ŭ������ �����ڿ��� ���� �������� � ���������� �����ϰ� ��.)
	// (���� �÷��̾ ���� �����ӿ��� �������� �ʴ� ��� �ı��� �������� �����ͷ� ����Ű�� �ְ� ��.)
	Player *pNewPlayer = NULL;
	for (vector<Player*>::iterator iterVector = pPlayerVector->begin(); iterVector != pPlayerVector->end(); ++iterVector) {
		pNewPlayer = new Player((*iterVector)->getPlayerNumber(), (*iterVector)->getPlayerName());	// �÷��̾� ������ ����
		delete (*iterVector);																		// �ٸ� �����ӿ��� ������ �÷��̾� ���� ����
		playerList.push_back(pNewPlayer);															// ����� �÷��̾� ���� ����

		if (pNewPlayer->getPlayerNumber() == iOwnPlayerNum) pOwnerPlayer = pNewPlayer;				// �÷��̾� ��ȣ�� ������ �÷��̾��� ��ȣ�� ��� ������ �÷��̾ ������ �÷��̾��
	}
	delete pPlayerVector;   // �÷��̾� ������ ���� �Ϸ�Ǿ����Ƿ� ���� ������ �÷��̾� ���� �Ҵ� ����
	pLobbyPlayer = NULL;
	pNewPlayer = NULL;

	/* �÷��̾���� ������ ���� */
	Vector3 posCenter(0.0, 0.0, 0.0);
	Vector3 pos, centerDirection;
	JetFighter *pJet = NULL;
	Player *pTempPlayer = NULL;
	int i = 0;

	for (list<Player*>::iterator iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		pTempPlayer = (*iterPlayer);
		
		// �÷��̾� ������ ���� �ð� �������� ���ư��鼭 ������ ��ǥ ����
		pos.x = cosf(mymath::deg2rad((i + 1) * (360.0 / playerList.size()))) * 5000.0;
		pos.y = (REAL)mymath::randomInt(2500, 4000);
		pos.z = sinf(mymath::deg2rad((i + 1) * (360.0 / playerList.size()))) * 5000.0;
		centerDirection = Vector3(0.0, pos.y, 0.0) - pos;					// �߾��� ���ϴ� ���� ����
		++i;

		// ������ ����
		centerDirection.normalize();
		pJet = new JetFighter(pos, pTempPlayer, "F-22 Raptor");				// ������ ��ü ����
		pTempPlayer->setPlayerJet(pJet);									// �÷��̾� ������ ������� ����
		pTempPlayer->getPlayerJet()->respawnFigther(pos, centerDirection);	// �߾��� �ٶ󺸵��� �ʱ�ȭ
		addWidget(pJet);													// ���� �����ӿ� ������ �߰�

		// �׽�Ʈ �޽���
	#ifdef __DEBUG__
		cout << "Version: " << GAME_VERSION << endl;
		cout << "PlayerList: " << (*iterPlayer)->getPlayerName() << "(" << (*iterPlayer)->getPlayerNumber() << ") / Jet_ID: " << (*iterPlayer)->getPlayerJet()->getID() << endl; // test
	#endif
	}

	// �߰� ��� ���� ( �� ������ ������ ���� ����Ʈ�� �����ʰ� ���� )
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
	// Ÿ�� ������ �ʱ�ȭ
	pGameManager->setTargetLPS(50.0);

	// ��� �ı�
	if (pHelpUIViewer != NULL) delete pHelpUIViewer;
	if (pExitUIViewer != NULL) delete pExitUIViewer;
	if (pFadeoutViewer != NULL) delete pFadeoutViewer;
	pHelpUIViewer = NULL;
	pExitUIViewer = NULL;
	pFadeoutViewer = NULL;

	// �� �ΰ��� ������ ������ �������� GameFrame ���� �����Ѵ�.
	// �÷��̾���� ResultFrame ���� ����, �ʿ��� ������ ��� �� �����Ѵ�.
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
	// ���� ���� ����
	char *pPtr = NULL;										// ������ ������ ����ȭ ��Ŷ ������ ���� ������
	DWORD dwElapsedTime = g_pGameManager->getElapsedTime();	// ���α׷� ��� �ð�
	DWORD dwLastSendTime = 0;								// ���������� ������ ������ �ð�
	bool bReadyForUpdate = false;							// ������Ʈ ���� �غ� ����
	list<Player*>::iterator iterPlayer;						// �÷��̾� ����Ʈ�� ��ȸ�ϱ� ���� �ݺ��� ����
	GamePacket recvPacket;									// �����κ��� ���� ��Ŷ �����͸� ������ ���� ����
	GamePacket sendPacket;									// ������ ������ ��Ŷ
	static int iUpdateWaitLimitCnt = 0;						// ������Ʈ �ִ� ��� �ð� ī����

	// ���� �ʱ�ȭ
	iUpdateWaitLimitCnt = 0;
	dwLastRecvTime = (DWORD)timeGetTime();
	for (iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		if (pOwnerPlayer->getPlayerNumber() == (*iterPlayer)->getPlayerNumber()) {
			(*iterPlayer)->setNetworkUpdated(true); // �ڱ� �ڽ����κ��ʹ� ����ȭ�� �ʿ� ������ ��
		}
		else if ((*iterPlayer)->getNetworkDisconnected() == true) {
			(*iterPlayer)->setNetworkUpdated(true);	// ���� ���� ��� ����ȭ�� �ʿ� ������ ��
		}
		else {
			(*iterPlayer)->setNetworkUpdated(false); // �� �� �÷��̾�� �÷��̾� ������Ʈ�� ���� �ʱ�ȭ
		}
	}

	// ���� ��Ŷ �ʱ�ȭ
	sendPacket.dwFrameTime = dwElapsedTime;
	sendPacket.sPlayerNumber = pOwnerPlayer->getPlayerNumber();
	sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_SYNC;
	
	// �÷��̾� ������ ������ ����
	pPtr = pOwnerPlayer->getPlayerJet()->toGamePacket(&sendPacket);
	
	// �÷��̾� ��� ������ ����
	for (int i = 0; i < playerControlVector.size(); ++i) {
		memcpy_s(pPtr, sizeof(int), &playerControlVector[i], sizeof(int));
		pPtr += sizeof(int);
	}

	// ������ ��Ŀ��Ʈ ���� Ȯ��
	if (bServerDisconnected == false) {
		// ���� ������Ʈ ����� ���ŵ��� �ʾҰų�, ��� �÷��̾��� ������Ʈ ��Ŷ�� �������� ���� ���
		while (bReadyForUpdate == false) {
			// �����κ��� 5�� �Ѱ� �޽��� �ƹ��� �޽��� ������ ���� ��� ��Ŀ��Ʈ ��ƾ ����
			// (������������ �ٸ� �÷��̾��� MSG_CLIENT_SYNC �޽����� ��� ���۵Ǳ� ����� �� MSG_CLIENT_WAIT �޽����� �������ش�)
			if ((DWORD)timeGetTime() > dwLastRecvTime + 5000) {
				// Ŭ���̾�Ʈ ��Ŀ�ؼ� ���·� ��ȯ
				bServerDisconnected = true;

				// ���� ���� �ð��� 0���� �����Ͽ� ���� ���� ����
				sessionLeftTime = 0.00;

				// ������ ������ �������� �ʰ� ����, ������ ������Ʈ�� �����ϸ鼭 �������� �����
				return;
			}
			// �����κ��� �޽����� ���������� �����ϰ� �ִ� ���
			else {
				// �����κ��� ���� Ŭ���̾�Ʈ ������ ���� ���θ� Ȯ�ι��� ���� ��� �� 0.05~0.1�� ���� ������ ����
				if (bRecvSendMsgFromServer == false && (DWORD)timeGetTime() > (dwLastSendTime + mymath::randomInt(50, 100))) {
					// ����� ����ȭ �޽��� ��Ŷ�� ������ ����
					pNetworkManager->sendMsgToServer(&sendPacket);
					dwLastSendTime = (DWORD)timeGetTime();
					cout << "[LeftTime:" << this->sessionLeftTime << "] �����κ��� ����ȭ�Ϸ� ��ȣ�� ���� ���� 0.1�� ������ ����." << endl; // test 
				}
				// �����κ��� ���� ���θ� Ȯ�ι��� ���Ĵ� 1.0 �ʸ��� ����ȭ �޽����� ���� (Ŭ���̾�Ʈ �����͸� ���� �޾Ҵ� ������ �� �޽����� ���۹����� MSG_CLIENT_WAIT �޽����� ������ �ش�)
				else if ((DWORD)timeGetTime() > dwLastSendTime + 1000) {
					pNetworkManager->sendMsgToServer(&sendPacket);
					dwLastSendTime = (DWORD)timeGetTime();
					cout << "[LeftTime:" << this->sessionLeftTime << "] ������ 1.0�� �ֱ� ����ȭ �޽��� ����." << endl; // test
				}
				// �����κ��� ���� �޽����� ���� �� ���� ���� ����
				while (pNetworkManager->recvMsgFromServer(&recvPacket) > 0) {
					// ���Ź��� �޽����� �м��Ͽ� Ŭ���̾�Ʈ�� ����
					translatePacket(&recvPacket, timeElapsed);
					dwLastRecvTime = (DWORD)timeGetTime();
				}
				// ��� �÷��̾�κ��� ����ȭ ������Ʈ �Ǿ����� Ȯ��
				bool bAllPlayerUpdated = true;
				for (iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
					if ((*iterPlayer)->getNetworkUpdated() == false) {
						bAllPlayerUpdated = false;
						printf("[LeftTime:%f] �÷��̾� %s(Num:%d)�� ������ ������Ʈ ��Ŷ�� ���� �������� �ʾҽ��ϴ�.\n", this->sessionLeftTime, (*iterPlayer)->getPlayerName(), (*iterPlayer)->getPlayerNumber()); // test
						break;
					}
				}
				// ������Ʈ ���� ���� �˻�
				if (bAllPlayerUpdated == true && bRecvUpdateMsgFromServer == true) {
					// �����κ��� ������Ʈ �㰡�� �ް�, ��� �÷��̾��� ����ȭ ������Ʈ�� �Ϸ�� ��� while���� Ż���ϸ� ������Ʈ ����
					bReadyForUpdate = true;
				}
				// �����κ��� ������Ʈ ����� �޾����� ��� �÷��̾��� ������Ʈ�� ���� �ʰ� ������Ʈ�� 0.1�� �̻� �����Ǵ� ��� ������ ������Ʈ ���� ����
				else if (bRecvUpdateMsgFromServer == true && bAllPlayerUpdated == false) {
					if (iUpdateWaitLimitCnt > 5) {
						bReadyForUpdate = true;
						printf("[LeftTime:%f] �÷��̾� ����ȭ ��� ��� �ʰ��� ���� ������Ʈ ����.", this->sessionLeftTime);
					}
				}
				else {
					cout << "[LeftTime:" << this->sessionLeftTime << "] ������Ʈ �Ұ�."; // test
					if (bAllPlayerUpdated == false) cout << " - ��� �÷��̾��� ������Ʈ�� ������� ����."; // test 
					if (bRecvUpdateMsgFromServer == false) cout << " - �����κ��� ������Ʈ �޽��� �������� ����.\n"; // test 
				}
			}

			++iUpdateWaitLimitCnt; // �� ������ �� ȸ ������ Ȯ��
		}
	}

	// ���⿡ �����ߴٸ� �Լ��� Ż���ϸ鼭 Ŭ���̾�Ʈ ������Ʈ�� �����.
	// ���� ������ ���� �� �ɹ� ������ �ʱ�ȭ.
	bRecvSendMsgFromServer = false;
	bRecvUpdateMsgFromServer = false;
	// ����ȭ�� ����Ǿ� �ϴ� ����� ���� ���͸� ���
	playerControlVector.clear();
	cout << "[LeftTime:" << this->sessionLeftTime << "] �ݺ� ȸ��(" << iUpdateWaitLimitCnt - 1 << "������Ʈ ����. \n"; // test
}

void CombatFrame::translatePacket(GamePacket *pPacket, REAL timeElapsed) {
	switch (pPacket->enMsgType) {
	// �����κ��� �����(���� ����) �޽���
	case GamePacket::MsgType::MSG_CLIENT_KICKED:
		bRecvUpdateMsgFromServer = true;	// ������Ʈ�� ������ �� �ֵ��� ����
		bServerDisconnected = true;			// ������ ���� ��Ŀ��Ƽ�� ���·� ��ȯ��
		sessionLeftTime = 0.00;				// ���� �ð��� ����Ǿ� ���� ��Ŀ��Ƽ�� �޽��� ��� �� ������ ���� ����
		break;

	// �ٸ� Ŭ���̾�Ʈ Ż����(���� ����) �޽���
	case GamePacket::MsgType::MSG_DISCONNECT:
		for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {	// �÷��̾� ����Ʈ �� �ش� ��ȣ�� �÷��̾ �ִ��� Ȯ��
			if ((*iter)->getPlayerNumber() == (int)pPacket->sPlayerNumber) {
				(*iter)->getPlayerJet()->setHiding(true);	// �ش� �÷��̾��� �����⸦ ��Ȱ��ȭ ó��
				(*iter)->setNetworkDisconnected(true);		// �ش� �÷��̾��� ��Ʈ��ũ ���� ���¸� ��Ŀ�ؼ����� ����
			}
		}
		break;

	// Ŭ���̾�Ʈ ��� �޽��� (�� �޽����� ������ Ŭ���̾�Ʈ�� ������Ʈ �޽��� ���ű��� ���� �۾��� ����)
	case GamePacket::MsgType::MSG_CLIENT_WAIT:
		bRecvSendMsgFromServer = true;
		break;
	
	// Ŭ���̾�Ʈ ����ȭ �޽��� (���� �Լ��� ��ȣ�� �Ű����� ���� ���۵�)
	case GamePacket::MsgType::MSG_CLIENT_SYNC:
		// Ŭ���̾�Ʈ -> ���� : 1. ����ȭ �޽��� ó�� ���� �� : �÷��̾��� ��ǥ, ����� ��ɵ��� ����.
		//						2. ����ȭ �޽��� �ι� ° ���� : �������� ���� Ŭ���̾�Ʈ�� ����Ǿ� ������ ����.
		// ���� -> Ŭ���̾�Ʈ : �ٸ� Ŭ���̾�Ʈ�� ����ȭ �ؾ� �� ��ɵ��� ����.
		for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
			if ((*iter)->getPlayerNumber() == pPacket->sPlayerNumber) {
				char *pBytePtr = NULL;
				// ������ ����ȭ
				pBytePtr = (*iter)->getPlayerJet()->fromGamePacket(pPacket);

				// ��� ����ȭ
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
						// ��Ŷ���� ������ ����
						pBytePtr = NetworkManager::unpackFromByteData(&argi[0], pBytePtr, sizeof(int));	// �Ű����� 1 : ���� ���� �÷��̾� ��ȣ ����
						pBytePtr = NetworkManager::unpackFromByteData(&argf, pBytePtr, sizeof(REAL));	// �Ű����� 2 : ���ط� ����
						pBytePtr = NetworkManager::unpackFromByteData(&argi[1], pBytePtr, sizeof(int));	// �Ű����� 3 : �ڻ� ���� ����
						// �÷��̾� ����Ʈ���� ������ Ÿ���� �Ǵ� �÷��̾��� �����⸦ ã��
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
				// �ش� �÷��̾� ������Ʈ ���� ����
				(*iter)->setNetworkUpdated(true);
				
				// �ٸ� �÷��̾� �˻� ����
				break;
			}
		}
		break;
	
	// Ŭ���̾�Ʈ ������Ʈ ��� �޽��� (�� �޽����� ���޹ޱ� �������� ������ ���Ÿ� �ݺ�)
	case GamePacket::MsgType::MSG_CLIENT_DOUPDATE:
		sessionLeftTime = (REAL)(pPacket->dwFrameTime / 1000.0);	// ������Ʈ ��ɿ����� ���� �ð��� �ƴ� ���� ���� �ð��� dwFrameTime�� ��Ƶ�
		bRecvUpdateMsgFromServer = true;
		break;
	
	// �� �� ��Ŷ �ջ� ������ �̻��� ���۵� �� ���� ó�� ����
	default:
		// ...
		;
	}
}

void CombatFrame::updateKey(REAL timeElapsed) {
	InputManager &inputMgr = *pInputManager;
	JetFighter *pJet = pOwnerPlayer->getPlayerJet();

#ifdef __DEBUG__
	// ���� ����
	if (bMultiplayMode == false) {
		if (inputMgr.getKeyState('1') == key_state::KEY_DOWN) {
			for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
				cout << "Player" << (*iter)->getPlayerNumber() << ": " << (*iter)->getPlayerName() << endl;
			}
			list<Player*>::iterator iterPlayer = find(playerList.begin(), playerList.end(), pOwnerPlayer); // �÷��̾� �ڽ��� ã��
			if (++iterPlayer != playerList.end()) { // ���� �÷��̾ �����ϸ� �ش� �÷��̾ ���� �÷��̾�� ��ȯ
				pOwnerPlayer = (*iterPlayer);
			}
			else {
				iterPlayer = playerList.begin(); // ���� �÷��̾ ������ ���� ù��° �÷��̾ ���� �÷��̾�� ��ȯ
				pOwnerPlayer = (*iterPlayer);
			}
		}
	}
	// ���� ���̾������� ���
	if (inputMgr.getKeyState('T') == key_state::KEY_DOWN) {
		if (pTerrain->getTextureID() != 0) pTerrain->setTextureID(0);
		else pTerrain->setTextureID(pResourceManager->getTextureID("res/Textures/map/terrainTexture.png"));
	}
	// ������ �ʱ�ȭ
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
	// ����
	if (inputMgr.getKeyState(VK_F1) == key_state::KEY_DOWN) {
		if (sessionExpired == false) bHelpUIIsShown = !(bHelpUIIsShown);
	}
	// ����
	if (inputMgr.getKeyState(VK_ESCAPE) == key_state::KEY_DOWN) {
		// ���� �ݱ�
		if (bExitUIIsShown == true) bExitUIIsShown = false;

		// ���� ����
		else if (sessionExpired == false) bExitUIIsShown = !(bExitUIIsShown);
	}
	// ����� ����
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
	if (inputMgr.getKeyState('W') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('W') == key_state::KEY_DOWN) { // ����� ����
		pJet->acceleration(timeElapsed);
	}
	if (inputMgr.getKeyState('S') == key_state::KEY_DOWNHOLDING || inputMgr.getKeyState('S') == key_state::KEY_DOWN) { // ����� ����
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

			// ��Ƽ�÷��� ��忴�ٸ� ���� ��Ŀ�ؼ� �޽��� ���� �� �̱��÷��� ���� ��ȯ
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

	// ���� ��� (Use Network)
	if (inputMgr.getKeyState(' ') == key_state::KEY_DOWN) {
		if (bMultiplayMode == true) { // ��Ƽ�÷����� ���
			playerControlVector.push_back(2);
		}
		pJet->getFirstWeaponPtr()->fireWeapon();
	}
	if (inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_DOWN || inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_DOWNHOLDING) {
		if (bMultiplayMode == true) { // ��Ƽ�÷����� ���
			playerControlVector.push_back(3);
		}
		pJet->getBaseWeaponPtr()->fireWeapon();
	}
	if (inputMgr.getKeyState(VK_SHIFT) == key_state::KEY_UP) {
		if (bMultiplayMode == true) { // ��Ƽ�÷����� ���
			playerControlVector.push_back(4);
		}
		pJet->getBaseWeaponPtr()->stopFireWeapon();
	}
	// �̻��� ���� ī�޶�
	if (inputMgr.getKeyHoldTime(' ') <= -0.5) {
		pOwnerPlayer->setCamChacingMissile(true);
	}
	if (inputMgr.getKeyState(' ') == key_state::KEY_UP) {
		pOwnerPlayer->setCamChacingMissile(false);
	}
	// �Ĺ� ī�޶�
	if (inputMgr.getKeyState('C') == key_state::KEY_DOWN) {
		pOwnerPlayer->setCamShowBehind(true);
	}
	if (inputMgr.getKeyState('C') == key_state::KEY_UP) {
		pOwnerPlayer->setCamShowBehind(false);
	}
	// ���� Ÿ�� ���� (Use Network)
	if (inputMgr.getKeyState('\t') == key_state::KEY_DOWN) {
		if (pJet->getFirstWeaponPtr() != NULL) {
			if (bMultiplayMode == true) { // ��Ƽ�÷����� ���
				playerControlVector.push_back(1);
			}
			pJet->getFirstWeaponPtr()->wpnTgtChangeRequest();
		}
	}
	// ī�޶� ���� ����
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

	/* ������ �ʱ�ȭ */
	drawList.clear();	// ��ο� ����Ʈ �ʱ�ȭ => ������ ������Ʈ �� ������ ������Ʈ ������ �κп��� �������� �ٽ� �߰�

	/* Ű �Է� ������Ʈ */
	updateKey(timeElapsed);

	/* ��Ƽ�÷��̽� ��Ʈ��ũ ������Ʈ */
	if (bMultiplayMode == true) {
		updateNetwork(timeElapsed);
	}

	/* ��� ���� ������Ʈ */
	for (list<GameWidget*>::iterator iter = widgetList.begin(); iter != widgetList.end();) {
		// ������Ʈ���� false�� ���� ���� ������ ��û�ϴ� ��� ����
		if ((*iter)->update(timeElapsed) == false) {
			// �����⳪ �̻����� ��쿡�� FlightObjectList������ �����ֵ��� ��
			int widgetType = (*iter)->getWidgetType();
			if (widgetType == GameWidget::WIDGET_JET || widgetType == GameWidget::WIDGET_MISSILE) { // ����ü�� ��� ����ü ����Ʈ������ ����
				flightObjList.remove(static_cast<FlightObject*>(*iter));
			}

			delete (*iter);

			iter = widgetList.erase(iter);
		}
		else {
			++iter;
		}
	}
	/* ���� ���� ������Ʈ */
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
	/* ���� ���� ������Ʈ */
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

	/* �÷��̾� ������Ʈ */
	for (list<Player*>::iterator iterPlayer = playerList.begin(); iterPlayer != playerList.end(); ++iterPlayer) {
		(*iterPlayer)->updatePlayer(timeElapsed);

		// ������ ���� �÷��̾��� ��� UI������ ������ ���� ����Ʈ�� �߰��� �ָ鼭 ������Ʈ
		if ((*iterPlayer) == pOwnerPlayer) {
			// ���� �÷��̾� ī�޶� ������Ʈ
			(*iterPlayer)->updateCamera(timeElapsed);

			// ���� �÷��̾� UI ������Ʈ
			(*iterPlayer)->updatePlayerUI(timeElapsed);

			// ��ο� ����Ʈ�� ���� �÷��̾� UI �߰�
			list<GameWidget*> *uiList = (*iterPlayer)->getPlayerUIDrawListPtr();
			for (list<GameWidget*>::iterator iterUI = uiList->begin(); iterUI != uiList->end(); ++iterUI) {
				drawList.push_back((*iterUI));
			}
		}
		else {
			// AI �׽�Ʈ
			if (bMultiplayMode == false) {
				if ((*iterPlayer)->getPlayerJet()->getPos().x > pTerrain->getScaledSize().cx / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().x < -pTerrain->getScaledSize().cx / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().z > pTerrain->getScaledSize().cy / 4.0 ||
					(*iterPlayer)->getPlayerJet()->getPos().z < -pTerrain->getScaledSize().cy / 4.0) // �� �������� ���� ����� ��
					(*iterPlayer)->getPlayerJet()->yawing(8.0 * timeElapsed);
				else {
					if ((*iterPlayer)->getPlayerJet()->currentSpeed < 1000.0) {
						(*iterPlayer)->getPlayerJet()->acceleration(timeElapsed);
					}
				}
			}
		}
	}

	/* ������ ���� ������Ʈ (�ٸ� ������ ��ġ���� ������Ʈ�� �� �䱸�Ǿ� ���⿡ ��ġ) */
	for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
		if ((*iter)->getPlayerJet() != NULL) {
			if ((*iter)->getPlayerJet()->getBaseWeaponPtr() != NULL)
				(*iter)->getPlayerJet()->getBaseWeaponPtr()->update(timeElapsed);
			if ((*iter)->getPlayerJet()->getFirstWeaponPtr() != NULL)
				(*iter)->getPlayerJet()->getFirstWeaponPtr()->update(timeElapsed);
		}
	}

	/* ����Ʈ ������Ʈ */
	for (list<GL_Effect*>::iterator iterEffect = effectList.begin(); iterEffect != effectList.end();) {
		if ((*iterEffect)->update(timeElapsed) == false) {
			delete (*iterEffect);
			iterEffect = effectList.erase(iterEffect);
		}
		else {
			++iterEffect;
		}
	}
	
	/* ���� ���� �ð� ���� */
	if (bMultiplayMode == false) { // �̱��÷����� ��� ��� �ð��� ���� ����, ��Ƽ�÷����� ��� ���� ������Ʈ ��ɿ� ����ȭ
		sessionLeftTime -= timeElapsed;
	}
	if (sessionLeftTime <= 0.0) {
		sessionLeftTime = 0.0;

		// ó�� ���� ���� ��
		if (sessionExpired == false) {
			sessionExpired = true;
			bHelpUIIsShown = false;
			bExitUIIsShown = false;
			new Sound(pCam->getPosition(), "res/Sounds/missionAlarm.wav", false, false);

			// ��Ƽ�÷��� ��忴�ٸ� ���� ��Ŀ�ؼ� �޽��� ���� �� �̱��÷��� ���� ��ȯ
			if (bMultiplayMode == true) {
				GamePacket toServerPacket;
				toServerPacket.dwFrameTime = g_pGameManager->getElapsedTime();
				toServerPacket.sPlayerNumber = pOwnerPlayer->getPlayerNumber();
				toServerPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
				pNetworkManager->sendMsgToServer(&toServerPacket);
				bMultiplayMode = false;
			}
		}

		// ��� ���� ���带 ����
		pBackgroundSound->setGain(pBackgroundSound->getGain() - (0.25 * timeElapsed));

		// ����(������) �ı�
		if (pOwnerPlayer->getUImanagerPtr()->getMissionMsgPtr()->getSessionDestructLeftTime() <= 0.00) { // UI�κ��� ���� ������� ���� �ð��� �޾ƿ�
			// ���� �����ӿ� �÷��̾ �����ϱ� ���� ���Ϳ� ����
			Player *pOwner = pOwnerPlayer;
			vector<Player*> playerVec;
			for (list<Player*>::iterator iter = playerList.begin(); iter != playerList.end(); ++iter) {
				playerVec.push_back(*iter); // �÷��̾� �����͸� ���Ϳ� ��� ����
			}

			// ���� �ı�
			delete this;

			// ���� ������ ���� (����!: �׻� ���� �ı� �� ���� �������� ������ �־�� �Ѵ�.)
			g_pGameManager->setGameFrame(new ResultFrame(playerVec, pOwner));

			// ���� ����
			return true;
		}
	}

	/* ������� �̻��ϵ��� �׸��� ����Ʈ�� �߰� (������ ������ �÷��̾��� ����ü�� �׻� �������� �߰�) */
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
	/* ���� UI�� ���̵�ƿ� �� �׸��� ����Ʈ�� �߰� */
	if (pFadeoutViewer != NULL) drawList.push_back(pFadeoutViewer);	// ���̵�ƿ� ���� �������뿡 
	if (pHelpUIViewer != NULL) drawList.push_back(pHelpUIViewer);	// ���� ���� �� ������
	if (pExitUIViewer != NULL) drawList.push_back(pExitUIViewer);	// ���� ���� ���� ��������

	return true;
}

list<FlightObject*>* CombatFrame::getFlightObjListPtr() {
	return &flightObjList;
}
