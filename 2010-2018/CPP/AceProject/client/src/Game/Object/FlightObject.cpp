#include "Game/Object/FlightObject.h"
#include "Game/Frame/CombatFrame.h"
#include "Engine/SoundManager.h"
#include "Game/Effect/GL_Effect.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Player.h"
#include "Game/UI/UI.h"

FlightObject::FlightObject(Vector3 pos) : GameWidget(pos) {
	// 플레이어, 모델 초기화
	pOwningPlayer = NULL;
	pModel = NULL;
	
	// 색상 및 오프셋 초기화
	for (int i = 0; i < 3; ++i) {
		rotateOffset[i] = 0.0;
		pivotOffset[i] = 0.0;
		scale[i] = 1.0;
		color[i] = 1.0;
	}
	color[3] = 1.0;

	// 피치 롤 요 및 생명력 초기화
	pitch = yaw = roll = 0.0;
	HP = maxHP = 100.0;

	// 벡터 초기화
	directVector = Vector3(1.0, 0.0, 0.0);
	upVector = Vector3(0.0, 1.0, 0.0);
	rightVector = Vector3(0.0, 0.0, 1.0);
	transform.fromAxisAngle(directVector, 0.0);
	pitch = yaw = roll = 0.0;
	default_pitchTurn = default_yawTurn = default_rollTurn = 0.0;
	boostLeftTime = 0.0;
	currentSpeed = maxSpeed = currentEngPower = engPowerInc = engPowerDec = engOfferSpeed = 0.0;
	currentBoost = 100.0;
	objectState = object_state::STATE_ALIVE;

	// 충돌체 공간 생성
	collisionAry.reserve(8); // 충돌체를 저장하기 위해 8개의 공간을 선점

	// CombatFrame의 비행체 리스트에 추가
	static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getFlightObjListPtr()->push_back(this);
}

FlightObject::FlightObject(FlightObject& _obj) : GameWidget() {
	position = _obj.position;
	pOwningPlayer = _obj.pOwningPlayer;
	pModel = _obj.pModel;

	for (int i = 0; i < 3; ++i) {
		rotateOffset[i] = _obj.rotateOffset[i];
		pivotOffset[i] = _obj.pivotOffset[i];
		scale[i] = _obj.scale[i];
		color[i] = _obj.color[i];
	}
	color[4] = _obj.color[4];

	pitch = _obj.pitch;	yaw = _obj.yaw;	roll = _obj.roll;
	HP = _obj.HP;	maxHP = _obj.maxHP;

	isAccelerating = false;
	isDeaccelerating = false;

	directVector = _obj.directVector;
	upVector = _obj.upVector;
	rightVector = _obj.rightVector;
	transform = _obj.transform;
	pitch = _obj.pitch;	yaw = _obj.yaw;	roll = _obj.roll;
	default_pitchTurn = _obj.default_pitchTurn;	default_yawTurn = _obj.default_yawTurn;	default_rollTurn = _obj.default_rollTurn;
	boostLeftTime = 0.0;
	currentSpeed = _obj.currentSpeed;	maxSpeed = _obj.maxSpeed;
	currentEngPower = _obj.currentEngPower;		currentBoost = _obj.currentBoost;
	engOfferSpeed = _obj.engOfferSpeed;			engPowerInc = _obj.engPowerInc;	
	engPowerDec = _obj.engPowerDec;				objectState = _obj.objectState;
}

FlightObject::~FlightObject() {
	pOwningPlayer = NULL;
	pModel = NULL;
}

void FlightObject::acceleration(REAL timeElapsed) {
	// 속도 가속
	currentEngPower += (engPowerInc * timeElapsed);
}

void FlightObject::deacceleration(REAL timeElapsed) {
	// 속도 감속
	isDeaccelerating = true;
	if (currentSpeed >= maxSpeed * 0.75) {
		currentSpeed -= (currentSpeed * 0.25 * timeElapsed);
	}
	else {
		currentSpeed -= (currentSpeed * 0.20 * timeElapsed);
	}
}

void FlightObject::boosting(REAL timeElapsed) {
	// 부스터가 남아 있고 부스터 시전중이 아닐 시
	if ((currentBoost / 50.0) > 1.0 && boostLeftTime <= 0.00) {
		currentBoost -= 50.0;					// 부스터 게이지 감소
		boostLeftTime = default_boostOfferTime;	// 디폴트 6초간 부스터 활성화

		// 사운드 재생
		Sound *pEffectSnd = new Sound(position, "res/Sounds/booster.wav", true, false);
		pEffectSnd->setGain(100.0); // 사운드 증폭 (카메라와 객체 사이의 거리가 있기 때문)
		pEffectSnd->attach(this); // 비행체에 부착

		// 소유주 플레이어의 화면에 이펙트 추가
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(default_boostOfferTime, 2, ScreenEffect::ScrEffectAction::SCREFFECT_BOOST, 0);
	}
}

