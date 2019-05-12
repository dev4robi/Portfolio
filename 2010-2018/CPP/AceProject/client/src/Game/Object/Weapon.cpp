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
	// . . . (���� �Լ�)
}

void Weapon::fireWeapon() {
	// . . . (���� �Լ�)
}

void Weapon::update(REAL timeElapsed) {
	// . . . (���� �Լ�)
}

/*************************************************************************************************************/
/*********************************************** WpnLockonMissile ********************************************/
/*************************************************************************************************************/

WpnLockonMissile::WpnLockonMissile(JetFighter *pOwner, int id) : Weapon(pOwner, id) {
	bool isLoopingSound = false;

	wpnNextFireMissileNum = 0;
	wpnTgtChangeRequested = false;

	// �̻��� ������ ���� �⺻ ���� �ε�
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
		// �� Weapon - - - WpnLockonMissile �� //
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
		// �� Weapon - - - WpnGatlingGun �� //
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
	// �� ���� ó��
	if (wpnLockonCount > wpnMissileCount) wpnLockonCount = wpnMissileCount;	// �ѹ��� ���� ������ Ÿ���� ���� �̻����� ���� �ʰ��� �� ����
	if (wpnLockonCount <= 0) wpnLockonCount = 1; // ������ 1�� �̻� �����ؾ� ��

	// ���� �ʱ�ȭ
	wpnFireLeftTime.reserve(wpnMissileCount); // std::vector �� ũ�⸦ �̻��� ������ �����صд�

	// ���� �������� �Ϸ��Ŵ
	for (int i = 0; i < wpnMissileCount; ++i) {
		wpnFireLeftTime.push_back(0.0);
	}

	// ���� ������ ���� ������ ����ŭ ������Ŵ
	for (int j = 0; j < wpnLockonCount; ++j) {
		wpnLockonUIList.push_back(new LockonUI(pOwner, lockonUITexture));
	}

	// ���� ���� ��� �ʱ�ȭ
	initLockonUIAll();

	// ���� ���� (�����ϴ� ������ ��� ���� ������ �̸� ����)
	if (isLoopingSound == true) {
		pWpnFireSound = new Sound(pWpnOwner->getPos(), wpnFireSoundName, true, true);
		if (pWpnFireSound != NULL) pWpnFireSound->stop();
	}
	else {
		pWpnFireSound = NULL;
	}
}

WpnLockonMissile::WpnLockonMissile(WpnLockonMissile& obj) : Weapon(obj) {
	// �̻��� ���� ����
	pWpnModel = obj.pWpnModel;
	wpnLockonCount = obj.wpnLockonCount;
	wpnMissileCount = obj.wpnMissileCount;
	wpnAoE = obj.wpnAoE;
	wpnMissileSpeed = obj.wpnMissileSpeed;
	wpnLockonSpeed = obj.wpnLockonSpeed;
	wpnLockonRadOnScr = obj.wpnLockonRadOnScr;
	wpnFireLeftTime.reserve(wpnMissileCount);
	wpnNextFireMissileNum = obj.wpnNextFireMissileNum;

	// �̻��� ���� �ð� ����
	for (int i = 0; i < wpnFireLeftTime.max_size(); ++i) {
		wpnFireLeftTime[i] = obj.wpnFireLeftTime[i];
	}

	// �̻��� ������ ���� ���� ���縦 ���� ���� �ڽ��� ������ ���� �� ����Ʈ ����
	list<LockonUI*>::iterator iterList = wpnLockonUIList.begin();
	for (; iterList != wpnLockonUIList.end(); ++iterList) {
		delete (*iterList);
	}
	wpnLockonUIList.clear();
	// ���� �� ���Ӱ� ���� �߰�
	for (int i = 0; i < wpnMissileCount; ++i)
		wpnLockonUIList.push_back(new LockonUI(obj.pWpnOwner, obj.lockonUITexture));
	
	// ���� ����
	pWpnFireSound = obj.pWpnFireSound;
}

