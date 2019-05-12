#include "Game/Frame/CombatFrame.h"
#include "Test/Test2DWidget.h"
#include "Test/Test3DWidget.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Terrain.h"
#include "Game/Object/Skybox.h"

#if 0
CombatFrame::CombatFrame() : GameFrame(TEXT("CombatFrame")) {
	// 테스트 코드
	// 리소스 로드
	pResourceManager->loadOBJ("res/Models/FA-22_Raptor/FA-22_Raptor.obj");		/* ONLY FOR TEST */
	pResourceManager->loadTexture("res/Textures/terrainTexture.jpg", false, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	pResourceManager->loadTexture("res/Textures/skybox.jpg", false, GL_LINEAR, GL_LINEAR, GL_REPEAT);

	// 카메라 설정
	pCamera->setPosition(Vector3(0.0, 2500.0, 20.0));							/* ONLY FOR TEST */
	pCamera->rotate(0.0, 0.0, 0.0);
	
	// 지형 및 하늘상자 추가
	addWidget(new Terrain(100.0, "res/Textures/terrainTexture.jpg", "res/textures/heightmap.jpg"));
	pSkybox = new Skybox("res/Textures/skybox.jpg");

	// 위젯 추가
	addWidget(new Test2DWidget(0.0, 0.0));				/* ONLY FOR TEST */
	playerJet = new JetFighter(0.0, 2500.0, 0.0, 1, "Player");
	addWidget(playerJet);

	// 테스트 코드
}

CombatFrame::~CombatFrame() {
	
}

bool CombatFrame::updateFrame() {
	bool *mousemap = pInputManager->getMousemapPtr();
	bool *keymap = pInputManager->getKeymapPtr();
	POINT mousePos = pInputManager->getMousePos();
	GameCamera *cam = pCamera;

	static float speed = 5.0;
	static float angle = 0.0;
	
	// 키 입력
	if (keymap['C']) {

	}
	if (keymap['X']) {

	}
	if (keymap['R']) {
		speed = 33.472;
	}
	if (keymap['T']) {
		speed = 1.0;
	}
	if (keymap['W']) {
		playerJet->pitching(-0.4);
	}
	if (keymap['S']) {
		playerJet->pitching(0.4);
	}
	if (keymap['A']) {
		playerJet->yawing(0.05);
	}
	if (keymap['D']) {
		playerJet->yawing(-0.05);
	}
	if (keymap['Q']) {
		playerJet->rolling(1.75);
	}
	if (keymap['E']) {
		playerJet->rolling(-1.75);
	}
	if (keymap['0']) {
		playerJet->setPos(0.0, 2500.0, 0.0);
		playerJet->directVector = Vector3(1.0, 0.0, 0.0);
		playerJet->upVector = Vector3(0.0, 1.0, 0.0);
		playerJet->rightVector = Vector3(0.0, 0.0, 1.0);
		playerJet->transform.identity();
		cam->setPosition(Vector3(playerJet->getPosX() - 20.0, playerJet->getPosY(), playerJet->getPosZ()));
		cam->lookAt(Vector3(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ()));
		Quaternion temp;
		temp.fromAxisAngle(Vector3(1.0, 0.0, 0.0), playerJet->rotateOffset[0]);
		playerJet->transform *= temp;
		temp.fromAxisAngle(Vector3(0.0, 1.0, 0.0), playerJet->rotateOffset[1]);
		playerJet->transform *= temp;
		temp.fromAxisAngle(Vector3(0.0, 0.0, 1.0), playerJet->rotateOffset[2]);
		playerJet->transform *= temp;
		playerJet->pitch = playerJet->yaw = playerJet->roll = 0.0;
	}
	if (keymap[' ']) { // 비행기 가속
		playerJet->currentSpeed += (playerJet->speedAcc * 0.05);
		playerJet->speedAcc += ((100.0 - playerJet->speedAcc) * 0.05 * 0.05);
	}
	else { // 비행기 감속
		if (playerJet->currentSpeed > 300.0) {
			playerJet->currentSpeed -= playerJet->currentSpeed * 0.05 * 0.05;
			playerJet->speedAcc -= 5.0 * 0.05;
		}
		else {
			playerJet->currentSpeed = 300.0;
		}
	}

	// 비행기 업데이트
	playerJet->update(0.05);

	// 기타 업데이트
	for (list<GameWidget*>::iterator iter = widgetList.begin(); iter != widgetList.end(); ++iter) {
		if (mousemap[MOUSE_LBUTTON] == true) {
			POINT srcPtr;
			//srcPtr.x = (*iter)->getPosX() / ((float)1.0 / 2.0) - 1.0);
			//srcPtr.y = (1.0 - ((*iter)->getPosY() / ((float)1.0 / 2.0)));
			(*iter)->setPos(-0.5 + mousePos.x / 1920.0, -0.5 + 1080.0 / mousePos.y, 0.0);
		}
	}
 
	if (mousemap[MOUSE_RBUTTON] == true) {
		for (list<GameWidget*>::iterator iter = widgetList.begin(); iter != widgetList.end(); ++iter) {
				POINT srcPtr;
				//srcPtr.x = (*iter)->getPosX() / ((float)1.0 / 2.0) - 1.0);
				//srcPtr.y = (1.0 - ((*iter)->getPosY() / ((float)1.0 / 2.0)));
				(*iter)->~GameWidget();
		}

		mousemap[MOUSE_RBUTTON] == false;
	}

	// 카메라 업데이트
	static float camDist = 15.0;

	// 회전 보정 카메라
	// 위치 보정
	Vector3 idealEye = Vector3(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ())
							   - (playerJet->directVector * camDist) + (playerJet->upVector * 5.0); // 이상적인 카메라의 위치
	Vector3 up(playerJet->upVector);																// 카메라의 업 방향
	Vector3 at(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ());					// 카메라가 바라보는 방향
	Vector3 eye = Vector3(cam->getPosition());														// 현재 카메라의 위치

	if (eye != idealEye) { // 이상적인 위치와 현재 카메라의 위치가 다르다면 위치 보정을 가함
		Vector3 moveDir = (idealEye - eye);	// 이상적인 위치와 카메라 현재 위치를 뺀 벡터
		float length = moveDir.magnitude();	// 이상적인 위치와 카메라 현재 위치의 거리
		float distToMove = 10.0 * length * 0.05; // 카메라가 이동해야 하는 거리로 앞의 상수는 회전 보정량, 뒤의 상수는 프레임 업데이트 주기

		moveDir.normalize();
		eye += moveDir * distToMove;	// 카메라를 보정된 위치로 이동시킴
		
		Vector3 newMoveDir = (idealEye - eye);	// 새로운 위치에서의 거리를 재측정
		if (newMoveDir.magnitude() > camDist) {	// 비행기 속도가 빨라져서 카메라 거리가 멀어지면 거리를 camDist값에 따라 가깝게 보정
			newMoveDir.normalize();
			eye += newMoveDir * (((idealEye - eye).magnitude()) - camDist);
		}
	}

	// 롤각 보정
	float oRoll, nRoll, fRoll, fPitch, fYaw;					// o = old, n = new, f = final
	cam->getOrientation().toHeadPitchRoll(fYaw, fPitch, oRoll);	// 카메라 이동 전의 오리엔테이션을 저장
	cam->lookAt(eye, at, up);
	cam->getOrientation().toHeadPitchRoll(fYaw, fPitch, nRoll);	// 카메라 이동 후의 오리엔테이션을 저장
	
	float dRoll = nRoll - oRoll; // d = distance
	static float lastdRoll = 0.0;
	if (oRoll > 0.0 && nRoll < 0.0 && dRoll < 0.0) { // 우로 회전 중 뒤집혔을 때
		dRoll = (180.0 - oRoll);
		dRoll += (180.0 + nRoll);
	}
	else if (oRoll < 0.0 && nRoll > 0.0 && dRoll > 0.0) { // 좌로 회전 중 뒤집혔을 때
		dRoll = (-180.0 - oRoll);
		dRoll += (nRoll - 180.0);
	}
	if (dRoll >= 5.0 || dRoll <= -5.0) { // dRoll 보정 후에도 값이 튈 경우 (피치를 올려 방향이 급속히 전환될 경우 등)
		dRoll = lastdRoll;
		fRoll = nRoll;
	}
	fRoll = nRoll - dRoll * 0.9;	// 최종 롤각 결정
	lastdRoll = dRoll;				// 결정된 롤각을 저장하여 이상각(dRoll 값이 너무 크거나 작음) 나올 시 저장된 값 대입

	if (fRoll > 180.0) { // 롤 회전각 범위 보정 (-180.0 ~ +180.0)
		fRoll = -360.0 + fRoll;
	}
	else if (fRoll < -180.0) {
		fRoll = 360.0 + fRoll;
	}

	// 카메라의 롤 값만 오리엔테이션을 변경
	Quaternion q;
	q.fromHeadPitchRoll(fYaw, fPitch, fRoll);
	cam->setOrientation(q);

	// 2. 고정 회전 카메라 (어색함)
	/*
	Vector3 eye = Vector3(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ()) - (playerJet->directVector * camDist) + (playerJet->upVector * 2.5);
	Vector3 at(eye + playerJet->directVector);
	Vector3 up(playerJet->upVector);
	cam->lookAt(eye, at, up);
	*/

	return true;
}
#endif