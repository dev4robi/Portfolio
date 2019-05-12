#include "Game/Object/Weapon.h"
#include "Game/Object/FlightObject.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Player.h"
#include "Game/Object/Missile.h"
#include "Game/UI/UI.h"
#include "Engine/ResourceManager.h"
#include "Engine/DrawManager.h"
#include "Engine/GameFrame.h"
#include "Engine/SoundManager.h"

/*************************************************************************************************************/
/**************************************************** Weapon *************************************************/
/*************************************************************************************************************/

Weapon::Weapon(JetFighter* pOwner, int id) : GameObject() {
	wpnType = weapon_type::WEAPON_DEFAULT;
	wpnID = id;
	pWpnOwner = pOwner;
	wpnName = "";
	pWpnModel = NULL;
	wpnQuantity = 0;
	wpnMaxQuantity = 0;
	wpnDamage = 0.0;
	wpnFirePerSecond = 0.0;
	wpnIsReady = true;
	wpnAoE = 0.0;
	wpnMissileDuration = 0.0;
	wpnMissileSpeed = 0.0;
}

Weapon::Weapon(Weapon& obj) {
	wpnType = obj.wpnType;
	wpnID = obj.wpnID;
	pWpnOwner = obj.pWpnOwner;
	wpnName = obj.wpnName;
	pWpnModel = obj.pWpnModel;
	wpnQuantity = obj.wpnQuantity;
	wpnMaxQuantity = obj.wpnMaxQuantity;
	wpnDamage = obj.wpnDamage;
	wpnFirePerSecond = obj.wpnFirePerSecond;
	wpnIsReady = obj.wpnIsReady;
	wpnAoE = obj.wpnAoE;
	wpnMissileDuration = obj.wpnMissileDuration;
	wpnMissileSpeed = obj.wpnMissileSpeed;
}

Weapon::~Weapon() {
	pWpnOwner = NULL;
}

JetFighter* Weapon::getWpnOwnerPtr() {
	return pWpnOwner;
}

void Weapon::setWpnType(weapon_type type) {
	wpnType = type;
}

Weapon::weapon_type Weapon::getWpnType() {
	return wpnType;
}

void Weapon::setQuantity(int quantity) {
	wpnQuantity = quantity;
}

int Weapon::getQuantity() {
	return wpnQuantity;
}

void Weapon::setMaxQuantity(int quantity) {
	wpnMaxQuantity = quantity;
}

int Weapon::getMaxQuantity() {
	return wpnMaxQuantity;
}

void Weapon::setWpnDamage(REAL damage) {
	wpnDamage = damage;
}

REAL Weapon::getWpnDamage() {
	return wpnDamage;
}

void Weapon::setFirePerSecond(REAL firePerSecond) {
	wpnFirePerSecond = firePerSecond;
}

REAL Weapon::getFirePerSecond() {
	return wpnFirePerSecond;
}

void Weapon::setWpnIsReady(bool ready) {
	wpnIsReady = ready;
}

bool Weapon::getWpnIsReady() {
	return wpnIsReady;
}

void Weapon::setWpnAoE(REAL aoe) {
	wpnAoE = aoe;
}

REAL Weapon::getWpnAoE() {
	return wpnAoE;
}

REAL Weapon::getMissileDuration() {
	return wpnMissileDuration;
}

REAL Weapon::getMissileSpeed() {
	return wpnMissileSpeed;
}

GLMmodel* Weapon::getWpnModelPtr() {
	return pWpnModel;
}

void Weapon::drawWeapon() {
	// . . . (가상 함수)
}

void Weapon::fireWeapon() {
	// . . . (가상 함수)
}

void Weapon::update(REAL timeElapsed) {
	// . . . (가상 함수)
}

/*************************************************************************************************************/
/*********************************************** WpnLockonMissile ********************************************/
/*************************************************************************************************************/

