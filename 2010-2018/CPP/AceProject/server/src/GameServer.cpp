#include "GameServer.h"
#include "DataParser.h"

float SESSION_LPS = 50.0f;
int SESSION_START_DELAY = 15000;
int SESSION_FINISH_TIME = 300000;
float SESSION_PLAYERSYNC_WAITING_TIME = 3000.0f;

GameServer::GameServer() {
	printf("[System] ����(%p) ������ ������ �����մϴ�.\n", this);

	/* ���� ���� ���� */
	int err = -1;			// ���� ��ȣ�� �����ϱ� ���� ����

	/* Settings.dat�κ��� ������ �Ľ� */
	DataParser parser;
	if (parser.openFile("dat/Settings.dat") == true) {
		if (parser.findHeader("ServerSettings") == true) {
			if (parser.findAttribute("TargetLPS") == true) {
				SESSION_LPS = parser.parseFloatFromAttribute();
			}
			if (parser.findAttribute("SessionStartDelay") == true) {
				SESSION_START_DELAY = parser.parseIntFromAttribute();
			}
			if (parser.findAttribute("SessionFinishTime") == true) {
				SESSION_FINISH_TIME = parser.parseIntFromAttribute();
			}
			if (parser.findAttribute("SessionPlayerSyncWaitingTime") == true) {
				SESSION_PLAYERSYNC_WAITING_TIME = parser.parseIntFromAttribute();
			}
		}
	}
	parser.printDataBuffer();

	/* ���� �ʱ�ȭ */
	initalizeForLobby();
	bSessionStarted = false;									// ���� ���� ���� �ʱ�ȭ
	bSessionStarting = false;									// ���� ������ ���� �ʱ�ȭ
	dwSessionStartLeftTime = SESSION_START_DELAY;				// ���� ���� �⺻ ��� 30��
	dwSessionFinsihLeftTime = SESSION_FINISH_TIME;				// ���� ���� �ð� �⺻ 600�� (�׽�Ʈ 60��)
	dwServerElapsedTime = 0;									// ���� ���� �ð� �ʱ�ȭ
	rSessionLPSbyFixedTime = (1.0 / SESSION_LPS) * 1000.0;		// ���� ������Ʈ �ð�
	memset(&wsaData, 0, sizeof(wsaData));						// WSADATA ����ü �ʱ�ȭ
	usPortNumber = 30549;										// ����� ��Ʈ ��ȣ ����
	iServerSocket = -1;											// ���� ���� ��ȣ �ʱ�ȭ
	memset(&serverAddr, 0, sizeof(serverAddr));					// ������ ���� ����ü �ʱ�ȭ
	clientInfoVector.reserve(GameServer::MAX_CLIENT);			// Ŭ���̾�Ʈ ���� ũ�� ����
	clientInfoVector.clear();									// Ŭ���̾�Ʈ ���� �ʱ�ȭ
	clientPacketVector.reserve(GameServer::MAX_CLIENT * 10);	// Ŭ���̾�Ʈ ��Ŷ ���� ũ�� ����
	clientPacketVector.clear();									// ���� �޽��� ���� �ʱ�ȭ
	iLastAssignedClientNumber = 0;								// ���������� �ο��� Ŭ���̾�Ʈ ��ȣ �ʱ�ȭ
	printf("[System] ����(%p) ���� �ʱ�ȭ �Ϸ�.\n", this);

	/* WSADATA ������ ���� 2.0dll ��� �ʱ�ȭ �� ���� ó�� */
	if ((err = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) {
		// ���� ��� �ʱ�ȭ ����
		switch (err) {
		case WSASYSNOTREADY:
			throw "Server::Server() - ����: ��Ʈ��ũ ���� �غ� �����Ͽ����ϴ�.\n";
			break;
		case WSAVERNOTSUPPORTED:
			throw "Server::Server() - ����: ��û�� ������ ���� ������ �������� �ʽ��ϴ�.\n";
			break;
		case WSAEINPROGRESS:
			throw "Server::Server() - ����: ���ŷ ������ �������Դϴ�.\n";
			break;
		case WSAEPROCLIM:
			throw "Server::Server() - ����: ���� ������ ���� ������ �ʰ��Ǿ����ϴ�.\n";
			break;
		case WSAEFAULT:
			throw "Server::Server() - ����: �� ��° �μ��� �����Ͱ� ��ȿ�Ǿ����ϴ�.\n";
			break;
		default:
			throw "Server::Server() - ����: �� �� ���� ������ �߻��Ͽ����ϴ�.\n";
		}
	}
	printf("[System] ����(%p) WSADATA �ʱ�ȭ �Ϸ�.\n", this);

	/* �ڽ��� IP�ּҸ� ȣ��Ʈ �̸��� ���� �޾ƿ� */
	PHOSTENT hostInfo;
	char hostName[64];
	char ipAddr[16];
	memset(hostName, 0, sizeof(hostName));
	memset(ipAddr, 0, sizeof(ipAddr));
	if ((err = gethostname(hostName, sizeof(hostName))) == 0) {
		hostInfo = gethostbyname(hostName);
		strcpy(ipAddr, inet_ntoa(*(struct in_addr*)hostInfo->h_addr_list[0]));
	}

	/* ���� �ּ� ����ü �ʱ�ȭ */
	serverAddr.sin_family = AF_INET;					// IPv4 ȯ������ ���
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// ���� ���� �ڽ��� �ּҸ� ���� IP�� ���
	serverAddr.sin_port = htons(usPortNumber);			// ���� ���Ͽ��� ����� ��Ʈ ��ȣ
	printf("[System] ����(%p) sockaddr_in ����ü �ʱ�ȭ �Ϸ�. (HOST: %s, IP: %s, PORT: %d)\n", this, hostName, ipAddr, usPortNumber);

	/* ���� �߽��� ���� UDP ���� ���� ���� */
	if ((iServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		// ���� ���� ����
		throw "Server::Server() - ����: ���� ������ �����Ͽ����ϴ�.\n";
	}
	printf("[System] ����(%p) ���� ���� �Ϸ�. (���� �ڵ�: %d)\n", this, iServerSocket);

	/* ������ UDP ������ Non-Blocking �������� ���� */
	u_long sockMode = 1; // [ 0: Blocking, 1: Non-Blocking ]
	if ((err = ioctlsocket(iServerSocket, FIONBIO, &sockMode)) == SOCKET_ERROR) {
		throw "Server::Server() - ����: ���� �Ӽ� ������ �����Ͽ����ϴ�.\n";
	}
	printf("[System] ����(%p) ���� Non-Blocking �Ӽ� ���� �Ϸ�.\n", this);

	/* ���� �ּ� ������ ������ ���ε� */
	if ((err = bind(iServerSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) == -1) {
		throw "Server::Server() - ����: ���� ���ε��� �����Ͽ����ϴ�.\n";
	}
	printf("[System] ����(%p) ���� ���ε� �Ϸ�.\n", this);
	printf("[System] ���� ���� ��� �ð�: %d\n", SESSION_START_DELAY);
	printf("[System] ���� LPS: %f\n", SESSION_LPS);
	printf("[System] ���� ���� �ð�: %d\n", SESSION_FINISH_TIME);
	
}

GameServer::GameServer(GameServer &obj) {
	/* ���� ������ ���� */
	bSessionStarted = obj.bSessionStarted;
	dwServerElapsedTime = obj.dwServerElapsedTime;
	memcpy_s(&wsaData, sizeof(wsaData), &obj.wsaData, sizeof(wsaData));
	usPortNumber = obj.usPortNumber;
	iServerSocket = obj.iServerSocket;
	memcpy_s(&serverAddr, sizeof(serverAddr), &obj.serverAddr, sizeof(serverAddr));
	clientInfoVector = obj.clientInfoVector;
	clientPacketVector = obj.clientPacketVector;
	iLastAssignedClientNumber = obj.iLastAssignedClientNumber;
}

GameServer::~GameServer() {
	/* ���� ���� ���� */
	int err = -1;			// ���� ��ȣ�� �����ϱ� ���� ����

	/* ���� ���� ���� */
	if ((err = closesocket(iServerSocket)) == SOCKET_ERROR) {
		printf_s("Server::~Server() - ���: ���� ��ȣ %d�� �ݱ� �۾��� �����Ͽ����ϴ�. (���� ��ȣ: %d)\n", iServerSocket, WSAGetLastError());
	}
	printf("[System] ����(%p) ����(%d) �ݱ� �Ϸ�.\n", this, iServerSocket);
	iServerSocket = -1;

	/* WSADATA ���ҽ� ��ȯ ��û */
	if ((err = WSACleanup()) == SOCKET_ERROR) {
		printf_s("Server::~Server() - ���: WSADATA ���ҽ� ��ȯ�� �����Ͽ����ϴ�. (���� ��ȣ: %d)\n", WSAGetLastError());
	}
	printf("[System] ����(%p) WSADATA ��ȯ �Ϸ�.\n", this);

	/* ��Ÿ �ɹ� ���� �ʱ�ȭ*/
	bSessionStarted = false;
	bSessionStarting = false;
	dwSessionStartLeftTime = 0;
	dwSessionFinsihLeftTime = 0;
	dwServerElapsedTime = 0;
	memset(&wsaData, 0, sizeof(wsaData));
	usPortNumber = 0;
	memset(&serverAddr, 0, sizeof(serverAddr));
	iLastAssignedClientNumber = 0;
	printf("[System] ����(%p) �ɹ� ���� �ʱ�ȭ �Ϸ�.", this);

	/* Ŭ���̾�Ʈ ���� ���� �� ���� �ʱ�ȭ */
	for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ++iter) {
		delete (*iter);
	}
	clientInfoVector.clear();
	printf("[System] ����(%p) Ŭ���̾�Ʈ ���� ���� ���� �Ϸ�.\n", this);

	/* �����ִ� ��Ŷ ���� ���� �� ���� �ʱ�ȭ */
	clientPacketVector.clear();
	printf("[System] ����(%p) ���� �޽��� ���� �Ϸ�.\n", this);
	printf("[System] ����(%p) �Ҹ��� ������ �Ϸ��Ͽ����ϴ�.\n", this);
}

void GameServer::initalizeForLobby() {
	bSessionStarted = false;									// ���� ���� ���� �ʱ�ȭ
	bSessionStarting = false;									// ���� ������ ���� �ʱ�ȭ
	dwSessionStartLeftTime = SESSION_START_DELAY;				// ���� ���� �⺻ ��� 30��
	dwSessionFinsihLeftTime = SESSION_FINISH_TIME;				// ���� ���� �ð� �⺻ 600�� (�׽�Ʈ 60��)
	for (int i = 0; clientInfoVector.size(); ++i) {				// Ŭ���̾�Ʈ ���� ����
		delete clientInfoVector[i];
	}
	clientInfoVector.clear();									// Ŭ���̾�Ʈ ���� �ʱ�ȭ
	clientPacketVector.clear();
	iLastAssignedClientNumber = 0;								// ���������� �ο��� Ŭ���̾�Ʈ ��ȣ �ʱ�ȭ
	printf("[System] ����(%p) �κ� ����� �غ� �ʱ�ȭ �Ϸ�.\n", this);
}

bool GameServer::startMainLoop() {
	bool exitMainLoop = false;
	DWORD dwCurrTime = 0;							// ������Ʈ�� ������ ���� �ð�
	DWORD dwLastUpdateTime = (DWORD)timeGetTime();	// ���������� ������Ʈ�� ����� �ð�
	DWORD dwTimeDelta = 0;							// �� �� �ð��� ��
	DWORD dwLastSessionTime = 0;					// ���������� ���� ���� ���� �ð�

	while (exitMainLoop == false) {
		/* ���� �ð� ������Ʈ */
		dwCurrTime = (DWORD)timeGetTime();
		dwTimeDelta = dwCurrTime - dwLastUpdateTime;
		dwServerElapsedTime += dwTimeDelta;
		dwLastUpdateTime = dwCurrTime;

		/* ������ ���۵��� �ʾ��� ��� Ŀ�ؼ� �� ��Ŀ�ؼ� ���� */
		if (bSessionStarted == false) {
			// ���� �ð� ����
			dwLastSessionTime = dwSessionStartLeftTime;
			dwSessionStartLeftTime -= dwTimeDelta;
			if (dwSessionStartLeftTime > dwLastSessionTime) { // ����!: ���� ���� �ð��� ����÷η� ���� ���� ġ���� ���
				bSessionStarting = true;	// ���� ������ Ȱ��ȭ
				printf("[Server] ���� ������ ������.\n");
			}

			// Ŭ���̾�Ʈ ���� �� ���� ����
			clientConnection(dwTimeDelta);
			checkSessionStart(dwTimeDelta);
		}
		/* ���� ����ȭ ���� */
		else {
			// (��� �÷��̾�κ��� �޽����� ���� ��� ����� �޽����� Ŭ���̾�Ʈ�鿡�� �ϰ�����)
			// (Ŭ���̾�Ʈ������ �޽����� �����κ��� ���� ��� ������Ʈ ���� �� ���� �����ӿ� ���� �޽����� ������ ����)
			// (��, ��� �÷��̾��� �������� ����ȭ�� �Ǹ�, ���� ���� �ӵ��� ���ۿ� ���� ������ ����ȭ�� �����)
			updateSession(dwTimeDelta);
		}
	}
	return true;
}

bool GameServer::clientConnection(DWORD timeElapsed) {
	/* ���� ���� */
	GamePacket fromClientPacket;				// Ŭ���̾�Ʈ�κ��� ������ ��Ŷ�� �ӽ÷� ���� ����ü
	GamePacket toClientPacket;					// Ŭ���̾�Ʈ�� ������ ��Ŷ
	sockaddr_in clientAddr;						// Ŭ���̾�Ʈ �ּҸ� �ӽ� ������ ����ü
	int clientAddrSize = sizeof(sockaddr_in);	// Ŭ���̾�Ʈ �ּ� ����ü ũ��
	char *pData = NULL;							// Byte ������ �ռ��� ���� ������

	/* Ŭ���̾�Ʈ�κ��� ���� ��Ŷ�� ���� */
	recvfrom(iServerSocket, (char*)&fromClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, &clientAddrSize);
	
	/* ��Ŷ�� Ŀ�ؼ� ��Ŷ���� Ȯ�� */
	if (fromClientPacket.enMsgType == GamePacket::MsgType::MSG_CONNECT) { // Ŀ�ؼ� ��û ��Ŷ�� ���
		// ������ ���۵Ǿ������� Ȯ��
		if (dwSessionStartLeftTime <= 0) {
			// ���� ���������� ���� ���� �Ұ� ��Ŷ ����
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_FAIL_STARTED; // �����κ��� MSG_CONNECT_FAIL_STARTED �� ���� �� ���� ���и� �ǹ�
			toClientPacket.sPlayerNumber = 0;

			// Ŭ���̾�Ʈ���� ����
			sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
			printf("[Server] Ŭ���̾�Ʈ %s�� ���� �ź� �޽��� ����: ���� ���۵�.\n", inet_ntoa(clientAddr.sin_addr));

			// ������ �˻� ����
			return true;
		}
		// �ο��� �� á������ Ȯ��
		else if (clientInfoVector.size() >= GameServer::MAX_CLIENT) {
			// �ο� �� ������ ���� ���� �Ұ� ��Ŷ ����
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MGS_CONNECT_FAIL_MAXPLAYER; // �����κ��� MGS_CONNECT_FAIL_MAXPLAYER �� ���� �� ���� ���и� �ǹ�
			toClientPacket.sPlayerNumber = 0;

			// Ŭ���̾�Ʈ���� ����
			sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
			printf("[Server] Ŭ���̾�Ʈ %s�� ���� �ź� �޽��� ����: ���� �ο� �ʰ�.\n", inet_ntoa(clientAddr.sin_addr));

			// ������ �˻� ����
			return true;
		}
		// �̹� ��ϵ� �÷��̾� ���̵������� Ȯ��
		else {
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (strcmp(clientInfoVector[i]->cClientName, fromClientPacket.byteData) == 0) { // ������ ID�� ������ Ȯ��
					// ���� �����̵�� ���� ���� �Ұ� ��Ŷ ����
					toClientPacket.dwFrameTime = dwServerElapsedTime;
					toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_FAIL_SAMEID; // �����κ��� MSG_CONNECT_FAIL_SAMEID �� ���� �� ���� ���и� �ǹ�
					toClientPacket.sPlayerNumber = 0;

					// Ŭ���̾�Ʈ���� ����
					sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
					printf("[Server] Ŭ���̾�Ʈ %s�� ���� �ź� �޽��� ����: ���̵� �ߺ�.\n", inet_ntoa(clientAddr.sin_addr));

					// ������ �˻� ����
					return true;
				}
			}
		}

		/* �� ���� �����ϸ� ���� ���� ������ Ŭ���̾�Ʈ ������ ���� */
		// ���̵� Ŭ���̾�Ʈ ���� ���Ϳ� ����ϰ� ���� ���� �޽����� �÷��̾� ��ȣ�� ����
		toClientPacket.dwFrameTime = dwServerElapsedTime;
		toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_SUCCESS; // �����κ��� MSG_CONNECT_SUCCESS �� ���� �� ���� ������ �˸�
		toClientPacket.sPlayerNumber = ++iLastAssignedClientNumber;			 // ���������� �ο��� �÷��̾� ��ȣ�� 1�� ������Ų ��ȣ�� �Ҵ�
		sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
		printf("[Server] ���� ���� �޽��� ���� Client IP: %s, PORT: %d\n", inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);
		
		// Ŭ���̾�Ʈ ������ ������ ����
		ClientInfo *pInfo = new ClientInfo;
		pInfo->bDataReceived = false;															// Ŭ���̾�Ʈ�κ��� ������ ���ſ��� �ʱ�ȭ
		strncpy(pInfo->cClientName, fromClientPacket.byteData, sizeof(char[9]));				// Ŭ���̾�Ʈ ���̵� ���� ����
		memcpy_s(&(pInfo->clientAddr), sizeof(sockaddr_in), &clientAddr, sizeof(sockaddr_in));	// Ŭ���̾�Ʈ �ּ� ���� ����
		pInfo->sClientNumber = toClientPacket.sPlayerNumber;									// Ŭ���̾�Ʈ���� �ο��� ��ȣ ����
		pInfo->dwLastRecvedTime = this->dwServerElapsedTime;									// Ŭ���̾�Ʈ�κ��� ���Ź޾��� �ñ��� ���� �ð� ����
		clientInfoVector.push_back(pInfo);														// Ŭ���̾�Ʈ ���� ���Ϳ� Ǫ�� ��
		printf("[Server] Ŭ���̾�Ʈ %s���� ��� �Ϸ�. �ο��� ��ȣ: %d ������ ��: %d\n", inet_ntoa(pInfo->clientAddr.sin_addr), (int)pInfo->sClientNumber, clientInfoVector.size());

		// ������ Ŭ���̾�Ʈ���� �̸� �����ߴ� �ٸ� Ŭ���̾�Ʈ ���� ����
		for (int i = 0; i < clientInfoVector.size(); ++i) {
			// �ڱ� �ڽ��� ������ �����ϰ� ����
			if (strcmp(pInfo->cClientName, clientInfoVector[i]->cClientName) != 0) {
				// �÷��̾� ���̵�, ��ȣ ������ ��� ��Ŷ ����
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_GIVE_CLIENTNAME;
				memcpy_s(pData, sizeof(char[9]), clientInfoVector[i]->cClientName, sizeof(char[9]));
				pData += sizeof(char[9]);
				memcpy_s(pData, sizeof(short), &clientInfoVector[i]->sClientNumber, sizeof(short));
				pData += sizeof(short);

				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(pInfo->clientAddr), clientAddrSize);
				printf("[Server] Ŭ���̾�Ʈ %s���� Ŭ���̾�Ʈ %s(%d)�� ������ ������.\n",
						pInfo->cClientName, clientInfoVector[i]->cClientName, (int)clientInfoVector[i]->sClientNumber);
			}
		}

		// �̸� �����ߴ� Ŭ���̾�Ʈ�鿡�� ������ Ŭ���̾�Ʈ�� ���� ����
		for (int j = 0; j < clientInfoVector.size(); ++j) {
			// ������ ������ ������ �ڱ� �ڽ��� ������ �ƴ� ��쿡�� ����
			if (strcmp(pInfo->cClientName, clientInfoVector[j]->cClientName) != 0) {
				// �÷��̾� ���̵�, ��ȣ ������ ��� ��Ŷ ����
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_GIVE_CLIENTNAME;	// Ŭ���̾�Ʈ �̸� �� ��ȣ ���� �޽���
				memcpy_s(pData, sizeof(char[9]), pInfo->cClientName, sizeof(char[9]));	// Ŭ���̾�Ʈ �̸� ����
				pData += sizeof(char[9]);
				memcpy_s(pData, sizeof(short), (short*)&pInfo->sClientNumber, sizeof(short)); // Ŭ���̾�Ʈ ��ȣ ����
				pData += sizeof(short);

				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), clientAddrSize);
				printf("[Server] Ŭ���̾�Ʈ %s���� Ŭ���̾�Ʈ %s(%d)�� ������ ������.\n",
						clientInfoVector[j]->cClientName, pInfo->cClientName, (int)pInfo->sClientNumber);
			}
		}
	}
	/* ��Ŷ�� ��Ŀ�ؼ� ��û ��Ŷ���� Ȯ�� */
	else if (fromClientPacket.enMsgType == GamePacket::MsgType::MSG_DISCONNECT) { // ��Ŀ�ؼ� ��û�� ���
		for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ++iter) {
			// ��ϵ� �÷��̾� ���̵� ã�� ��� �÷��̾� ���� ����
			if (strcmp((*iter)->cClientName, fromClientPacket.byteData) == 0) {
				// �÷��̾� ���� ����
				printf("[Server] Ŭ���̾�Ʈ %s(ID: %s) ���� �����.\n", inet_ntoa((*iter)->clientAddr.sin_addr), (*iter)->cClientName);
				delete (*iter);
				clientInfoVector.erase(iter);
				
				// �ٸ� Ŭ���̾�Ʈ�鿡�� ���� ����� Ŭ���̾�Ʈ�� ���� ����
				for (int j = 0; j < clientInfoVector.size(); ++j) {
					// �÷��̾� ���̵�, ��ȣ ������ ��� ��Ŷ ����
					pData = toClientPacket.byteData;
					toClientPacket.dwFrameTime = dwServerElapsedTime;
					toClientPacket.enMsgType = GamePacket::MsgType::MSG_REMOVE_CLIENTNAME; // Ŭ���̾�Ʈ �̸� �� ��ȣ ���� �޽���
					memcpy_s(pData, sizeof(char[9]), fromClientPacket.byteData, sizeof(char[9])); // Ŭ���̾�Ʈ �̸� ����
					pData += sizeof(char[9]);
					memcpy_s(pData, sizeof(short), (short*)&clientInfoVector[j]->sClientNumber, sizeof(short)); // Ŭ���̾�Ʈ ��ȣ ����
					pData += sizeof(short);

					// ���ӵ� ��� Ŭ���̾�Ʈ���� ����
					sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), clientAddrSize);
				}

				// ������ �˻� ����
				return true;
			}
		}
	}
	else { // �� �� ������ ��Ŷ�� ��� �ƹ� �ൿ�� ���� �ʰ� �Լ� ����
		return true;
	}
}

