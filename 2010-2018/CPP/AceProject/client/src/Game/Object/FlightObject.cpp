#include "Game/Object/FlightObject.h"
#include "Game/Frame/CombatFrame.h"
#include "Engine/SoundManager.h"
#include "Game/Effect/GL_Effect.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Player.h"
#include "Game/UI/UI.h"

FlightObject::FlightObject(Vector3 pos) : GameWidget(pos) {
	// �÷��̾�, �� �ʱ�ȭ
	pOwningPlayer = NULL;
	pModel = NULL;
	
	// ���� �� ������ �ʱ�ȭ
	for (int i = 0; i < 3; ++i) {
		rotateOffset[i] = 0.0;
		pivotOffset[i] = 0.0;
		scale[i] = 1.0;
		color[i] = 1.0;
	}
	color[3] = 1.0;

	// ��ġ �� �� �� ����� �ʱ�ȭ
	pitch = yaw = roll = 0.0;
	HP = maxHP = 100.0;

	// ���� �ʱ�ȭ
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

	// �浹ü ���� ����
	collisionAry.reserve(8); // �浹ü�� �����ϱ� ���� 8���� ������ ����

	// CombatFrame�� ����ü ����Ʈ�� �߰�
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
	// �ӵ� ����
	currentEngPower += (engPowerInc * timeElapsed);
}

void FlightObject::deacceleration(REAL timeElapsed) {
	// �ӵ� ����
	isDeaccelerating = true;
	if (currentSpeed >= maxSpeed * 0.75) {
		currentSpeed -= (currentSpeed * 0.25 * timeElapsed);
	}
	else {
		currentSpeed -= (currentSpeed * 0.20 * timeElapsed);
	}
}

void FlightObject::boosting(REAL timeElapsed) {
	// �ν��Ͱ� ���� �ְ� �ν��� �������� �ƴ� ��
	if ((currentBoost / 50.0) > 1.0 && boostLeftTime <= 0.00) {
		currentBoost -= 50.0;					// �ν��� ������ ����
		boostLeftTime = default_boostOfferTime;	// ����Ʈ 6�ʰ� �ν��� Ȱ��ȭ

		// ���� ���
		Sound *pEffectSnd = new Sound(position, "res/Sounds/booster.wav", true, false);
		pEffectSnd->setGain(100.0); // ���� ���� (ī�޶�� ��ü ������ �Ÿ��� �ֱ� ����)
		pEffectSnd->attach(this); // ����ü�� ����

		// ������ �÷��̾��� ȭ�鿡 ����Ʈ �߰�
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(default_boostOfferTime, 2, ScreenEffect::ScrEffectAction::SCREFFECT_BOOST, 0);
	}
}

void FlightObject::pitching(REAL angle, bool spdDecomp) {
	// �ӵ� ���� ��� �� (��ʽ�: �ְ� �ӵ����� 30% ���� ����)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// ���� ���� ����
	directVector = ( (directVector * cosf(Math::degreesToRadians(angle))) + (upVector * sinf(Math::degreesToRadians(angle))) );
	directVector.normalize();

	// ������ ����
	upVector = Vector3::cross(rightVector, directVector);

	// Ʈ������ ���ʹϾ� ����
	Quaternion axis;
	axis.fromAxisAngle(rightVector, angle);
	axis.normalize();
	transform.normalize();
	transform = transform * axis;
}

void FlightObject::yawing(REAL angle, bool spdDecomp) {
	// �ӵ� ���� ��� �� (��ʽ�: �ְ� �ӵ����� 30% ���� ����)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// ���� ���� ����
	rightVector = ( (rightVector * cosf(Math::degreesToRadians(angle))) + (directVector * sinf(Math::degreesToRadians(angle))) );
	rightVector.normalize();

	// ���� ���� ����
	directVector = Vector3::cross(upVector, rightVector);

	// Ʈ������ ���ʹϾ� ����
	Quaternion axis;
	axis.fromAxisAngle(upVector, angle);
	axis.normalize();
	transform.normalize();
	transform = transform * axis;

}