WpnLockonMissile::WpnLockonMissile(JetFighter *pOwner, int id) : Weapon(pOwner, id) {
	bool isLoopingSound = false;

	wpnNextFireMissileNum = 0;
	wpnTgtChangeRequested = false;

	// 미사일 종류에 따른 기본 설정 로드
	switch (id) {
	default:
		wpnType = weapon_type::WEAPON_MISSILE;
		wpnName = "AIM-120D";
		lockonUITexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/weaponLockon.png");
		pWpnModel = g_pGameManager->getResourceManagerPtr()->getObjModelPtr("res/Models/Missile/AIM120D/AIM120D.obj");
		wpnMaxQuantity = 4000; // 120;
		wpnQuantity = wpnMaxQuantity;
		wpnDamage = 45.0;
		wpnFirePerSecond = 0.2;
		wpnIsReady = true;
		wpnMissileDuration = 10.0;
		wpnMissileSpeed = 3672.0;
		// ↑ Weapon - - - WpnLockonMissile ↓ //
		wpnLockonCount = 1;
		wpnMissileCount = 4;
		wpnAoE = 10.0;
		wpnLockonSpeed = 700.0;
		wpnLockonRadOnScr = 350.0;
		wpnLockonRange = 5000.0;
		wpnFireSoundName = "res/Sounds/msslFire.wav";
		isLoopingSound = false;
		break;
	case 1: // GUN
		wpnType = weapon_type::WEAPON_GUN;
		wpnName = "GUN - 100";
		lockonUITexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/gatlingAim.png");
		pWpnModel = NULL;
		wpnMaxQuantity = 800;
		wpnQuantity = wpnMaxQuantity;
		wpnDamage = 3.0;
		wpnFirePerSecond = 16;
		wpnIsReady = true;
		wpnMissileDuration = 1.0;
		wpnMissileSpeed = 3672.0;
		// ↑ Weapon - - - WpnGatlingGun ↓ //
		wpnLockonCount = 1;
		wpnMissileCount = 1;
		wpnAoE = 3.0;
		wpnLockonSpeed = 150.0;
		wpnLockonRadOnScr = 275.0;
		wpnLockonRange = 550.0;
		wpnFireSoundName = "res/Sounds/gunFire.wav";
		isLoopingSound = true;
		break;
	}
	// 논리 오류 처리
	if (wpnLockonCount > wpnMissileCount) wpnLockonCount = wpnMissileCount;	// 한번에 락온 가능한 타겟의 수는 미사일의 수를 초과할 수 없음
	if (wpnLockonCount <= 0) wpnLockonCount = 1; // 락온은 1명 이상 가능해야 함

	// 벡터 초기화
	wpnFireLeftTime.reserve(wpnMissileCount); // std::vector 의 크기를 미사일 개수로 예약해둔다

	// 무기 재장전을 완료시킴
	for (int i = 0; i < wpnMissileCount; ++i) {
		wpnFireLeftTime.push_back(0.0);
	}

	// 락온 위젯을 락온 가능한 수만큼 생성시킴
	for (int j = 0; j < wpnLockonCount; ++j) {
		wpnLockonUIList.push_back(new LockonUI(pOwner, lockonUITexture));
	}

	// 락온 위젯 모두 초기화
	initLockonUIAll();

	// 사운드 설정 (루프하는 사운드의 경우 동적 생성을 미리 수행)
	if (isLoopingSound == true) {
		pWpnFireSound = new Sound(pWpnOwner->getPos(), wpnFireSoundName, true, true);
		if (pWpnFireSound != NULL) pWpnFireSound->stop();
	}
	else {
		pWpnFireSound = NULL;
	}
}

WpnLockonMissile::WpnLockonMissile(WpnLockonMissile& obj) : Weapon(obj) {
	// 미사일 정보 복사
	pWpnModel = obj.pWpnModel;
	wpnLockonCount = obj.wpnLockonCount;
	wpnMissileCount = obj.wpnMissileCount;
	wpnAoE = obj.wpnAoE;
	wpnMissileSpeed = obj.wpnMissileSpeed;
	wpnLockonSpeed = obj.wpnLockonSpeed;
	wpnLockonRadOnScr = obj.wpnLockonRadOnScr;
	wpnFireLeftTime.reserve(wpnMissileCount);
	wpnNextFireMissileNum = obj.wpnNextFireMissileNum;

	// 미사일 남은 시간 복사
	for (int i = 0; i < wpnFireLeftTime.max_size(); ++i) {
		wpnFireLeftTime[i] = obj.wpnFireLeftTime[i];
	}

	// 미사일 락온을 위한 위젯 복사를 위해 기존 자신의 위젯을 삭제 후 리스트 정리
	list<LockonUI*>::iterator iterList = wpnLockonUIList.begin();
	for (; iterList != wpnLockonUIList.end(); ++iterList) {
		delete (*iterList);
	}
	wpnLockonUIList.clear();
	// 정리 후 새롭게 위젯 추가
	for (int i = 0; i < wpnMissileCount; ++i)
		wpnLockonUIList.push_back(new LockonUI(obj.pWpnOwner, obj.lockonUITexture));
	
	// 사운드 복사
	pWpnFireSound = obj.pWpnFireSound;
}