bool GameServer::checkSessionStart(DWORD timeElapsed) {
	/* ���� ���� �ʱ�ȭ */
	bool sendSessionStartMsgToClient = false;	// Ŭ���̾�Ʈ���� ���� ���� �޽����� �����ϴ� ����
	bool allClientReadyToStart = false;			// ��� Ŭ���̾�Ʈ�� ���� �� �غ� �Ǿ����� ����
	int clientAddrSize = sizeof(sockaddr_in);	// Ŭ���̾�Ʈ �ּ� ����ü ũ��
	int clientCnt = clientInfoVector.size();	// ���� ����� Ŭ���̾�Ʈ�� ��
	static DWORD dwLastUpdateTime = 0;			// ���������� ������Ʈ�� ������ �ð�
	DWORD dwCurrentTime = (DWORD)timeGetTime();	// ������Ʈ�� �����ϴ� ���� �ð�
	GamePacket toClientPacket;					// Ŭ���̾�Ʈ���� ���� ��Ŷ ����ü
	char *pData = NULL;							// ��Ŷ ����ü�� Byte �����͸� ����Ű�� ������

	/* ���� ��� ��: ���ӵ� �÷��̾ 1�� ������ �� ���� ���ð� �ʱ�ȭ */
	if (clientCnt < 1) {
		dwSessionStartLeftTime = SESSION_START_DELAY;
		sendSessionStartMsgToClient = false;
		bSessionStarted = false;
	}
	/* ���� ������ ���� �÷��̾� ��� ��: ���ӵ� �÷��̾�� ���� ���ð��� ����� ���� ���� ����ȭ��Ŵ */
	else if (bSessionStarting == false) {
		sendSessionStartMsgToClient = false;
	}
	/* ���� ���� ���� ����(Ŭ���̾�Ʈ�� 2�� �̻��̰� ���ð� ����): �÷��̾�� ���� ���� �޽��� ���� */
	else if (bSessionStarting == true) {
		dwSessionStartLeftTime = 0;
		sendSessionStartMsgToClient = true;
	}

	/* �� 1�ʸ��� �޽��� ���� */
	if (dwCurrentTime > (dwLastUpdateTime + 1000)) {
		dwLastUpdateTime = dwCurrentTime; // ���������� ������Ʈ �� �ð� ����

		/* ���� ���� ��� ���� ��� */
		if (sendSessionStartMsgToClient == false) {
			/* Ŭ���̾�Ʈ���� ���� ���� ���ð� �� ���ӵ� ������ ���� */
			for (int i = 0; i < clientCnt; ++i) {
				// Ŭ���̾�Ʈ���� ���� ���� ���ð� �� ���ӵ� ������ ���� ��Ŷ ����
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_SESSION_START_LEFTTIME; // ���� ���� �����ð� ����ȭ �޽���
				memcpy_s(pData, sizeof(DWORD), (DWORD*)&dwSessionStartLeftTime, sizeof(DWORD)); // ���� �ð�
				pData += sizeof(DWORD);
				memcpy_s(pData, sizeof(int), (int*)&clientCnt, sizeof(int)); // ���ӵ� �ο� ��
				pData += sizeof(int);

				// Ŭ���̾�Ʈ���� ����
				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), clientAddrSize);
				printf("[Server] Ŭ���̾�Ʈ %s(%s)���� ��� �޽��� ����. (�ð�: %d, �ο�: %d)\n",
						inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName, dwSessionStartLeftTime, clientInfoVector.size());
			}
		}
		/* ���� ���� �غ��� ��� */
		else {
			// ���� ������ ���� ���
			GamePacket tempPacket;
			sockaddr_in tempSockaddr;
			while (recvfrom(iServerSocket, (char*)&tempPacket, sizeof(GamePacket), 0, (sockaddr*)&tempSockaddr, &clientAddrSize) > 0) {}

			// Ŭ���̾�Ʈ���� ������ ��Ŷ ����
			REAL sessionLPS = SESSION_LPS;
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MSG_SESSION_START; // ���ӵ� Ŭ���̾�Ʈ�鿡�� ���� ���� �޽��� ����
			pData = toClientPacket.byteData;
			memcpy_s(pData, sizeof(REAL), &sessionLPS, sizeof(REAL));	// ���� ���� ������Ʈ �ֱ⸦ ����
			pData += sizeof(REAL);
			
			// Ŭ���̾�Ʈ�鿡�� ����
			for (int i = 0; i < clientCnt; ++i) {
				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), sizeof(sockaddr));
				printf("[Server] Ŭ���̾�Ʈ %s(%s)���� ���� ���� �޽��� ����.\n", inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName);
			}

			// Ŭ���̾�Ʈ���� ������ ���� �ð��� �ʱ�ȭ
			for (int j = 0; j < clientInfoVector.size(); ++j) {
				clientInfoVector[j]->bDataReceived = false;
				clientInfoVector[j]->dwLastRecvedTime = dwServerElapsedTime;
			}

			// ���� ���¸� ���� �������� ����
			bSessionStarted = true;		// ���۵���  TRUE �� ��ȯ
			bSessionStarting = false;	// �������� FALSE �� ��ȯ
			printf("\n\n[Server] ������ ���۵Ǿ����ϴ�. (TgtLPS: %f, ServerElapsedTime:%lf��)\n", sessionLPS, (double)(dwServerElapsedTime / 1000.0));

			// ������ ���¸� ���
			printf("[Server] ������ ��� (%d��) \n", clientInfoVector.size());
			printf("No.\tID\t\tIP\t\tNUM\n");
			for (int k = 0; k < clientInfoVector.size(); ++k) {
				printf("[%02d]\t%8s\t%15s\t%d\n", (k + 1), clientInfoVector[k]->cClientName,
						inet_ntoa(clientInfoVector[k]->clientAddr.sin_addr), (int)clientInfoVector[k]->sClientNumber);
			}
			printf("\n");
		}
	}
	else {
		// ... 1�ʰ� �� �Ǵ� ��쿡�� IDLE ���
	}

	return true;
}

