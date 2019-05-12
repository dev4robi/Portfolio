#include "Game/Object/Player.h"
#include "Engine/GameCamera.h"
#include "Engine/DrawManager.h"
#include "Engine/ResourceManager.h"
#include "Game/Object/JetFighter.h"
#include "Game/UI/UI.h"
#include "Game/Object/Weapon.h"
#include "Game/Object/Missile.h"
#include "Game/Frame/CombatFrame.h"

Player::Player(int _number, string _name) : GameObject() {
	number = _number;
	name = _name;

	// �÷��̾� ������ ������
	pJet = NULL;

	// �÷��̾� ���� ī�޶�
	camOption = camera_option::CAM_THIRD_PERSON;
	DrawManager* pDrawMgr = g_pGameManager->getDrawManagerPtr();
	Size2D* pResolution = pDrawMgr->getResolutionPtr();
	pCamera = new GameCamera();
	pCamera->setBehavior(GameCamera::CAMERA_BEHAVIOR_FLIGHT); // ���� ����
	pCamera->perspective(95.0, pResolution->getWidth() / pResolution->getHeight(), 0.1, 50000.0); // ���� ����

	// �ɹ� ���� �ʱ�ȭ
	bNetworkDisconnected = false;
	bNetworkUpdated = false;

	// �÷��̾� UI ������
	pUImanager = new UImaster(this);

	// �÷��̾� ���� ���� �ʱ�ȭ
	playerKills = playerScore = playerDeathCount = 0;
}

Player::~Player() {
	// �÷��̾� ���� ������ ������ NULL
	pJet = NULL;

	// ī�޶� ����
	if (pCamera != NULL) delete pCamera;
	pCamera = NULL;
	
	// UI ����
	if (pUImanager != NULL) delete pUImanager;
	pUImanager = NULL;
}


bool Player::getNetworkDisconnected() {
	return bNetworkDisconnected;
}

void Player::setNetworkDisconnected(bool disconnected) {
	bNetworkDisconnected = disconnected;
}

bool Player::getNetworkUpdated() {
	return bNetworkUpdated;
}

void Player::setNetworkUpdated(bool updated) {
	bNetworkUpdated = updated;
}

int Player::getPlayerKills() {
	return playerKills;
}

void Player::setPlayerKills(int val) {
	playerKills = val;
}

int Player::getPlayerScore() {
	return playerScore;
}

void Player::setPlayerScore(int val) {
	REAL sessionLeftTime = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime(); // ���� ���� �ð��� �޾ƿ�
	
	// ���� �� ����ÿ��� ���� ����
	if (sessionLeftTime > 0.00)
		playerScore = val;
	
	if (playerScore < 0) playerScore = 0; // ������ ������ ���� ����
}

int Player::getPlayerDeathCount() {
	return playerDeathCount;
}

void Player::setPlayerDeathCount(int val) {
	playerDeathCount = val;
}

void Player::setCamChacingMissile(bool chacing) {
	camChacingMissile = chacing;
};

camera_option Player::getCameraOption() {
	return camOption;
}

void Player::setCameraOption(camera_option option) {
	camOption = option;
}

bool Player::getCamShowBehind() {
	return camShowBehind;
}

void Player::setCamShowBehind(bool useBehind) {
	camShowBehind = useBehind;
}

GameCamera* Player::getPlayerCamera() {
	return pCamera;
}

void Player::setPlayerJet(JetFighter* jet) {
	pJet = jet;
}

JetFighter* Player::getPlayerJet() {
	return pJet;
}

UImaster* Player::getUImanagerPtr() {
	return pUImanager;
}

list<GameWidget*>* Player::getPlayerUIDrawListPtr() {
	return &playerUIDrawList;
}

list<RadarLockon*>* Player::getRadarLockonUIListPtr() {
	if (pUImanager != NULL) {
		return pUImanager->getRadarLockonUIListPtr();
	}
	
	return NULL;
}

SpeedAltMeter* Player::getSpeedAltMeterPtr() {
	if (pUImanager != NULL) {
		return pUImanager->getSpeedAltMeterPtr();
	}

	return NULL;
}

RadarMap* Player::getRadarMapPtr() {
	if (pUImanager != NULL) {
		return pUImanager->getRadarMapPtr();
	}

	return NULL;
}

void Player::setPlayerNumber(int playerNumber) {
	number = playerNumber;
}

int Player::getPlayerNumber() {
	return number;
}

void Player::setPlayerName(char *newName) {
	name = string(newName);
}

string Player::getPlayerName() {
	return name;
}

string* Player::getPlayerNamePtr() {
	return &name;
}

bool Player::cmpScore(const Player *p1, const Player *p2) {
	if (p1->playerScore != p2->playerScore) {			// ���� �켱������ ų �켱�������� ����
		return (p1->playerScore > p2->playerScore);
	}
	else {												// ������ ���� ��� ų �켱������ �Ǵ� 
		return (p1->playerKills > p2->playerKills);
	}
}

bool Player::cmpNumber(const Player *p1, const Player *p2)	{
	return (p1->number < p2->number); // ����Ʈ �������� ����
}