WpnLockonMissile::~WpnLockonMissile() {
	pWpnModel = NULL;
	// 동적 할당된 무기 락온 UI 삭제
	list<LockonUI*>::iterator iterList = wpnLockonUIList.begin();
	for (; iterList != wpnLockonUIList.end(); ++iterList) {
		delete (*iterList);
	}

	// 사운드 파괴
	if (pWpnFireSound != NULL)
		pWpnFireSound->destory();
	pWpnFireSound = NULL;
}

void WpnLockonMissile::wpnTgtChangeRequest() {
	wpnTgtChangeRequested = true;
}

int WpnLockonMissile::getWpnMissileCount() {
	return wpnMissileCount;
}

list<LockonUI*>* WpnLockonMissile::getWpnLockonUIListPtr() {
	return &wpnLockonUIList;
}

vector<REAL>* WpnLockonMissile::getWpnFireLeftTimePtr() {
	return &wpnFireLeftTime;
}

void WpnLockonMissile::drawWeapon() {
	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // 소유주가 생존한 경우에만 그림
		if (pWpnModel != NULL && wpnQuantity > 0) { // 무기 모델이 존재하고 미사일이 0개 이상 남아 있으면
			int missileCnt = wpnMissileCount;
			REAL misHalfCnt = missileCnt / 2.0;
			REAL maxDist = 3.5, minDist = 2.0;
			REAL misDist = (maxDist - minDist) / misHalfCnt;
			for (int i = 1; i <= missileCnt; ++i) {
				if (wpnFireLeftTime[i - 1] == 0.0) { // 미사일이 장전 완료된 상태라면 그림
					glPushMatrix();															// -참고:
					{																		// <전투기>
						if (i % 2 == 1) // 홀수												// 1 3 4 2 (미사일이 4개라면 이와 같은 순서로 그린다)			
							glTranslatef(-3.0, -0.925, -maxDist + (misDist * (-i + 1)));
						else // 짝수
							glTranslatef(-3.0, -0.925, maxDist - (misDist * (-i + 2)));

						glmDraw(pWpnModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
					}
					glPopMatrix();
				}
			}
		}
	}
}

void WpnLockonMissile::fireWeapon() {
	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // 소유주가 생존한 경우에만 발사 가능
		// 사운드 재생 (루프 사운드 - 기관총)
		if (pWpnFireSound != NULL) {
			pWpnFireSound->play();
		}

		// 한 번에 여러 대상을 공격 가능한 무기라면 발사 준비된(락온 + 미사일 충전완료) 모든 미사일을 한번에 발사
		int fireCnt = 0;
		list<LockonUI*>::iterator iterLockon = wpnLockonUIList.begin();
		LockonUI *pLockonUI = NULL;
		for (; iterLockon != wpnLockonUIList.end(); ++iterLockon) { // 락온 UI리스트를 돌면서
			pLockonUI = (*iterLockon);
			if (wpnFireLeftTime[wpnNextFireMissileNum] == 0.00 && wpnQuantity > 0) { // 락온 되어있고, 발사 가능하며, 탄이 남았을 경우 발사
				// 미사일 발사 추가
				if (pLockonUI->getLockonSuccess() != false || ((*iterLockon == wpnLockonUIList.back()) && (fireCnt == 0))) { // 마지막 UI검사를 할 때까지 발사 기록이 없으면
					++fireCnt;

					if (wpnType == weapon_type::WEAPON_MISSILE) {
						if (pLockonUI->getLockonSuccess() == false) pLockonUI->setTargetFighter(NULL);
						Missile *pMissile = new Missile(pWpnOwner, pLockonUI, this, false);	// 주의!: pLockonUI->getTargetJetPtr() 의 값이 NULL일 수 있음
						g_pGameManager->getGameFramePtr()->addWidget(pMissile);				// WidgetList와 FlightObjList에 추가한다
					}
					else { // (wpnType == weapon_type::WEAPON_GUN or DEFAULT)
						if (pLockonUI->getLockonSuccess() == false) pLockonUI->setTargetFighter(NULL);
						Missile *pMissile = new Missile(pWpnOwner, pLockonUI, this, true); // 미사일(총알)의 모델을 파티클로 사용하도록 해줌
						g_pGameManager->getGameFramePtr()->addWidget(pMissile);
					}

					// 미사일 재장전 시간 초기화
					wpnFireLeftTime[wpnNextFireMissileNum] = 1.00 / wpnFirePerSecond;

					// 탄수 감소
					--wpnQuantity;

					// 사운드 재생 (단일 사운드 - 미사일)
					if (pWpnFireSound == NULL) {
						Sound *pSound = new Sound(this->pWpnOwner->getPos(), wpnFireSoundName, true, false);
						pSound->attach(this->getWpnOwnerPtr());
					}

					// 다음 발사될 미사일 번호 설정
					(wpnNextFireMissileNum + 1 < wpnMissileCount) ? (++wpnNextFireMissileNum) : (wpnNextFireMissileNum = 0);
				}
			}
		}
	}
}

