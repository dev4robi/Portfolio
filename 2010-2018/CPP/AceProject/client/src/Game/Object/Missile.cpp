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
	// Missile 클래스 초기화 및 FlightObject 클래스 생성자
	widgetType = GameWidget::WIDGET_MISSILE;
	isBulletStyle = bulletStyle;
	guideDelayTime = 0.0;
	pFireJet = pMissileOwner;
	pLockonTgt = pLockonUI->getTargetFighterPtr();
	pOwningPlayer = pFireJet->pOwningPlayer;
	pFireWeapon = pWeapon;
	damage = pWeapon->getWpnDamage();
	dmgAoE = pWeapon->getWpnAoE();
	homingLevel = 0.8; // 높을수록 유도력이 상승됨.

	// 미사일 타입인 경우
	if (isBulletStyle == false) {
		// 꼬리 연기 효과 부착
		pMsslTrailEffect = new TrailEffect(position, 25);
		pMsslTrailEffect->setAttachTgt(this);
		pMsslTrailEffect->setAttachOffset(Vector3(-2.00, 0.0, 0.0));

		// 미사일 엔진 점화 효과 부착
		pFlameEffect = new JetFlameEffect(position, 0, 0, 0);
		pFlameEffect->setAttachTgt(this);
		pFlameEffect->setAttachOffset(Vector3(-1.99, 0.0, 0.0));
		pFlameEffect->setGlowColor(Vector3(0.75, 0.35, 0.05));
		pFlameEffect->setScale(Vector3(1.75, 1.75, 1.75));
	}
	// 총알 타입인 경우
	else {
		pMsslTrailEffect = new TrailEffect(position, 2, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/gunfire.png"));
		pMsslTrailEffect->setAttachTgt(this);
		pFlameEffect = NULL;
	}

	// FlightObject 나머지 초기화
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
	currentEngPower = 100.0;	// 미사일은 항상 최고 속도를 유지할 수 있도록 함
	engPowerInc = 100.0;
	engPowerDec = 0.0;
	engOfferSpeed = 1000.0;
}

Missile::Missile(Missile& obj) :FlightObject(obj) {
	// 추후 채워넣음...
}

