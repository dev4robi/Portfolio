#include "GameServer.h"
#include "DataParser.h"

float SESSION_LPS = 50.0f;
int SESSION_START_DELAY = 15000;
int SESSION_FINISH_TIME = 300000;
float SESSION_PLAYERSYNC_WAITING_TIME = 3000.0f;

GameServer::GameServer() {
	printf("[System] 서버(%p) 생성자 수행을 시작합니다.\n", this);

	/* 지역 변수 선언 */
	int err = -1;			// 에러 번호를 저장하기 위한 변수

	/* Settings.dat로부터 데이터 파싱 */
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

	/* 변수 초기화 */
	initalizeForLobby();
	bSessionStarted = false;									// 세션 시작 여부 초기화
	bSessionStarting = false;									// 세션 시작중 여부 초기화
	dwSessionStartLeftTime = SESSION_START_DELAY;				// 세션 시작 기본 대기 30초
	dwSessionFinsihLeftTime = SESSION_FINISH_TIME;				// 세션 종료 시간 기본 600초 (테스트 60초)
	dwServerElapsedTime = 0;									// 서버 구동 시간 초기화
	rSessionLPSbyFixedTime = (1.0 / SESSION_LPS) * 1000.0;		// 세션 업데이트 시간
	memset(&wsaData, 0, sizeof(wsaData));						// WSADATA 구조체 초기화
	usPortNumber = 30549;										// 사용할 포트 번호 설정
	iServerSocket = -1;											// 서버 소켓 번호 초기화
	memset(&serverAddr, 0, sizeof(serverAddr));					// 서버측 소켓 구조체 초기화
	clientInfoVector.reserve(GameServer::MAX_CLIENT);			// 클라이언트 벡터 크기 설정
	clientInfoVector.clear();									// 클라이언트 벡터 초기화
	clientPacketVector.reserve(GameServer::MAX_CLIENT * 10);	// 클라이언트 패킷 벡터 크기 설정
	clientPacketVector.clear();									// 게임 메시지 벡터 초기화
	iLastAssignedClientNumber = 0;								// 마지막으로 부여된 클라이언트 번호 초기화
	printf("[System] 서버(%p) 변수 초기화 완료.\n", this);

	/* WSADATA 윈도우 소켓 2.0dll 사용 초기화 및 오류 처리 */
	if ((err = WSAStartup(MAKEWORD(2, 0), &wsaData)) != 0) {
		// 소켓 사용 초기화 실패
		switch (err) {
		case WSASYSNOTREADY:
			throw "Server::Server() - 실패: 네트워크 접속 준비를 실패하였습니다.\n";
			break;
		case WSAVERNOTSUPPORTED:
			throw "Server::Server() - 실패: 요청된 윈도우 소켓 버전이 지원되지 않습니다.\n";
			break;
		case WSAEINPROGRESS:
			throw "Server::Server() - 실패: 블로킹 조작이 수행중입니다.\n";
			break;
		case WSAEPROCLIM:
			throw "Server::Server() - 실패: 지원 가능한 소켓 개수가 초과되었습니다.\n";
			break;
		case WSAEFAULT:
			throw "Server::Server() - 실패: 두 번째 인수의 포인터가 무효되었습니다.\n";
			break;
		default:
			throw "Server::Server() - 실패: 알 수 없는 오류가 발생하였습니다.\n";
		}
	}
	printf("[System] 서버(%p) WSADATA 초기화 완료.\n", this);

	/* 자신의 IP주소를 호스트 이름을 통해 받아옴 */
	PHOSTENT hostInfo;
	char hostName[64];
	char ipAddr[16];
	memset(hostName, 0, sizeof(hostName));
	memset(ipAddr, 0, sizeof(ipAddr));
	if ((err = gethostname(hostName, sizeof(hostName))) == 0) {
		hostInfo = gethostbyname(hostName);
		strcpy(ipAddr, inet_ntoa(*(struct in_addr*)hostInfo->h_addr_list[0]));
	}

	/* 서버 주소 구조체 초기화 */
	serverAddr.sin_family = AF_INET;					// IPv4 환경으로 통신
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// 현재 서버 자신의 주소를 소켓 IP로 사용
	serverAddr.sin_port = htons(usPortNumber);			// 서버 소켓에서 사용할 포트 번호
	printf("[System] 서버(%p) sockaddr_in 구조체 초기화 완료. (HOST: %s, IP: %s, PORT: %d)\n", this, hostName, ipAddr, usPortNumber);

	/* 수신 발신을 위한 UDP 서버 소켓 생성 */
	if ((iServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		// 소켓 생성 실패
		throw "Server::Server() - 실패: 소켓 생성을 실패하였습니다.\n";
	}
	printf("[System] 서버(%p) 소켓 생성 완료. (소켓 핸들: %d)\n", this, iServerSocket);

	/* 생성된 UDP 소켓을 Non-Blocking 소켓으로 설정 */
	u_long sockMode = 1; // [ 0: Blocking, 1: Non-Blocking ]
	if ((err = ioctlsocket(iServerSocket, FIONBIO, &sockMode)) == SOCKET_ERROR) {
		throw "Server::Server() - 실패: 소켓 속성 설정에 실패하였습니다.\n";
	}
	printf("[System] 서버(%p) 소켓 Non-Blocking 속성 설정 완료.\n", this);

	/* 서버 주소 정보와 소켓을 바인딩 */
	if ((err = bind(iServerSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) == -1) {
		throw "Server::Server() - 실패: 소켓 바인딩을 실패하였습니다.\n";
	}
	printf("[System] 서버(%p) 소켓 바인딩 완료.\n", this);
	printf("[System] 세션 시작 대기 시간: %d\n", SESSION_START_DELAY);
	printf("[System] 세션 LPS: %f\n", SESSION_LPS);
	printf("[System] 세션 진행 시간: %d\n", SESSION_FINISH_TIME);
	
}

GameServer::GameServer(GameServer &obj) {
	/* 복사 생성자 수행 */
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
	/* 지역 변수 선언 */
	int err = -1;			// 에러 번호를 저장하기 위한 변수

	/* 서버 소켓 닫음 */
	if ((err = closesocket(iServerSocket)) == SOCKET_ERROR) {
		printf_s("Server::~Server() - 경고: 소켓 번호 %d의 닫기 작업이 실패하였습니다. (오류 번호: %d)\n", iServerSocket, WSAGetLastError());
	}
	printf("[System] 서버(%p) 소켓(%d) 닫기 완료.\n", this, iServerSocket);
	iServerSocket = -1;

	/* WSADATA 리소스 반환 요청 */
	if ((err = WSACleanup()) == SOCKET_ERROR) {
		printf_s("Server::~Server() - 경고: WSADATA 리소스 반환에 실패하였습니다. (오류 번호: %d)\n", WSAGetLastError());
	}
	printf("[System] 서버(%p) WSADATA 반환 완료.\n", this);

	/* 기타 맴버 변수 초기화*/
	bSessionStarted = false;
	bSessionStarting = false;
	dwSessionStartLeftTime = 0;
	dwSessionFinsihLeftTime = 0;
	dwServerElapsedTime = 0;
	memset(&wsaData, 0, sizeof(wsaData));
	usPortNumber = 0;
	memset(&serverAddr, 0, sizeof(serverAddr));
	iLastAssignedClientNumber = 0;
	printf("[System] 서버(%p) 맴버 변수 초기화 완료.", this);

	/* 클라이언트 정보 삭제 및 벡터 초기화 */
	for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ++iter) {
		delete (*iter);
	}
	clientInfoVector.clear();
	printf("[System] 서버(%p) 클라이언트 소켓 정보 삭제 완료.\n", this);

	/* 남아있는 패킷 벡터 삭제 및 벡터 초기화 */
	clientPacketVector.clear();
	printf("[System] 서버(%p) 남은 메시지 삭제 완료.\n", this);
	printf("[System] 서버(%p) 소멸자 수행을 완료하였습니다.\n", this);
}

void GameServer::initalizeForLobby() {
	bSessionStarted = false;									// 세션 시작 여부 초기화
	bSessionStarting = false;									// 세션 시작중 여부 초기화
	dwSessionStartLeftTime = SESSION_START_DELAY;				// 세션 시작 기본 대기 30초
	dwSessionFinsihLeftTime = SESSION_FINISH_TIME;				// 세션 종료 시간 기본 600초 (테스트 60초)
	for (int i = 0; clientInfoVector.size(); ++i) {				// 클라이언트 정보 삭제
		delete clientInfoVector[i];
	}
	clientInfoVector.clear();									// 클라이언트 벡터 초기화
	clientPacketVector.clear();
	iLastAssignedClientNumber = 0;								// 마지막으로 부여된 클라이언트 번호 초기화
	printf("[System] 서버(%p) 로비 재시작 준비 초기화 완료.\n", this);
}

bool GameServer::startMainLoop() {
	bool exitMainLoop = false;
	DWORD dwCurrTime = 0;							// 업데이트가 가해질 때의 시간
	DWORD dwLastUpdateTime = (DWORD)timeGetTime();	// 마지막으로 업데이트가 수행된 시간
	DWORD dwTimeDelta = 0;							// 위 두 시간의 차
	DWORD dwLastSessionTime = 0;					// 마지막으로 계산된 남은 세션 시간

	while (exitMainLoop == false) {
		/* 서버 시간 업데이트 */
		dwCurrTime = (DWORD)timeGetTime();
		dwTimeDelta = dwCurrTime - dwLastUpdateTime;
		dwServerElapsedTime += dwTimeDelta;
		dwLastUpdateTime = dwCurrTime;

		/* 세션이 시작되지 않았을 경우 커넥션 및 디스커넥션 관리 */
		if (bSessionStarted == false) {
			// 세션 시간 감소
			dwLastSessionTime = dwSessionStartLeftTime;
			dwSessionStartLeftTime -= dwTimeDelta;
			if (dwSessionStartLeftTime > dwLastSessionTime) { // 주의!: 남은 세션 시간이 언더플로로 인해 값이 치솟은 경우
				bSessionStarting = true;	// 세션 시작중 활성화
				printf("[Server] 세션 시작중 설정됨.\n");
			}

			// 클라이언트 접속 및 접속 해제
			clientConnection(dwTimeDelta);
			checkSessionStart(dwTimeDelta);
		}
		/* 세션 동기화 관리 */
		else {
			// (모든 플레이어로부터 메시지를 받은 경우 저장된 메시지를 클라이언트들에게 일괄전송)
			// (클라이언트에서는 메시지를 서버로부터 받은 경우 업데이트 수행 및 다음 프레임에 관한 메시지를 서버로 전송)
			// (즉, 모든 플레이어의 프레임은 동기화가 되며, 가장 느린 속도의 전송에 게임 프레임 동기화가 수행됨)
			updateSession(dwTimeDelta);
		}
	}
	return true;
}

bool GameServer::clientConnection(DWORD timeElapsed) {
	/* 지역 변수 */
	GamePacket fromClientPacket;				// 클라이언트로부터 수신한 패킷을 임시로 담을 구조체
	GamePacket toClientPacket;					// 클라이언트로 응답할 패킷
	sockaddr_in clientAddr;						// 클라이언트 주소를 임시 보관할 구조체
	int clientAddrSize = sizeof(sockaddr_in);	// 클라이언트 주소 구조체 크기
	char *pData = NULL;							// Byte 데이터 합성을 위한 포인터

	/* 클라이언트로부터 받은 패킷을 수신 */
	recvfrom(iServerSocket, (char*)&fromClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, &clientAddrSize);
	
	/* 패킷이 커넥션 패킷인지 확인 */
	if (fromClientPacket.enMsgType == GamePacket::MsgType::MSG_CONNECT) { // 커넥션 요청 패킷인 경우
		// 게임이 시작되었는지를 확인
		if (dwSessionStartLeftTime <= 0) {
			// 세션 시작함으로 인한 접속 불가 패킷 생성
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_FAIL_STARTED; // 서버로부터 MSG_CONNECT_FAIL_STARTED 를 받을 시 접속 실패를 의미
			toClientPacket.sPlayerNumber = 0;

			// 클라이언트에게 전송
			sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
			printf("[Server] 클라이언트 %s의 연결 거부 메시지 전송: 세션 시작됨.\n", inet_ntoa(clientAddr.sin_addr));

			// 나머지 검사 생략
			return true;
		}
		// 인원이 다 찼는지를 확인
		else if (clientInfoVector.size() >= GameServer::MAX_CLIENT) {
			// 인원 다 참으로 인한 접속 불가 패킷 생성
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MGS_CONNECT_FAIL_MAXPLAYER; // 서버로부터 MGS_CONNECT_FAIL_MAXPLAYER 를 받을 시 접속 실패를 의미
			toClientPacket.sPlayerNumber = 0;

			// 클라이언트에게 전송
			sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
			printf("[Server] 클라이언트 %s의 연결 거부 메시지 전송: 수용 인원 초과.\n", inet_ntoa(clientAddr.sin_addr));

			// 나머지 검사 생략
			return true;
		}
		// 이미 등록된 플레이어 아이디인지를 확인
		else {
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (strcmp(clientInfoVector[i]->cClientName, fromClientPacket.byteData) == 0) { // 전달한 ID와 같은지 확인
					// 동일 아이이디로 인한 접속 불가 패킷 생성
					toClientPacket.dwFrameTime = dwServerElapsedTime;
					toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_FAIL_SAMEID; // 서버로부터 MSG_CONNECT_FAIL_SAMEID 를 받을 시 접속 실패를 의미
					toClientPacket.sPlayerNumber = 0;

					// 클라이언트에게 전송
					sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
					printf("[Server] 클라이언트 %s의 연결 거부 메시지 전송: 아이디 중복.\n", inet_ntoa(clientAddr.sin_addr));

					// 나머지 검사 생략
					return true;
				}
			}
		}

		/* 이 곳에 도달하면 접속 성공 정보를 클라이언트 측으로 전달 */
		// 아이디를 클라이언트 정보 벡터에 기록하고 접속 성공 메시지와 플레이어 번호를 전송
		toClientPacket.dwFrameTime = dwServerElapsedTime;
		toClientPacket.enMsgType = GamePacket::MsgType::MSG_CONNECT_SUCCESS; // 서버로부터 MSG_CONNECT_SUCCESS 를 받을 시 접속 성공을 알림
		toClientPacket.sPlayerNumber = ++iLastAssignedClientNumber;			 // 마지막으로 부여된 플레이어 번호에 1을 증가시킨 번호로 할당
		sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, clientAddrSize);
		printf("[Server] 접속 승인 메시지 전송 Client IP: %s, PORT: %d\n", inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);
		
		// 클라이언트 정보를 서버에 저장
		ClientInfo *pInfo = new ClientInfo;
		pInfo->bDataReceived = false;															// 클라이언트로부터 데이터 수신여부 초기화
		strncpy(pInfo->cClientName, fromClientPacket.byteData, sizeof(char[9]));				// 클라이언트 아이디 정보 복사
		memcpy_s(&(pInfo->clientAddr), sizeof(sockaddr_in), &clientAddr, sizeof(sockaddr_in));	// 클라이언트 주소 정보 복사
		pInfo->sClientNumber = toClientPacket.sPlayerNumber;									// 클라이언트에게 부여된 번호 복사
		pInfo->dwLastRecvedTime = this->dwServerElapsedTime;									// 클라이언트로부터 수신받았을 시기의 서버 시간 저장
		clientInfoVector.push_back(pInfo);														// 클라이언트 정보 벡터에 푸시 백
		printf("[Server] 클라이언트 %s정보 등록 완료. 부여된 번호: %d 접속자 수: %d\n", inet_ntoa(pInfo->clientAddr.sin_addr), (int)pInfo->sClientNumber, clientInfoVector.size());

		// 접속한 클라이언트에게 미리 접속했던 다른 클라이언트 정보 전송
		for (int i = 0; i < clientInfoVector.size(); ++i) {
			// 자기 자신의 정보를 제외하고 전송
			if (strcmp(pInfo->cClientName, clientInfoVector[i]->cClientName) != 0) {
				// 플레이어 아이디, 번호 정보가 담긴 패킷 생성
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_GIVE_CLIENTNAME;
				memcpy_s(pData, sizeof(char[9]), clientInfoVector[i]->cClientName, sizeof(char[9]));
				pData += sizeof(char[9]);
				memcpy_s(pData, sizeof(short), &clientInfoVector[i]->sClientNumber, sizeof(short));
				pData += sizeof(short);

				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(pInfo->clientAddr), clientAddrSize);
				printf("[Server] 클라이언트 %s에게 클라이언트 %s(%d)의 정보를 전송함.\n",
						pInfo->cClientName, clientInfoVector[i]->cClientName, (int)clientInfoVector[i]->sClientNumber);
			}
		}

		// 미리 접속했던 클라이언트들에게 접속한 클라이언트의 정보 전송
		for (int j = 0; j < clientInfoVector.size(); ++j) {
			// 보내는 정보가 수신자 자기 자신의 정보가 아닌 경우에만 전송
			if (strcmp(pInfo->cClientName, clientInfoVector[j]->cClientName) != 0) {
				// 플레이어 아이디, 번호 정보가 담긴 패킷 생성
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_GIVE_CLIENTNAME;	// 클라이언트 이름 및 번호 전송 메시지
				memcpy_s(pData, sizeof(char[9]), pInfo->cClientName, sizeof(char[9]));	// 클라이언트 이름 저장
				pData += sizeof(char[9]);
				memcpy_s(pData, sizeof(short), (short*)&pInfo->sClientNumber, sizeof(short)); // 클라이언트 번호 저장
				pData += sizeof(short);

				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), clientAddrSize);
				printf("[Server] 클라이언트 %s에게 클라이언트 %s(%d)의 정보를 전송함.\n",
						clientInfoVector[j]->cClientName, pInfo->cClientName, (int)pInfo->sClientNumber);
			}
		}
	}
	/* 패킷이 디스커넥션 요청 패킷인지 확인 */
	else if (fromClientPacket.enMsgType == GamePacket::MsgType::MSG_DISCONNECT) { // 디스커넥션 요청인 경우
		for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ++iter) {
			// 등록된 플레이어 아이디를 찾은 경우 플레이어 정보 삭제
			if (strcmp((*iter)->cClientName, fromClientPacket.byteData) == 0) {
				// 플레이어 정보 삭제
				printf("[Server] 클라이언트 %s(ID: %s) 접속 종료됨.\n", inet_ntoa((*iter)->clientAddr.sin_addr), (*iter)->cClientName);
				delete (*iter);
				clientInfoVector.erase(iter);
				
				// 다른 클라이언트들에게 접속 종료된 클라이언트의 정보 전송
				for (int j = 0; j < clientInfoVector.size(); ++j) {
					// 플레이어 아이디, 번호 정보가 담긴 패킷 생성
					pData = toClientPacket.byteData;
					toClientPacket.dwFrameTime = dwServerElapsedTime;
					toClientPacket.enMsgType = GamePacket::MsgType::MSG_REMOVE_CLIENTNAME; // 클라이언트 이름 및 번호 삭제 메시지
					memcpy_s(pData, sizeof(char[9]), fromClientPacket.byteData, sizeof(char[9])); // 클라이언트 이름 저장
					pData += sizeof(char[9]);
					memcpy_s(pData, sizeof(short), (short*)&clientInfoVector[j]->sClientNumber, sizeof(short)); // 클라이언트 번호 저장
					pData += sizeof(short);

					// 접속된 모든 클라이언트에게 전송
					sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), clientAddrSize);
				}

				// 나머지 검사 생략
				return true;
			}
		}
	}
	else { // 그 외 종류의 패킷인 경우 아무 행동도 하지 않고 함수 종료
		return true;
	}
}

