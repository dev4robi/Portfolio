#include "Game/Object/JetFighter.h"
#include "Game/Object/Player.h"
#include "Game/Object/Weapon.h"
#include "Game/Object/Missile.h"
#include "Game/Object/Terrain.h"
#include "Game/UI/UI.h"
#include "Game/Frame/CombatFrame.h"
#include "Engine/ResourceManager.h"
#include "Engine/GameFrame.h"
#include "Game/Effect/GL_Effect.h"
#include "Engine/SoundManager.h"
#include "Engine/Network/GamePacket.h"

JetFighter::JetFighter(Vector3 pos, Player *_player, char *_craftName) : FlightObject(pos) {
	// ������ �̸�
	craftName = "F-22 RAPTOR";

	// ���� ���� ����
	widgetType = GameWidget::WIDGET_JET;

	// �÷��̾� ����
	pOwningPlayer = _player;

	// ������ �� ����
	pModel = g_pGameManager->getResourceManagerPtr()->getObjModelPtr("res/Models/Fighter/FA-22_Raptor/FA-22_Raptor.obj");

	// �� ������ ����
	rotateOffset[0] = 0.0;
	rotateOffset[1] = 0.0;
	rotateOffset[2] = 0.0;
	Quaternion temp;
	temp.fromAxisAngle(directVector, rotateOffset[0]);
	transform *= temp;
	temp.fromAxisAngle(upVector, rotateOffset[1]);
	transform *= temp;
	temp.fromAxisAngle(rightVector, rotateOffset[2]);
	transform *= temp;

	// ������ ���� �� ���
	jetOffcourseTime = 0.00;
	jetWarningLockon = jet_warning::JET_LOCKON_NONE;
	jetWarningAltitude = false;
	jetWarningStall = false;

	// ���̴� ����
	radarRadius = 10000.0;

	// ��� �ε�
	// ȸ�� �ӵ�
	default_pitchTurn = 28.0;
	default_yawTurn = 8.0;
	default_rollTurn = 205.0;
	default_maxSpeed = 2200.0;
	default_boostOfferTime = 6.0;
	default_engOfferSpeed = 125.0;

	// �ӵ� �� ���� �Ŀ�
	maxSpeed = default_maxSpeed;				// �ִ� �ӵ�
	currentSpeed = maxSpeed * 0.75;				// �ʱ� �ӵ�
	currentEngPower = 0.0;						// �ʱ� ���� �Ŀ� (0.0 ~ 100.0)
	engPowerInc = 40.0;							// �ʴ� ���� �Ŀ� ������
	engPowerDec = 20.0;							// �ʴ� ���� �Ŀ� ���Һ�
	engOfferSpeed = default_engOfferSpeed;		// �ְ� �Ŀ��� �� ������ �����ϴ� �ӵ�

	// ����
	pBaseWeapon = new WpnLockonMissile(this, 1);
	pFirstWeapon = new WpnLockonMissile(this, 0);

	// �浹 ����ü �߰�
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(1.0, -0.5, 0.0), Vector3(8.75, 1.0, 2.0)));	// �߾�
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(-2.5, -0.75, 0.0), Vector3(2.5, 0.25, 7.0)));	// �ճ���
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(-7.0, -0.75, 0.0), Vector3(2.0, 0.25, 4.5)));	// �޳���

	// �ΰ� ����
	jetIsRolling = false;
	jetRollingTime = 0.00;

	// ���� Ʈ����
	for (int i = 0; i < 2; ++i) {
		pWingTrail[i] = new TrailEffect(position, 25);
		pWingTrail[i]->setAttachTgt(this);
		pWingTrail[i]->setAttachOffset(Vector3(-2.5, -1.0, (i == 0 ? 7.0 : -7.0)));
		pWingTrail[i]->setTrailCreatable(false);
	}

	// ���� ��� �Ҳ� ȿ��
	pFlameLeft = new JetFlameEffect(position, 0.5, 5, 5); // ��
	pFlameLeft->setAttachTgt(this);
	pFlameLeft->setAttachOffset(Vector3(-6.75, -0.55, -0.625));
	pFlameLeft->setVisible(false);
	pFlameRight = new JetFlameEffect(position, 0.5, 5, 5); // ��
	pFlameRight->setAttachTgt(this);
	pFlameRight->setAttachOffset(Vector3(-6.75, -0.55, 0.625));
	pFlameRight->setVisible(false);

	// ���� ��� ����
	pSndEngine = new Sound(position, "res/Sounds/jetEngine.wav", true, true);
	pSndEngine->setGain(0.0);

	// ������ ����
	jetUseRespawn = true;
	jetRespawnReqTime = 6.00;
	jetRespawnLeftTime = jetRespawnReqTime;
}

JetFighter::JetFighter(JetFighter& _obj) : FlightObject(_obj) {
	// ���� ä�� ����
}