void WpnLockonMissile::stopFireWeapon() {
	// 사운드 루프 일시 정지
	if (pWpnFireSound != NULL) {
		pWpnFireSound->pause();
	}
}

void WpnLockonMissile::update(REAL timeElapsed) {
	// 소리 위치 업데이트
	if (pWpnFireSound != NULL) {
		pWpnFireSound->setPosition(pWpnOwner->getPos());
	}

	// 무기 시간 업데이트
	for (int i = 0; i < wpnMissileCount; ++i) {
		if (wpnMissileCount != 0) (wpnFireLeftTime[i] - timeElapsed > 0.00) ? (wpnFireLeftTime[i] -= timeElapsed) : (wpnFireLeftTime[i] = 0.00);
	}

	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // 소유주가 생존한 경우에만 이후 업데이트
		// 락온 타겟 업데이트
		list<FlightObject*> *pRadarList = pWpnOwner->getRadarListPtr();
		list<FlightObject*>::iterator iterFlightObjList = pRadarList->begin();

		wpnLockonTgtList.clear();
		for (; iterFlightObjList != pRadarList->end(); ++iterFlightObjList) { // 전투기의 레이더 리스트를 순회하면서
			if ((*iterFlightObjList)->getWidgetType() == GameWidget::WIDGET_JET) { // 탐지한 비행체가 전투기이고,
				if (WpnLockonMissile::isJetInLockonRad(static_cast<JetFighter*>(*iterFlightObjList)) == true && // 화면상 자동 조준 범위안 &&
					pWpnOwner->getRadarRadius() >= FlightObject::distanceBetweenObj(pWpnOwner, (*iterFlightObjList))) { // 전투기 레이더 감지 거리에 있는지 확인
					list<JetFighter*>::iterator iterJetList = find(wpnLockonTgtList.begin(), wpnLockonTgtList.end(), (*iterFlightObjList)); // 이미 리스트에 추가되어 있는지 확인하고

					if (iterJetList == wpnLockonTgtList.end()) { // 추가되어 있지 않다면
						wpnLockonTgtList.push_back(static_cast<JetFighter*>(*iterFlightObjList)); // 범위내면 해당 전투기를 락온 가능 타겟 리스트(wpnLockonTgtList)에 추가
					}
				}
				else { // 자동 조준 범위안에 없거나 레이더 감지 거리에 없으면 대상을 리스트에서 제거하고 UI타겟도 NULL로 변경
					wpnLockonTgtList.remove(static_cast<JetFighter*>(*iterFlightObjList)); // 제거
				}
			}
		}
		if (wpnLockonTgtList.size() == 0) { // 락온할 대상이 없을 시 이후 작업 생략
			initLockonUIAll();
			return;
		}

		/* 무기 락온 위젯 업데이트 */
		list<JetFighter*>::iterator iterJetList = wpnLockonTgtList.begin();		// 락온 타겟 iterator
		list<LockonUI*>::iterator iterLockonUI = wpnLockonUIList.begin();	// 락온 UI iterator

		/** 락온 타겟 변경 요청을 받음 **/
		if (wpnTgtChangeRequested == true) {
			wpnTgtChangeRequested = false;

			// 첫 번째 UI가 타겟이 지정되어 있는 상태이고 락온 대상의 수가 UI가 락온하고있는 수보다 많으면
			if (((*iterLockonUI)->getTargetFighterPtr() != NULL) && (getLockonUITgtCount() < wpnLockonTgtList.size())) {
				wpnLockonTgtList.push_front(wpnLockonTgtList.back()); // 맨 뒤의 전투기를 맨 앞에 추가하고
				wpnLockonTgtList.pop_back();						  // 맨 뒤의 전투기를 제거하여 원형 큐처럼 순환하도록 함
				initLockonUIAll();									  // 모든 락온 UI 초기화
			}
		}

		/** 락온 타겟 업데이트 **/
		for (; iterLockonUI != wpnLockonUIList.end(); ++iterLockonUI) { // 락온 UI 리스트를 순회하면서
			JetFighter *pJet = (*iterJetList);
			LockonUI *pLockonUI = (*iterLockonUI);

			// 락온 타겟이 정해지지 않았다면
			if (pLockonUI->getTargetFighterPtr() == NULL) {
				// 다른 UI가 선택하지 않은 타겟을 찾아 정해줌
				for (list<JetFighter*>::iterator iterJet = wpnLockonTgtList.begin(); iterJet != wpnLockonTgtList.end(); ++iterJet) {
					bool isSelectedTgt = false; // 탈출 스위치로 다른 UI가 선택하지 않은 전투기 선택이 성공하면 나머지 루프를 바로 탈출하도록 함
					pLockonUI->setTargetFighter(*iterJet); // 일단 대상을 선택

					// 무기의 락온 가능 범위를 초과하는 위치일 경우 다음 타겟 선택
					if (FlightObject::distanceBetweenObj(pWpnOwner, (*iterJet)) > wpnLockonRange) {
						initLockonUI(pLockonUI);
						break;
					}

					// 선택된 대상이 이미 선점된 전투기인 경우인지 확인
					if (pLockonUI->getTargetFighterPtr() != NULL) {
						for (list<LockonUI*>::iterator iterUI = wpnLockonUIList.begin(); iterUI != wpnLockonUIList.end(); ++iterUI) {
							if (((*iterUI) != pLockonUI) && ((*iterUI)->getTargetFighterPtr() == *iterJet)) { // 자신이 아닌 UI가 이미 선택한 경우
								initLockonUI(pLockonUI); // 선택을 취소토록 하고 위치를 초기화
								isSelectedTgt = true;
								break;
							}
						}
					}

					if (isSelectedTgt == false) {
						break;
					}
				}
			}
			// 락온 타겟이 레이더 락온 범위 내에서 사라졌거나
			else if (isTargtInLockonList(pLockonUI->getTargetFighterPtr()) == false ||
				(FlightObject::distanceBetweenObj(pWpnOwner, pLockonUI->getTargetFighterPtr())) > pWpnOwner->getRadarRadius() || // 전투기의 레이더 락온 범위나
				(FlightObject::distanceBetweenObj(pWpnOwner, pLockonUI->getTargetFighterPtr()) > wpnLockonRange)) { // 무기의 락온 유지 범위에서 벗어난 경우
				initLockonUI(pLockonUI); // 타겟과 위치 초기화
			}
			// 그 외 정상적으로 락온이 진행되고 있는 상태라면 UI위치 업데이트 및 오류 감지
			else {
				JetFighter *pJet = pLockonUI->getTargetFighterPtr();
				Vector3 uiPos = pLockonUI->getPos();
				Vector3 tgtPos = pLockonUI->getTargetFighterPtr()->getScreenPosition();
				REAL dist = (uiPos - tgtPos).magnitude(); // 락온 UI와 전투기 2차원 좌표와의 거리
				REAL rockonDist = pLockonUI->getLockonSuccess() == true ? 30.0 : 8.0; // 락온된 적이 없던 상태이면 더 오래 유지해야 락온이 됨

				if (dist > rockonDist) { // rockonDist픽셀 범위내로 도달해야 락온 성공 
					Vector3 direction = (tgtPos - uiPos); // 락온 UI가 이동해야 할 방향
					direction.normalize();
					REAL speed = wpnLockonSpeed * timeElapsed; // 락온 UI가 움직이는 속도
					pLockonUI->setPos(uiPos + direction * speed);
					pLockonUI->setLockonSuccess(false); // 락온 시도중
				}
				else { // rockonDist픽셀 내로 도달했으면 락온 성공
					pLockonUI->setPos(tgtPos);
					pLockonUI->setLockonSuccess(true);

					// RadarLockon UI도 붉게 그려주기 위함
					updateRadarLockonIsLocked(pLockonUI, true);
				}
			}
		}
	}
	else {
		initLockonUIAll();
	}
}

