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
	// 전투기 이름
	craftName = "F-22 RAPTOR";

	// 위젯 종류 설정
	widgetType = GameWidget::WIDGET_JET;

	// 플레이어 설정
	pOwningPlayer = _player;

	// 전투기 모델 설정
	pModel = g_pGameManager->getResourceManagerPtr()->getObjModelPtr("res/Models/Fighter/FA-22_Raptor/FA-22_Raptor.obj");

	// 모델 오프셋 조정
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

	// 전투기 상태 및 경고
	jetOffcourseTime = 0.00;
	jetWarningLockon = jet_warning::JET_LOCKON_NONE;
	jetWarningAltitude = false;
	jetWarningStall = false;

	// 레이더 범위
	radarRadius = 10000.0;

	// 상수 로드
	// 회전 속도
	default_pitchTurn = 28.0;
	default_yawTurn = 8.0;
	default_rollTurn = 205.0;
	default_maxSpeed = 2200.0;
	default_boostOfferTime = 6.0;
	default_engOfferSpeed = 125.0;

	// 속도 및 엔진 파워
	maxSpeed = default_maxSpeed;				// 최대 속도
	currentSpeed = maxSpeed * 0.75;				// 초기 속도
	currentEngPower = 0.0;						// 초기 엔진 파워 (0.0 ~ 100.0)
	engPowerInc = 40.0;							// 초당 엔진 파워 증가분
	engPowerDec = 20.0;							// 초당 엔진 파워 감소분
	engOfferSpeed = default_engOfferSpeed;		// 최고 파워일 때 엔진이 제공하는 속도

	// 무기
	pBaseWeapon = new WpnLockonMissile(this, 1);
	pFirstWeapon = new WpnLockonMissile(this, 0);

	// 충돌 감지체 추가
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(1.0, -0.5, 0.0), Vector3(8.75, 1.0, 2.0)));	// 중앙
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(-2.5, -0.75, 0.0), Vector3(2.5, 0.25, 7.0)));	// 앞날개
	collisionAry.push_back(new Collision_OBB(pOwningPlayer->getPlayerNumber(), position, Vector3(-7.0, -0.75, 0.0), Vector3(2.0, 0.25, 4.5)));	// 뒷날개

	// 부가 정보
	jetIsRolling = false;
	jetRollingTime = 0.00;

	// 날개 트레일
	for (int i = 0; i < 2; ++i) {
		pWingTrail[i] = new TrailEffect(position, 25);
		pWingTrail[i]->setAttachTgt(this);
		pWingTrail[i]->setAttachOffset(Vector3(-2.5, -1.0, (i == 0 ? 7.0 : -7.0)));
		pWingTrail[i]->setTrailCreatable(false);
	}

	// 엔진 출력 불꽃 효과
	pFlameLeft = new JetFlameEffect(position, 0.5, 5, 5); // 좌
	pFlameLeft->setAttachTgt(this);
	pFlameLeft->setAttachOffset(Vector3(-6.75, -0.55, -0.625));
	pFlameLeft->setVisible(false);
	pFlameRight = new JetFlameEffect(position, 0.5, 5, 5); // 우
	pFlameRight->setAttachTgt(this);
	pFlameRight->setAttachOffset(Vector3(-6.75, -0.55, 0.625));
	pFlameRight->setVisible(false);

	// 엔진 출력 사운드
	pSndEngine = new Sound(position, "res/Sounds/jetEngine.wav", true, true);
	pSndEngine->setGain(0.0);

	// 리스폰 설정
	jetUseRespawn = true;
	jetRespawnReqTime = 6.00;
	jetRespawnLeftTime = jetRespawnReqTime;
}

JetFighter::JetFighter(JetFighter& _obj) : FlightObject(_obj) {
	// 추후 채워 넣음
}