void Player::updateCamera(REAL timeElapsed) {
	/* ī�޶� ������Ʈ */
	float camDist = 20.0 + ((6.5 * (pJet->currentSpeed / pJet->default_maxSpeed)) + (3.5 * (pJet->currentEngPower / 100.0)));
	float camHeight = 4.5;

	if (pJet->getState() == FlightObject::object_state::STATE_ALIVE) { // ������ ��� ī�޶� ����
		FlightObject *pCamTgt = pJet;
		// ī�޶� �̻����� �i�� �ɼ������� Ȯ��
		if (camChacingMissile == true) {
			list<GameWidget*> *pList = g_pGameManager->getGameFramePtr()->getWidgetListPtr();
			list<GameWidget*>::iterator iter = pList->end();
			for (; iter != pList->begin();) {
				--iter;
				if ((*iter)->getWidgetType() == GameWidget::WIDGET_MISSILE) { // ���� ������ �̻����̰�
					Missile *pMissile = static_cast<Missile*>(*iter);
					if (pMissile->pOwningPlayer == this && pMissile->getStyle() != true) {	// �÷��̾� �����̰�, �Ѿ��� �ƴ� ���
						pCamTgt = pMissile;
						break;
					}
				}
			}
		}

		Vector3 idealEye;
		Vector3 eye;
		Vector3 at(eye + pCamTgt->directVector);
		Vector3 up(pCamTgt->upVector);
		
		// 1. �Ĺ� ī�޶�
		if (camShowBehind == true) {
			camDist = 100.0;
			camHeight = 10.0;
			eye = pCamTgt->getPos() + (pCamTgt->directVector * camDist) + (pCamTgt->upVector * camHeight);
			at = pCamTgt->getPos();
			up = Vector3(0.0, 1.0, 0.0);
			pCamera->lookAt(eye, at, up);
		}
		// 2. ���� ȸ�� ī�޶� (1��Ī ī�޶�)
		else if (camOption == camera_option::CAM_FIRST_PERSON) {
			eye = pCamTgt->getPos() + (pCamTgt->directVector * 5.5) + (pCamTgt->upVector * 0.75);
			at = (eye + pCamTgt->directVector);
			pCamera->lookAt(eye, at, up);
		}
		// 3. ȸ�� ���� ī�޶� (3��Ī ī�޶� - ����Ʈ)
		else {
			// ��ġ ����
			idealEye = pCamTgt->getPos() - (pCamTgt->directVector * camDist) + (pCamTgt->upVector * camHeight);	// �̻����� ī�޶��� ��ġ
			up = pCamTgt->upVector;			// ī�޶��� �� ����
			at = pCamTgt->getPos();			// ī�޶� �ٶ󺸴� ����
			eye = pCamera->getPosition();	// ���� ī�޶��� ��ġ

			if (eye != idealEye) { // �̻����� ��ġ�� ���� ī�޶��� ��ġ�� �ٸ��ٸ� ��ġ ������ ����
				Vector3 moveDir = (idealEye - eye);	// �̻����� ��ġ�� ī�޶� ���� ��ġ�� �� ����
				float length = moveDir.magnitude();	// �̻����� ��ġ�� ī�޶� ���� ��ġ�� �Ÿ�
				float distToMove = (length * 0.5) * timeElapsed; // ī�޶� �̵��ؾ� �ϴ� �Ÿ�

				moveDir.normalize();
				eye += moveDir * distToMove;	// ī�޶� ������ ��ġ�� �̵���Ŵ

				Vector3 newMoveDir = (idealEye - eye);	// ���ο� ��ġ������ �Ÿ��� ������

				if (newMoveDir.magnitude() > camDist) {	// ����� �ӵ��� �������� ī�޶� �Ÿ��� �־����� �Ÿ��� camDist���� ���� ������ ����
					newMoveDir.normalize();

					eye += newMoveDir * (((idealEye - eye).magnitude()) - camDist);
				}
			}

			// �Ѱ� ����
			float oRoll, nRoll, fRoll, fPitch, fYaw;						// o = old, n = new, f = final
			pCamera->getOrientation().toHeadPitchRoll(fYaw, fPitch, oRoll);	// ī�޶� �̵� ���� ���������̼��� ����
			pCamera->lookAt(eye, at, up);
			pCamera->getOrientation().toHeadPitchRoll(fYaw, fPitch, nRoll);	// ī�޶� �̵� ���� ���������̼��� ����

			float dRoll = (nRoll - oRoll); // d = distance
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
			fRoll = nRoll - dRoll * timeElapsed; // ���� �Ѱ� ����
			lastdRoll = dRoll;					 // ������ �Ѱ��� �����Ͽ� �̻�(dRoll ���� �ʹ� ũ�ų� ����) ���� �� ����� �� ����

			if (fRoll > 180.0) { // �� ȸ���� ���� ���� (-180.0 ~ +180.0)
				fRoll = -360.0 + fRoll;
			}
			else if (fRoll < -180.0) {
				fRoll = 360.0 + fRoll;
			}

			// ī�޶��� �� ���� ���������̼��� ����
			Quaternion q;
			q.fromHeadPitchRoll(fYaw, fPitch, fRoll);
			pCamera->setOrientation(q);
		}
	}
	else { // ����� ��� ī�޶� ����
		Vector3 eye = pJet->getPos();
		Vector3 at = eye;
		Vector3 up(0.0, 1.0, 0.0);
		eye += Vector3(cosf(pJet->getRespawnLeftTime() / 4.0) * 1000.0, 1000.0, sinf(pJet->getRespawnLeftTime() / 4.0) * 1000.0);
		pCamera->lookAt(eye, at, up);
	}
}

void Player::updatePlayer(REAL timeElapsed) {
	// . . .
}

void Player::updatePlayerUI(REAL timeElapsed) {
	/* UI���� ��ο� ����Ʈ�� �߰�*/
	playerUIDrawList.clear();

	/* UI���� ������Ʈ �� ��ο� ����Ʈ�� �߰� */
	pUImanager->updateUI(timeElapsed);

	// . . .
}