void WpnLockonMissile::updateRadarLockonIsLocked(LockonUI *pLockonUI, bool isLocked) {
	list<RadarLockon*> *pRadarLockonList = pWpnOwner->pOwningPlayer->getRadarLockonUIListPtr();
	list<RadarLockon*>::iterator iterRadarLockon = pRadarLockonList->begin();
	for (; iterRadarLockon != pRadarLockonList->end(); ++iterRadarLockon) {
		RadarLockon *pRadarLockon = (*iterRadarLockon);
		if (pRadarLockon->getTargetFighterPtr() == pLockonUI->getTargetFighterPtr()) { // 락온 완료된 전투기가 플레이어 UI의 락온 완료 전투기를 의미하면
			pRadarLockon->setLockedByWeapon(isLocked);
			break; // 나머지 루프를 돌지 않고 바로 탈출
		}
	}
}

bool WpnLockonMissile::isJetInLockonRad(JetFighter *pJet) {
	// 전투기가 내 앞에 있는지 확인
	if (FlightObject::isObjPlaceFront(pWpnOwner, pJet) == true) {
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();		// 화면 해상도를 얻어옴
		REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();		// 화면의 폭과 높이
		POINT scrJetPos = { pJet->getScreenPosition().x, pJet->getScreenPosition().y };		// 전투기의 화면상 좌표
		POINT scrCenter = { scrWidth / 2, scrHeight / 2 };									// 화면의 중앙 좌표
		REAL ovalHalfWidth = wpnLockonRadOnScr * scrWidth / 1920.0;							// 해상도에 따른 타원의 폭 절반과 높이 절반
		REAL ovalHalfHeight = wpnLockonRadOnScr * scrHeight / 1080.0;						// (기본 해상도일 경우에는 폭과 높이가 같은 원이 됨)

		// 화면상 무기 락온 범위(타원) 안에 위치하는지 확인
		if (mymath::isPointInOval(scrJetPos, scrCenter, ovalHalfWidth, ovalHalfHeight) == true) { 
			return true;
		}
	}

	return false;
}