JetFighter::~JetFighter() {
	// 주무기 보조무기 파괴
	if (pBaseWeapon != NULL) delete pBaseWeapon;
	if (pFirstWeapon != NULL) delete pFirstWeapon;
	pBaseWeapon = NULL;
	pFirstWeapon = NULL;

	// 날개 트레일 이펙트
	for (int i = 0; i < 2; ++i) {
		pWingTrail[i] = NULL;
	}
	
	// 애프터버너 이펙트
	pFlameLeft = NULL;
	pFlameRight = NULL;

	// 사운드 파괴
	pSndEngine->setLooping(false);
	pSndEngine->stop();
	pSndEngine->destory();
	pSndEngine = NULL;
}

char* JetFighter::toGamePacket(GamePacket *pPacket) {
	char *pRtPtr = pPacket->byteData;

	// 전투기 네트워크 동기화 정보를 패킷으로 추출
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

	// 전투기 네트워크 동기화 정보를 패킷으로부터 추출
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

	// RightVector 를 추출받은 값으로 구해냄
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
	// 기본 설정
	objectState = FlightObject::object_state::STATE_ALIVE;
	position = pos;
	directVector = direction;
	upVector = Vector3(0.0, 1.0, 0.0);
	rightVector = Vector3::cross(directVector, upVector);
	HP = maxHP;																				// 생명력 회복
	if (pBaseWeapon != NULL) pBaseWeapon->setQuantity(pBaseWeapon->getMaxQuantity());		// 기총 탄약을 채워넣음
	if (pFirstWeapon != NULL) pFirstWeapon->setQuantity(pFirstWeapon->getMaxQuantity());	// 미사일 탄약을 채워넣음
	maxSpeed = default_maxSpeed;
	currentSpeed = maxSpeed * 0.75;
	currentEngPower = 0.00;
	currentBoost = 100.0;
	boostLeftTime = 0.00;

	// 엔진 사운드 온
	pSndEngine->play();

	// 나머지 정보 갱신을 위한 업데이트
	this->update(0.00);

	// 모델 트랜스폼 매트릭스 재설정
	Quaternion temp;
	rotateOffset[0] = 0.0;
	rotateOffset[1] = 0.0;
	rotateOffset[2] = 0.0;
	transform.identity();
	// 모델 오프셋 회전
	temp.fromAxisAngle(directVector, rotateOffset[0]);
	transform *= temp;
	temp.fromAxisAngle(upVector, rotateOffset[1]);
	transform *= temp;
	temp.fromAxisAngle(rightVector, rotateOffset[2]);
	transform *= temp;
	// 바라보는 방향에 따른 회전
	temp.fromAxisAngle(directVector, roll);
	transform *= temp;
	temp.fromAxisAngle(upVector, yaw);
	transform *= temp;
	temp.fromAxisAngle(rightVector, pitch);
	transform *= temp;
}