WpnLockonMissile::~WpnLockonMissile() {
	pWpnModel = NULL;
	// ���� �Ҵ�� ���� ���� UI ����
	list<LockonUI*>::iterator iterList = wpnLockonUIList.begin();
	for (; iterList != wpnLockonUIList.end(); ++iterList) {
		delete (*iterList);
	}

	// ���� �ı�
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
	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // �����ְ� ������ ��쿡�� �׸�
		if (pWpnModel != NULL && wpnQuantity > 0) { // ���� ���� �����ϰ� �̻����� 0�� �̻� ���� ������
			int missileCnt = wpnMissileCount;
			REAL misHalfCnt = missileCnt / 2.0;
			REAL maxDist = 3.5, minDist = 2.0;
			REAL misDist = (maxDist - minDist) / misHalfCnt;
			for (int i = 1; i <= missileCnt; ++i) {
				if (wpnFireLeftTime[i - 1] == 0.0) { // �̻����� ���� �Ϸ�� ���¶�� �׸�
					glPushMatrix();															// -����:
					{																		// <������>
						if (i % 2 == 1) // Ȧ��												// 1 3 4 2 (�̻����� 4����� �̿� ���� ������ �׸���)			
							glTranslatef(-3.0, -0.925, -maxDist + (misDist * (-i + 1)));
						else // ¦��
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
	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // �����ְ� ������ ��쿡�� �߻� ����
		// ���� ��� (���� ���� - �����)
		if (pWpnFireSound != NULL) {
			pWpnFireSound->play();
		}

		// �� ���� ���� ����� ���� ������ ������ �߻� �غ��(���� + �̻��� �����Ϸ�) ��� �̻����� �ѹ��� �߻�
		int fireCnt = 0;
		list<LockonUI*>::iterator iterLockon = wpnLockonUIList.begin();
		LockonUI *pLockonUI = NULL;
		for (; iterLockon != wpnLockonUIList.end(); ++iterLockon) { // ���� UI����Ʈ�� ���鼭
			pLockonUI = (*iterLockon);
			if (wpnFireLeftTime[wpnNextFireMissileNum] == 0.00 && wpnQuantity > 0) { // ���� �Ǿ��ְ�, �߻� �����ϸ�, ź�� ������ ��� �߻�
				// �̻��� �߻� �߰�
				if (pLockonUI->getLockonSuccess() != false || ((*iterLockon == wpnLockonUIList.back()) && (fireCnt == 0))) { // ������ UI�˻縦 �� ������ �߻� ����� ������
					++fireCnt;

					if (wpnType == weapon_type::WEAPON_MISSILE) {
						if (pLockonUI->getLockonSuccess() == false) pLockonUI->setTargetFighter(NULL);
						Missile *pMissile = new Missile(pWpnOwner, pLockonUI, this, false);	// ����!: pLockonUI->getTargetJetPtr() �� ���� NULL�� �� ����
						g_pGameManager->getGameFramePtr()->addWidget(pMissile);				// WidgetList�� FlightObjList�� �߰��Ѵ�
					}
					else { // (wpnType == weapon_type::WEAPON_GUN or DEFAULT)
						if (pLockonUI->getLockonSuccess() == false) pLockonUI->setTargetFighter(NULL);
						Missile *pMissile = new Missile(pWpnOwner, pLockonUI, this, true); // �̻���(�Ѿ�)�� ���� ��ƼŬ�� ����ϵ��� ����
						g_pGameManager->getGameFramePtr()->addWidget(pMissile);
					}

					// �̻��� ������ �ð� �ʱ�ȭ
					wpnFireLeftTime[wpnNextFireMissileNum] = 1.00 / wpnFirePerSecond;

					// ź�� ����
					--wpnQuantity;

					// ���� ��� (���� ���� - �̻���)
					if (pWpnFireSound == NULL) {
						Sound *pSound = new Sound(this->pWpnOwner->getPos(), wpnFireSoundName, true, false);
						pSound->attach(this->getWpnOwnerPtr());
					}

					// ���� �߻�� �̻��� ��ȣ ����
					(wpnNextFireMissileNum + 1 < wpnMissileCount) ? (++wpnNextFireMissileNum) : (wpnNextFireMissileNum = 0);
				}
			}
		}
	}
}

void WpnLockonMissile::stopFireWeapon() {
	// ���� ���� �Ͻ� ����
	if (pWpnFireSound != NULL) {
		pWpnFireSound->pause();
	}
}

void WpnLockonMissile::update(REAL timeElapsed) {
	// �Ҹ� ��ġ ������Ʈ
	if (pWpnFireSound != NULL) {
		pWpnFireSound->setPosition(pWpnOwner->getPos());
	}

	// ���� �ð� ������Ʈ
	for (int i = 0; i < wpnMissileCount; ++i) {
		if (wpnMissileCount != 0) (wpnFireLeftTime[i] - timeElapsed > 0.00) ? (wpnFireLeftTime[i] -= timeElapsed) : (wpnFireLeftTime[i] = 0.00);
	}

	if (pWpnOwner->getState() == JetFighter::object_state::STATE_ALIVE) { // �����ְ� ������ ��쿡�� ���� ������Ʈ
		// ���� Ÿ�� ������Ʈ
		list<FlightObject*> *pRadarList = pWpnOwner->getRadarListPtr();
		list<FlightObject*>::iterator iterFlightObjList = pRadarList->begin();

		wpnLockonTgtList.clear();
		for (; iterFlightObjList != pRadarList->end(); ++iterFlightObjList) { // �������� ���̴� ����Ʈ�� ��ȸ�ϸ鼭
			if ((*iterFlightObjList)->getWidgetType() == GameWidget::WIDGET_JET) { // Ž���� ����ü�� �������̰�,
				if (WpnLockonMissile::isJetInLockonRad(static_cast<JetFighter*>(*iterFlightObjList)) == true && // ȭ��� �ڵ� ���� ������ &&
					pWpnOwner->getRadarRadius() >= FlightObject::distanceBetweenObj(pWpnOwner, (*iterFlightObjList))) { // ������ ���̴� ���� �Ÿ��� �ִ��� Ȯ��
					list<JetFighter*>::iterator iterJetList = find(wpnLockonTgtList.begin(), wpnLockonTgtList.end(), (*iterFlightObjList)); // �̹� ����Ʈ�� �߰��Ǿ� �ִ��� Ȯ���ϰ�

					if (iterJetList == wpnLockonTgtList.end()) { // �߰��Ǿ� ���� �ʴٸ�
						wpnLockonTgtList.push_back(static_cast<JetFighter*>(*iterFlightObjList)); // �������� �ش� �����⸦ ���� ���� Ÿ�� ����Ʈ(wpnLockonTgtList)�� �߰�
					}
				}
				else { // �ڵ� ���� �����ȿ� ���ų� ���̴� ���� �Ÿ��� ������ ����� ����Ʈ���� �����ϰ� UIŸ�ٵ� NULL�� ����
					wpnLockonTgtList.remove(static_cast<JetFighter*>(*iterFlightObjList)); // ����
				}
			}
		}
		if (wpnLockonTgtList.size() == 0) { // ������ ����� ���� �� ���� �۾� ����
			initLockonUIAll();
			return;
		}

		/* ���� ���� ���� ������Ʈ */
		list<JetFighter*>::iterator iterJetList = wpnLockonTgtList.begin();		// ���� Ÿ�� iterator
		list<LockonUI*>::iterator iterLockonUI = wpnLockonUIList.begin();	// ���� UI iterator

		/** ���� Ÿ�� ���� ��û�� ���� **/
		if (wpnTgtChangeRequested == true) {
			wpnTgtChangeRequested = false;

			// ù ��° UI�� Ÿ���� �����Ǿ� �ִ� �����̰� ���� ����� ���� UI�� �����ϰ��ִ� ������ ������
			if (((*iterLockonUI)->getTargetFighterPtr() != NULL) && (getLockonUITgtCount() < wpnLockonTgtList.size())) {
				wpnLockonTgtList.push_front(wpnLockonTgtList.back()); // �� ���� �����⸦ �� �տ� �߰��ϰ�
				wpnLockonTgtList.pop_back();						  // �� ���� �����⸦ �����Ͽ� ���� ťó�� ��ȯ�ϵ��� ��
				initLockonUIAll();									  // ��� ���� UI �ʱ�ȭ
			}
		}

		/** ���� Ÿ�� ������Ʈ **/
		for (; iterLockonUI != wpnLockonUIList.end(); ++iterLockonUI) { // ���� UI ����Ʈ�� ��ȸ�ϸ鼭
			JetFighter *pJet = (*iterJetList);
			LockonUI *pLockonUI = (*iterLockonUI);

			// ���� Ÿ���� �������� �ʾҴٸ�
			if (pLockonUI->getTargetFighterPtr() == NULL) {
				// �ٸ� UI�� �������� ���� Ÿ���� ã�� ������
				for (list<JetFighter*>::iterator iterJet = wpnLockonTgtList.begin(); iterJet != wpnLockonTgtList.end(); ++iterJet) {
					bool isSelectedTgt = false; // Ż�� ����ġ�� �ٸ� UI�� �������� ���� ������ ������ �����ϸ� ������ ������ �ٷ� Ż���ϵ��� ��
					pLockonUI->setTargetFighter(*iterJet); // �ϴ� ����� ����

					// ������ ���� ���� ������ �ʰ��ϴ� ��ġ�� ��� ���� Ÿ�� ����
					if (FlightObject::distanceBetweenObj(pWpnOwner, (*iterJet)) > wpnLockonRange) {
						initLockonUI(pLockonUI);
						break;
					}

					// ���õ� ����� �̹� ������ �������� ������� Ȯ��
					if (pLockonUI->getTargetFighterPtr() != NULL) {
						for (list<LockonUI*>::iterator iterUI = wpnLockonUIList.begin(); iterUI != wpnLockonUIList.end(); ++iterUI) {
							if (((*iterUI) != pLockonUI) && ((*iterUI)->getTargetFighterPtr() == *iterJet)) { // �ڽ��� �ƴ� UI�� �̹� ������ ���
								initLockonUI(pLockonUI); // ������ ������ �ϰ� ��ġ�� �ʱ�ȭ
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
			// ���� Ÿ���� ���̴� ���� ���� ������ ������ų�
			else if (isTargtInLockonList(pLockonUI->getTargetFighterPtr()) == false ||
				(FlightObject::distanceBetweenObj(pWpnOwner, pLockonUI->getTargetFighterPtr())) > pWpnOwner->getRadarRadius() || // �������� ���̴� ���� ������
				(FlightObject::distanceBetweenObj(pWpnOwner, pLockonUI->getTargetFighterPtr()) > wpnLockonRange)) { // ������ ���� ���� �������� ��� ���
				initLockonUI(pLockonUI); // Ÿ�ٰ� ��ġ �ʱ�ȭ
			}
			// �� �� ���������� ������ ����ǰ� �ִ� ���¶�� UI��ġ ������Ʈ �� ���� ����
			else {
				JetFighter *pJet = pLockonUI->getTargetFighterPtr();
				Vector3 uiPos = pLockonUI->getPos();
				Vector3 tgtPos = pLockonUI->getTargetFighterPtr()->getScreenPosition();
				REAL dist = (uiPos - tgtPos).magnitude(); // ���� UI�� ������ 2���� ��ǥ���� �Ÿ�
				REAL rockonDist = pLockonUI->getLockonSuccess() == true ? 30.0 : 8.0; // ���µ� ���� ���� �����̸� �� ���� �����ؾ� ������ ��

				if (dist > rockonDist) { // rockonDist�ȼ� �������� �����ؾ� ���� ���� 
					Vector3 direction = (tgtPos - uiPos); // ���� UI�� �̵��ؾ� �� ����
					direction.normalize();
					REAL speed = wpnLockonSpeed * timeElapsed; // ���� UI�� �����̴� �ӵ�
					pLockonUI->setPos(uiPos + direction * speed);
					pLockonUI->setLockonSuccess(false); // ���� �õ���
				}
				else { // rockonDist�ȼ� ���� ���������� ���� ����
					pLockonUI->setPos(tgtPos);
					pLockonUI->setLockonSuccess(true);

					// RadarLockon UI�� �Ӱ� �׷��ֱ� ����
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
		if (pRadarLockon->getTargetFighterPtr() == pLockonUI->getTargetFighterPtr()) { // ���� �Ϸ�� �����Ⱑ �÷��̾� UI�� ���� �Ϸ� �����⸦ �ǹ��ϸ�
			pRadarLockon->setLockedByWeapon(isLocked);
			break; // ������ ������ ���� �ʰ� �ٷ� Ż��
		}
	}
}

bool WpnLockonMissile::isJetInLockonRad(JetFighter *pJet) {
	// �����Ⱑ �� �տ� �ִ��� Ȯ��
	if (FlightObject::isObjPlaceFront(pWpnOwner, pJet) == true) {
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();		// ȭ�� �ػ󵵸� ����
		REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();		// ȭ���� ���� ����
		POINT scrJetPos = { pJet->getScreenPosition().x, pJet->getScreenPosition().y };		// �������� ȭ��� ��ǥ
		POINT scrCenter = { scrWidth / 2, scrHeight / 2 };									// ȭ���� �߾� ��ǥ
		REAL ovalHalfWidth = wpnLockonRadOnScr * scrWidth / 1920.0;							// �ػ󵵿� ���� Ÿ���� �� ���ݰ� ���� ����
		REAL ovalHalfHeight = wpnLockonRadOnScr * scrHeight / 1080.0;						// (�⺻ �ػ��� ��쿡�� ���� ���̰� ���� ���� ��)

		// ȭ��� ���� ���� ����(Ÿ��) �ȿ� ��ġ�ϴ��� Ȯ��
		if (mymath::isPointInOval(scrJetPos, scrCenter, ovalHalfWidth, ovalHalfHeight) == true) { 
			return true;
		}
	}

	return false;
}

bool WpnLockonMissile::isTargtInLockonList(JetFighter *target) {
	bool rtBool = false;
	list<JetFighter*>::iterator iterJet = wpnLockonTgtList.begin();

	// ���� ����Ʈ�� ��ȸ�ϸ鼭
	for (; iterJet != wpnLockonTgtList.end(); ++iterJet) {
		if ((*iterJet) == target) { // �Ű������� ���޹��� �����Ⱑ ����Ʈ�� ������
			rtBool = true; // ��ȯ���� true�� ����
			break; // ������ �˻縦 �ǳ� ��
		}
	}

	return rtBool;
}

void WpnLockonMissile::initLockonUI(LockonUI *pObj) {
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	POINT scrCenter = { pResolution->getWidth() / 2, pResolution->getHeight() / 2 };

	pObj->setTargetFighter(NULL); // Ÿ���� NULL��
	pObj->setPos(Vector3(scrCenter.x, scrCenter.y, 0.0)); // ��ġ�� ȭ�� �߾����� �ʱ�ȭ
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