JetFighter::~JetFighter() {
	// �ֹ��� �������� �ı�
	if (pBaseWeapon != NULL) delete pBaseWeapon;
	if (pFirstWeapon != NULL) delete pFirstWeapon;
	pBaseWeapon = NULL;
	pFirstWeapon = NULL;

	// ���� Ʈ���� ����Ʈ
	for (int i = 0; i < 2; ++i) {
		pWingTrail[i] = NULL;
	}
	
	// �����͹��� ����Ʈ
	pFlameLeft = NULL;
	pFlameRight = NULL;

	// ���� �ı�
	pSndEngine->setLooping(false);
	pSndEngine->stop();
	pSndEngine->destory();
	pSndEngine = NULL;
}

char* JetFighter::toGamePacket(GamePacket *pPacket) {
	char *pRtPtr = pPacket->byteData;

	// ������ ��Ʈ��ũ ����ȭ ������ ��Ŷ���� ����
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &position.x, sizeof(position.x));					// 01. Pos-X (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &position.y, sizeof(position.y));					// 02. Pos-Y (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &position.z, sizeof(position.z));					// 03. Pos-Z (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &transform.x, sizeof(transform.x));					// 04. Transfrom-X (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &transform.y, sizeof(transform.y));					// 05. Transfrom-Y (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &transform.z, sizeof(transform.z));					// 06. Transfrom-Z (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &transform.w, sizeof(transform.w));					// 07. Transfrom-W (4)			- 28Byte
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &directVector.x, sizeof(directVector.x));			// 08. DirectVector-X (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &directVector.y, sizeof(directVector.y));			// 09. DirectVector-Y (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &directVector.z, sizeof(directVector.z));			// 10. DirectVector-Z (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &upVector.x, sizeof(upVector.x));					// 11. UpVector-X (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &upVector.y, sizeof(upVector.y));					// 12. UpVector-Y (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &upVector.z, sizeof(upVector.z));					// 13. UpVector-Z (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &HP, sizeof(HP));									// 14. HP (4)					- 56Byte
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &objectState, sizeof(objectState));					// 15. objectState (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &currentSpeed, sizeof(currentSpeed));				// 16. currentSpeed (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &maxSpeed, sizeof(maxSpeed));						// 17. maxSpeed (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &currentEngPower, sizeof(currentEngPower));			// 18. currentEngPower (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &engOfferSpeed, sizeof(engOfferSpeed));				// 19. engOfferSpeed (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &currentBoost, sizeof(currentBoost));				// 20. currentBoost (4) 
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &boostLeftTime, sizeof(boostLeftTime));				// 21. boostLeftTime (4) 
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &jetUseRespawn, sizeof(jetUseRespawn));				// 22. jetUseRespawn (1)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &jetRespawnLeftTime, sizeof(jetRespawnLeftTime));	// 23. jetRespawnLeftTime (4)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &jetIsRolling, sizeof(jetIsRolling));				// 24. jetIsRolling (1)
	pRtPtr = NetworkManager::packToByteData(pRtPtr, &jetRollingTime, sizeof(jetRollingTime));			// 25. jetRollingTime (4)		- 94Byte

	return pRtPtr;
}