void FlightObject::pitching(REAL angle, bool spdDecomp) {
	// 속도 보정 사용 시 (비례식: 최고 속도에서 30% 감소 보정)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// 방향 벡터 변경
	directVector = ( (directVector * cosf(Math::degreesToRadians(angle))) + (upVector * sinf(Math::degreesToRadians(angle))) );
	directVector.normalize();

	// 업벡터 변경
	upVector = Vector3::cross(rightVector, directVector);

	// 트랜스폼 쿼터니언 변경
	Quaternion axis;
	axis.fromAxisAngle(rightVector, angle);
	axis.normalize();
	transform.normalize();
	transform = transform * axis;
}

void FlightObject::yawing(REAL angle, bool spdDecomp) {
	// 속도 보정 사용 시 (비례식: 최고 속도에서 30% 감소 보정)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// 우향 벡터 변경
	rightVector = ( (rightVector * cosf(Math::degreesToRadians(angle))) + (directVector * sinf(Math::degreesToRadians(angle))) );
	rightVector.normalize();

	// 방향 벡터 변경
	directVector = Vector3::cross(upVector, rightVector);

	// 트랜스폼 쿼터니언 변경
	Quaternion axis;
	axis.fromAxisAngle(upVector, angle);
	axis.normalize();
	transform.normalize();
	transform = transform * axis;

}

void FlightObject::rolling(REAL angle, bool spdDecomp) {
	// 속도 보정 사용 시 (비례식: 최고 속도에서 30% 감소 보정)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// 전투기인 경우에는 롤링 상태 설정
	if (widgetType == GameWidget::WIDGET_JET) {
		static_cast<JetFighter*>(this)->setJetIsRolling(true);
	}

	// 우향 벡터 변경
	rightVector = ( (rightVector * cosf(Math::degreesToRadians(angle))) + (upVector * sinf(Math::degreesToRadians(angle))) );
	rightVector.normalize();

	// 업벡터 변경
	upVector = Vector3::cross(rightVector, directVector);

	// 트랜스폼 쿼터니언 변경
	Quaternion axis;
	axis.fromAxisAngle(directVector, -angle);
	axis.normalize();
	transform.normalize();
	transform = transform * axis;
}

FlightObject::object_state FlightObject::getState() {
	return objectState;
}

void FlightObject::setState(object_state state) {
	objectState = state;
}