bool WpnLockonMissile::isTargtInLockonList(JetFighter *target) {
	bool rtBool = false;
	list<JetFighter*>::iterator iterJet = wpnLockonTgtList.begin();

	// 락온 리스트를 순회하면서
	for (; iterJet != wpnLockonTgtList.end(); ++iterJet) {
		if ((*iterJet) == target) { // 매개변수로 전달받은 전투기가 리스트에 있으면
			rtBool = true; // 반환값을 true로 변경
			break; // 나머지 검사를 건너 뜀
		}
	}

	return rtBool;
}

void WpnLockonMissile::initLockonUI(LockonUI *pObj) {
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	POINT scrCenter = { pResolution->getWidth() / 2, pResolution->getHeight() / 2 };

	pObj->setTargetFighter(NULL); // 타겟을 NULL로
	pObj->setPos(Vector3(scrCenter.x, scrCenter.y, 0.0)); // 위치를 화면 중앙으로 초기화
	pObj->setLockonSuccess(false);
}

void WpnLockonMissile::initLockonUIAll() {
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	POINT scrCenter = { pResolution->getWidth() / 2, pResolution->getHeight() / 2 };

	
	for (list<LockonUI*>::iterator iterUI = wpnLockonUIList.begin(); iterUI != wpnLockonUIList.end(); ++iterUI) {
		initLockonUI(*iterUI);
	}
}

int WpnLockonMissile::getLockonUITgtCount() {
	int rtCounter = 0;
	list<LockonUI*>::iterator iterLockonUI = wpnLockonUIList.begin();
	for (; iterLockonUI != wpnLockonUIList.end(); ++iterLockonUI) {
		if ((*iterLockonUI)->getTargetFighterPtr() != NULL)
			++rtCounter;
	}

	return rtCounter;
}
