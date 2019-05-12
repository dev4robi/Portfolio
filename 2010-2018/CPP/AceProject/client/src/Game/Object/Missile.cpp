#include "Game/Object/Missile.h"
#include "Game/Object/Weapon.h"
#include "Game/Object/JetFighter.h"
#include "Game/Frame/CombatFrame.h"
#include "Game/Object/Player.h"
#include "Game/Object/Terrain.h"
#include "Game/UI/UI.h"
#include "Game/Effect/GL_Effect.h"
#include "Engine/SoundManager.h"

Missile::Missile(JetFighter *pMissileOwner, LockonUI *pLockonUI, Weapon *pWeapon, bool bulletStyle) : FlightObject(pMissileOwner->getPos()) {
	// Missile Ŭ���� �ʱ�ȭ �� FlightObject Ŭ���� ������
	widgetType = GameWidget::WIDGET_MISSILE;
	isBulletStyle = bulletStyle;
	guideDelayTime = 0.0;
	pFireJet = pMissileOwner;
	pLockonTgt = pLockonUI->getTargetFighterPtr();
	pOwningPlayer = pFireJet->pOwningPlayer;
	pFireWeapon = pWeapon;
	damage = pWeapon->getWpnDamage();
	dmgAoE = pWeapon->getWpnAoE();
	homingLevel = 0.8; // �������� �������� ��µ�.

	// �̻��� Ÿ���� ���
	if (isBulletStyle == false) {
		// ���� ���� ȿ�� ����
		pMsslTrailEffect = new TrailEffect(position, 25);
		pMsslTrailEffect->setAttachTgt(this);
		pMsslTrailEffect->setAttachOffset(Vector3(-2.00, 0.0, 0.0));

		// �̻��� ���� ��ȭ ȿ�� ����
		pFlameEffect = new JetFlameEffect(position, 0, 0, 0);
		pFlameEffect->setAttachTgt(this);
		pFlameEffect->setAttachOffset(Vector3(-1.99, 0.0, 0.0));
		pFlameEffect->setGlowColor(Vector3(0.75, 0.35, 0.05));
		pFlameEffect->setScale(Vector3(1.75, 1.75, 1.75));
	}
	// �Ѿ� Ÿ���� ���
	else {
		pMsslTrailEffect = new TrailEffect(position, 2, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/gunfire.png"));
		pMsslTrailEffect->setAttachTgt(this);
		pFlameEffect = NULL;
	}

	// FlightObject ������ �ʱ�ȭ
	pModel = pWeapon->getWpnModelPtr();
	directVector = pFireJet->directVector;
	upVector = pFireJet->upVector;
	rightVector = pFireJet->rightVector;
	transform = pFireJet->transform;
	pitch = pFireJet->pitch;
	yaw = pFireJet->yaw;
	roll = pFireJet->roll;
	maxHP = pWeapon->getMissileDuration();
	HP = maxHP;
	default_pitchTurn = 1.00;
	default_yawTurn = 1.00;
	default_rollTurn = 1.00;
	default_maxSpeed = currentSpeed = pWeapon->getMissileSpeed();
	maxSpeed = default_maxSpeed;
	currentSpeed = maxSpeed;
	currentEngPower = 100.0;	// �̻����� �׻� �ְ� �ӵ��� ������ �� �ֵ��� ��
	engPowerInc = 100.0;
	engPowerDec = 0.0;
	engOfferSpeed = 1000.0;
}

Missile::Missile(Missile& obj) :FlightObject(obj) {
	// ���� ä������...
}

Missile::~Missile() {
	// �پ��ִ� ����Ʈ���� �������� �����Ͽ� �ڿ����� �ı��� ����
	if (pMsslTrailEffect != NULL)
		pMsslTrailEffect->setPermanency(false);
	pMsslTrailEffect = NULL;
	if (pFlameEffect != NULL)
		pFlameEffect->setPermanency(false);
	pFlameEffect = NULL;
}

JetFighter* Missile::getLockonTgt() {
	return pLockonTgt;
}

void Missile::setDamage(REAL dmg) {
	damage = dmg;
}

REAL Missile::getDamage() {
	return damage;
}

void Missile::setDmgAoE(REAL AoE) {
	dmgAoE = AoE;
}

REAL Missile::getDmgAoE() {
	return dmgAoE;
}

bool Missile::getStyle() {
	return isBulletStyle;
}

bool Missile::checkObjectCollision() {
	return false;
}

bool Missile::draw() {
	// �̻��� �׸���
	if (isVisible == true && objectState == object_state::STATE_ALIVE) {
		// ���� �ִ� �̻����� ���
		if (pModel != NULL) {
			glMatrixMode(GL_MODELVIEW);

			// ���� ��ǥ �̵�
			glTranslatef(getPosX(), getPosY(), getPosZ());

			glPushMatrix();
			{
				// �̻��� ȸ��
				glMultMatrixf(&transform.toMatrix4()[0][0]); // ��ü�� transform ���ʹϾ��� 4x4��Ʈ������ ��ȯ�Ͽ� �𵨺信 ����

				// �̻��� �����ϸ�
				glScalef(scale[0], scale[1], scale[2]);

				// .obj �� �׸���
				glmDraw(pModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
			}
			glPopMatrix();
		}
		else {
			return false;
		}
	}

	return true;
}

bool Missile::destory() {
	return true;
}

bool Missile::update(REAL timeElapsed) {
	// �̻����� �ν��� ����
	currentBoost = 0.00;
	boostLeftTime = 0.00;

	// ���� �����ð� ����
	if (guideDelayTime > 0.00) guideDelayTime -= timeElapsed;

	// ���� ����
	HP -= timeElapsed;

	// �ı�, �浹, �̵�, ���� ������Ʈ
	if (HP <= 0.00) { // �̻��� ������ ��������
		return false; // false�� ��ȯ�Ͽ� ���� �ı�ó���� �� �� �ֵ��� �Ѵ�
	}
	else { // �̻��� ������ ���� ������ ���� ���� ������Ʈ
		// ���� ��� �������� ���� 
		if (guideDelayTime <= 0.00) {
			if (pLockonTgt != NULL && FlightObject::isObjPlaceFront(this, pLockonTgt) == true) { // ���� ����� �����ϰ�, �տ� �ִٸ�
				Vector3 dirVec = (pLockonTgt->getPos() - position);
				if (dirVec.magnitude() > currentSpeed * timeElapsed) { // ��ǥ���� �Ÿ��� �̻����� �� �����Ӵ� �̵��� �Ÿ����� �� ��쿡�� ���� ���
					dirVec.normalize();

					// �̻����� ���⺤�͸� �����ϰ� �����Ϳ� ����Ʈ���� ���� ���� ����
					directVector = directVector + (dirVec * homingLevel); // homingLevel ���� �������� ��ӿ����� �������� ��������
					directVector.normalize();
					upVector = Vector3::cross(Vector3::cross(directVector, Vector3(0.0, 1.0, 0.0)), directVector);
					upVector.normalize();
					rightVector = Vector3::cross(directVector, upVector);
					rightVector.normalize();

					/* Ʈ������ ���ʹϾ��� �̻��� ���⿡ ���� ���� */
					Quaternion q;
					transform.identity();
					//q.fromAxisAngle(directVector, roll);
					//transform *= q;
					q.fromAxisAngle(upVector, yaw);
					transform *= q;
					q.fromAxisAngle(rightVector, pitch);
					transform *= q;
				}
			}
			else { // ���� ����� �������� �ʰų�, �ڿ� �ִٸ� Ÿ�� �ʱ�ȭ
				pLockonTgt = NULL;
			}
		}

		/* �ӵ� ������Ʈ */
		updateSpeed(timeElapsed);

		/* ��ġ ������Ʈ */
		REAL updateCounter = 1.0;
		REAL movePerUpdate = mymath::kph2kps(currentSpeed) * timeElapsed;
		if (movePerUpdate > 9.0) {					// �̵� �ӵ��� �ʹ� ���� �浹 ������ ����� �� �ֱ� ������
			updateCounter = movePerUpdate / 9.0;	// ��� �̵��� ���(1620km/h = 9.0m/update �̻�) ª�� �ð��� ������Ʈ�� ���� �� ���Ѵ� (��Ȯ�� ������ �ƴԿ� ����)
		}
		REAL newElapsedTime = (timeElapsed / updateCounter);
		for (int i = 0; i < (int)(updateCounter + 0.99); ++i) {
			updatePosition(newElapsedTime);

			/* ����ü�� �浹 ������Ʈ (�̻����� �浹ü�� ����Ͽ� �浹�� �������� �ʰ� ���� ������ ���� ����Ͽ� ������ ����) */
			list<FlightObject*> *pFlightObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
			list<FlightObject*>::iterator iterFlightObj = pFlightObjList->begin();
			JetFighter *pJetFighter = NULL;

			// �̻����� ���
			if (isBulletStyle == false) {
				REAL rgba[4] = { 0.1, 0.1, 0.1, 1.0 };
				
				for (; iterFlightObj != pFlightObjList->end(); ++iterFlightObj) { // �������� ����ü ����Ʈ�� ���캸�� ������� �浹�� �߻��ߴ��� �ľ�
					if ((*iterFlightObj)->getWidgetType() == GameWidget::WIDGET_JET) {
						pJetFighter = static_cast<JetFighter*>(*iterFlightObj);
						if (this->pOwningPlayer != pJetFighter->pOwningPlayer && FlightObject::distanceBetweenObj(this, pJetFighter) <= dmgAoE &&
							pJetFighter->getState() == FlightObject::object_state::STATE_ALIVE) // ���� ���� �����̰�, �̻��� ���� ���� �̳���, ������ ���¶��
						{
							if (this->dealtDamage(pJetFighter, damage, false) <= 0.00) { // ���ظ� ���ϰ� ����� �׾��� ���
								this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(true, 2.5); // 2.5 �ʰ� DESTROYED �޽��� �˸�
							}
							else { // ����� ���ظ� �԰� ���� �ʾ���
								this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(false, 1.5); // 1.5 �ʰ� HIT �޽��� �˸�
							}

							// ����Ʈ ȿ�� ���� (�̻���)
							SmokeEffect *pEffect = new SmokeEffect(this->getPos(), 2.0, 2.0, 4.0, 8.0, 0); // ���� ����
							pEffect->setColor(rgba);
							pEffect = new SmokeEffect(this->getPos(), 0.5, 2.0, 6.0, 7.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionCircleA.png")); // �����
							pEffect = new SmokeEffect(this->getPos(), 0.5, 8.0, 2.0, 6.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionB.png")); // ȭ��
							pEffect = NULL;

							// ���� ��� (�̻���)
							Sound *pSnd = new Sound(position, "res/Sounds/msslExplosion.wav", true, false);
							pSnd->setGain(50.0);

							return false; // �̻��� �ı� ��û
						}
					}
				}
			}
			// �Ѿ��� ���
			else {
				// ���� �浹�ڽ� ����
				REAL boxLength = this->pFireWeapon->getWpnAoE();
				Collision_OBB *pOBB = new Collision_OBB(this->pOwningPlayer->getPlayerNumber(), position, Vector3(0.0, 0.0, 0.0), Vector3(boxLength, boxLength, boxLength));
				for (; iterFlightObj != pFlightObjList->end(); ++iterFlightObj) { // �������� ����ü ����Ʈ�� ���鼭
					if ((*iterFlightObj)->getState() == FlightObject::object_state::STATE_ALIVE && (*iterFlightObj)->pOwningPlayer != this->pOwningPlayer &&
						(*iterFlightObj)->getWidgetType() == GameWidget::WIDGET_JET && FlightObject::distanceBetweenObj(this, (*iterFlightObj)) <= this->dmgAoE * 2.0) { // �ڱ� ������ �ƴϰ� ����ִ� �������̰� ���� �� �������̸�
						pJetFighter = static_cast<JetFighter*>(*iterFlightObj);
						for (int i = 0; i < pJetFighter->collisionAry.size(); ++i) { // �ش� ����ü�� �浹ü �迭�� ��� �浹ü����� �浹 ���θ� �˻�
							if (pJetFighter->collisionAry[i]->getType() == CollisionObject::collision_type::COL_OBB) { // OBB �浹ü�� ���
								if (static_cast<Collision_OBB*>(pJetFighter->collisionAry[i])->detectCollision(pOBB) == true) { // �浹 �˻�

									if (this->dealtDamage(pJetFighter, damage, false) <= 0.00) { // ���ظ� ���ϰ� ����� �׾��� ���
										this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(true, 2.5); // 2.5 �ʰ� DESTROY �޽��� �˸�
									}
									else { // ����� ���ظ� �԰� ���� �ʾ���
										this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(false, 1.0); // 1.0 �ʰ� HIT �޽��� �˸�
									}

									// ����Ʈ ȿ�� ���� (�Ѿ�)
									// . . .

									// ���� �浹�ڽ� �ı�
									delete pOBB;	pOBB = NULL;

									return false; // �Ѿ� �ı� ��û
								}
							}
						}
					}
				}

				// ���� �浹�ڽ� �ı�
				delete pOBB;	pOBB = NULL;
			}

			/* ���� �浹 ������Ʈ */
			REAL terrainHeight = g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(position);
			if (getPosY() <= terrainHeight) {
				// �ش� ��ġ�� ����Ʈ ȿ�� ����
				Vector3 effPos = this->getPos() + Vector3(0.0, 3.0, 0.0);
				SmokeEffect *pEffect = new SmokeEffect(effPos, 2.0, 2.0, 4.0, 8.0, 0); // �Ͼ� ����
				if (isBulletStyle == false) {
					pEffect = new SmokeEffect(effPos, 0.5, 2.0, 6.0, 7.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionCircleA.png")); // �����(�̻��� ����)
					Sound *pSnd = new Sound(position, "res/Sounds/msslExplosion.wav", true, false); // ���� (�̻��� ����)
					pSnd->setGain(100.0);
				}
				else {
					// ���� (���� ����)

				}

				return false; // �̻��� �ı� ��û
			}
		}

		/* ��ġ, ��, �� ���� ������Ʈ */
		updatePitchRollYaw(timeElapsed);

		return true;
	}
}