bool JetFighter::checkObjectCollision() {
	/* 비행체끼리 충돌 검사 */
	list<FlightObject*> *pObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
	list<FlightObject*>::iterator iterFlightObj = pObjList->begin();
	FlightObject *pObj = NULL;

	/* 비행체끼리 충돌 여부 검사 */
	for (; iterFlightObj != pObjList->end(); ++iterFlightObj) {
		pObj = (*iterFlightObj);

		// 사망한 다른 플레이어의 30m 이내에 접근하면 먼지 효과 추가
		if (pObj != this && pObj->getState() == object_state::STATE_DEAD && FlightObject::distanceBetweenObj(this, pObj) <= 30.0) {
			this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(5.00, 3, ScreenEffect::ScrEffectAction::SCREENEFFECT_DUST, 0);
		}
		
		// 사망한 비행체와의 충돌은 배제하고 거리가 너무 먼 비행체도 배제한다
		if (pObj->getState() != object_state::STATE_DEAD && FlightObject::distanceBetweenObj(this, pObj) <= 50.0) {
			// 자기 소유 비행체와의 충돌은 배제
			if (pObj->pOwningPlayer != this->pOwningPlayer) {
				// 자기 소유의 모든 충돌체가
				for (int i = 0; i < collisionAry.size(); ++i) {
					// 검사 대상 소유의 모든 충돌체와 충돌여부 검사
					for (int j = 0; j < pObj->collisionAry.size(); ++j) {
						// 둘 다 OBB인 경우
						if (collisionAry[i]->getType() == CollisionObject::collision_type::COL_OBB &&
							pObj->collisionAry[j]->getType() == CollisionObject::collision_type::COL_OBB) {
							if (static_cast<Collision_OBB*>(collisionAry[i])->detectCollision(static_cast<Collision_OBB*>(pObj->collisionAry[j])) == true) {
								// 충돌 발생 시 처리 루틴
								dealtDamage(this, 100.0, true, false); // 자기 자신의 피해는 네트워크 전송하지 않음
								dealtDamage(pObj, 100.0, true);	// 피해를 주되, 자살 체크로 피해를 줌
								break;
							}
							else {
								// 충돌 비발생시 처리 루틴
								// . . .
							}
						}
					}
				}
			}
		}
	}

	/* 지면과의 충돌 검사 */
	bool isCrashed = false;
	if (this->position.y - g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(this->position) <= 50.0) { // 지형과의 거리가 50.0 이내일 때만 계산
		for (int k = 0; k < collisionAry.size(); ++k) {
			// 충돌체 종류가 OBB인 경우에
			if (collisionAry[k]->getType() == CollisionObject::collision_type::COL_OBB) {
				// OBB 각 꼭지점의 좌표가 해당 위치의 지형(Terrain)보다 낮은 곳에 위치하는지를 검사
				Vector3 pt[8];
				Collision_OBB *pOBB = static_cast<Collision_OBB*>(collisionAry[k]);
				Vector3 center = pOBB->getCenter();
				Vector3 lengthXYZ = pOBB->getLengthXYZ();
				Quaternion tf = pOBB->getTransform();
				Matrix4 tfMat = tf.toMatrix4();

				pt[0] = center + (Vector3(-lengthXYZ.x, lengthXYZ.y, -lengthXYZ.z) * tfMat);	// 상단 1			// 1	 2
				pt[1] = center + (Vector3(lengthXYZ.x, lengthXYZ.y, -lengthXYZ.z) * tfMat);		// 상단 2			//  상/하
				pt[2] = center + (Vector3(-lengthXYZ.x, lengthXYZ.y, lengthXYZ.z) * tfMat);		// 상단 3			// 3	 4
				pt[3] = center + (Vector3(lengthXYZ.x, lengthXYZ.y, lengthXYZ.z) * tfMat);		// 상단 4
				pt[4] = center + (Vector3(-lengthXYZ.x, -lengthXYZ.y, -lengthXYZ.z) * tfMat);	// 하단 1
				pt[5] = center + (Vector3(lengthXYZ.x, -lengthXYZ.y, -lengthXYZ.z) * tfMat);	// 하단 2
				pt[6] = center + (Vector3(-lengthXYZ.x, -lengthXYZ.y, lengthXYZ.z) * tfMat);	// 하단 3
				pt[7] = center + (Vector3(lengthXYZ.x, -lengthXYZ.y, lengthXYZ.z) * tfMat);		// 하단 4

				for (int l = 0; l < 8; ++l) {
					if (pt[l].y <= g_pGameManager->getGameFramePtr()->getTerrainPtr()->getHeightAtPos(pt[l])) {
						// 충돌 감지시 나머지 검사 건너뜀
						isCrashed = true;
						break;
					}
				}
			}

			if (isCrashed == true) {
				dealtDamage(this, 100.0, true, false); // 자기 자신의 피해는 네트워크 전송하지 않음
				break;
			}
		}
	}

	return false;
}