bool GameServer::updateSession(DWORD timeElapsed) {
	// ���� ����
	GamePacket recvPacket;
	GamePacket sendPacket;
	sockaddr_in clientAddr;
	int iSockaddrSize = sizeof(sockaddr);
	DWORD dwRecvStartTime = 0;

	// ����� Ŭ���̾�Ʈ�� ���� Ȯ���Ͽ� ���ӵ� Ŭ���̾�Ʈ�� ���� �� ���� ����
	if (clientInfoVector.size() < 1) {
		bSessionStarted = false;
		initalizeForLobby();
		printf("\n[Server] ���ӵ� Ŭ���̾�Ʈ�� ���� ������ ����Ǹ�, ���� ���� ��� ���·� ���ư��ϴ�.\n\n");
		return true;
	}

	// �ִ� 0.5�� ���� Ŭ���̾�Ʈ�κ��� ���� �޽��� ���� �� ���� �� ����
	dwRecvStartTime = (DWORD)timeGetTime();
	while (recvfrom(iServerSocket, (char*)&recvPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, &iSockaddrSize) > 0) {
		// Ŭ���̾�Ʈ�� ����ȭ �޽����� ������ ���
		if (recvPacket.enMsgType == GamePacket::MsgType::MSG_CLIENT_SYNC) {
			// ������ �÷��̾��� ������ ���Ϳ��� ã��
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (clientInfoVector[i]->sClientNumber == recvPacket.sPlayerNumber) {
					// �̹� ������Ʈ�� �ش� �÷��̾�κ��� ó�� �����͸� ������ ���
					if (clientInfoVector[i]->bDataReceived == false) {
						clientInfoVector[i]->bDataReceived = true;	// �����͸� ���������� ����
						clientPacketVector.push_back(recvPacket);	// ������ �����͸� ��Ŷ ���Ϳ� Ǫ�� ��
						printf("[Time:%lu] �÷��̾� %d�κ��� ù ����ȭ ���� �Ϸ�.\n", this->dwSessionFinsihLeftTime, recvPacket.sPlayerNumber); // test
					}

					// Ŭ���̾�Ʈ���� �޽��� ������ �޽��� ����, ���������� ������ �ð� ����, ������ �÷��̾�� �˻� ����
					sendPacket.dwFrameTime = dwServerElapsedTime;
					sendPacket.sPlayerNumber = recvPacket.sPlayerNumber;
					sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_WAIT;
					sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), iSockaddrSize);
					clientInfoVector[i]->dwLastRecvedTime = dwServerElapsedTime;
					break;
				}
			}
		}
		// Ŭ���̾�Ʈ�κ��� ��Ŀ��Ʈ �޽����� ������ ���
		else if (recvPacket.enMsgType == GamePacket::MsgType::MSG_DISCONNECT) {
			// �ý��� �޽��� ���
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (clientInfoVector[i]->sClientNumber == recvPacket.sPlayerNumber) {
					printf("[Server] Ŭ���̾�Ʈ %s (%s)�� ������ �����Ͽ����ϴ�. (Time: %lf)\n",
							inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName, (dwServerElapsedTime / 1000.0));

					// �ٸ� �÷��̾�鿡�� ��Ŀ�ؼ� �޽��� ����
					for (int j = 0; j < clientInfoVector.size(); ++j) {
						if (i != j) {
							sendPacket.dwFrameTime = dwServerElapsedTime;
							sendPacket.sPlayerNumber = recvPacket.sPlayerNumber;
							sendPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
							sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);
						}
					}
					break;
				}
			}
			// �ش� �÷��̾� ��ȣ�� Ȯ�� �� Ŭ���̾�Ʈ ���Ϳ��� ����
			removeClientInfo(recvPacket.sPlayerNumber);
		}

		// ������ ���� �� ó���ϴ� �ð��� 0.5�ʸ� �ʰ��ϴ��� �˻��Ͽ� �ʰ��Ͽ����� ���� �۾� �˻縦 ���� Ż��
		if ((DWORD)timeGetTime > dwRecvStartTime + 500) break;
	}

	// ��� Ŭ���̾�Ʈ�� �����͸� �����ߴ��� Ȯ��
	bool isCompleted = true;
	for (int i = 0; i < clientInfoVector.size(); ++i) {
		if (clientInfoVector[i]->bDataReceived == false) {
			isCompleted = false;
			printf("[Time:%lu] �÷��̾� %s�� ���� ����ȭ ���� �����.\n", this->dwSessionFinsihLeftTime, clientInfoVector[i]->cClientName); // test
			break;
		}
	}
	// ��� Ŭ���̾�Ʈ���� �����͸� ���������� �������� ��Ŷ �����ͷ� �ϰ� ������Ʈ�� �ϵ��� �޽��� ����
	if (isCompleted == true) {
		dwSessionFinsihLeftTime -= rSessionLPSbyFixedTime;	// ���� ������ ����Ʈ�� ���� ���� �ð��� ������Ʈ�� ����
		if (dwSessionFinsihLeftTime > SESSION_FINISH_TIME) dwSessionFinsihLeftTime = 0; // ����÷� ����

		for (int j = 0; j < clientInfoVector.size(); ++j) {
			// ����� ����ȭ �޽����� ����
			for (int k = 0; k < clientPacketVector.size(); ++k) {
				// �ڱⰡ ������ �ڽ��� ����ȭ ������ ����
				if (clientInfoVector[j]->sClientNumber != clientPacketVector[k].sPlayerNumber) {
					sendto(iServerSocket, (char*)&clientPacketVector[k], sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);
				}
			}

			// ������Ʈ �޽��� ����
			sendPacket.dwFrameTime = dwSessionFinsihLeftTime; // ���� ���� �ð��� ����
			sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
			sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_DOUPDATE;
			sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);
			
			// �ش� Ŭ���̾�Ʈ�� ������Ʈ ���� �ʱ�ȭ
			clientInfoVector[j]->bDataReceived = false;
			clientInfoVector[j]->dwLastRecvedTime = dwServerElapsedTime;
		printf("[Time:%lu] ��� �÷��̾� �޽��� ���� �Ϸ�. ����ȭ �޽����� ������Ʈ �޽��� ����.\n\n", this->dwSessionFinsihLeftTime); // test
		}

		// �������̴� �޽����� ����
		clientPacketVector.clear();
	}
	// ��� Ŭ���̾�Ʈ�κ��� �����͸� ���Ź��� ���� ��쿡�� ��Ⱓ(3.0�� �ʰ�) ����ȭ ������ ���� Ŭ���̾�Ʈ�� ã�Ƽ� ��Ŀ��Ʈ ��Ŵ
	else {
		for (int j = 0; j < clientInfoVector.size(); ++j) {
			if (clientInfoVector[j]->bDataReceived == false && (clientInfoVector[j]->dwLastRecvedTime + SESSION_PLAYERSYNC_WAITING_TIME) < dwServerElapsedTime) {
				// Ŭ���̾�Ʈ���� ű �޽��� ����
				sendPacket.dwFrameTime = dwServerElapsedTime;
				sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
				sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_KICKED;
				sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);

				// �ý��� �޽���
				printf("[Server] Ŭ���̾�Ʈ %s (%s)�� %f�ʰ� �޽��� ������ ���� �߹�Ǿ����ϴ�.\n",
						inet_ntoa(clientInfoVector[j]->clientAddr.sin_addr), clientInfoVector[j]->cClientName,
						(float)(dwServerElapsedTime - clientInfoVector[j]->dwLastRecvedTime) / 1000.0);

				// �ٸ� �÷��̾�� �÷��̾� �߹�� �޽��� ����
				for (int k = 0; k < clientInfoVector.size(); ++k) {
					if (j != k) {
						sendPacket.dwFrameTime = dwServerElapsedTime;
						sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
						sendPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
						sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[k]->clientAddr), iSockaddrSize);
					}
				}

				// �ش� Ŭ���̾�Ʈ�� ���Ϳ��� ����
				removeClientInfo(clientInfoVector[j]->sClientNumber);
			}
		}
	}

	return true;
}

void GameServer::removeClientInfo(short number) {
	for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ) {
		if ((*iter)->sClientNumber == number) {
			printf("[Server] Ŭ���̾�Ʈ ID: %s / IP: %s / NUM: %hd �� ���� ���� ����. (���� Ŭ���̾�Ʈ: %d)\n",
					(*iter)->cClientName, inet_ntoa((*iter)->clientAddr.sin_addr)), (*iter)->sClientNumber, clientInfoVector.size() - 1;
			
			delete (*iter);

			iter = clientInfoVector.erase(iter);
			break;
		}
		else {
			++iter;
		}
	}
}