REAL FlightObject::dealtDamage(FlightObject *pFlightObj, REAL amount, bool suicide, bool msgToNetwork) {
	Player *pDealtPlayer = this->pOwningPlayer; // 피해 입힌 플레이어
	REAL leftHP = (pFlightObj->HP - amount);	// 감소된 생명력 선 연산
	bool isMultiplayMode = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isMultiplayMode(); // 멀티플레이모드 여부 저장

	// < 멀티플레이의 경우 >
	// 피해 상황은 자기 클라이언트의 프레임 상황을 기점으로 수행된다.
	// 즉, A컴퓨터에서 B컴퓨터의 전투기에 피해를 입혔다면, B컴퓨터는 A컴퓨터에서 보낸 패킷에 의해 피해량이 적용된다.

	// 멀티플레이 모드인 경우
	if (isMultiplayMode == true) {
		// 네트워크로 보낼 패킷 생성을 위한 경우 (자기가 입힌 피해만 네트워크 패킷 및 업데이트에 사용)
		if (msgToNetwork == true) {
			// 멀티플레이시 동기화 메시지를 패킷에 추가
			if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getOwnerPlayerPtr() == this->pOwningPlayer) {
				dealtDamage_Net(pFlightObj->pOwningPlayer->getPlayerNumber(), amount, suicide);

				// 자신 클라이언트의 오브젝트에게 피해를 입힘
				if (leftHP > 0.00) { // 선 연산된 남은 생명력이 0보다 크면 생명력 감소 적용
					if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼 증가

#ifdef __DEBUG__
					cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해를 입혔습니다! (남은 HP: " << leftHP << ")" << endl;
#endif
					pFlightObj->HP = leftHP;
					pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
				}
				else { // 선 연산된 생명력이 0이하이면 생명력은 0으로 고정하고 오버킬된 피해량 반환
					if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼만 증가
#ifdef __DEBUG__
					cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해로 처형했습니다!" << endl;
#endif
					pFlightObj->HP = 0.0;
					pFlightObj->killFighter(suicide == false ? this : NULL); // 스스로 피해입힘 여부도 확인
				}
			}
		}
		// 네트워크로 보낼 패킷이 아닌 경우 피해 수행 (클라이언트A -> 클라이언트B 로 수신받은 메시지일 경우가 대부분)
		else {
			// 자신 클라이언트의 오브젝트에게 피해를 입힘
			if (leftHP > 0.00) { // 선 연산된 남은 생명력이 0보다 크면 생명력 감소 적용
				if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼 증가
#ifdef __DEBUG__
				cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해를 입혔습니다! (남은 HP: " << leftHP << ")" << endl;
#endif
				pFlightObj->HP = leftHP;
				pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
			}
			else { // 선 연산된 생명력이 0이하이면 생명력은 0으로 고정하고 오버킬된 피해량 반환
				if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼만 증가
#ifdef __DEBUG__
				cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해로 처형했습니다!" << endl;
#endif
				pFlightObj->HP = 0.0;
				pFlightObj->killFighter(suicide == false ? this : NULL); // 스스로 피해입힘 여부도 확인
			}
		}
	}
	// 멀티플레이 모드가 아닌 경우
	else {
		if (leftHP > 0.00) { // 선 연산된 남은 생명력이 0보다 크면 생명력 감소 적용
			if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼 증가
#ifdef __DEBUG__
			cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해를 입혔습니다! (남은 HP: " << leftHP << ")" << endl;
#endif
			pFlightObj->HP = leftHP;
			pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
		}
		else { // 선 연산된 생명력이 0이하이면 생명력은 0으로 고정하고 오버킬된 피해량 반환
			if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // 피해 입힌 대상 플레이어의 점수가 입힌 피해량만큼만 증가
#ifdef __DEBUG__
			cout << "플레이어 " << this->pOwningPlayer->getPlayerName() << "가 " << pFlightObj->pOwningPlayer->getPlayerName() << "에게 " << amount << "의 피해로 처형했습니다!" << endl;
#endif
			pFlightObj->HP = 0.0;
			pFlightObj->killFighter(suicide == false ? this : NULL); // 스스로 피해입힘 여부도 확인
		}
	}

	return leftHP;
}

void FlightObject::dealtDamage_Net(short targetPlayerNum, REAL amount, bool suicide) { // (네트워크 동기화 메시지)
	vector<int> *pControlVec = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getPlayerControlVectorPtr();
	int targetPlayerNumber = (int)targetPlayerNum;
	int dealtAmount = 0;
	int isSuicide = 0;
	memcpy_s(&dealtAmount, sizeof(dealtAmount), &amount, sizeof(dealtAmount)); // 피해량 바이트 정보(4byte)를 카피해서 넣음
	memset(&isSuicide, '\0', sizeof(isSuicide));
	memcpy_s(&isSuicide, sizeof(suicide), &suicide, sizeof(suicide)); // 자살 여부 바이트 정보(1byte)를 카피해서 넣음

	pControlVec->push_back(5);					// ID: 5 가 데미지 관련 부분
	pControlVec->push_back(targetPlayerNumber);	// 매개변수 1 : 피해 입은 플레이어 번호
	pControlVec->push_back(dealtAmount);		// 매개변수 2 : 피해량
	pControlVec->push_back(isSuicide);			// 매개변수 3 : 자살 여부
#ifdef __DEBUG__
	cout << "네트워크 패킷에 다음 정보를 추가합니다. 타겟num: " << targetPlayerNumber << "피해량: " << amount << "자살 여부: " << isSuicide << endl;
#endif
}