void FlightObject::rolling(REAL angle, bool spdDecomp) {
	// �ӵ� ���� ��� �� (��ʽ�: �ְ� �ӵ����� 30% ���� ����)
	if (spdDecomp == true) {
		angle -= ((angle * 0.30) * (this->currentSpeed / this->maxSpeed));
	}

	// �������� ��쿡�� �Ѹ� ���� ����
	if (widgetType == GameWidget::WIDGET_JET) {
		static_cast<JetFighter*>(this)->setJetIsRolling(true);
	}

	// ���� ���� ����
	rightVector = ( (rightVector * cosf(Math::degreesToRadians(angle))) + (upVector * sinf(Math::degreesToRadians(angle))) );
	rightVector.normalize();

	// ������ ����
	upVector = Vector3::cross(rightVector, directVector);

	// Ʈ������ ���ʹϾ� ����
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
	Player *pDealtPlayer = this->pOwningPlayer; // ���� ���� �÷��̾�
	REAL leftHP = (pFlightObj->HP - amount);	// ���ҵ� ����� �� ����
	bool isMultiplayMode = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isMultiplayMode(); // ��Ƽ�÷��̸�� ���� ����

	// < ��Ƽ�÷����� ��� >
	// ���� ��Ȳ�� �ڱ� Ŭ���̾�Ʈ�� ������ ��Ȳ�� �������� ����ȴ�.
	// ��, A��ǻ�Ϳ��� B��ǻ���� �����⿡ ���ظ� �����ٸ�, B��ǻ�ʹ� A��ǻ�Ϳ��� ���� ��Ŷ�� ���� ���ط��� ����ȴ�.

	// ��Ƽ�÷��� ����� ���
	if (isMultiplayMode == true) {
		// ��Ʈ��ũ�� ���� ��Ŷ ������ ���� ��� (�ڱⰡ ���� ���ظ� ��Ʈ��ũ ��Ŷ �� ������Ʈ�� ���)
		if (msgToNetwork == true) {
			// ��Ƽ�÷��̽� ����ȭ �޽����� ��Ŷ�� �߰�
			if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getOwnerPlayerPtr() == this->pOwningPlayer) {
				dealtDamage_Net(pFlightObj->pOwningPlayer->getPlayerNumber(), amount, suicide);

				// �ڽ� Ŭ���̾�Ʈ�� ������Ʈ���� ���ظ� ����
				if (leftHP > 0.00) { // �� ����� ���� ������� 0���� ũ�� ����� ���� ����
					if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ ����

#ifdef __DEBUG__
					cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ظ� �������ϴ�! (���� HP: " << leftHP << ")" << endl;
#endif
					pFlightObj->HP = leftHP;
					pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
				}
				else { // �� ����� ������� 0�����̸� ������� 0���� �����ϰ� ����ų�� ���ط� ��ȯ
					if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ�� ����
#ifdef __DEBUG__
					cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ط� ó���߽��ϴ�!" << endl;
#endif
					pFlightObj->HP = 0.0;
					pFlightObj->killFighter(suicide == false ? this : NULL); // ������ �������� ���ε� Ȯ��
				}
			}
		}
		// ��Ʈ��ũ�� ���� ��Ŷ�� �ƴ� ��� ���� ���� (Ŭ���̾�ƮA -> Ŭ���̾�ƮB �� ���Ź��� �޽����� ��찡 ��κ�)
		else {
			// �ڽ� Ŭ���̾�Ʈ�� ������Ʈ���� ���ظ� ����
			if (leftHP > 0.00) { // �� ����� ���� ������� 0���� ũ�� ����� ���� ����
				if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ ����
#ifdef __DEBUG__
				cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ظ� �������ϴ�! (���� HP: " << leftHP << ")" << endl;
#endif
				pFlightObj->HP = leftHP;
				pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
			}
			else { // �� ����� ������� 0�����̸� ������� 0���� �����ϰ� ����ų�� ���ط� ��ȯ
				if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ�� ����
#ifdef __DEBUG__
				cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ط� ó���߽��ϴ�!" << endl;
#endif
				pFlightObj->HP = 0.0;
				pFlightObj->killFighter(suicide == false ? this : NULL); // ������ �������� ���ε� Ȯ��
			}
		}
	}
	// ��Ƽ�÷��� ��尡 �ƴ� ���
	else {
		if (leftHP > 0.00) { // �� ����� ���� ������� 0���� ũ�� ����� ���� ����
			if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + amount); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ ����
#ifdef __DEBUG__
			cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ظ� �������ϴ�! (���� HP: " << leftHP << ")" << endl;
#endif
			pFlightObj->HP = leftHP;
			pFlightObj->pOwningPlayer->getUImanagerPtr()->setDmgBlinkTime(500);
		}
		else { // �� ����� ������� 0�����̸� ������� 0���� �����ϰ� ����ų�� ���ط� ��ȯ
			if (this != pFlightObj && suicide == false) pDealtPlayer->setPlayerScore(pDealtPlayer->getPlayerScore() + (amount + leftHP)); // ���� ���� ��� �÷��̾��� ������ ���� ���ط���ŭ�� ����
#ifdef __DEBUG__
			cout << "�÷��̾� " << this->pOwningPlayer->getPlayerName() << "�� " << pFlightObj->pOwningPlayer->getPlayerName() << "���� " << amount << "�� ���ط� ó���߽��ϴ�!" << endl;
#endif
			pFlightObj->HP = 0.0;
			pFlightObj->killFighter(suicide == false ? this : NULL); // ������ �������� ���ε� Ȯ��
		}
	}

	return leftHP;
}