char* JetFighter::fromGamePacket(GamePacket *pPacket) {
	char *pRtPtr = pPacket->byteData;

	// ������ ��Ʈ��ũ ����ȭ ������ ��Ŷ���κ��� ����
	pRtPtr = NetworkManager::unpackFromByteData(&position.x, pRtPtr, sizeof(position.x));					// 01. Pos-X (4)
	pRtPtr = NetworkManager::unpackFromByteData(&position.y, pRtPtr, sizeof(position.y));					// 02. Pos-Y (4)
	pRtPtr = NetworkManager::unpackFromByteData(&position.z, pRtPtr, sizeof(position.z));					// 03. Pos-Z (4)
	pRtPtr = NetworkManager::unpackFromByteData(&transform.x, pRtPtr, sizeof(transform.x));					// 04. Transfrom-X (4)
	pRtPtr = NetworkManager::unpackFromByteData(&transform.y, pRtPtr, sizeof(transform.y));					// 05. Transfrom-Y (4)
	pRtPtr = NetworkManager::unpackFromByteData(&transform.z, pRtPtr, sizeof(transform.z));					// 06. Transfrom-Z (4)
	pRtPtr = NetworkManager::unpackFromByteData(&transform.w, pRtPtr, sizeof(transform.w));					// 07. Transfrom-W (4)			- 28Byte
	pRtPtr = NetworkManager::unpackFromByteData(&directVector.x, pRtPtr, sizeof(directVector.x));			// 08. DirectVector-X (4)
	pRtPtr = NetworkManager::unpackFromByteData(&directVector.y, pRtPtr, sizeof(directVector.y));			// 09. DirectVector-Y (4)
	pRtPtr = NetworkManager::unpackFromByteData(&directVector.z, pRtPtr, sizeof(directVector.z));			// 10. DirectVector-Z (4)
	pRtPtr = NetworkManager::unpackFromByteData(&upVector.x, pRtPtr, sizeof(upVector.x));					// 11. UpVector-X (4)
	pRtPtr = NetworkManager::unpackFromByteData(&upVector.y, pRtPtr, sizeof(upVector.y));					// 12. UpVector-Y (4)
	pRtPtr = NetworkManager::unpackFromByteData(&upVector.z, pRtPtr, sizeof(upVector.z));					// 13. UpVector-Z (4)
	pRtPtr = NetworkManager::unpackFromByteData(&HP, pRtPtr, sizeof(HP));									// 14. HP (4)					- 56Byte
	pRtPtr = NetworkManager::unpackFromByteData(&objectState, pRtPtr, sizeof(objectState));					// 15. objectState (4)
	pRtPtr = NetworkManager::unpackFromByteData(&currentSpeed, pRtPtr, sizeof(currentSpeed));				// 16. currentSpeed (4)
	pRtPtr = NetworkManager::unpackFromByteData(&maxSpeed, pRtPtr, sizeof(maxSpeed));						// 17. maxSpeed (4)
	pRtPtr = NetworkManager::unpackFromByteData(&currentEngPower, pRtPtr, sizeof(currentEngPower));			// 18. currentEngPower (4)
	pRtPtr = NetworkManager::unpackFromByteData(&engOfferSpeed, pRtPtr, sizeof(engOfferSpeed));				// 19. engOfferSpeed (4)
	pRtPtr = NetworkManager::unpackFromByteData(&currentBoost, pRtPtr, sizeof(currentBoost));				// 20. currentBoost (4) 
	pRtPtr = NetworkManager::unpackFromByteData(&boostLeftTime, pRtPtr, sizeof(boostLeftTime));				// 21. boostLeftTime (4) 
	pRtPtr = NetworkManager::unpackFromByteData(&jetUseRespawn, pRtPtr, sizeof(jetUseRespawn));				// 22. jetUseRespawn (1)
	pRtPtr = NetworkManager::unpackFromByteData(&jetRespawnLeftTime, pRtPtr, sizeof(jetRespawnLeftTime));	// 23. jetRespawnLeftTime (4)
	pRtPtr = NetworkManager::unpackFromByteData(&jetIsRolling, pRtPtr, sizeof(jetIsRolling));				// 24. jetIsRolling (1)
	pRtPtr = NetworkManager::unpackFromByteData(&jetRollingTime, pRtPtr, sizeof(jetRollingTime));			// 25. jetRollingTime (4)		- 94Byte

	// RightVector �� ������� ������ ���س�
	rightVector = Vector3::cross(directVector, upVector);

	return pRtPtr;
}

void JetFighter::setRespawnLeftTime(REAL time) {
	jetRespawnLeftTime = time;
}

REAL JetFighter::getRespawnLeftTime() {
	return jetRespawnLeftTime;
}

REAL JetFighter::getRespawnReqTime() {
	return jetRespawnReqTime;
}

REAL JetFighter::getOffCourseTime() {
	return jetOffcourseTime;
}

void JetFighter::setOffCourseTime(REAL time) {
	jetOffcourseTime = time;
}

string* JetFighter::getCraftNamePtr() {
	return &craftName;
}

list<FlightObject*>* JetFighter::getRadarListPtr() {
	return &radarList;
}

void JetFighter::setLockonWarningState(jet_warning state) {
	jetWarningLockon = state;
}

void JetFighter::setAltitudeWarningState(bool warning) {
	jetWarningAltitude = warning;
}

bool JetFighter::getAltitudeWarningState() {
	return jetWarningAltitude;
}

void JetFighter::setStallWarningState(bool warning) {
	jetWarningStall = warning;
}

bool JetFighter::getStallWarningState() {
	return jetWarningStall;
}

JetFighter::jet_warning JetFighter::getLockonWarningState() {
	return jetWarningLockon;
}

void JetFighter::setRadarRadius(REAL rad) {
	radarRadius = rad;
}

REAL JetFighter::getRadarRadius() {
	return radarRadius;
}

WpnLockonMissile* JetFighter::getBaseWeaponPtr() {
	return pBaseWeapon;
}

WpnLockonMissile* JetFighter::getFirstWeaponPtr() {
	return pFirstWeapon;
}

bool JetFighter::getJetIsRolling() {
	return jetIsRolling;
}

void JetFighter::setJetIsRolling(bool rolling) {
	jetIsRolling = rolling;
}