bool JetFighter::draw() {
	// 숨김 여부 확인
	if (getHiding() == true) return true;

	// 비행기 그리기
	if (pModel != NULL && isVisible == true && objectState == object_state::STATE_ALIVE) {
		glMatrixMode(GL_MODELVIEW);
		
		// 모드 초기화
		glEnable(GL_DEPTH_TEST);

		// 월드 좌표 이동
		glTranslatef(getPosX(), getPosY(), getPosZ());

		// 색상 적용
		glColor4f(color[0], color[1], color[2], color[3]);

		glPushMatrix();
		{
			// 비행기 회전
			glMultMatrixf(&transform.toMatrix4()[0][0]); // 객체의 transform 쿼터니언을 4x4매트릭스로 변환하여 모델뷰에 곱함

			// 비행기 스케일링
			glScalef(scale[0], scale[1], scale[2]);

			// .obj 모델 그리기
			glmDraw(pModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);

			// 날개에 무기 그리기 (미사일류 무기일 경우)
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
	/* 숨김 여부 확인 */
	if (getHiding() == true) return true;

	/* 생존 확인 */
	if (this->objectState == object_state::STATE_ALIVE) {
		/* 레이더 업데이트 (충돌 판정에 쓰이기 때문에 항상 그 위에 위치해야 함) */
		updateRadar();

		/* 전투기 속도 업데이트 */
		updateSpeed(timeElapsed);

		/* 부스터 처리 */
		updateBoost(timeElapsed);

		/* 전투기 이동 및 충돌 */
		REAL updateCounter = 1.0;
		REAL movePerUpdate = mymath::kph2kps(currentSpeed) * timeElapsed;
		if (movePerUpdate > 9.0) {					// 이동 속도가 너무 빨라서 충돌 판정을 벗어나갈 수 있기 때문에
			updateCounter = movePerUpdate / 9.0;	// 고속 이동의 경우(1620km/h = 9.0m/update 이상) 짧은 시간의 업데이트를 여러 번 가한다 (정확한 공식이 아님에 유의)
		}
		REAL newElapsedTime = (timeElapsed / updateCounter);
		for (int i = 0; i < (int)(updateCounter + 0.99); ++i) {
			updatePosition(newElapsedTime);
			updateCollisionPos(position, &transform);
			checkObjectCollision();
		}

		/* 피치, 롤, 요 정보 업데이트 */
		updatePitchRollYaw(timeElapsed);
		
		/* 락온 상태와 고도 조사 후 업데이트 */
		updateWarning();

		/* 오프 코스 검사 */
		updateOffCourse(timeElapsed);

		/* 스톨(실속) 검사 */
		if (getPosY() > JET_MAX_ALTITUDE && jetWarningStall == false) { // 처음 스톨 진입
			Sound *pSnd = new Sound(position, "res/Sounds/beepWarning.wav", false, false);
			pSnd->setChannelNum(pOwningPlayer->getPlayerNumber());
			jetWarningStall = true;
		}
		else if (getPosY() > JET_MAX_ALTITUDE && jetWarningStall == true) { // 스톨 상태
			if (pitch != -90.0 && roll < 90.0) pitching(-60.0 * timeElapsed);
			else if (pitch != -90.0 && roll > 90.0) pitching(60.0 * timeElapsed);
		}
		else if (getPosY() < JET_MAX_ALTITUDE * 0.95 && jetWarningStall == true) { // 스톨 복구
			jetWarningStall = false;
		}

		/* 속도에 따른 불꽃 색상 */
		updateFlameColor(timeElapsed);

		/* 롤링 시간 저장 및 롤링 시간에 따른 트레일 생성 효과 */
		updateTrail(timeElapsed);

		/* 엔진 소리 */
		if (objectState == object_state::STATE_ALIVE) {
			pSndEngine->setPosition(position);
			pSndEngine->setVelocity(directVector * currentSpeed);
			pSndEngine->setGain(currentEngPower / 20.0);
		}
	}
	// 사망 상태 시
	else {
		pSndEngine->setGain(0.0); // 엔진 소리 끔
		if (jetUseRespawn == true) { // 리스폰 사용 시
			if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) { // 세션이 만료되지 않았으면
				// 리스폰 대기 시간 감소
				if ((jetRespawnLeftTime -= timeElapsed) <= 0.0) { 
					// 리스폰
					jetRespawnLeftTime = 0.0; // 리스폰 시간 초기화

					// 엔진 사운드 오프
					pSndEngine->pause();
					// 지형 정보를 가져옴
					Terrain *pMap = g_pGameManager->getGameFramePtr()->getTerrainPtr();
					// 부활 지점 ( 맵 크기의 80% 부근에서 원형으로 무작위로 지정 )
					REAL ptX = (pMap->getScaledSize().cx / 2.0) * 0.8, ptZ = (pMap->getScaledSize().cy / 2.0) * 0.8;
					Vector3 respawnPos(cosf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptX, 0.0, sinf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptZ);
					// 부활 높이는 해당 지점 높이의 최고 높이를 최저로 두고 무작위로 지정
					REAL minHeight = pMap->getHeightAtPos(respawnPos);
					respawnPos.y = minHeight + mymath::randomInt(750, pMap->getStallHeight() * 0.25);
					// 부활 후 맵의 중앙을 바라보도록 함
					Vector3 directPos(Vector3(0.0, respawnPos.y, 0.0) - respawnPos);
					directPos.normalize();
					// 리스폰 함수 수행
					respawnFigther(respawnPos, directPos);
				}
			}
		}
	}
	this->setJetIsRolling(false); // 전투기 롤링 상태 초기화 (롤링중이라면 다음 업데이트때에도 true로 설정될 것이기 때문에 마지막에 false로 초기화)

	return true;
}

void JetFighter::updateOffCourse(REAL timeElapsed) {
	// 프레임으로부터 지형 정보를 가져옴
	Terrain *pMap = g_pGameManager->getGameFramePtr()->getTerrainPtr();
	REAL mapWidth = pMap->getScaledSize().cx / 2.0, mapHeight = pMap->getScaledSize().cy / 2.0;

	//   맵 우측					맵 좌측						맵 상단						맵 하단
	if ((position.x > mapWidth) || (position.x < -mapWidth) || (position.z) < -mapHeight || (position.z) > mapHeight) {
		// 처음 오프 코스 시 사운드 재생
		if (jetOffcourseTime <= 0.00) new Sound(position, "res/Sounds/beepWarning.wav", false, false);

		// 코스 이탈 시간 증가
		(jetOffcourseTime += timeElapsed);
	}
	else { 
		jetOffcourseTime = 0.00; // 이탈 시간 초기화
	}

	// 10 초 이상 맵을 벗어난 경우 전투기 파괴
	if (jetOffcourseTime > 10.00) {
		this->dealtDamage(this, 100.0, true);
	}
}

void JetFighter::updateRadar() {
	GameFrame *pGameFrame = g_pGameManager->getGameFramePtr();
	list<FlightObject*> *pFlightObj = static_cast<CombatFrame*>(pGameFrame)->getFlightObjListPtr();

	// 레이더 리스트를 비움
	radarList.clear();

	// CombatFrame의 FlightObjList를 돌면서 해당 비행체가 레이더 범위안에 있으면 레이더 리스트에 추가
	for (list<FlightObject*>::iterator iter = pFlightObj->begin(); iter != pFlightObj->end(); ++iter) {
		if ((*iter)->getState() == object_state::STATE_ALIVE) { // 살아 있는 경우에만 레이더에 추가
			if (FlightObject::distanceBetweenObj(this, (*iter)) <= radarRadius && (*iter) != this) {
				radarList.push_back(static_cast<FlightObject*>(*iter));
			}
		}
	}
	
}

void JetFighter::updateWarning() {
	/* 미사일 경고 */
	bool checkBreak = false;
	list<FlightObject*> *pObjList = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr();
	list<FlightObject*>::iterator iterObj = pObjList->begin();
	Missile *pMssl = NULL;

	// 비행체 리스트를 돌면서 미사일이 자신을 추적중인지를 확인
	for (; iterObj != pObjList->end(); ++iterObj) {
		if ((*iterObj)->getWidgetType() == GameWidget::WIDGET_MISSILE) {
			pMssl = static_cast<Missile*>(*iterObj);
			if (pMssl->getLockonTgt() == this && pMssl->getStyle() == false) { // 미사일(총알 제외) 추적 대상이 본인이면 미사일 락온 상태로 변환 후 나머지 검사 생략
				jetWarningLockon = jet_warning::JET_LOCKON_BY_MISSILE;
				checkBreak = true;
				break; // 나머지 검사 탈출
			}
		}
	}

	// 미사일이 추적중이지 않으면 전투기의 락온 경고 확인
	if (checkBreak == false) {
		/* 락온 경고 */
		JetFighter *pJet = NULL;
		WpnLockonMissile *pWpn = NULL;
		list<LockonUI*> *pUIList = NULL;
		list<LockonUI*>::iterator iterUI;
		LockonUI *pUI = NULL;

		// 비행체 리스트를 돌면서 전투기가 자신을 락온하고 있는지를 확인
		for (iterObj = pObjList->begin(); iterObj != pObjList->end(); ++iterObj) {
			if ((*iterObj)->getWidgetType() == GameWidget::WIDGET_JET) {
				pJet = static_cast<JetFighter*>(*iterObj);
				if ((pWpn = pJet->getFirstWeaponPtr()) != NULL) { // 미사일류 무기가 있고
					if ((pUIList = pWpn->getWpnLockonUIListPtr()) != NULL) { // 락온 UI가 존재하면
						for (iterUI = pUIList->begin(); iterUI != pUIList->end(); ++iterUI) { // 락온 UI를 돌면서
							pUI = (*iterUI);
							if (pUI->getTargetFighterPtr() == this && pUI->getLockonSuccess() == true) { // UI의 타겟이 본인이고, 락온이 성공한 상태라면 락온 경고
								jetWarningLockon = jet_warning::JET_LOCKON_BY_ENEMY;
								checkBreak = true;
								break; // 나머지 검사 탈출
							}
						}
					}
				}

			}

			if (checkBreak == true) break; // 나머지 검사 탈출
		}

		/* 락온 경고도, 미사일 경고도 없는 상태 */
		if (checkBreak == false) {
			jetWarningLockon = jet_warning::JET_LOCKON_NONE;
		}
	}

	/* 고도 경고*/
	Terrain *pTerrain = g_pGameManager->getGameFramePtr()->getTerrainPtr();
	if (pTerrain != NULL) {
		if (position.y - pTerrain->getHeightAtPos(position) <= 500.0) { // 전투기 높이가 지형 충돌 높이에서 500.0m 이내이면
			if (jetWarningAltitude == false) {
				Sound *pSnd = new Sound(position, "res/Sounds/beepWarning.wav", false, false);
				pSnd->setChannelNum(pOwningPlayer->getPlayerNumber());
			}

			jetWarningAltitude = true; // 고도 경고 활성화
		}
		else {
			jetWarningAltitude = false; // 고도 경고 비활성화
		}
	}
}

void JetFighter::updateBoost(REAL timeElapsed) {
	if (boostLeftTime > 0.00) { // 부스터 사용 중
		// 미사일보다 10km/h 빠른 속도로 최고 시속을 점차 변경
		if ((maxSpeed += (750.0 * timeElapsed)) > 3682.0) {
			maxSpeed = 3682.0;
		}

		// 엔진이 제공하는 속도를 뻥튀기
		engOfferSpeed = 750.0;

		// 엔진 사용을 최대가 되도록 점차 상승시킴
		if ((currentEngPower += (75.0 * timeElapsed)) > 100.0) {
			currentEngPower = 100.0;
		}

		// 부스터 지속 시간 감소
		if ((boostLeftTime -= timeElapsed) < 0.00) {
			boostLeftTime = 0.00;
		}
	}
	else { // 부스터 사용 안하는 중
		if ((maxSpeed -= ((maxSpeed * 0.35) * timeElapsed)) < default_maxSpeed) maxSpeed = default_maxSpeed;	// 최고 시속을 점차 초기화
		engOfferSpeed = default_engOfferSpeed;	// 엔진이 제공하는 속도 초가화

		if ((currentBoost += (2.5 * timeElapsed)) > 100.0) { // 부스터 회복
			currentBoost = 100.0;
		}
	}
}

void JetFighter::updateTrail(REAL timeElapsed) {
	// 부스터 사용 중에도 트레일을 그림
	if (boostLeftTime > 0.00) jetIsRolling = true;

	// 트레일 리는 여부 확인
	if (jetIsRolling == true) {
		if ((jetRollingTime += timeElapsed) > 1.6) jetRollingTime = 1.6;	// 최대 저장수치는 1.6
	}
	else {
		if ((jetRollingTime -= timeElapsed) < 0.00) jetRollingTime = 0.00;	// 최소 저장수치는 0.00
	}
	REAL baseAlpha = 0.00;
	for (int i = 0; i < 2; ++i) {
		if (jetRollingTime > 1.5) { // 1.5 초 초과로 롤링 중인 경우 트레일 생성 시작
			pWingTrail[i]->setTrailCreatable(true);
			if ((baseAlpha = pWingTrail[i]->getBaseAlpha()) < 1.00) { // 알파 값 상승으로 자연스럽게 나타남 표현
				if ((baseAlpha += (timeElapsed)) > 1.00) baseAlpha = 1.00;
				pWingTrail[i]->setBaseAlpha(baseAlpha);
			}
		}
		else {
			pWingTrail[i]->setTrailCreatable(false);
			if ((baseAlpha = pWingTrail[i]->getBaseAlpha()) > 0.00) { // 알파 값 감소로 자연스럽게 사라짐 표현
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
		REAL flameAlpha = pFlameEffect->getBaseAlpha();	// 불꽃의 현재 알파
		if (currentEngPower >= 75.0) { // 엔진이 75%이상 가동 중
			REAL fadeInVal = flameAlpha + (fadeConst * timeElapsed);	// 불꽃의 페이드 인중 알파
			// 불꽃 페이드 인
			if (fadeInVal < 1.0) {
				pFlameEffect->setBaseAlpha(fadeInVal);
			}
			else {
				pFlameEffect->setBaseAlpha(1.0);
			}
			pFlameEffect->setVisible(true);
		}
		else { // 엔진이 꺼짐
			REAL fadeOutVal = flameAlpha - (fadeConst / 2.0 * timeElapsed);	// 불꽃의 페이드 아웃중인 알파
			// 불꽃 페이드 아웃
			if (fadeOutVal > 0.0) {
				pFlameEffect->setBaseAlpha(fadeOutVal);
			}
			else {
				pFlameEffect->setBaseAlpha(0.0);
				pFlameEffect->setVisible(false);
			}
		}

		// 불꽃 색상 결정
		if (boostLeftTime > 0.00) { // 부스터 사용 중일 시
			static const Vector3 outter(0.95, 0.95, 0.95);
			static const Vector3 inner(0.55, 0.55, 0.55);
			pFlameEffect->setBlowColor(false, inner); // 안쪽
			pFlameEffect->setBlowColor(true, outter); // 바깥쪽	
		}
		else { // 부스터 사용중이 아닐 시 속도에 따라 결정
			REAL speedPercent = (currentSpeed / default_maxSpeed);	// 최고 고온 색상을 엔진 사용량의 75%로 보정
			if (speedPercent > 1.00) speedPercent = 1.00;			// 초과 보정
			static const Vector3 maxInner(0.8, 0.2, 0.8);
			static const Vector3 maxOutter(0.2, 0.2, 0.9);
			static const Vector3 minInner(0.5, 0.5, 0.1);
			static const Vector3 minOutter(0.9, 0.5, 0.25);
			static const Vector3 dtInner = (maxInner - minInner);
			static const Vector3 dtOutter = (maxOutter - minOutter);
			pFlameEffect->setBlowColor(false, minInner + dtInner * speedPercent); // 안쪽
			pFlameEffect->setBlowColor(true, minOutter + dtOutter * speedPercent); // 바깥쪽		
		}
	}
}