void FlightObject::dealtDamage_Net(short targetPlayerNum, REAL amount, bool suicide) { // (��Ʈ��ũ ����ȭ �޽���)
	vector<int> *pControlVec = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getPlayerControlVectorPtr();
	int targetPlayerNumber = (int)targetPlayerNum;
	int dealtAmount = 0;
	int isSuicide = 0;
	memcpy_s(&dealtAmount, sizeof(dealtAmount), &amount, sizeof(dealtAmount)); // ���ط� ����Ʈ ����(4byte)�� ī���ؼ� ����
	memset(&isSuicide, '\0', sizeof(isSuicide));
	memcpy_s(&isSuicide, sizeof(suicide), &suicide, sizeof(suicide)); // �ڻ� ���� ����Ʈ ����(1byte)�� ī���ؼ� ����

	pControlVec->push_back(5);					// ID: 5 �� ������ ���� �κ�
	pControlVec->push_back(targetPlayerNumber);	// �Ű����� 1 : ���� ���� �÷��̾� ��ȣ
	pControlVec->push_back(dealtAmount);		// �Ű����� 2 : ���ط�
	pControlVec->push_back(isSuicide);			// �Ű����� 3 : �ڻ� ����
#ifdef __DEBUG__
	cout << "��Ʈ��ũ ��Ŷ�� ���� ������ �߰��մϴ�. Ÿ��num: " << targetPlayerNumber << "���ط�: " << amount << "�ڻ� ����: " << isSuicide << endl;
#endif
}

void FlightObject::killFighter(FlightObject *pKiller) {
	new ExplosionEffect(this->position, 5.0, 0); // ����Ʈ ����
	Sound *pSnd = new Sound(this->position, "res/Sounds/jetExplosion.wav", true, false); // ���� ���� ���
	pSnd->setGain(100.0);
	this->setState(object_state::STATE_DEAD); // ��� ���� ����

	// ���� �ʱ�ȭ
	if (this->getWidgetType() == GameWidget::WIDGET_JET) {
		JetFighter *pJet = static_cast<JetFighter*>(this);
		pJet->setAltitudeWarningState(false);									// �� ��� ���� ����
		pJet->setStallWarningState(false);										// ���� ��� ���� ����
		pJet->setLockonWarningState(JetFighter::jet_warning::JET_LOCKON_NONE);	// ���� ��� ���� ����
		pJet->setRespawnLeftTime(pJet->getRespawnReqTime());					// ��Ȱ ���� �ð� ����

		// ó���� �÷��̾��� �ٸ� ����Ʈ�� �����, �۷ο� ����Ʈ�� �߰�
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->clearEffects();
		this->pOwningPlayer->getUImanagerPtr()->getScreenEffectPtr()->createEffect(6.0, 1, ScreenEffect::ScrEffectAction::SCREFFECT_WARNGLOW, 0);
	}

	Player *pDeathPlayer = this->pOwningPlayer;
	if (pKiller == NULL) { // �ڻ��� ���
		// ���� ���� ���� �̺�Ʈ ȿ�� �ο�
		pDeathPlayer->getUImanagerPtr()->getRespawnUIPtr()->setKillerPlayer(NULL);
	}
	else { // Ÿ���� ���
		Player *pKillPlayer = pKiller->pOwningPlayer;

		// ó���� ���� ���
		if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) { // ������ ������� ���� ��쿡�� ���� ���
			pKillPlayer->setPlayerKills(pKillPlayer->getPlayerKills() + 1); // ó�� ����
			pKillPlayer->setPlayerScore(pKillPlayer->getPlayerScore() + 100); // �Ϲ� ����
		}

		// óġ�� �÷��̾ UI�� �˸�
		this->pOwningPlayer->getUImanagerPtr()->getRespawnUIPtr()->setKillerPlayer(pKiller->pOwningPlayer);
	}
}