void JetFighter::respawnFigther(Vector3 pos, Vector3 direction) {
	// �⺻ ����
	objectState = FlightObject::object_state::STATE_ALIVE;
	position = pos;
	directVector = direction;
	upVector = Vector3(0.0, 1.0, 0.0);
	rightVector = Vector3::cross(directVector, upVector);
	HP = maxHP;																				// ����� ȸ��
	if (pBaseWeapon != NULL) pBaseWeapon->setQuantity(pBaseWeapon->getMaxQuantity());		// ���� ź���� ä������
	if (pFirstWeapon != NULL) pFirstWeapon->setQuantity(pFirstWeapon->getMaxQuantity());	// �̻��� ź���� ä������
	maxSpeed = default_maxSpeed;
	currentSpeed = maxSpeed * 0.75;
	currentEngPower = 0.00;
	currentBoost = 100.0;
	boostLeftTime = 0.00;

	// ���� ���� ��
	pSndEngine->play();

	// ������ ���� ������ ���� ������Ʈ
	this->update(0.00);

	// �� Ʈ������ ��Ʈ���� �缳��
	Quaternion temp;
	rotateOffset[0] = 0.0;
	rotateOffset[1] = 0.0;
	rotateOffset[2] = 0.0;
	transform.identity();
	// �� ������ ȸ��
	temp.fromAxisAngle(directVector, rotateOffset[0]);
	transform *= temp;
	temp.fromAxisAngle(upVector, rotateOffset[1]);
	transform *= temp;
	temp.fromAxisAngle(rightVector, rotateOffset[2]);
	transform *= temp;
	// �ٶ󺸴� ���⿡ ���� ȸ��
	temp.fromAxisAngle(directVector, roll);
	transform *= temp;
	temp.fromAxisAngle(upVector, yaw);
	transform *= temp;
	temp.fromAxisAngle(rightVector, pitch);
	transform *= temp;
}

bool JetFighter::checkObjectCollision() {
	/* ����ü���� �浹 �˻� */
	list<FlightObject*> *pObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
	list<FlightObject*>::iterator iterFlightObj = pObjList->begin();
	FlightObject *pObj = NULL;

	/* ����ü���� �浹 ���� �˻� */
	for (; iterFlightObj != pObjList->end(); ++iterFlightObj) {
		pObj = (*iterFlightObj);

		// ����� �ٸ� �÷��̾��� 30m �̳��� �����ϸ� ���� ȿ�� �߰�
		if (pObj != this && pObj->getState() == object_state::STATE_DEAD && FlightObject::distanceBetweenObj(this, pObj) <= 30.0) {
			this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(5.00, 3, ScreenEffect::ScrEffectAction::SCREENEFFECT_DUST, 0);
		}
		
		// ����� ����ü���� �浹�� �����ϰ� �Ÿ��� �ʹ� �� ����ü�� �����Ѵ�
		if (pObj->getState() != object_state::STATE_DEAD && FlightObject::distanceBetweenObj(this, pObj) <= 50.0) {
			// �ڱ� ���� ����ü���� �浹�� ����
			if (pObj->pOwningPlayer != this->pOwningPlayer) {
				// �ڱ� ������ ��� �浹ü��
				for (int i = 0; i < collisionAry.size(); ++i) {
					// �˻� ��� ������ ��� �浹ü�� �浹���� �˻�
					for (int j = 0; j < pObj->collisionAry.size(); ++j) {
						// �� �� OBB�� ���
						if (collisionAry[i]->getType() == CollisionObject::collision_type::COL_OBB &&
							pObj->collisionAry[j]->getType() == CollisionObject::collision_type::COL_OBB) {
							if (static_cast<Collision_OBB*>(collisionAry[i])->detectCollision(static_cast<Collision_OBB*>(pObj->collisionAry[j])) == true) {
								// �浹 �߻� �� ó�� ��ƾ
								dealtDamage(this, 100.0, true, false); // �ڱ� �ڽ��� ���ش� ��Ʈ��ũ �������� ����
								dealtDamage(pObj, 100.0, true);	// ���ظ� �ֵ�, �ڻ� üũ�� ���ظ� ��
								break;
							}
							else {
								// �浹 ��߻��� ó�� ��ƾ
								// . . .
							}
						}
					}
				}
			}
		}
	}

	/* ������� �浹 �˻� */
	bool isCrashed = false;
	if (this->position.y - g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(this->position) <= 50.0) { // �������� �Ÿ��� 50.0 �̳��� ���� ���
		for (int k = 0; k < collisionAry.size(); ++k) {
			// �浹ü ������ OBB�� ��쿡
			if (collisionAry[k]->getType() == CollisionObject::collision_type::COL_OBB) {
				// OBB �� �������� ��ǥ�� �ش� ��ġ�� ����(Terrain)���� ���� ���� ��ġ�ϴ����� �˻�
				Vector3 pt[8];
				Collision_OBB *pOBB = static_cast<Collision_OBB*>(collisionAry[k]);
				Vector3 center = pOBB->getCenter();
				Vector3 lengthXYZ = pOBB->getLengthXYZ();
				Quaternion tf = pOBB->getTransform();
				Matrix4 tfMat = tf.toMatrix4();

				pt[0] = center + (Vector3(-lengthXYZ.x, lengthXYZ.y, -lengthXYZ.z) * tfMat);	// ��� 1			// 1	 2
				pt[1] = center + (Vector3(lengthXYZ.x, lengthXYZ.y, -lengthXYZ.z) * tfMat);		// ��� 2			//  ��/��
				pt[2] = center + (Vector3(-lengthXYZ.x, lengthXYZ.y, lengthXYZ.z) * tfMat);		// ��� 3			// 3	 4
				pt[3] = center + (Vector3(lengthXYZ.x, lengthXYZ.y, lengthXYZ.z) * tfMat);		// ��� 4
				pt[4] = center + (Vector3(-lengthXYZ.x, -lengthXYZ.y, -lengthXYZ.z) * tfMat);	// �ϴ� 1
				pt[5] = center + (Vector3(lengthXYZ.x, -lengthXYZ.y, -lengthXYZ.z) * tfMat);	// �ϴ� 2
				pt[6] = center + (Vector3(-lengthXYZ.x, -lengthXYZ.y, lengthXYZ.z) * tfMat);	// �ϴ� 3
				pt[7] = center + (Vector3(lengthXYZ.x, -lengthXYZ.y, lengthXYZ.z) * tfMat);		// �ϴ� 4

				for (int l = 0; l < 8; ++l) {
					if (pt[l].y <= g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(pt[l])) {
						// �浹 ������ ������ �˻� �ǳʶ�
						isCrashed = true;
						break;
					}
				}
			}

			if (isCrashed == true) {
				dealtDamage(this, 100.0, true, false); // �ڱ� �ڽ��� ���ش� ��Ʈ��ũ �������� ����
				break;
			}
		}
	}

	return false;
}