bool GameServer::checkSessionStart(DWORD timeElapsed) {
	/* 지역 변수 초기화 */
	bool sendSessionStartMsgToClient = false;	// 클라이언트에게 세션 시작 메시지를 전송하는 여부
	bool allClientReadyToStart = false;			// 모든 클라이언트가 시작 할 준비가 되었는지 여부
	int clientAddrSize = sizeof(sockaddr_in);	// 클라이언트 주소 구조체 크기
	int clientCnt = clientInfoVector.size();	// 현재 연결된 클라이언트의 수
	static DWORD dwLastUpdateTime = 0;			// 마지막으로 업데이트를 수행한 시간
	DWORD dwCurrentTime = (DWORD)timeGetTime();	// 업데이트를 수행하는 현재 시간
	GamePacket toClientPacket;					// 클라이언트에게 보낼 패킷 구조체
	char *pData = NULL;							// 패킷 구조체의 Byte 데이터를 가리키는 포인터

	/* 접속 대기 중: 접속된 플레이어가 1명 이하일 시 시작 대기시간 초기화 */
	if (clientCnt < 1) {
		dwSessionStartLeftTime = SESSION_START_DELAY;
		sendSessionStartMsgToClient = false;
		bSessionStarted = false;
	}
	/* 세션 생성을 위한 플레이어 대기 중: 접속된 플레이어에게 남은 대기시간과 연결된 유저 수를 동기화시킴 */
	else if (bSessionStarting == false) {
		sendSessionStartMsgToClient = false;
	}
	/* 세션 시작 조건 충족(클라이언트가 2명 이상이고 대기시간 마감): 플레이어에게 게임 시작 메시지 전송 */
	else if (bSessionStarting == true) {
		dwSessionStartLeftTime = 0;
		sendSessionStartMsgToClient = true;
	}

	/* 매 1초마다 메시지 전송 */
	if (dwCurrentTime > (dwLastUpdateTime + 1000)) {
		dwLastUpdateTime = dwCurrentTime; // 마지막으로 업데이트 한 시간 저장

		/* 세션 시작 대기 중인 경우 */
		if (sendSessionStartMsgToClient == false) {
			/* 클라이언트에게 남은 세션 대기시간 및 접속된 유저수 전송 */
			for (int i = 0; i < clientCnt; ++i) {
				// 클라이언트에게 보낼 세션 대기시간 및 접속된 유저수 정보 패킷 생성
				pData = toClientPacket.byteData;
				toClientPacket.dwFrameTime = dwServerElapsedTime;
				toClientPacket.enMsgType = GamePacket::MsgType::MSG_SESSION_START_LEFTTIME; // 세션 시작 남은시간 동기화 메시지
				memcpy_s(pData, sizeof(DWORD), (DWORD*)&dwSessionStartLeftTime, sizeof(DWORD)); // 남은 시간
				pData += sizeof(DWORD);
				memcpy_s(pData, sizeof(int), (int*)&clientCnt, sizeof(int)); // 접속된 인원 수
				pData += sizeof(int);

				// 클라이언트에게 전송
				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), clientAddrSize);
				printf("[Server] 클라이언트 %s(%s)에게 대기 메시지 전송. (시간: %d, 인원: %d)\n",
						inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName, dwSessionStartLeftTime, clientInfoVector.size());
			}
		}
		/* 세션 시작 준비인 경우 */
		else {
			// 서버 수신측 버퍼 비움
			GamePacket tempPacket;
			sockaddr_in tempSockaddr;
			while (recvfrom(iServerSocket, (char*)&tempPacket, sizeof(GamePacket), 0, (sockaddr*)&tempSockaddr, &clientAddrSize) > 0) {}

			// 클라이언트에게 전송할 패킷 생성
			REAL sessionLPS = SESSION_LPS;
			toClientPacket.dwFrameTime = dwServerElapsedTime;
			toClientPacket.enMsgType = GamePacket::MsgType::MSG_SESSION_START; // 접속된 클라이언트들에게 게임 시작 메시지 전송
			pData = toClientPacket.byteData;
			memcpy_s(pData, sizeof(REAL), &sessionLPS, sizeof(REAL));	// 세션 로직 업데이트 주기를 담음
			pData += sizeof(REAL);
			
			// 클라이언트들에게 전송
			for (int i = 0; i < clientCnt; ++i) {
				sendto(iServerSocket, (char*)&toClientPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), sizeof(sockaddr));
				printf("[Server] 클라이언트 %s(%s)에게 세션 시작 메시지 전송.\n", inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName);
			}

			// 클라이언트들의 마지막 연결 시간을 초기화
			for (int j = 0; j < clientInfoVector.size(); ++j) {
				clientInfoVector[j]->bDataReceived = false;
				clientInfoVector[j]->dwLastRecvedTime = dwServerElapsedTime;
			}

			// 서버 상태를 세션 시작으로 설정
			bSessionStarted = true;		// 시작됨은  TRUE 로 전환
			bSessionStarting = false;	// 시작중은 FALSE 로 전환
			printf("\n\n[Server] 세션이 시작되었습니다. (TgtLPS: %f, ServerElapsedTime:%lf초)\n", sessionLPS, (double)(dwServerElapsedTime / 1000.0));

			// 접속자 상태를 출력
			printf("[Server] 접속자 명단 (%d명) \n", clientInfoVector.size());
			printf("No.\tID\t\tIP\t\tNUM\n");
			for (int k = 0; k < clientInfoVector.size(); ++k) {
				printf("[%02d]\t%8s\t%15s\t%d\n", (k + 1), clientInfoVector[k]->cClientName,
						inet_ntoa(clientInfoVector[k]->clientAddr.sin_addr), (int)clientInfoVector[k]->sClientNumber);
			}
			printf("\n");
		}
	}
	else {
		// ... 1초가 안 되는 경우에는 IDLE 대기
	}

	return true;
}