void FlightObject::updateCollisionPos(Vector3 _position, Quaternion *_pTransform) {
	// ���͸� ���鼭 ��� �浹ü ��ġ���� ����
	for (int i = 0; i < collisionAry.size(); ++i) {
		collisionAry[i]->update(_position, _pTransform);
	}
}

bool FlightObject::checkObjectCollision() {
	// ���� �Լ�
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
	// ���� �Լ�
	return true;
}

bool FlightObject::destroy() {
	// ���� �Լ�
	return true;
}

bool FlightObject::update(REAL timeElapsed) {
	// ���� �Լ�
	return true;
}

Vector3 FlightObject::getUpdatedPosition(REAL timeElapsed, int cnt) {
	Vector3 newPos = position;
	
	newPos += (directVector * mymath::kph2kps((currentSpeed) * cnt) * timeElapsed); // test;

	// ���⿡ ��ġ ���濡 ���� �ٸ� ���ĵ� �߰� . . . ( ���� ������ �߰��ȴٸ� cnt �� �̾Ƽ� �ݺ������� ¥�� �� ��... )
	// for (int i = 0; i < cnt; ++i) { ... }

	return newPos;
}

void FlightObject::updatePosition(REAL timeElapsed) {
	Vector3 oldPosition = position; // test

	position = getUpdatedPosition(timeElapsed);
}

void FlightObject::updatePitchRollYaw(REAL timeElapsed) {
	// ��ġ
	pitch = mymath::rad2deg(-asinf(-directVector.y));

	// ��
	REAL tyaw = mymath::rad2deg(-atan2f(directVector.x, directVector.z));
	yaw = tyaw = mymath::degree360(abs(tyaw - 270.0));

	// ��
	roll = mymath::rad2deg(acosf(Vector3::dot(Vector3(0.0, 1.0, 0.0), upVector)));
}

void FlightObject::updateSpeed(REAL timeElapsed) {
	// ���׿� ���� �ڵ� ����
	currentSpeed -= (((currentSpeed * 0.04) - 10.0) * timeElapsed); // ���� �ӵ��� 4% �� �߰��� 10�� ����

	// �ӵ� ��� �� ����
	currentSpeed += ((engOfferSpeed * timeElapsed) * (currentEngPower / 100.0));
	if (currentSpeed > maxSpeed) (currentSpeed = maxSpeed);		// �ִ� �ӵ�
	else if (currentSpeed < 455.0) (currentSpeed = 455.0);		// ���� �ӵ�

	// ���� �Ŀ� ������ ��� �� ����
	if (currentEngPower > 0.0) currentEngPower -= (engPowerDec * timeElapsed);	// �ڿ������� �پ��� ���� �Ŀ�
	if (currentEngPower < 0.0) currentEngPower = 0.0;							// ���� ���� �Ŀ�   0.0
	else if (currentEngPower > 100.0) currentEngPower = 100.0;					// �ְ� ���� �Ŀ� 100.0
}