Missile::~Missile() {
	// 붙어있던 이펙트들의 영구성을 제거하여 자연스런 파괴를 유도
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
	// 미사일 그리기
	if (isVisible == true && objectState == object_state::STATE_ALIVE) {
		// 모델이 있는 미사일의 경우
		if (pModel != NULL) {
			glMatrixMode(GL_MODELVIEW);

			// 월드 좌표 이동
			glTranslatef(getPosX(), getPosY(), getPosZ());

			glPushMatrix();
			{
				// 미사일 회전
				glMultMatrixf(&transform.toMatrix4()[0][0]); // 객체의 transform 쿼터니언을 4x4매트릭스로 변환하여 모델뷰에 곱함

				// 미사일 스케일링
				glScalef(scale[0], scale[1], scale[2]);

				// .obj 모델 그리기
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
	// 미사일은 부스터 없음
	currentBoost = 0.00;
	boostLeftTime = 0.00;

	// 유도 지연시간 감소
	if (guideDelayTime > 0.00) guideDelayTime -= timeElapsed;

	// 수명 감소
	HP -= timeElapsed;

	// 파괴, 충돌, 이동, 정보 업데이트
	if (HP <= 0.00) { // 미사일 수명이 다했으면
		return false; // false를 반환하여 위젯 파괴처리를 할 수 있도록 한다
	}
	else { // 미사일 수명이 남아 있으면 남은 사항 업데이트
		// 락온 대상 방향으로 추적 
		if (guideDelayTime <= 0.00) {
			if (pLockonTgt != NULL && FlightObject::isObjPlaceFront(this, pLockonTgt) == true) { // 락온 대상이 존재하고, 앞에 있다면
				Vector3 dirVec = (pLockonTgt->getPos() - position);
				if (dirVec.magnitude() > currentSpeed * timeElapsed) { // 목표와의 거리가 미사일이 한 프레임당 이동할 거리보다 멀 경우에만 유도 사용
					dirVec.normalize();

					// 미사일의 방향벡터를 갱신하고 업벡터와 라이트벡터 또한 새로 구함
					directVector = directVector + (dirVec * homingLevel); // homingLevel 값이 높을수록 고속에서도 유도력이 높아진다
					directVector.normalize();
					upVector = Vector3::cross(Vector3::cross(directVector, Vector3(0.0, 1.0, 0.0)), directVector);
					upVector.normalize();
					rightVector = Vector3::cross(directVector, upVector);
					rightVector.normalize();

					/* 트랜스폼 쿼터니언을 미사일 방향에 따라 수정 */
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
			else { // 락온 대상이 존재하지 않거나, 뒤에 있다면 타겟 초기화
				pLockonTgt = NULL;
			}
		}

		/* 속도 업데이트 */
		updateSpeed(timeElapsed);

		/* 위치 업데이트 */
		REAL updateCounter = 1.0;
		REAL movePerUpdate = mymath::kph2kps(currentSpeed) * timeElapsed;
		if (movePerUpdate > 9.0) {					// 이동 속도가 너무 빨라서 충돌 판정을 벗어나갈 수 있기 때문에
			updateCounter = movePerUpdate / 9.0;	// 고속 이동의 경우(1620km/h = 9.0m/update 이상) 짧은 시간의 업데이트를 여러 번 가한다 (정확한 공식이 아님에 유의)
		}
		REAL newElapsedTime = (timeElapsed / updateCounter);
		for (int i = 0; i < (int)(updateCounter + 0.99); ++i) {
			updatePosition(newElapsedTime);

			/* 비행체와 충돌 업데이트 (미사일은 충돌체를 사용하여 충돌을 감지하지 않고 원형 범위로 직접 계산하여 연산을 줄임) */
			list<FlightObject*> *pFlightObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
			list<FlightObject*>::iterator iterFlightObj = pFlightObjList->begin();
			JetFighter *pJetFighter = NULL;

			// 미사일의 경우
			if (isBulletStyle == false) {
				REAL rgba[4] = { 0.1, 0.1, 0.1, 1.0 };
				
				for (; iterFlightObj != pFlightObjList->end(); ++iterFlightObj) { // 프레임의 비행체 리스트를 살펴보며 전투기와 충돌이 발생했는지 파악
					if ((*iterFlightObj)->getWidgetType() == GameWidget::WIDGET_JET) {
						pJetFighter = static_cast<JetFighter*>(*iterFlightObj);
						if (this->pOwningPlayer != pJetFighter->pOwningPlayer && FlightObject::distanceBetweenObj(this, pJetFighter) <= dmgAoE &&
							pJetFighter->getState() == FlightObject::object_state::STATE_ALIVE) // 서로 적대 관계이고, 미사일 폭파 범위 이내고, 생존한 상태라면
						{
							if (this->dealtDamage(pJetFighter, damage, false) <= 0.00) { // 피해를 가하고 대상이 죽었을 경우
								this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(true, 2.5); // 2.5 초간 DESTROYED 메시지 알림
							}
							else { // 대상이 피해만 입고 죽지 않았음
								this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(false, 1.5); // 1.5 초간 HIT 메시지 알림
							}

							// 이펙트 효과 생성 (미사일)
							SmokeEffect *pEffect = new SmokeEffect(this->getPos(), 2.0, 2.0, 4.0, 8.0, 0); // 검은 연기
							pEffect->setColor(rgba);
							pEffect = new SmokeEffect(this->getPos(), 0.5, 2.0, 6.0, 7.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionCircleA.png")); // 충격파
							pEffect = new SmokeEffect(this->getPos(), 0.5, 8.0, 2.0, 6.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionB.png")); // 화염
							pEffect = NULL;

							// 사운드 재생 (미사일)
							Sound *pSnd = new Sound(position, "res/Sounds/msslExplosion.wav", true, false);
							pSnd->setGain(50.0);

							return false; // 미사일 파괴 요청
						}
					}
				}
			}
			// 총알의 경우
			else {
				// 더미 충돌박스 생성
				REAL boxLength = this->pFireWeapon->getWpnAoE();
				Collision_OBB *pOBB = new Collision_OBB(this->pOwningPlayer->getPlayerNumber(), position, Vector3(0.0, 0.0, 0.0), Vector3(boxLength, boxLength, boxLength));
				for (; iterFlightObj != pFlightObjList->end(); ++iterFlightObj) { // 프레임의 비행체 리스트를 돌면서
					if ((*iterFlightObj)->getState() == FlightObject::object_state::STATE_ALIVE && (*iterFlightObj)->pOwningPlayer != this->pOwningPlayer &&
						(*iterFlightObj)->getWidgetType() == GameWidget::WIDGET_JET && FlightObject::distanceBetweenObj(this, (*iterFlightObj)) <= this->dmgAoE * 2.0) { // 자기 소유가 아니고 살아있는 전투기이고 범위 내 전투기이면
						pJetFighter = static_cast<JetFighter*>(*iterFlightObj);
						for (int i = 0; i < pJetFighter->collisionAry.size(); ++i) { // 해당 비행체의 충돌체 배열의 모든 충돌체들과의 충돌 여부를 검사
							if (pJetFighter->collisionAry[i]->getType() == CollisionObject::collision_type::COL_OBB) { // OBB 충돌체의 경우
								if (static_cast<Collision_OBB*>(pJetFighter->collisionAry[i])->detectCollision(pOBB) == true) { // 충돌 검사

									if (this->dealtDamage(pJetFighter, damage, false) <= 0.00) { // 피해를 가하고 대상이 죽었을 경우
										this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(true, 2.5); // 2.5 초간 DESTROY 메시지 알림
									}
									else { // 대상이 피해만 입고 죽지 않았음
										this->pOwningPlayer->getUImanagerPtr()->getWarningSysPtr()->setSystemAlarm(false, 1.0); // 1.0 초간 HIT 메시지 알림
									}

									// 이펙트 효과 생성 (총알)
									// . . .

									// 더미 충돌박스 파괴
									delete pOBB;	pOBB = NULL;

									return false; // 총알 파괴 요청
								}
							}
						}
					}
				}

				// 더미 충돌박스 파괴
				delete pOBB;	pOBB = NULL;
			}

			/* 지면 충돌 업데이트 */
			REAL terrainHeight = g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(position);
			if (getPosY() <= terrainHeight) {
				// 해당 위치에 이펙트 효과 생성
				Vector3 effPos = this->getPos() + Vector3(0.0, 3.0, 0.0);
				SmokeEffect *pEffect = new SmokeEffect(effPos, 2.0, 2.0, 4.0, 8.0, 0); // 하얀 연기
				if (isBulletStyle == false) {
					pEffect = new SmokeEffect(effPos, 0.5, 2.0, 6.0, 7.0, g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionCircleA.png")); // 충격파(미사일 전용)
					Sound *pSnd = new Sound(position, "res/Sounds/msslExplosion.wav", true, false); // 사운드 (미사일 전용)
					pSnd->setGain(100.0);
				}
				else {
					// 사운드 (기총 전용)

				}

				return false; // 미사일 파괴 요청
			}
		}

		/* 피치, 롤, 요 정보 업데이트 */
		updatePitchRollYaw(timeElapsed);

		return true;
	}
}
