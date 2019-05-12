#include "Game/Frame/CombatFrame.h"
#include "Test/Test2DWidget.h"
#include "Test/Test3DWidget.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Terrain.h"
#include "Game/Object/Skybox.h"

#if 0
CombatFrame::CombatFrame() : GameFrame(TEXT("CombatFrame")) {
	// �׽�Ʈ �ڵ�
	// ���ҽ� �ε�
	pResourceManager->loadOBJ("res/Models/FA-22_Raptor/FA-22_Raptor.obj");		/* ONLY FOR TEST */
	pResourceManager->loadTexture("res/Textures/terrainTexture.jpg", false, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	pResourceManager->loadTexture("res/Textures/skybox.jpg", false, GL_LINEAR, GL_LINEAR, GL_REPEAT);

	// ī�޶� ����
	pCamera->setPosition(Vector3(0.0, 2500.0, 20.0));							/* ONLY FOR TEST */
	pCamera->rotate(0.0, 0.0, 0.0);
	
	// ���� �� �ϴû��� �߰�
	addWidget(new Terrain(100.0, "res/Textures/terrainTexture.jpg", "res/textures/heightmap.jpg"));
	pSkybox = new Skybox("res/Textures/skybox.jpg");

	// ���� �߰�
	addWidget(new Test2DWidget(0.0, 0.0));				/* ONLY FOR TEST */
	playerJet = new JetFighter(0.0, 2500.0, 0.0, 1, "Player");
	addWidget(playerJet);

	// �׽�Ʈ �ڵ�
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
	
	// Ű �Է�
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
	if (keymap[' ']) { // ����� ����
		playerJet->currentSpeed += (playerJet->speedAcc * 0.05);
		playerJet->speedAcc += ((100.0 - playerJet->speedAcc) * 0.05 * 0.05);
	}
	else { // ����� ����
		if (playerJet->currentSpeed > 300.0) {
			playerJet->currentSpeed -= playerJet->currentSpeed * 0.05 * 0.05;
			playerJet->speedAcc -= 5.0 * 0.05;
		}
		else {
			playerJet->currentSpeed = 300.0;
		}
	}

	// ����� ������Ʈ
	playerJet->update(0.05);

	// ��Ÿ ������Ʈ
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

	// ī�޶� ������Ʈ
	static float camDist = 15.0;

	// ȸ�� ���� ī�޶�
	// ��ġ ����
	Vector3 idealEye = Vector3(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ())
							   - (playerJet->directVector * camDist) + (playerJet->upVector * 5.0); // �̻����� ī�޶��� ��ġ
	Vector3 up(playerJet->upVector);																// ī�޶��� �� ����
	Vector3 at(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ());					// ī�޶� �ٶ󺸴� ����
	Vector3 eye = Vector3(cam->getPosition());														// ���� ī�޶��� ��ġ

	if (eye != idealEye) { // �̻����� ��ġ�� ���� ī�޶��� ��ġ�� �ٸ��ٸ� ��ġ ������ ����
		Vector3 moveDir = (idealEye - eye);	// �̻����� ��ġ�� ī�޶� ���� ��ġ�� �� ����
		float length = moveDir.magnitude();	// �̻����� ��ġ�� ī�޶� ���� ��ġ�� �Ÿ�
		float distToMove = 10.0 * length * 0.05; // ī�޶� �̵��ؾ� �ϴ� �Ÿ��� ���� ����� ȸ�� ������, ���� ����� ������ ������Ʈ �ֱ�

		moveDir.normalize();
		eye += moveDir * distToMove;	// ī�޶� ������ ��ġ�� �̵���Ŵ
		
		Vector3 newMoveDir = (idealEye - eye);	// ���ο� ��ġ������ �Ÿ��� ������
		if (newMoveDir.magnitude() > camDist) {	// ����� �ӵ��� �������� ī�޶� �Ÿ��� �־����� �Ÿ��� camDist���� ���� ������ ����
			newMoveDir.normalize();
			eye += newMoveDir * (((idealEye - eye).magnitude()) - camDist);
		}
	}

	// �Ѱ� ����
	float oRoll, nRoll, fRoll, fPitch, fYaw;					// o = old, n = new, f = final
	cam->getOrientation().toHeadPitchRoll(fYaw, fPitch, oRoll);	// ī�޶� �̵� ���� ���������̼��� ����
	cam->lookAt(eye, at, up);
	cam->getOrientation().toHeadPitchRoll(fYaw, fPitch, nRoll);	// ī�޶� �̵� ���� ���������̼��� ����
	
	float dRoll = nRoll - oRoll; // d = distance
	static float lastdRoll = 0.0;
	if (oRoll > 0.0 && nRoll < 0.0 && dRoll < 0.0) { // ��� ȸ�� �� �������� ��
		dRoll = (180.0 - oRoll);
		dRoll += (180.0 + nRoll);
	}
	else if (oRoll < 0.0 && nRoll > 0.0 && dRoll > 0.0) { // �·� ȸ�� �� �������� ��
		dRoll = (-180.0 - oRoll);
		dRoll += (nRoll - 180.0);
	}
	if (dRoll >= 5.0 || dRoll <= -5.0) { // dRoll ���� �Ŀ��� ���� ƥ ��� (��ġ�� �÷� ������ �޼��� ��ȯ�� ��� ��)
		dRoll = lastdRoll;
		fRoll = nRoll;
	}
	fRoll = nRoll - dRoll * 0.9;	// ���� �Ѱ� ����
	lastdRoll = dRoll;				// ������ �Ѱ��� �����Ͽ� �̻�(dRoll ���� �ʹ� ũ�ų� ����) ���� �� ����� �� ����

	if (fRoll > 180.0) { // �� ȸ���� ���� ���� (-180.0 ~ +180.0)
		fRoll = -360.0 + fRoll;
	}
	else if (fRoll < -180.0) {
		fRoll = 360.0 + fRoll;
	}

	// ī�޶��� �� ���� ���������̼��� ����
	Quaternion q;
	q.fromHeadPitchRoll(fYaw, fPitch, fRoll);
	cam->setOrientation(q);

	// 2. ���� ȸ�� ī�޶� (�����)
	/*
	Vector3 eye = Vector3(playerJet->getPosX(), playerJet->getPosY(), playerJet->getPosZ()) - (playerJet->directVector * camDist) + (playerJet->upVector * 2.5);
	Vector3 at(eye + playerJet->directVector);
	Vector3 up(playerJet->upVector);
	cam->lookAt(eye, at, up);
	*/

	return true;
}
#endif