bool JetFighter::draw() {
	// ���� ���� Ȯ��
	if (getHiding() == true) return true;

	// ����� �׸���
	if (pModel != NULL && isVisible == true && objectState == object_state::STATE_ALIVE) {
		glMatrixMode(GL_MODELVIEW);
		
		// ��� �ʱ�ȭ
		glEnable(GL_DEPTH_TEST);

		// ���� ��ǥ �̵�
		glTranslatef(getPosX(), getPosY(), getPosZ());

		// ���� ����
		glColor4f(color[0], color[1], color[2], color[3]);

		glPushMatrix();
		{
			// ����� ȸ��
			glMultMatrixf(&transform.toMatrix4()[0][0]); // ��ü�� transform ���ʹϾ��� 4x4��Ʈ������ ��ȯ�Ͽ� �𵨺信 ����

			// ����� �����ϸ�
			glScalef(scale[0], scale[1], scale[2]);

			// .obj �� �׸���
			glmDraw(pModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);

			// ������ ���� �׸��� (�̻��Ϸ� ������ ���)
			//if (pFirstWeapon != NULL && pFirstWeapon->getWpnType() == Weapon::weapon_type::WEAPON_MISSILE) {
			//	pFirstWeapon->drawWeapon();
			//}
		}
		glPopMatrix();
	}
	else {
		return false;
	}

	return true;
}

bool JetFighter::destroy() { 
	return true;
}

