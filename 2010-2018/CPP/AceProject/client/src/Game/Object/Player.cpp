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

	// 플레이어 소유의 전투기
	pJet = NULL;

	// 플레이어 소유 카메라
	camOption = camera_option::CAM_THIRD_PERSON;
	DrawManager* pDrawMgr = g_pGameManager->getDrawManagerPtr();
	Size2D* pResolution = pDrawMgr->getResolutionPtr();
	pCamera = new GameCamera();
	pCamera->setBehavior(GameCamera::CAMERA_BEHAVIOR_FLIGHT); // 비행 시점
	pCamera->perspective(95.0, pResolution->getWidth() / pResolution->getHeight(), 0.1, 50000.0); // 원근 투영

	// 맴버 변수 초기화
	bNetworkDisconnected = false;
	bNetworkUpdated = false;

	// 플레이어 UI 관리자
	pUImanager = new UImaster(this);

	// 플레이어 점수 정보 초기화
	playerKills = playerScore = playerDeathCount = 0;
}

Player::~Player() {
	// 플레이어 소유 전투기 포인터 NULL
	pJet = NULL;

	// 카메라 제거
	if (pCamera != NULL) delete pCamera;
	pCamera = NULL;
	
	// UI 제거
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
	REAL sessionLeftTime = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime(); // 세션 만료 시간을 받아옴
	
	// 세션 비 만료시에만 점수 수정
	if (sessionLeftTime > 0.00)
		playerScore = val;
	
	if (playerScore < 0) playerScore = 0; // 점수는 음수가 되지 않음
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
	if (p1->playerScore != p2->playerScore) {			// 점수 우선순위가 킬 우선순위보다 높음
		return (p1->playerScore > p2->playerScore);
	}
	else {												// 점수가 같은 경우 킬 우선순위로 판단 
		return (p1->playerKills > p2->playerKills);
	}
}

bool Player::cmpNumber(const Player *p1, const Player *p2)	{
	return (p1->number < p2->number); // 디폴트 오름차순 정렬
}

void Player::updateCamera(REAL timeElapsed) {
	/* 카메라 업데이트 */
	float camDist = 20.0 + ((6.5 * (pJet->currentSpeed / pJet->default_maxSpeed)) + (3.5 * (pJet->currentEngPower / 100.0)));
	float camHeight = 4.5;

	if (pJet->getState() == FlightObject::object_state::STATE_ALIVE) { // 생존한 경우 카메라 설정
		FlightObject *pCamTgt = pJet;
		// 카메라가 미사일을 쫒는 옵션인지를 확인
		if (camChacingMissile == true) {
			list<GameWidget*> *pList = g_pGameManager->getGameFramePtr()->getWidgetListPtr();
			list<GameWidget*>::iterator iter = pList->end();
			for (; iter != pList->begin();) {
				--iter;
				if ((*iter)->getWidgetType() == GameWidget::WIDGET_MISSILE) { // 위젯 종류가 미사일이고
					Missile *pMissile = static_cast<Missile*>(*iter);
					if (pMissile->pOwningPlayer == this && pMissile->getStyle() != true) {	// 플레이어 소유이고, 총알이 아닌 경우
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
		
		// 1. 후방 카메라
		if (camShowBehind == true) {
			camDist = 100.0;
			camHeight = 10.0;
			eye = pCamTgt->getPos() + (pCamTgt->directVector * camDist) + (pCamTgt->upVector * camHeight);
			at = pCamTgt->getPos();
			up = Vector3(0.0, 1.0, 0.0);
			pCamera->lookAt(eye, at, up);
		}
		// 2. 고정 회전 카메라 (1인칭 카메라)
		else if (camOption == camera_option::CAM_FIRST_PERSON) {
			eye = pCamTgt->getPos() + (pCamTgt->directVector * 5.5) + (pCamTgt->upVector * 0.75);
			at = (eye + pCamTgt->directVector);
			pCamera->lookAt(eye, at, up);
		}
		// 3. 회전 보정 카메라 (3인칭 카메라 - 디폴트)
		else {
			// 위치 보정
			idealEye = pCamTgt->getPos() - (pCamTgt->directVector * camDist) + (pCamTgt->upVector * camHeight);	// 이상적인 카메라의 위치
			up = pCamTgt->upVector;			// 카메라의 업 방향
			at = pCamTgt->getPos();			// 카메라가 바라보는 방향
			eye = pCamera->getPosition();	// 현재 카메라의 위치

			if (eye != idealEye) { // 이상적인 위치와 현재 카메라의 위치가 다르다면 위치 보정을 가함
				Vector3 moveDir = (idealEye - eye);	// 이상적인 위치와 카메라 현재 위치를 뺀 벡터
				float length = moveDir.magnitude();	// 이상적인 위치와 카메라 현재 위치의 거리
				float distToMove = (length * 0.5) * timeElapsed; // 카메라가 이동해야 하는 거리

				moveDir.normalize();
				eye += moveDir * distToMove;	// 카메라를 보정된 위치로 이동시킴

				Vector3 newMoveDir = (idealEye - eye);	// 새로운 위치에서의 거리를 재측정

				if (newMoveDir.magnitude() > camDist) {	// 비행기 속도가 빨라져서 카메라 거리가 멀어지면 거리를 camDist값에 따라 가깝게 보정
					newMoveDir.normalize();

					eye += newMoveDir * (((idealEye - eye).magnitude()) - camDist);
				}
			}

			// 롤각 보정
			float oRoll, nRoll, fRoll, fPitch, fYaw;						// o = old, n = new, f = final
			pCamera->getOrientation().toHeadPitchRoll(fYaw, fPitch, oRoll);	// 카메라 이동 전의 오리엔테이션을 저장
			pCamera->lookAt(eye, at, up);
			pCamera->getOrientation().toHeadPitchRoll(fYaw, fPitch, nRoll);	// 카메라 이동 후의 오리엔테이션을 저장

			float dRoll = (nRoll - oRoll); // d = distance
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
			fRoll = nRoll - dRoll * timeElapsed; // 최종 롤각 결정
			lastdRoll = dRoll;					 // 결정된 롤각을 저장하여 이상각(dRoll 값이 너무 크거나 작음) 나올 시 저장된 값 대입

			if (fRoll > 180.0) { // 롤 회전각 범위 보정 (-180.0 ~ +180.0)
				fRoll = -360.0 + fRoll;
			}
			else if (fRoll < -180.0) {
				fRoll = 360.0 + fRoll;
			}

			// 카메라의 롤 값만 오리엔테이션을 변경
			Quaternion q;
			q.fromHeadPitchRoll(fYaw, fPitch, fRoll);
			pCamera->setOrientation(q);
		}
	}
	else { // 사망한 경우 카메라 설정
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
	/* UI위젯 드로우 리스트에 추가*/
	playerUIDrawList.clear();

	/* UI위젯 업데이트 및 드로우 리스트에 추가 */
	pUImanager->updateUI(timeElapsed);

	// . . .
}