void FlightObject::killFighter(FlightObject *pKiller) {
	new ExplosionEffect(this->position, 5.0, 0); // 이펙트 생성
	Sound *pSnd = new Sound(this->position, "res/Sounds/jetExplosion.wav", true, false); // 폭발 사운드 재생
	pSnd->setGain(100.0);
	this->setState(object_state::STATE_DEAD); // 사망 판정 설정

	// 설정 초기화
	if (this->getWidgetType() == GameWidget::WIDGET_JET) {
		JetFighter *pJet = static_cast<JetFighter*>(this);
		pJet->setAltitudeWarningState(false);									// 고도 경고 상태 설정
		pJet->setStallWarningState(false);										// 스톨 경고 상태 설정
		pJet->setLockonWarningState(JetFighter::jet_warning::JET_LOCKON_NONE);	// 락온 경고 상태 설정
		pJet->setRespawnLeftTime(pJet->getRespawnReqTime());					// 부활 남은 시간 설정

		// 처형된 플레이어의 다른 이펙트를 지우고, 글로우 이펙트를 추가
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->clearEffects();
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(6.0, 1, ScreenEffect::ScrEffectAction::SCREFFECT_WARNGLOW, 0);
	}

	Player *pDeathPlayer = this->pOwningPlayer;
	if (pKiller == NULL) { // 자살의 경우
		// 점수 감소 등의 이벤트 효과 부여
		pDeathPlayer->getUImanagerPtr()->getRespawnUIPtr()->setKillerPlayer(NULL);
	}
	else { // 타살의 경우
		Player *pKillPlayer = pKiller->pOwningPlayer;

		// 처형자 점수 상승
		if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) { // 세션이 만료되지 않은 경우에만 점수 상승
			pKillPlayer->setPlayerKills(pKillPlayer->getPlayerKills() + 1); // 처형 점수
			pKillPlayer->setPlayerScore(pKillPlayer->getPlayerScore() + 100); // 일반 점수
		}

		// 처치한 플레이어를 UI에 알림
		this->pOwningPlayer->getUImanagerPtr()->getRespawnUIPtr()->setKillerPlayer(pKiller->pOwningPlayer);
	}
}

void FlightObject::updateCollisionPos(Vector3 _position, Quaternion *_pTransform) {
	// 벡터를 돌면서 모든 충돌체 위치들을 갱신
	for (int i = 0; i < collisionAry.size(); ++i) {
		collisionAry[i]->update(_position, _pTransform);
	}
}

bool FlightObject::checkObjectCollision() {
	// 가상 함수
	return false;
}

REAL FlightObject::distanceBetweenObj(FlightObject *obj1, FlightObject *obj2) {
	return (obj1->position - obj2->position).magnitude();
}

bool FlightObject::isObjPlaceFront(FlightObject *obj1, FlightObject *obj2) {
	if (Vector3::dot(obj1->directVector, (obj1->getPos() - obj2->getPos())) < 0)
		return true;
	else
		return false;
}

bool FlightObject::draw() {
	// 가상 함수
	return true;
}

bool FlightObject::destroy() {
	// 가상 함수
	return true;
}

bool FlightObject::update(REAL timeElapsed) {
	// 가상 함수
	return true;
}

Vector3 FlightObject::getUpdatedPosition(REAL timeElapsed, int cnt) {
	Vector3 newPos = position;
	
	newPos += (directVector * mymath::kph2kps((currentSpeed) * cnt) * timeElapsed); // test;

	// 여기에 위치 변경에 대한 다른 공식들 추가 . . . ( 만약 공식이 추가된다면 cnt 를 뽑아서 반복문으로 짜야 할 것... )
	// for (int i = 0; i < cnt; ++i) { ... }

	return newPos;
}

void FlightObject::updatePosition(REAL timeElapsed) {
	Vector3 oldPosition = position; // test

	position = getUpdatedPosition(timeElapsed);
}

void FlightObject::updatePitchRollYaw(REAL timeElapsed) {
	// 피치
	pitch = mymath::rad2deg(-asinf(-directVector.y));

	// 요
	REAL tyaw = mymath::rad2deg(-atan2f(directVector.x, directVector.z));
	yaw = tyaw = mymath::degree360(abs(tyaw - 270.0));

	// 롤
	roll = mymath::rad2deg(acosf(Vector3::dot(Vector3(0.0, 1.0, 0.0), upVector)));
}

void FlightObject::updateSpeed(REAL timeElapsed) {
	// 저항에 의한 자동 감속
	currentSpeed -= (((currentSpeed * 0.04) - 10.0) * timeElapsed); // 현재 속도의 4% 에 추가로 10씩 감소

	// 속도 계산 및 보정
	currentSpeed += ((engOfferSpeed * timeElapsed) * (currentEngPower / 100.0));
	if (currentSpeed > maxSpeed) (currentSpeed = maxSpeed);		// 최대 속도
	else if (currentSpeed < 455.0) (currentSpeed = 455.0);		// 최저 속도

	// 엔진 파워 증가분 계산 및 보정
	if (currentEngPower > 0.0) currentEngPower -= (engPowerDec * timeElapsed);	// 자연스럽게 줄어드는 엔진 파워
	if (currentEngPower < 0.0) currentEngPower = 0.0;							// 최저 엔진 파워   0.0
	else if (currentEngPower > 100.0) currentEngPower = 100.0;					// 최고 엔진 파워 100.0
}