bool JetFighter::update(REAL timeElapsed) {
	/* ���� ���� Ȯ�� */
	if (getHiding() == true) return true;

	/* ���� Ȯ�� */
	if (this->objectState == object_state::STATE_ALIVE) {
		/* ���̴� ������Ʈ (�浹 ������ ���̱� ������ �׻� �� ���� ��ġ�ؾ� ��) */
		updateRadar();

		/* ������ �ӵ� ������Ʈ */
		updateSpeed(timeElapsed);

		/* �ν��� ó�� */
		updateBoost(timeElapsed);

		/* ������ �̵� �� �浹 */
		REAL updateCounter = 1.0;
		REAL movePerUpdate = mymath::kph2kps(currentSpeed) * timeElapsed;
		if (movePerUpdate > 9.0) {					// �̵� �ӵ��� �ʹ� ���� �浹 ������ ����� �� �ֱ� ������
			updateCounter = movePerUpdate / 9.0;	// ��� �̵��� ���(1620km/h = 9.0m/update �̻�) ª�� �ð��� ������Ʈ�� ���� �� ���Ѵ� (��Ȯ�� ������ �ƴԿ� ����)
		}
		REAL newElapsedTime = (timeElapsed / updateCounter);
		for (int i = 0; i < (int)(updateCounter + 0.99); ++i) {
			updatePosition(newElapsedTime);
			updateCollisionPos(position, &transform);
			checkObjectCollision();
		}

		/* ��ġ, ��, �� ���� ������Ʈ */
		updatePitchRollYaw(timeElapsed);
		
		/* ���� ���¿� �� ���� �� ������Ʈ */
		updateWarning();

		/* ���� �ڽ� �˻� */
		updateOffCourse(timeElapsed);

		/* ����(�Ǽ�) �˻� */
		if (getPosY() > JET_MAX_ALTITUDE && jetWarningStall == false) { // ó�� ���� ����
			Sound *pSnd = new Sound(position, "res/Sounds/beepWarning.wav", false, false);
			pSnd->setChannelNum(pOwningPlayer->getPlayerNumber());
			jetWarningStall = true;
		}
		else if (getPosY() > JET_MAX_ALTITUDE && jetWarningStall == true) { // ���� ����
			if (pitch != -90.0 && roll < 90.0) pitching(-60.0 * timeElapsed);
			else if (pitch != -90.0 && roll > 90.0) pitching(60.0 * timeElapsed);
		}
		else if (getPosY() < JET_MAX_ALTITUDE * 0.95 && jetWarningStall == true) { // ���� ����
			jetWarningStall = false;
		}

		/* �ӵ��� ���� �Ҳ� ���� */
		updateFlameColor(timeElapsed);

		/* �Ѹ� �ð� ���� �� �Ѹ� �ð��� ���� Ʈ���� ���� ȿ�� */
		updateTrail(timeElapsed);

		/* ���� �Ҹ� */
		if (objectState == object_state::STATE_ALIVE) {
			pSndEngine->setPosition(position);
			pSndEngine->setVelocity(directVector * currentSpeed);
			pSndEngine->setGain(currentEngPower / 20.0);
		}
	}
	// ��� ���� ��
	else {
		pSndEngine->setGain(0.0); // ���� �Ҹ� ��
		if (jetUseRespawn == true) { // ������ ��� ��
			if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) { // ������ ������� �ʾ�����
				// ������ ��� �ð� ����
				if ((jetRespawnLeftTime -= timeElapsed) <= 0.0) { 
					// ������
					jetRespawnLeftTime = 0.0; // ������ �ð� �ʱ�ȭ

					// ���� ���� ����
					pSndEngine->pause();
					// ���� ������ ������
					Terrain *pMap = g_pGameManager->getGameFramePtr()->getTerrainPtr();
					// ��Ȱ ���� ( �� ũ���� 80% �αٿ��� �������� �������� ���� )
					REAL ptX = (pMap->getScaledSize().cx / 2.0) * 0.8, ptZ = (pMap->getScaledSize().cy / 2.0) * 0.8;
					Vector3 respawnPos(cosf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptX, 0.0, sinf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptZ);
					// ��Ȱ ���̴� �ش� ���� ������ �ְ� ���̸� ������ �ΰ� �������� ����
					REAL minHeight = pMap->getHeightAtPos(respawnPos);
					respawnPos.y = minHeight + mymath::randomInt(750, pMap->getStallHeight() * 0.25);
					// ��Ȱ �� ���� �߾��� �ٶ󺸵��� ��
					Vector3 directPos(Vector3(0.0, respawnPos.y, 0.0) - respawnPos);
					directPos.normalize();
					// ������ �Լ� ����
					respawnFigther(respawnPos, directPos);
				}
			}
		}
	}
	this->setJetIsRolling(false); // ������ �Ѹ� ���� �ʱ�ȭ (�Ѹ����̶�� ���� ������Ʈ������ true�� ������ ���̱� ������ �������� false�� �ʱ�ȭ)

	return true;
}

void JetFighter::updateOffCourse(REAL timeElapsed) {
	// ���������κ��� ���� ������ ������
	Terrain *pMap = g_pGameManager->getGameFramePtr()->getTerrainPtr();
	REAL mapWidth = pMap->getScaledSize().cx / 2.0, mapHeight = pMap->getScaledSize().cy / 2.0;

	//   �� ����					�� ����						�� ���						�� �ϴ�
	if ((position.x > mapWidth) || (position.x < -mapWidth) || (position.z) < -mapHeight || (position.z) > mapHeight) {
		// ó�� ���� �ڽ� �� ���� ���
		if (jetOffcourseTime <= 0.00) new Sound(position, "res/Sounds/beepWarning.wav", false, false);

		// �ڽ� ��Ż �ð� ����
		(jetOffcourseTime += timeElapsed);
	}
	else { 
		jetOffcourseTime = 0.00; // ��Ż �ð� �ʱ�ȭ
	}

	// 10 �� �̻� ���� ��� ��� ������ �ı�
	if (jetOffcourseTime > 10.00) {
		this->dealtDamage(this, 100.0, true);
	}
}

void JetFighter::updateRadar() {
	GameFrame *pGameFrame = g_pGameManager->getGameFramePtr();
	list<FlightObject*> *pFlightObj = static_cast<CombatFrame*>(pGameFrame)->getFlightObjListPtr();

	// ���̴� ����Ʈ�� ���
	radarList.clear();

	// CombatFrame�� FlightObjList�� ���鼭 �ش� ����ü�� ���̴� �����ȿ� ������ ���̴� ����Ʈ�� �߰�
	for (list<FlightObject*>::iterator iter = pFlightObj->begin(); iter != pFlightObj->end(); ++iter) {
		if ((*iter)->getState() == object_state::STATE_ALIVE) { // ��� �ִ� ��쿡�� ���̴��� �߰�
			if (FlightObject::distanceBetweenObj(this, (*iter)) <= radarRadius && (*iter) != this) {
				radarList.push_back(static_cast<FlightObject*>(*iter));
			}
		}
	}
	
}