bool GameServer::updateSession(DWORD timeElapsed) {
	// 지역 변수
	GamePacket recvPacket;
	GamePacket sendPacket;
	sockaddr_in clientAddr;
	int iSockaddrSize = sizeof(sockaddr);
	DWORD dwRecvStartTime = 0;

	// 연결된 클라이언트의 수를 확인하여 접속된 클라이언트가 없을 시 세션 종료
	if (clientInfoVector.size() < 1) {
		bSessionStarted = false;
		initalizeForLobby();
		printf("\n[Server] 접속된 클라이언트가 없어 세션이 종료되며, 세션 시작 대기 상태로 돌아갑니다.\n\n");
		return true;
	}

	// 최대 0.5초 동안 클라이언트로부터 받은 메시지 수신 후 응답 및 저장
	dwRecvStartTime = (DWORD)timeGetTime();
	while (recvfrom(iServerSocket, (char*)&recvPacket, sizeof(GamePacket), 0, (sockaddr*)&clientAddr, &iSockaddrSize) > 0) {
		// 클라이언트로 동기화 메시지를 수신한 경우
		if (recvPacket.enMsgType == GamePacket::MsgType::MSG_CLIENT_SYNC) {
			// 전송한 플레이어의 정보를 벡터에서 찾음
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (clientInfoVector[i]->sClientNumber == recvPacket.sPlayerNumber) {
					// 이번 업데이트에 해당 플레이어로부터 처음 데이터를 수신한 경우
					if (clientInfoVector[i]->bDataReceived == false) {
						clientInfoVector[i]->bDataReceived = true;	// 데이터를 수신함으로 설정
						clientPacketVector.push_back(recvPacket);	// 수신한 데이터를 패킷 벡터에 푸시 백
						printf("[Time:%lu] 플레이어 %d로부터 첫 동기화 수신 완료.\n", this->dwSessionFinsihLeftTime, recvPacket.sPlayerNumber); // test
					}

					// 클라이언트에게 메시지 수신함 메시지 전송, 마지막으로 수신한 시간 갱신, 나머지 플레이어는 검사 생략
					sendPacket.dwFrameTime = dwServerElapsedTime;
					sendPacket.sPlayerNumber = recvPacket.sPlayerNumber;
					sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_WAIT;
					sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[i]->clientAddr), iSockaddrSize);
					clientInfoVector[i]->dwLastRecvedTime = dwServerElapsedTime;
					break;
				}
			}
		}
		// 클라이언트로부터 디스커넥트 메시지를 수신한 경우
		else if (recvPacket.enMsgType == GamePacket::MsgType::MSG_DISCONNECT) {
			// 시스템 메시지 출력
			for (int i = 0; i < clientInfoVector.size(); ++i) {
				if (clientInfoVector[i]->sClientNumber == recvPacket.sPlayerNumber) {
					printf("[Server] 클라이언트 %s (%s)가 접속을 종료하였습니다. (Time: %lf)\n",
							inet_ntoa(clientInfoVector[i]->clientAddr.sin_addr), clientInfoVector[i]->cClientName, (dwServerElapsedTime / 1000.0));

					// 다른 플레이어들에게 디스커넥션 메시지 전송
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
			// 해당 플레이어 번호를 확인 후 클라이언트 벡터에서 제거
			removeClientInfo(recvPacket.sPlayerNumber);
		}

		// 데이터 수신 후 처리하는 시간이 0.5초를 초과하는지 검사하여 초과하였으면 다음 작업 검사를 위해 탈출
		if ((DWORD)timeGetTime > dwRecvStartTime + 500) break;
	}

	// 모든 클라이언트가 데이터를 전송했는지 확인
	bool isCompleted = true;
	for (int i = 0; i < clientInfoVector.size(); ++i) {
		if (clientInfoVector[i]->bDataReceived == false) {
			isCompleted = false;
			printf("[Time:%lu] 플레이어 %s로 부터 동기화 수신 대기중.\n", this->dwSessionFinsihLeftTime, clientInfoVector[i]->cClientName); // test
			break;
		}
	}
	// 모든 클라이언트에게 데이터를 수신했으면 보관중인 패킷 데이터로 일괄 업데이트를 하도록 메시지 전송
	if (isCompleted == true) {
		dwSessionFinsihLeftTime -= rSessionLPSbyFixedTime;	// 고정 프레임 레이트로 인한 남은 시간의 업데이트를 유도
		if (dwSessionFinsihLeftTime > SESSION_FINISH_TIME) dwSessionFinsihLeftTime = 0; // 언더플로 방지

		for (int j = 0; j < clientInfoVector.size(); ++j) {
			// 저장된 동기화 메시지들 전송
			for (int k = 0; k < clientPacketVector.size(); ++k) {
				// 자기가 전송한 자신의 동기화 정보는 제외
				if (clientInfoVector[j]->sClientNumber != clientPacketVector[k].sPlayerNumber) {
					sendto(iServerSocket, (char*)&clientPacketVector[k], sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);
				}
			}

			// 업데이트 메시지 전송
			sendPacket.dwFrameTime = dwSessionFinsihLeftTime; // 세션 남은 시간을 전송
			sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
			sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_DOUPDATE;
			sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);
			
			// 해당 클라이언트의 업데이트 상태 초기화
			clientInfoVector[j]->bDataReceived = false;
			clientInfoVector[j]->dwLastRecvedTime = dwServerElapsedTime;
		printf("[Time:%lu] 모든 플레이어 메시지 수신 완료. 동기화 메시지와 업데이트 메시지 전송.\n\n", this->dwSessionFinsihLeftTime); // test
		}

		// 보관중이던 메시지를 정리
		clientPacketVector.clear();
	}
	// 모든 클라이언트로부터 데이터를 수신받지 못한 경우에는 장기간(3.0초 초과) 동기화 전송이 없는 클라이언트를 찾아서 디스커넥트 시킴
	else {
		for (int j = 0; j < clientInfoVector.size(); ++j) {
			if (clientInfoVector[j]->bDataReceived == false && (clientInfoVector[j]->dwLastRecvedTime + SESSION_PLAYERSYNC_WAITING_TIME) < dwServerElapsedTime) {
				// 클라이언트에게 킥 메시지 전송
				sendPacket.dwFrameTime = dwServerElapsedTime;
				sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
				sendPacket.enMsgType = GamePacket::MsgType::MSG_CLIENT_KICKED;
				sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[j]->clientAddr), iSockaddrSize);

				// 시스템 메시지
				printf("[Server] 클라이언트 %s (%s)가 %f초간 메시지 전송이 없어 추방되었습니다.\n",
						inet_ntoa(clientInfoVector[j]->clientAddr.sin_addr), clientInfoVector[j]->cClientName,
						(float)(dwServerElapsedTime - clientInfoVector[j]->dwLastRecvedTime) / 1000.0);

				// 다른 플레이어에게 플레이어 추방됨 메시지 전송
				for (int k = 0; k < clientInfoVector.size(); ++k) {
					if (j != k) {
						sendPacket.dwFrameTime = dwServerElapsedTime;
						sendPacket.sPlayerNumber = clientInfoVector[j]->sClientNumber;
						sendPacket.enMsgType = GamePacket::MsgType::MSG_DISCONNECT;
						sendto(iServerSocket, (char*)&sendPacket, sizeof(GamePacket), 0, (sockaddr*)&(clientInfoVector[k]->clientAddr), iSockaddrSize);
					}
				}

				// 해당 클라이언트를 벡터에서 삭제
				removeClientInfo(clientInfoVector[j]->sClientNumber);
			}
		}
	}

	return true;
}

void GameServer::removeClientInfo(short number) {
	for (vector<ClientInfo*>::iterator iter = clientInfoVector.begin(); iter != clientInfoVector.end(); ) {
		if ((*iter)->sClientNumber == number) {
			printf("[Server] 클라이언트 ID: %s / IP: %s / NUM: %hd 의 정보 삭제 수행. (남은 클라이언트: %d)\n",
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