void JetFighter::updateWarning() {
	/* �̻��� ��� */
	bool checkBreak = false;
	list<FlightObject*> *pObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
	list<FlightObject*>::iterator iterObj = pObjList->begin();
	Missile *pMssl = NULL;

	// ����ü ����Ʈ�� ���鼭 �̻����� �ڽ��� ������������ Ȯ��
	for (; iterObj != pObjList->end(); ++iterObj) {
		if ((*iterObj)->getWidgetType() == GameWidget::WIDGET_MISSILE) {
			pMssl = static_cast<Missile*>(*iterObj);
			if (pMssl->getLockonTgt() == this && pMssl->getStyle() == false) { // �̻���(�Ѿ� ����) ���� ����� �����̸� �̻��� ���� ���·� ��ȯ �� ������ �˻� ����
				jetWarningLockon = jet_warning::JET_LOCKON_BY_MISSILE;
				checkBreak = true;
				break; // ������ �˻� Ż��
			}
		}
	}

	// �̻����� ���������� ������ �������� ���� ��� Ȯ��
	if (checkBreak == false) {
		/* ���� ��� */
		JetFighter *pJet = NULL;
		WpnLockonMissile *pWpn = NULL;
		list<LockonUI*> *pUIList = NULL;
		list<LockonUI*>::iterator iterUI;
		LockonUI *pUI = NULL;

		// ����ü ����Ʈ�� ���鼭 �����Ⱑ �ڽ��� �����ϰ� �ִ����� Ȯ��
		for (iterObj = pObjList->begin(); iterObj != pObjList->end(); ++iterObj) {
			if ((*iterObj)->getWidgetType() == GameWidget::WIDGET_JET) {
				pJet = static_cast<JetFighter*>(*iterObj);
				if ((pWpn = pJet->getFirstWeaponPtr()) != NULL) { // �̻��Ϸ� ���Ⱑ �ְ�
					if ((pUIList = pWpn->getWpnLockonUIListPtr()) != NULL) { // ���� UI�� �����ϸ�
						for (iterUI = pUIList->begin(); iterUI != pUIList->end(); ++iterUI) { // ���� UI�� ���鼭
							pUI = (*iterUI);
							if (pUI->getTargetFighterPtr() == this && pUI->getLockonSuccess() == true) { // UI�� Ÿ���� �����̰�, ������ ������ ���¶�� ���� ���
								jetWarningLockon = jet_warning::JET_LOCKON_BY_ENEMY;
								checkBreak = true;
								break; // ������ �˻� Ż��
							}
						}
					}
				}

			}

			if (checkBreak == true) break; // ������ �˻� Ż��
		}

		/* ���� ���, �̻��� ��� ���� ���� */
		if (checkBreak == false) {
			jetWarningLockon = jet_warning::JET_LOCKON_NONE;
		}
	}

	/* �� ���*/
	Terrain *pTerrain = g_pGameManager->getGameFramePtr()->getTerrainPtr();
	if (pTerrain != NULL) {
		if (position.y - pTerrain->getHeightAtPos(position) <= 500.0) { // ������ ���̰� ���� �浹 ���̿��� 500.0m �̳��̸�
			if (jetWarningAltitude == false) {
				Sound *pSnd = new Sound(position, "res/Sounds/beepWarning.wav", false, false);
				pSnd->setChannelNum(pOwningPlayer->getPlayerNumber());
			}

			jetWarningAltitude = true; // �� ��� Ȱ��ȭ
		}
		else {
			jetWarningAltitude = false; // �� ��� ��Ȱ��ȭ
		}
	}
}

void JetFighter::updateBoost(REAL timeElapsed) {
	if (boostLeftTime > 0.00) { // �ν��� ��� ��
		// �̻��Ϻ��� 10km/h ���� �ӵ��� �ְ� �ü��� ���� ����
		if ((maxSpeed += (750.0 * timeElapsed)) > 3682.0) {
			maxSpeed = 3682.0;
		}

		// ������ �����ϴ� �ӵ��� ��Ƣ��
		engOfferSpeed = 750.0;

		// ���� ����� �ִ밡 �ǵ��� ���� ��½�Ŵ
		if ((currentEngPower += (75.0 * timeElapsed)) > 100.0) {
			currentEngPower = 100.0;
		}

		// �ν��� ���� �ð� ����
		if ((boostLeftTime -= timeElapsed) < 0.00) {
			boostLeftTime = 0.00;
		}
	}
	else { // �ν��� ��� ���ϴ� ��
		if ((maxSpeed -= ((maxSpeed * 0.35) * timeElapsed)) < default_maxSpeed) maxSpeed = default_maxSpeed;	// �ְ� �ü��� ���� �ʱ�ȭ
		engOfferSpeed = default_engOfferSpeed;	// ������ �����ϴ� �ӵ� �ʰ�ȭ

		if ((currentBoost += (2.5 * timeElapsed)) > 100.0) { // �ν��� ȸ��
			currentBoost = 100.0;
		}
	}
}

void JetFighter::updateTrail(REAL timeElapsed) {
	// �ν��� ��� �߿��� Ʈ������ �׸�
	if (boostLeftTime > 0.00) jetIsRolling = true;

	// Ʈ���� ���� ���� Ȯ��
	if (jetIsRolling == true) {
		if ((jetRollingTime += timeElapsed) > 1.6) jetRollingTime = 1.6;	// �ִ� �����ġ�� 1.6
	}
	else {
		if ((jetRollingTime -= timeElapsed) < 0.00) jetRollingTime = 0.00;	// �ּ� �����ġ�� 0.00
	}
	REAL baseAlpha = 0.00;
	for (int i = 0; i < 2; ++i) {
		if (jetRollingTime > 1.5) { // 1.5 �� �ʰ��� �Ѹ� ���� ��� Ʈ���� ���� ����
			pWingTrail[i]->setTrailCreatable(true);
			if ((baseAlpha = pWingTrail[i]->getBaseAlpha()) < 1.00) { // ���� �� ������� �ڿ������� ��Ÿ�� ǥ��
				if ((baseAlpha += (timeElapsed)) > 1.00) baseAlpha = 1.00;
				pWingTrail[i]->setBaseAlpha(baseAlpha);
			}
		}
		else {
			pWingTrail[i]->setTrailCreatable(false);
			if ((baseAlpha = pWingTrail[i]->getBaseAlpha()) > 0.00) { // ���� �� ���ҷ� �ڿ������� ����� ǥ��
				if ((baseAlpha -= (2.0 * timeElapsed)) < 0.00) {
					baseAlpha = 0.00;
					pWingTrail[i]->initTrail();
				}
				pWingTrail[i]->setBaseAlpha(baseAlpha);
			}
		}
	}
}

void JetFighter::updateFlameColor(REAL timeElapsed) {
	JetFlameEffect *pFlameEffect = NULL;
	static const REAL fadeConst = 1.0;
	for (int i = 0; i < 2; ++i) {
		pFlameEffect = ((i == 0) ? (pFlameLeft) : (pFlameRight));
		REAL flameAlpha = pFlameEffect->getBaseAlpha();	// �Ҳ��� ���� ����
		if (currentEngPower >= 75.0) { // ������ 75%�̻� ���� ��
			REAL fadeInVal = flameAlpha + (fadeConst * timeElapsed);	// �Ҳ��� ���̵� ���� ����
			// �Ҳ� ���̵� ��
			if (fadeInVal < 1.0) {
				pFlameEffect->setBaseAlpha(fadeInVal);
			}
			else {
				pFlameEffect->setBaseAlpha(1.0);
			}
			pFlameEffect->setVisible(true);
		}
		else { // ������ ����
			REAL fadeOutVal = flameAlpha - (fadeConst / 2.0 * timeElapsed);	// �Ҳ��� ���̵� �ƿ����� ����
			// �Ҳ� ���̵� �ƿ�
			if (fadeOutVal > 0.0) {
				pFlameEffect->setBaseAlpha(fadeOutVal);
			}
			else {
				pFlameEffect->setBaseAlpha(0.0);
				pFlameEffect->setVisible(false);
			}
		}

		// �Ҳ� ���� ����
		if (boostLeftTime > 0.00) { // �ν��� ��� ���� ��
			static const Vector3 outter(0.95, 0.95, 0.95);
			static const Vector3 inner(0.55, 0.55, 0.55);
			pFlameEffect->setBlowColor(false, inner); // ����
			pFlameEffect->setBlowColor(true, outter); // �ٱ���	
		}
		else { // �ν��� ������� �ƴ� �� �ӵ��� ���� ����
			REAL speedPercent = (currentSpeed / default_maxSpeed);	// �ְ� ��� ������ ���� ��뷮�� 75%�� ����
			if (speedPercent > 1.00) speedPercent = 1.00;			// �ʰ� ����
			static const Vector3 maxInner(0.8, 0.2, 0.8);
			static const Vector3 maxOutter(0.2, 0.2, 0.9);
			static const Vector3 minInner(0.5, 0.5, 0.1);
			static const Vector3 minOutter(0.9, 0.5, 0.25);
			static const Vector3 dtInner = (maxInner - minInner);
			static const Vector3 dtOutter = (maxOutter - minOutter);
			pFlameEffect->setBlowColor(false, minInner + dtInner * speedPercent); // ����
			pFlameEffect->setBlowColor(true, minOutter + dtOutter * speedPercent); // �ٱ���		
		}
	}
}
