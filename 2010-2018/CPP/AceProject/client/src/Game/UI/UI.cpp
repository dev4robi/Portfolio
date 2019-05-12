#include "Game/UI/UI.h"
#include "Game/Object/JetFighter.h"
#include "Game/Object/Player.h"
#include "Game/Object/Terrain.h"
#include "Game/Object/Weapon.h"
#include "Game/Object/Missile.h"
#include "Game/Frame/CombatFrame.h"
#include "Engine/DrawManager.h"
#include "Engine/GameCamera.h"
#include "Engine/GameFrame.h"
#include "Engine/ResourceManager.h"
#include "Engine/SoundManager.h"
#include "Library/GLTextureFont.h"

/*************************************************************************************************************/
/************************************************** UImaster *************************************************/
/*************************************************************************************************************/

UImaster::UImaster(Player *_pOwnPlayer) : GameObject() {
	// ���� ���� �� �����ϴ� �ð� �ʱ�ȭ
	dmgBlinkLeftTime = 0.0;

	// ���� �÷��̾�� �ش� �÷��̾��� UI���� ����Ʈ�� ������ ����
	pOwnPlayer = _pOwnPlayer;
	pUIDrawList = pOwnPlayer->getPlayerUIDrawListPtr();

	// �÷��̾� ���� �ý��� UI
	pStateSys = new StateSys(this);

	// �÷��̾� �ӵ���� ���� UI
	pSpeedAltMeter = new SpeedAltMeter(this);

	// �÷��̾� �� UI
	pRadarMap = new RadarMap(this);

	// ������ ���� �溸 �� �˸� UI
	pWarningSys = new WarningSys(this);

	// ������ UI
	pRespawnUI = new RespawnUI(this);

	// ���ھ�� UI
	pScoreBoard = new ScoreBoard(this);

	// �̼� �޽��� UI
	pMissionMsg = new MissionMsg(this);

	// ��ũ�� ����Ʈ UI
	pScreenEffect = new ScreenEffect(this);
}

UImaster::UImaster(UImaster &obj) : GameObject(obj) {
	pOwnPlayer = obj.pOwnPlayer;
	pUIDrawList = obj.pUIDrawList;
	pStateSys = obj.pStateSys;
	pSpeedAltMeter = obj.pSpeedAltMeter;
	pRadarMap = obj.pRadarMap;
	pWarningSys = obj.pWarningSys;
	pScoreBoard = obj.pScoreBoard;
	pMissionMsg = obj.pMissionMsg;
}

UImaster::~UImaster() {
	if (pSpeedAltMeter != NULL)
		delete pSpeedAltMeter;
	pSpeedAltMeter = NULL;

	if (pStateSys != NULL)
		delete pStateSys;
	pStateSys = NULL;

	if (pSpeedAltMeter != NULL)
		delete pSpeedAltMeter;
	pSpeedAltMeter = NULL;

	if (pRadarMap != NULL)
		delete pRadarMap;
	pRadarMap = NULL;

	if (pWarningSys != NULL)
		delete pWarningSys;
	pWarningSys = NULL;

	if (pScoreBoard != NULL)
		delete pScoreBoard;
	pScoreBoard = NULL;

	if (pMissionMsg != NULL)
		delete pMissionMsg;
	pMissionMsg = NULL;
}

REAL UImaster::getScreenWidth() {
	return scrWidth;
}

REAL UImaster::getScreenHeight() {
	return scrHeight;
}

Player* UImaster::getOwnPlayerPtr() {
	return pOwnPlayer;
}

REAL UImaster::getDmgBlinkTime() {
	return dmgBlinkLeftTime;
}

void UImaster::setDmgBlinkTime(REAL time) {
	dmgBlinkLeftTime = time;
}

StateSys* UImaster::getStateSysPtr() {
	return pStateSys;
}

SpeedAltMeter* UImaster::getSpeedAltMeterPtr() {
	return pSpeedAltMeter;
}

list<RadarLockon*>* UImaster::getRadarLockonUIListPtr() {
	return &radarLockonUIList;
}

RadarMap* UImaster::getRadarMapPtr() {
	return pRadarMap;
}

WarningSys* UImaster::getWarningSysPtr() {
	return pWarningSys;
}

RespawnUI* UImaster::getRespawnUIPtr() {
	return pRespawnUI;
}

MissionMsg* UImaster::getMissionMsgPtr() {
	return pMissionMsg;
}

ScreenEffect* UImaster::getScreenEffectPtr() {
	return pScreenEffect;
}

void UImaster::updateUI(REAL timeElapsed) {
	// ���� ����
	JetFighter *pJet = pOwnPlayer->getPlayerJet();
	if (pUIDrawList == NULL || pJet == NULL) return;

	// �ػ� ����
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	scrWidth = pResolution->getWidth();
	scrHeight = pResolution->getHeight();

	// �������� ���� ���� �ð� ������Ʈ
	if ((dmgBlinkLeftTime -= (timeElapsed * 1000.0)) < 0.0) dmgBlinkLeftTime = 0.0;

	/*** UI�� ������Ʈ ***/
	/** RadarLockon Widget **/
	// ���� Ÿ���� radarLockonUI List�� �߰��� ���� ������ �߰���
	list<FlightObject*> *pJetRadar = pJet->getRadarListPtr();		// �÷��̾� ��Ʈ���� ���̴� ���µ� ����ü ����Ʈ�� ������
	list<FlightObject*>::iterator iterRadar = pJetRadar->begin();	// �÷��̾� ��Ʈ���� ���̴� ���� ����Ʈ�� ������ ������
	for (; iterRadar != pJetRadar->end(); ++iterRadar) {
		if ((*iterRadar)->getWidgetType() == GameWidget::WIDGET_JET) { // �߿�!: ���̴� ���� UI�� �������� ��쿡�� ǥ������� �Ѵ�! (���� ���� ����ϸ� �̻����� ���� �߻�)
			// �� ���� �߰��� ���� ��� ����Ʈ�� ��� �ִ� ��쿡�� �ٷ� �߰�
			if (radarLockonUIList.size() == 0) {
				radarLockonUIList.push_back(new RadarLockon(pJet, static_cast<JetFighter*>(*iterRadar)));
			}
			// ����Ʈ�� �̹� �߰��� ���� �ִٸ� �̹� �߰��Ǿ� �ִ� ������� Ȯ��
			else {
				bool isExist = false;
				list<RadarLockon*>::iterator iterLockon = radarLockonUIList.begin();
				for (; iterLockon != radarLockonUIList.end(); ++iterLockon) {
					// ���̴� Ÿ���� ����Ʈ�� �̹� ��� �ִ� ��� ��ġ�� ������ �ְ� Ż��
					if ((*iterRadar) == (*iterLockon)->getTargetFighterPtr()) {
						isExist = true;
						break;
					}
				}
				// �߰��� ���� ���� ��쿡�� �߰�
				if (isExist == false) {
					radarLockonUIList.push_back(new RadarLockon(pJet, static_cast<JetFighter*>(*iterRadar)));
				}
			}
		}
	}
	// ���� Ÿ���� radarList�� �������� ������ ������
	for (list<RadarLockon*>::iterator uiIter = radarLockonUIList.begin(); uiIter != radarLockonUIList.end();) {
		bool isInRadar = false;
		for (iterRadar = pJetRadar->begin(); iterRadar != pJetRadar->end(); ++iterRadar) {
			// �ش� ui�� ���̴� ����Ʈ �ȿ� �ִ��� Ȯ��
			if ((*uiIter)->getTargetFighterPtr() == (*iterRadar)) {
				// �ִٸ� Ż��
				isInRadar = true;
				break;
			}
		}
		if (isInRadar == false) { // ���̴� ����Ʈ �ȿ� ���ٸ� ����
			delete (*uiIter);
			uiIter = radarLockonUIList.erase(uiIter);
		}
		else {
			++uiIter;
		}
	}
	// ���̴� ���� ����Ʈ ������Ʈ
	for (list<RadarLockon*>::iterator iter = radarLockonUIList.begin(); iter != radarLockonUIList.end(); ++iter) {
		(*iter)->update(timeElapsed);
	}

	/** StateSys **/
	if (pStateSys != NULL) pStateSys->update(timeElapsed);

	/** SpeedAltMeter **/
	if (pSpeedAltMeter != NULL) pSpeedAltMeter->update(timeElapsed);

	/** RadarMap **/
	if (pRadarMap != NULL) pRadarMap->update(timeElapsed);

	/** WarningSys **/
	if (pWarningSys != NULL) pWarningSys->update(timeElapsed);

	/** ScoreBoard **/
	if (pScoreBoard != NULL) pScoreBoard->update(timeElapsed);

	/** MissionMsg **/
	if (pMissionMsg != NULL) pMissionMsg->update(timeElapsed);

	/** ScreenEffect **/
	if (pScreenEffect != NULL) pScreenEffect->update(timeElapsed);

	//////////////////////////			//////////////////////////			//////////////////////////			//////////////////////////

	REAL sessionLeftTime = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime(); // ���� ���� �ð��� �޾ƿ�

	// ���� �� ���� ��
	if (sessionLeftTime > 0.00) {
		if (pOwnPlayer->getPlayerJet()->getState() == FlightObject::object_state::STATE_ALIVE) { // �����Ⱑ ������ ���� ���� �׸��� UI
			/*** UI�� ��ο� ����Ʈ�� �߰� ***/
			/** ���̴� ���� ����Ʈ **/
			for (list<RadarLockon*>::iterator iter = radarLockonUIList.begin(); iter != radarLockonUIList.end(); ++iter) {
				pUIDrawList->push_back(*iter);
			}

			/** ���� �ý��� **/
			pUIDrawList->push_back(pStateSys);

			/** �ӵ��� ���� **/
			pUIDrawList->push_back(pSpeedAltMeter);

			/** ���̴� �� **/
			pUIDrawList->push_back(pRadarMap);

			/** ��� �ý��� **/
			pUIDrawList->push_back(pWarningSys);

			/** ���� ���� UI **/
			// �ֹ���(�����)
			WpnLockonMissile *pWpn = pJet->getFirstWeaponPtr();
			list<LockonUI*> *pLockonUIList = NULL;
			list<LockonUI*>::iterator iterLockonUI;
			if (pWpn != NULL) {
				pLockonUIList = pWpn->getWpnLockonUIListPtr();
				if (pLockonUIList != NULL) {
					iterLockonUI = pLockonUIList->begin();

					for (; iterLockonUI != pLockonUIList->end(); ++iterLockonUI) {
						if ((*iterLockonUI)->getTargetFighterPtr() != NULL)
							pUIDrawList->push_back(*iterLockonUI);
					}
				}
			}
			// �ֹ���(�̻���)
			pWpn = pJet->getBaseWeaponPtr();
			if (pWpn != NULL) {
				pLockonUIList = pWpn->getWpnLockonUIListPtr();
				if (pLockonUIList != NULL) {
					iterLockonUI = pLockonUIList->begin();

					for (; iterLockonUI != pLockonUIList->end(); ++iterLockonUI) {
						if ((*iterLockonUI)->getTargetFighterPtr() != NULL)
							pUIDrawList->push_back(*iterLockonUI);
					}
				}
			}
		}
		else { // �����Ⱑ ������ ���� ���� �� �׸��⿡ �߰��ϴ� UI
			pUIDrawList->push_back(pRespawnUI);
		}

		// ���� �� ���� �� �׸��⿡ �߰��ϴ� UI
		/** ���ھ� ���� **/
		pUIDrawList->push_back(pScoreBoard);
	}

	// �׻� �׸��⿡ �߰��ϴ� UI (�̼� �޽���, ��ũ�� ����Ʈ)
	pUIDrawList->push_back(pMissionMsg);
	pUIDrawList->push_back(pScreenEffect);
}

/*************************************************************************************************************/
/************************************************ RadarLockon ************************************************/
/*************************************************************************************************************/

RadarLockon::RadarLockon() {
	is2D = true;
	uiScale = 0.75;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/radarLockon.png");
	arrowTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/radarArrow.png");
	widgetType = GameWidget::WIDGET_UI_RADARLOCKON;
	lockedByWeapon = false;
	type = enemy;
}

RadarLockon::RadarLockon(JetFighter *caster, JetFighter *target) {
	is2D = true;
	uiScale = 0.75;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/radarLockon.png");
	arrowTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/radarArrow.png");
	widgetType = GameWidget::WIDGET_UI_RADARLOCKON;
	lockedByWeapon = false;
	type = enemy;
	targetFighter = target;
	ownerFighter = caster;
	position = Vector3(target->getPosX(), target->getPosY(), target->getPosZ());
}

RadarLockon::RadarLockon(RadarLockon& obj) : GameWidget(obj) {
	is2D = true;
	uiScale = obj.uiScale;
	textureID = obj.textureID;
	widgetType = GameWidget::WIDGET_UI_RADARLOCKON;
	lockedByWeapon = obj.lockedByWeapon;
	type = obj.type;
}

RadarLockon::~RadarLockon() { }

void RadarLockon::setLockedByWeapon(bool isLocked) {
	lockedByWeapon = isLocked;
}

JetFighter* RadarLockon::getTargetFighterPtr() {
	return targetFighter;
}

bool RadarLockon::draw() {
	if (isVisible == true) {
		glMatrixMode(GL_MODELVIEW);

		GameCamera *pCam = g_pGameManager->getDrawManagerPtr()->getCameraFromFrame();
		// ī�޶� ���� ������ �׸��� ���� (ī�޶��� ���� ����� ������Ʈ�� ī�޶��� ��ġ�� �� ���⺤�͸� �����Ͽ� > 0 �� ���)
		if (Vector3::dot(pCam->getViewDirection(), (pCam->getPosition() - this->getPos())) > 0) {
			return true;
		}
		// ī�޶� �տ� ������ �׸��� �۾� ���� ( t < 0 )
		else {
			// ��ũ�� ��ǥ�� ��ȯ
			Size2D *resolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
			Vector3 screenPos = Vector3(getPosX(), getPosY(), getPosZ());
			screenPos = mymath::Vecor3Transform(screenPos, pCam->getViewMatrix());
			screenPos = mymath::Vecor3Transform(screenPos, pCam->getProjectionMatrix());
			screenPos.x = screenPos.x / (screenPos.z == 0 ? 0.0001 : screenPos.z);
			screenPos.y = screenPos.y / (screenPos.z == 0 ? 0.0001 : screenPos.z);
			screenPos.x = (screenPos.x + 1) * resolution->getWidth() / 2;
			screenPos.y = (screenPos.y + 1) * resolution->getHeight() / 2;

			// ��ũ�� �������� ���ϰ� ����� �ش� ��鿡 �ִٴ� ���� ȭ��ǥ�� ǥ���ϱ� ���� �غ�
			bool bOutRightside = false, bOutLeftside = false;
			bool bOutTopside = false, bOutDownside = false;
			bool bOutbound = false;
			if (screenPos.x > resolution->getWidth() + 50.0) { // ��
				screenPos.x = resolution->getWidth() - 50.0;
				bOutRightside = true;
			}
			if (screenPos.x < -50.0) { // ��
				screenPos.x = 50.0;
				bOutLeftside = true;
			}
			if (screenPos.y > resolution->getHeight() + 50.0) { // ��
				screenPos.y = resolution->getHeight() - 50.0;
				bOutTopside = true;
			}
			if (screenPos.y < -50.0) { // ��
				screenPos.y = 50.0;
				bOutDownside = true;
			}
			bOutbound = (bOutRightside || bOutLeftside || bOutTopside || bOutDownside);

			// ������ ��ũ�� ��ǥ�� �̵�
			glTranslatef(screenPos.x, screenPos.y, 0.0);
			
			// ��� ���⿡ ���� ȭ��ǥ�� �����̼�
			if (bOutbound == true) {
				if (bOutRightside == true && bOutTopside == true) // ���� ����
					glRotatef(45.0, 0.0, 0.0, -1.0);
				else if (bOutRightside == true && bOutDownside == true) // ���� ����
					glRotatef(135.0, 0.0, 0.0, -1.0);
				else if (bOutRightside == true) // ����
					glRotatef(90.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true && bOutTopside == true) // ���� ����
					glRotatef(315.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true && bOutDownside == true) // ���� ����
					glRotatef(225.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true)
					glRotatef(270.0, 0.0, 0.0, -1.0);
				else if (bOutDownside == true)
					glRotatef(180.0, 0.0, 0.0, -1.0);
			}

			// �� ���� ����
			Vector3 uiColor;
			if (ownerFighter->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // �̻��Ϸ� ���µ� ���¸� ����
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				if (lockedByWeapon == true) { // ����� ���µ� ���¶��
					uiColor = ui::GetSavedUIColor(ui::red);
				}
				else { // ����� ���µ��� ���� ���¶��
					if (type == RadarLockonType::ally) uiColor = ui::GetSavedUIColor(ui::blue);
					else if (type == RadarLockonType::enemy) uiColor = ui::GetSavedUIColor(ui::cyan);
				}
			}
			glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65); // ������ ��� û�ϻ����� �׸�
			glScalef(uiScale, uiScale, uiScale);

			// �� �׸���(�簢��)
			static const float size = 50.0; // ũ��
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, bOutbound == false ? textureID : arrowTextureID); // ����!: glBindTexture() �� �׻� glBegin() ������ ȣ��Ǿ�� �Ѵ�!
			glBegin(GL_QUADS);
			{
				float width = resolution->getWidth() * size / 1920;
				float height = resolution->getHeight() * size / 1080;
				glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			// ȭ��ǥ�� ǥ���� �� ��� �ɼ� ����
			if (bOutbound == true) {
				// ������ ��µǴ� ���� ������ ���� ��� ����
				if (bOutRightside == true && bOutTopside == true) return true;
				else if (bOutRightside == true && bOutDownside == true) return true;
				else if (bOutLeftside == true && bOutTopside == true) return true;
				else if (bOutLeftside == true && bOutDownside == true) return true;
				// �Ʒ��� ����� ���� ȸ���Ͽ� �ùٸ� �޽����� ��µǵ��� ��
				else if (bOutDownside == true) glRotatef(180.0, 0.0, 0.0, 1.0);
			}

			// �Ÿ��� ���ڷ� ǥ��(�Ҽ��� ����)
			int dist = (ownerFighter->getPos() - targetFighter->getPos()).magnitude(); // �� ����ü ������ �Ÿ�
			if (dist < 0 || dist > 100000000) dist = 0;
			char distChar[10];	// �ִ� 9999999999(99��) ���� ���� ����
			itoa(dist, distChar, 10); // ������ 10���� �ƽ�Ű�� ġȯ
			GLTextureFont distFont(distChar, 8.5);
			glTranslatef(60.0, -20.0, 0.0);
			distFont.drawText();

			// �����⸦ ���ڷ� ǥ��
			string *pName = targetFighter->getCraftNamePtr();
			if (pName != NULL) {
				GLTextureFont nameFont(*pName, 7.0);
				glTranslatef(0.0, 40.0, 0.0);
				nameFont.drawText();
			}

			// ���̵� ���ڷ� ǥ��
			pName = targetFighter->pOwningPlayer->getPlayerNamePtr();
			if (pName != NULL) {
				GLTextureFont nameFont(*pName, 7.0);
				glTranslatef(0.0, 27.5, 0.0);
				nameFont.drawText();
			}
		}
	}
	else {
		return false;
	}

	return true;
}

bool RadarLockon::destroy() {
	return true;
}

bool RadarLockon::update(REAL timeElapsed) {
	// �̻��� ���� ���¸� �ƴ����� ����
	lockedByWeapon = false;

	// Ÿ�� ����ü�� ��ġ�� �����ͼ� �ڽ��� ��ġ�� ����ȭ
	setPos(targetFighter->getPos());

	return true;
}

/***************************************************************************************************************/
/************************************************ SpeedAltMeter ************************************************/
/***************************************************************************************************************/

SpeedAltMeter::SpeedAltMeter(UImaster *pMaster) {
	pUIMaster = pMaster;
	is2D = true;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/uiRect.png");
	breakTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/breakAlarm.png");
	boostingTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/boosting.png");
	pTargetJet = pMaster->getOwnPlayerPtr()->getPlayerJet();
	speedMeter = 0.0;
	altMeter = 0.0;
}

SpeedAltMeter::SpeedAltMeter(SpeedAltMeter& obj) : GameWidget() {
	pUIMaster = obj.pUIMaster;
	is2D = true;
	textureID = obj.textureID;
	pTargetJet = obj.pTargetJet;
	speedMeter = obj.speedMeter;
	altMeter = obj.altMeter;
}

SpeedAltMeter::~SpeedAltMeter() { }

void SpeedAltMeter::setSpeed(REAL speed) {
	speedMeter = speed;
}

void SpeedAltMeter::setAlt(REAL alt) {
	altMeter = alt;
}

void SpeedAltMeter::setTargetJet(JetFighter *jet) {
	pTargetJet = jet;
}

JetFighter* SpeedAltMeter::getTargetJetPtr() {
	return pTargetJet;
}

bool SpeedAltMeter::draw() {
	if (pTargetJet != NULL) {
		// �ػ󵵸� ������
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// ���� ���� (���� ������ ���� ���¶�� �������, �̻��� ���� �� �� ���� ��������)
		Vector3 uiColor;
		REAL dmgBlinkTime = pUIMaster->getDmgBlinkTime();
		if (dmgBlinkTime <= 0.00) { // �ֱٿ� ���� ���ذ� ������ ���¿� ���� �⺻ ���� ����
			if (pTargetJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				uiColor = ui::GetSavedUIColor(ui::cyan);
			}
		}
		else { // �ֱٿ� ���� ���ذ� ������ ���� �ð��� ���� ���� ����
			if ((int)dmgBlinkTime % 100 == 0) uiColor = ui::GetSavedUIColor(ui::cyan);
			else uiColor = ui::GetSavedUIColor(ui::red);
		}
		glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65);

		// �ӵ���
		glPushMatrix();
		{
			// �׸�1
			glPushMatrix();
			{
				static const float rectWidth = 80.0;
				static const float rectHeight = 28.5;
				float width = scrWidth * rectWidth / 1920;
				float height = scrHeight * rectHeight / 1080;
				glTranslatef(scrWidth * 0.265, scrHeight * 0.530, 0.0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// SPEED ����
			glPushMatrix();
			{
				GLTextureFont speedMeterText("SPEED", 10.0);
				glTranslatef(scrWidth * 0.238, scrHeight * 0.530, 0.0);
				speedMeterText.drawText();
			}
			glPopMatrix();

			// SPEED ����
			glPushMatrix();
			{
				GLTextureFont speedText("", 10.0);
				char speedStr[6]; // ��� ������ �ִ� �ӵ��� 999999
				itoa(speedMeter, speedStr, 10);
				speedText.setText(speedStr);
				glTranslatef(scrWidth * 0.24, scrHeight * 0.470, 0.0);
				speedText.drawText();
			}
			glPopMatrix();

			// BREAK �˶��� BOOST �˶�
			float rectWidth = 20.0;
			float rectHeight = 10.0;
			float width = scrWidth * rectWidth / 1920;
			float height = scrHeight * rectHeight / 1080;

			if (pTargetJet->isDeaccelerating == true) {
				glPushMatrix(); // BREAK
				{
					glTranslatef(scrWidth * 0.295, scrHeight * 0.57, 0.0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, breakTextureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();
			}

			if (pTargetJet->boostLeftTime > 0.00) {
				glPushMatrix(); // BOOST
				{
					rectWidth = 40.0;
					rectHeight = 10.0;
					width = scrWidth * rectWidth / 1920;
					height = scrHeight * rectHeight / 1080;

					glTranslatef(scrWidth * 0.245, scrHeight * 0.57, 0.0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, boostingTextureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();
			}
		}
		glPopMatrix();

		// ����
		glPushMatrix();
		{
			// �׸�2
			glPushMatrix();
			{
				static const float rectWidth = 65.0;
				static const float rectHeight = 28.5;
				float width = scrWidth * rectWidth / 1920;
				float height = scrHeight * rectHeight / 1080;
	
				glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65);
				glTranslatef(scrWidth * 0.725, scrHeight * 0.530, 0.0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// ALT ����
			glPushMatrix();
			{
				GLTextureFont speedMeterText("ALT", 10.0);
				glTranslatef(scrWidth * 0.705, scrHeight * 0.530, 0.0);
				speedMeterText.drawText();
			}
			glPopMatrix();

			// ALT ����
			GLTextureFont altText("", 10.0);
			char altStr[64];
			itoa(altMeter, altStr, 10);
			altText.setText(altStr);
			glTranslatef(scrWidth * 0.705, scrHeight * 0.470, 0.0);
			altText.drawText();
		}
		glPopMatrix();
	}
	
	return true;
}

bool SpeedAltMeter::destroy() {
	return true;
}

bool SpeedAltMeter::update(REAL timeElapsed) {
	// Ÿ�� ������ ����
	pTargetJet = pUIMaster->getOwnPlayerPtr()->getPlayerJet();

	// �ӵ��� �� ������ ������κ��� ����
	if (pTargetJet != NULL) {
		setSpeed(pTargetJet->currentSpeed);	// �ӵ�
		setAlt(pTargetJet->getPosY());		// ��
	}

	return true;
}

/***************************************************************************************************************/
/*************************************************** RadarMap **************************************************/
/***************************************************************************************************************/

RadarMap::RadarMap(UImaster *pMaster) : GameWidget() {
	pUIMaster = pMaster;
	is2D = true;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/uiRect.png");
	jetTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/mapJet.png");
	missileTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/mapMissile.png");
	mapGridTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/mapGrid.png");
	radarRadTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/radarMapRad.png");
	pTerrain = g_pGameManager->getGameFramePtr()->getTerrainPtr();
	pPlayer = pMaster->getOwnPlayerPtr();
	pFlightObjList = NULL;
	currentScanDist = 0.0;
	scanSpeed = 6500.0;
};
RadarMap::RadarMap(RadarMap& obj) : GameWidget() {
	pUIMaster = obj.pUIMaster;
	is2D = true;
	mapGridTextureID = obj.mapGridTextureID;
	jetTextureID = obj.jetTextureID;
	missileTextureID = obj.missileTextureID;
	pTerrain = obj.pTerrain;
	pPlayer = obj.pPlayer;
	pFlightObjList = obj.pFlightObjList;
	currentScanDist = obj.currentScanDist;
	scanSpeed = obj.scanSpeed;
};

RadarMap::~RadarMap() { };

void RadarMap::setTerrain(Terrain *terrain) {
	pTerrain = terrain;
}

void RadarMap::setPlayer(Player *player) {
	pPlayer = player;
}

void RadarMap::setScanSpeed(REAL speed) {
	scanSpeed = speed;
}

bool RadarMap::draw() {
	if (pPlayer != NULL && pTerrain != NULL) {
		// �ػ󵵸� ������
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// �÷��̾��� �����⸦ �޾� ��
		JetFighter *pOwnerJet = pPlayer->getPlayerJet();
		
		// ���̴� �� �׸���
		glPushMatrix();
		{
			// ���� ����1
			Vector3 uiColor;
			REAL baseAlpha = 0.75;
			REAL dmgBlinkTime = pUIMaster->getDmgBlinkTime();
			if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				if (dmgBlinkTime == 0.00) {
					uiColor = ui::GetSavedUIColor(ui::cyan);
				}
				else {
					if ((int)dmgBlinkTime % 100 == 0) uiColor = ui::GetSavedUIColor(ui::cyan);
					else uiColor = ui::GetSavedUIColor(ui::red);
				}
			}

			// XZ ��ǥ ���
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha - 0.1);
			glPushMatrix();
			{
				char jetCoord[16];
				GLTextureFont jetCoordText(string(""), 7.0);
				glPushMatrix();	// X��
				{
					sprintf(jetCoord, "X: %d", (int)pOwnerJet->getPosX());
					jetCoordText.setText(jetCoord);
					glTranslatef(scrWidth * 0.79, scrHeight * 0.40, 0.0); 
					jetCoordText.drawText();
				}
				glPopMatrix();
				glPushMatrix();	// Y(Z)��
				{
					sprintf(jetCoord, "Y: %d", (int)-pOwnerJet->getPosZ());
					jetCoordText.setText(jetCoord);
					glTranslatef(scrWidth * 0.89, scrHeight * 0.40, 0.0);
					jetCoordText.drawText();
				}
				glPopMatrix();
			}
			glPopMatrix();

			// ���
			glColor4f(uiColor.x - 0.5, uiColor.y - 0.5, uiColor.z - 0.5, baseAlpha - 0.5);
			static const float mapWidth = 200.0;
			static const float mapHeight = 200.0;
			float width = scrWidth * mapWidth / 1920;
			float height = scrHeight * mapHeight / 1080;
			glTranslatef(scrWidth * 0.89, scrHeight * 0.195, 0.0); // ���̴� �� UI�� ȭ��� ��ġ
			glBegin(GL_QUADS);
			{
				 glVertex3f(-width, height, 0.0);
				 glVertex3f(-width, -height, 0.0);
				 glVertex3f(width, -height, 0.0);
				 glVertex3f(width, height, 0.0);
			}
			glEnd();

			// Ʋ
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			static const float frameWidth = 202.0;
			static const float frameHeight = 202.0;
			width = scrWidth * frameWidth / 1920;
			height = scrHeight * frameHeight / 1080;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			// ���ٽ� ���� ����
			width = (scrWidth * (mapWidth - 5)) / 1920;
			height = (scrHeight * (mapHeight - 5)) / 1080;
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glEnable(GL_STENCIL_TEST);
			{
				glStencilFunc(GL_ALWAYS, 1, 1);
				glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
				glBegin(GL_QUADS);
				{
					glVertex3f(-width, height, 0.0);
					glVertex3f(-width, -height, 0.0);
					glVertex3f(width, -height, 0.0);
					glVertex3f(width, height, 0.0);
				}
				glEnd();
			}
			glDisable(GL_STENCIL_TEST);

			// ���ٽ� ���� ���
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glEnable(GL_STENCIL_TEST);
			{
				glStencilFunc(GL_EQUAL, 1, 0xffffffff);
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				
				// ����
				glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha - 0.65);
				SIZE terrainSize = pTerrain->getScaledSize();
				REAL gridSize = 5000.0;
				REAL tWidth = terrainSize.cx;		REAL cx = tWidth / gridSize;// / 2.0;
				REAL tHeight = terrainSize.cy;		REAL cy = tHeight / gridSize;// / 2.0;
				width = (scrWidth * gridSize / mapWidth * 2.0) / 1920.0;
				height = (scrHeight * gridSize / mapWidth * 2.0) / 1080.0;
				
				for (int y = -cy; y < cy; ++y) {
					for (int x = -cx; x < cx; ++x) {
						glPushMatrix();
						{
							glTranslatef(width * x, height * y, 0.0);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, mapGridTextureID);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
								glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
							}
							glEnd();
							glDisable(GL_TEXTURE_2D);
						}
						glPopMatrix();
					}
				}

				// ������ �� �̻���
				if (pFlightObjList != NULL) {
					drawObjIcon(pFlightObjList, mapWidth, mapHeight);
				}

				// ���̴� ����
				REAL radarMaxRad = pOwnerJet->getRadarRadius();
				glPushMatrix();
				{
					glColor4f(uiColor.x, uiColor.y, uiColor.z, (0.2 / (currentScanDist / radarMaxRad)));

					glTranslatef(((pOwnerJet->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0,
								 -((pOwnerJet->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // ������ ��ġ�� ��ǥ �̵�

					// �׸���
					REAL radarScanMaxWidth = (scrWidth * radarMaxRad / mapWidth * 2.0) / 1920.0;
					REAL radarScanMaxHeight = (scrHeight * radarMaxRad / mapHeight * 2.0) / 1080.0;
					REAL radarWidth = (radarScanMaxWidth * currentScanDist) / radarMaxRad;
					REAL radarHeight = (radarScanMaxHeight * currentScanDist) / radarMaxRad;
					width = (scrWidth * radarWidth) / 1920.0;
					height = (scrHeight * radarHeight) / 1080.0;
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, radarRadTextureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();
			}
			// ���ٽ� ���� ��� ����
			glDisable(GL_STENCIL_TEST);

			// ���ٽ� ���� �ʱ�ȭ
			glClear(GL_STENCIL_BUFFER_BIT);
		}
		glPopMatrix();

		return true;
	}
	else {
		return false;
	}
}

bool RadarMap::destroy() {
	return true;
}

bool RadarMap::update(REAL timeElapsed) {
	if (pPlayer != NULL && pTerrain != NULL) {
		// ������ ������� ���̴� �ݰ� ����
		GameFrame *pFrame = g_pGameManager->getGameFramePtr();
		JetFighter *pJet = pPlayer->getPlayerJet();
		REAL radarRad = pJet->getRadarRadius();
		// ������ �÷��̾� ����
		pPlayer = pFrame->getOwnerPlayerPtr();
		// �÷��̾� ������ ������κ��� ���̴� ����Ʈ�� �ҷ���
		pFlightObjList = static_cast<CombatFrame*>(pFrame)->getFlightObjListPtr();

		// ��ĵ ���� ������Ʈ
		if (radarRad > currentScanDist) {
			currentScanDist += scanSpeed * timeElapsed;
			if (currentScanDist > radarRad) {
				currentScanDist = radarRad;
			}
		}
		else if (radarRad == currentScanDist) {
			currentScanDist = 0.0;
		}
	}

	return true;
}

void RadarMap::drawObjIcon(list<FlightObject*> *pObjList, REAL mapWidth, REAL mapHeight) {
	list<FlightObject*> ownerFlightObj;								// �������� �̻���, ����ü�� ���� �������� �׷��ֱ� ���� ������ ����Ʈ
	list<FlightObject*>::iterator iterFlight = pObjList->begin();	// ���������κ��� �޾ƿ� ����ü ����Ʈ
	REAL crRed = 0.0, crGreen = 0.0, crBlue = 0.0, crAlpha = 0.0;	// ���� ����

	// �ػ󵵸� ������
	REAL scrWidth = pUIMaster->getScreenWidth();
	REAL scrHeight = pUIMaster->getScreenHeight();

	// ��� ����ü ����Ʈ�� ���鼭 �ڱ� ������ ����ü�� ������ ����Ʈ��, �� ������ ����ü�� ������ �׸��� �۾��� ����
	for (; iterFlight != pObjList->end(); ++iterFlight) { // begin for1
		FlightObject *pObj = (*iterFlight);
		glPushMatrix();
		{
			if (pObj->getState() == FlightObject::object_state::STATE_ALIVE) { // ��� �ִ� ������ ���� �׸�
				if (pObj->pOwningPlayer == pPlayer) { // ���� ����ü�� ������ ����Ʈ�� ��Ƽ� ���� �������� �׸��� ����
					ownerFlightObj.push_back(pObj);
				}
				else { // begin else
					crRed = 1.0; crGreen = 0.0; crBlue = 0.0; crAlpha = 0.9;
					glColor4f(crRed, crGreen, crBlue, crAlpha);

					// �̴ϸ� ������ �׸���
					static float iconWidth = (pObj->getWidgetType() == GameWidget::WIDGET_JET ? 10.0 : 5.0);
					static float iconHeight = iconWidth;
					float width = scrWidth * iconWidth / 1920;
					float height = scrHeight * iconHeight / 1080;
					float scaledMapWidth = 200.0;
					float scaledMapHeight = 200.0;
					glTranslatef(((pObj->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0, -((pObj->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // ����ü�� ��ġ�� ��ǥ �̵�
					glRotatef(pObj->yaw - 90.0, 0.0, 0.0, 1.0);	// �ٶ󺸴� ����(yaw)�� ���� ������ ȸ��
					glEnable(GL_TEXTURE_2D);
					if (pObj->getWidgetType() == GameWidget::WIDGET_JET) { // ������
						glBindTexture(GL_TEXTURE_2D, jetTextureID);
						glBegin(GL_QUADS);
						{
							glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
							glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
							glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
							glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
						}
						glEnd();
					}
					else if (pObj->getWidgetType() == GameWidget::WIDGET_MISSILE) { // �̻���
						if (static_cast<Missile*>(pObj)->getStyle() != true) { // �Ѿ��� �׸��� ����
							glBindTexture(GL_TEXTURE_2D, missileTextureID);
							glBegin(GL_QUADS);
							{
								glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
								glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
							}
							glEnd();
						}
						glDisable(GL_TEXTURE_2D);
					}
				} // end else
			}
		}
		glPopMatrix();
	} // end for1

	// �ڱ� ������ ����ü���� �̴ϸʿ� �׸��� ���� ���� �ݺ� �۾��� ���ش�. (�ϰ����� �������� �������, ���� ���� ����)
	for (iterFlight = ownerFlightObj.begin(); iterFlight != ownerFlightObj.end(); ++iterFlight) { // begin for2
		FlightObject *pObj = (*iterFlight);
		glPushMatrix();
		{
			crRed = 1.0; crGreen = 1.0; crBlue = 1.0; crAlpha = 0.9;
			glColor4f(crRed, crGreen, crBlue, crAlpha);

			// �̴ϸ� ������ �׸���
			static float iconWidth = (pObj->getWidgetType() == GameWidget::WIDGET_JET ? 10.0 : 5.0);
			static float iconHeight = iconWidth;
			float width = scrWidth * iconWidth / 1920;
			float height = scrHeight * iconHeight / 1080;
			glTranslatef(((pObj->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0, -((pObj->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // ����ü�� ��ġ�� ��ǥ �̵�
			glRotatef(pObj->yaw - 90.0, 0.0, 0.0, 1.0);	// �ٶ󺸴� ����(yaw)�� ���� ������ ȸ��
			glEnable(GL_TEXTURE_2D);
			if (pObj->getWidgetType() == GameWidget::WIDGET_JET) { // ������
				glBindTexture(GL_TEXTURE_2D, jetTextureID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
			}
			else if (pObj->getWidgetType() == GameWidget::WIDGET_MISSILE) { // �̻���
				if (static_cast<Missile*>(pObj)->getStyle() != true) { // �Ѿ��� �׸��� ����
					glBindTexture(GL_TEXTURE_2D, missileTextureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
				}
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	} // end for2
}

/***************************************************************************************************************/
/**************************************************** LockonUI *************************************************/
/***************************************************************************************************************/

LockonUI::LockonUI(JetFighter *pOwnerJet, GLuint texID) : GameWidget() {
	is2D = true;
	uiScale = 0.75;
	textureID = texID;
	pOwnerFighter = pOwnerJet;
	pTargetFighter = NULL;
	isLockonSuccess = false;
}

LockonUI::LockonUI(LockonUI& obj) : GameWidget(obj) {
	is2D = obj.is2D;
	uiScale = obj.uiScale;
	textureID = obj.textureID;
	pOwnerFighter = obj.pOwnerFighter;
	pTargetFighter = obj.pTargetFighter;
	isLockonSuccess = obj.isLockonSuccess;
}

LockonUI::~LockonUI() {
	pOwnerFighter = NULL;
	pTargetFighter = NULL;
}

void LockonUI::setLockonSuccess(bool success) {
	isLockonSuccess = success;
}

bool LockonUI::getLockonSuccess() {
	return isLockonSuccess;
}

void LockonUI::setTargetFighter(JetFighter *fighter) {
	pTargetFighter = fighter;
}

JetFighter* LockonUI::getTargetFighterPtr() {
	return pTargetFighter;
}

bool LockonUI::draw() {
	if (isVisible == true /*&& pTargetFighter != NULL*/) { // test
		glMatrixMode(GL_MODELVIEW);

		// ��ũ�� ��ǥ�� ��ȯ
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
		POINT screenPos = { getPosX(), getPosY() };

		if (screenPos.x > pResolution->getWidth() + 100.0 || screenPos.y > pResolution->getHeight() + 100.0 ||
			screenPos.x < -100.0 || screenPos.y < -100.0) // ��ũ�� �������� ���ϰ� ����� �׸��� ����
		{
			return true;
		}

		glTranslatef(screenPos.x, screenPos.y, 0.0);

		// �� ���� ����
		Vector3 uiColor;
		if (isLockonSuccess == true) { // ����� ������ �Ϸ�� �����̸�
			uiColor = ui::GetSavedUIColor(ui::red);
		}
		else { // ���� ������ �Ϸ���� ���� ������ ��
			if (pOwnerFighter->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) uiColor = ui::GetSavedUIColor(ui::red); // ������ ���µ� ���
			else uiColor = ui::GetSavedUIColor(ui::cyan); // ���� ������ ���
		}
		glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65);
		glScalef(uiScale, uiScale, uiScale);

		// �� �׸���(�簢��)
		static const float size = 50.0; // ũ��
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBegin(GL_QUADS);
		{
			float width = pResolution->getWidth() * size / 1920;
			float height = pResolution->getHeight() * size / 1080;
			glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else {
		return false;
	}

	return true;
}

bool LockonUI::destroy() {
	return true;
}

bool LockonUI::update(REAL timeElapsed) {
	return true;
}

/***************************************************************************************************************/
/**************************************************** StateSys *************************************************/
/***************************************************************************************************************/

StateSys::StateSys(UImaster *pMaster) : GameWidget() {
	pUIMaster = pMaster;
	is2D = true;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/uiRect.png");
	pOwnerPlayer = pMaster->getOwnPlayerPtr();
	pOwnerJet = NULL;
	jetTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/jetUI.png");
	misTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/missileUI.png");
	engTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/uiRectWide.png");
}

StateSys::StateSys(StateSys &obj) : GameWidget(obj) {
	pUIMaster = obj.pUIMaster;
	pOwnerPlayer = obj.pOwnerPlayer;
	textureID = obj.textureID;
	pOwnerJet = obj.pOwnerJet;
	jetTexture = obj.jetTexture;
	misTexture = obj.misTexture;
}

StateSys::~StateSys() {
	pUIMaster = NULL;
	pOwnerPlayer = NULL;
	pOwnerJet = NULL;
}

bool StateSys::draw() {
	if (pOwnerPlayer != NULL && pOwnerJet != NULL && isVisible == true && isHiding == false) {
		// �ػ󵵸� ������
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// ���� �ý��� �׸���
		glPushMatrix();
		{
			// ���� ����1
			Vector3 uiColor;
			REAL baseAlpha = 0.75;
			REAL dmgBlinkTime = pUIMaster->getDmgBlinkTime();
			if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				if (dmgBlinkTime == 0.0) {
					uiColor = ui::GetSavedUIColor(ui::cyan);
				}
				else {
					if ((int)dmgBlinkTime % 100 == 0) uiColor = ui::GetSavedUIColor(ui::cyan);
					else uiColor = ui::GetSavedUIColor(ui::red);
				}
			}
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.01, scrHeight * 0.4, 0.0); // UI�� ȭ��� ���� ��ġ (�»��)

			// ���̵�, ��ü��
			GLTextureFont playerName("PILOT: " + pOwnerJet->pOwningPlayer->getPlayerName(), 5.0);
			GLTextureFont jetName("JET: " + *(pOwnerJet->getCraftNamePtr()), 5.0);
			glTranslatef(0.0, -scrHeight * 0.02, 0.0);
			playerName.drawText();
			glTranslatef(-scrWidth * 0.003, -scrHeight * 0.025, 0.0);
			jetName.drawText();

			// ������ �̹���
			static const float jetSizeX = 105.0; // �ʺ�
			static const float jetSizeY = 120.0; // ����
			float width = scrWidth * jetSizeX / 1920;
			float height = scrHeight * jetSizeY / 1080;
			int dmgPercent = 100.0 - ((REAL)pOwnerJet->HP / pOwnerJet->maxHP) * 100;

			glPushMatrix();
			{
				glTranslatef(scrWidth * 0.06, -scrHeight * 0.1475, 0.0);

				Vector3 uiColor2;
				if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
					uiColor2 = ui::GetSavedUIColor(ui::red);
				}
				else { // �⺻ ���¿����� DMG�� ���� ���� ����, �̻��� ���� ���¿����� ������ �������� ����
					uiColor2.x = ((1.00 - 0.33) * dmgPercent) / 100.0 + 0.33;
					uiColor2.y = ((0.15 - 1.00) * dmgPercent) / 100.0 + 1.00;
					uiColor2.z = ((0.15 - 1.00) * dmgPercent) / 100.0 + 1.00;
					// red:	 rtVec.x = 1.00; rtVec.y = 0.15; rtVec.z = 0.15; break; // ����
					// cyan: rtVec.x = 0.33; rtVec.y = 1.00; rtVec.z = 1.00; break; // û��
				}
				glColor4f(uiColor2.x, uiColor2.y, uiColor2.z, baseAlpha);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, jetTexture);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);

				// ���� ���� (�簢��)
				glPushMatrix();
				{
					glTranslatef(-scrWidth * 0.005, -scrHeight * 0.155, 0.0);
					glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);

					static const float rect1SizeX = 100.0; // �ʺ�
					static const float rect1SizeY = 25.0; // ����
					width = scrWidth * rect1SizeX / 1920;
					height = scrHeight * rect1SizeY / 1080;

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, textureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();

				// ���� ����
				glTranslatef(-scrWidth * 0.045, -scrHeight * 0.155, 0.0);
				char dmg[5];
				itoa(dmgPercent, dmg, 10);
				GLTextureFont getDmgTxt("DMG: " + string(dmg) + "%", 7.8);
				getDmgTxt.drawText();

				// ���� ���� (����)
				static const float rect2SizeX = 110.0;	// �ʺ�
				static const float rect2SizeY = 10.0;	// ����
				width = scrWidth * rect2SizeX / 1920;
				height = scrHeight * rect2SizeY / 1080;

				GLTextureFont engPowerTxt(" ENG:", 5.0);
				GLTextureFont boostTxt("BOOST:", 5.0);
				glTranslatef(scrWidth * 0.1, scrHeight * 0.015, 0.0);
				engPowerTxt.drawText();

				glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);

				// (���� �Ŀ� ����)
				glPushMatrix();
				{
					glTranslatef(scrWidth * 0.095, 0.0, 0.0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, engTexture);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);

					glBegin(GL_QUADS);
					{
						glVertex3f(-width, height, 0.0);
						glVertex3f(-width, -height, 0.0);
						glVertex3f(width * (pOwnerJet->currentEngPower / 50.0) - width, -height, 0.0);
						glVertex3f(width * (pOwnerJet->currentEngPower / 50.0) - width, height, 0.0);
					}
					glEnd();
				}
				glPopMatrix();

				// (�ν�Ʈ ����)
				static const float rect3SizeX = 52.5; // �ʺ�
				static const float rect3SizeY = 10.0; // ����
				width = scrWidth * rect3SizeX / 1920;
				height = scrHeight * rect3SizeY / 1080;

				glTranslatef(0.0, -scrHeight * 0.03, 0.0);

				if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // ���� ����
					uiColor2 = ui::GetSavedUIColor(ui::red);
				}
				else {
					uiColor2 = ui::GetSavedUIColor(ui::cyan);
				}

				glColor4f(uiColor2.x, uiColor2.y, uiColor2.z, baseAlpha);
				boostTxt.drawText();
				glTranslatef(-width, 0.0, 0.0);

				//  ��&�� �ν�Ʈ ������ (2ȸ)
				for (int i = 0; i < 2; ++i) {
					glPushMatrix();
					{
						// �׵θ�
						glTranslatef(scrWidth * 0.095, 0.0, 0.0);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, engTexture);
						glBegin(GL_QUADS);
						{
							glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
							glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
							glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
							glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
						}
						glEnd();
						glDisable(GL_TEXTURE_2D);

						// ä��
						if (pOwnerJet->currentBoost > 50.0) {
							if (i == 0) {
								glBegin(GL_QUADS);
								{
									glVertex3f(-width, height, 0.0);
									glVertex3f(-width, -height, 0.0);
									glVertex3f(width, -height, 0.0);
									glVertex3f(width, height, 0.0);
								}
								glEnd();
							}
							else if (i == 1) {
								glBegin(GL_QUADS);
								{
									glVertex3f(-width, height, 0.0);
									glVertex3f(-width, -height, 0.0);
									glVertex3f(width * ((pOwnerJet->currentBoost - 50.0) / 25.0) - width, -height, 0.0);
									glVertex3f(width * ((pOwnerJet->currentBoost - 50.0) / 25.0) - width, height, 0.0);
								}
								glEnd();
							}
						}
						else {
							if (i == 0) {
								glBegin(GL_QUADS);
								{
									glVertex3f(-width, height, 0.0);
									glVertex3f(-width, -height, 0.0);
									glVertex3f(width * (pOwnerJet->currentBoost / 25.0) - width, -height, 0.0);
									glVertex3f(width * (pOwnerJet->currentBoost / 25.0) - width, height, 0.0);
								}
								glEnd();
							}
						}
					}
					glPopMatrix();

					glTranslatef((width + 2.0) * 2.0, 0.0, 0.0);
				}
				glPopMatrix();
			}

			// ���� �� ź�� ����
			// �⺻ ���� (�����)
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.17, -scrHeight * 0.05, 0.0);
			static const float rect2SizeX = 50.0; // �ʺ�
			static const float rect2SizeY = 25.0; // ����
			width = scrWidth * rect2SizeX / 1920;
			height = scrHeight * rect2SizeY / 1080;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glPushMatrix();
			{
				glTranslatef(-scrWidth * 0.0155, 0.0, 0.0);
				int gunAmmo = 0;
				char gunAmmoTxt[6];
				if (pOwnerJet->getBaseWeaponPtr() != NULL) {
					gunAmmo = pOwnerJet->getBaseWeaponPtr()->getQuantity();
				}
				itoa(gunAmmo, gunAmmoTxt, 10);
				GLTextureFont gunTxt("GUN   " + string(gunAmmoTxt), 7.5);
				gunTxt.drawText();
			}
			glPopMatrix();

			// ù��° ���� (�̻���)
			glTranslatef(0.0, -scrHeight * 0.05, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			WpnLockonMissile *pFirstWpn = pOwnerJet->getFirstWeaponPtr();
			glPushMatrix();
			{
				glTranslatef(-scrWidth * 0.017, 0.0, 0.0);
				int msslAmmo = 0;
				char msslAmmoTxt[6];
				if (pFirstWpn != NULL) {
					msslAmmo = pFirstWpn->getQuantity();
				}
				itoa(msslAmmo, msslAmmoTxt, 10);
				GLTextureFont msslTxt("MSSL  " + string(msslAmmoTxt), 7.5);
				msslTxt.drawText();
			}
			glPopMatrix();

			// ������ ���� ����
			int value = (int)pOwnerJet->pitch;
			if (value < -180.0 || value > 180.0) value = 0;
			char valStr[64];
			glTranslatef(scrWidth * 0.02, 0.0, 0.0);
			glPushMatrix();
			{
				itoa(value, valStr, 10);
				glTranslatef(scrWidth * 0.01, -scrHeight * 0.0725, 0.0);
				GLTextureFont pitchTxt("PITCH:  " + string(valStr), 5.0);
				pitchTxt.drawText();

				glTranslatef(-scrWidth * 0.002, -scrHeight * 0.03, 0.0);
				if (value < 0.0 || value > 360.0) value = 0;
				value = (int)pOwnerJet->yaw;
				itoa(value, valStr, 10);
				GLTextureFont yawTxt(" YAW:  " + string(valStr), 5.0);
				yawTxt.drawText();

				glTranslatef(-scrWidth * 0.002, -scrHeight * 0.03, 0.0);
				value = (int)pOwnerJet->roll;
				if (value < -180.0 || value > 180.0) value = 0; // ���� ����÷� �߻�. ���� ������ �������� ������ �ذ���� �ƴϴ�.
				itoa(value, valStr, 10);
				GLTextureFont rollTxt(" ROLL:  " + string(valStr), 5.0);
				rollTxt.drawText();
			}
			glPopMatrix();

			// �̻��� ����
			if (pFirstWpn != NULL) {
				int msslCnt = pFirstWpn->getWpnMissileCount();											// �ѹ��� ������ �� �ִ� �̻����� ��
				REAL msslDelayMax = pFirstWpn->getFirePerSecond();										// �̻��� ������ �ӵ�
				vector<REAL> *pFireLeftTime = pOwnerJet->getFirstWeaponPtr()->getWpnFireLeftTimePtr();	// �̻��ϵ��� ���� ���ð��� ��� ����

				glTranslatef(-scrWidth * 0.05, -scrHeight * 0.1, 0.0);
				float msslSizeX = 60.0 / msslCnt; // �ʺ� (4�� ���� => 15.0)
				msslSizeX > 15.0 ? msslSizeX = 15.0 : 0;
				const float msslSizeY = 65.0;	  // ����
				width = scrWidth * msslSizeX / 1920;
				height = scrHeight * msslSizeY / 1080;

				for (int i = 0; i < msslCnt; ++i) {
					// �̻��� ���
					glColor4f(uiColor.x - 0.25, uiColor.y - 0.25, uiColor.z - 0.25, baseAlpha);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, misTexture);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0);	glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);

					// �̻��� ���� ��Ȳ %�� ��ĥ�ϴ� ������ ǥ��
					glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, misTexture);
					REAL msslRdyTime = pFireLeftTime->at(i);
					REAL msslRdyPercent = (msslRdyTime * msslDelayMax); // (0.0 ~ 1.0)
				
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0 - msslRdyPercent / 2.0);	glVertex3f(-width, height * (1.0 - msslRdyPercent), 0.0);
						glTexCoord2f(0.0, 0.0 + msslRdyPercent / 2.0);	glVertex3f(-width, -height * (1.0 - msslRdyPercent), 0.0);
						glTexCoord2f(1.0, 0.0 + msslRdyPercent / 2.0);	glVertex3f(width, -height * (1.0 - msslRdyPercent), 0.0);
						glTexCoord2f(1.0, 1.0 - msslRdyPercent / 2.0);	glVertex3f(width, height * (1.0 - msslRdyPercent), 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);

					glTranslatef(scrWidth * (0.06 / msslCnt), 0.0, 0.0); // (4�� ���� => scrHeight * 0.015)
				}
			}
		}
		glPopMatrix();
	}

	return true;
}

bool StateSys::destroy() {
	return true;
}

bool StateSys::update(REAL timeElapsed) {
	// ������ ǥ���� �����⸦ �÷��̾��� ������� �׻� ����
	pOwnerJet = pOwnerPlayer->getPlayerJet();

	return true;
}

/***************************************************************************************************************/
/*************************************************** WarningSys ************************************************/
/***************************************************************************************************************/

WarningSys::WarningSys(UImaster *pMaster) : GameWidget() {
	pUIMaster = pMaster;
	is2D = true;
	pOwnerPlayer = pMaster->getOwnPlayerPtr();
	pOwnerJet = pOwnerPlayer->getPlayerJet();
	isMsgDestroy = false;
	sysLeftDuration = 0.00;

	sysTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/sys.png");
	hitSysTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/sysHit.png");
	destroySysTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/sysDestroy.png");
	missileAlertTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/msslAlert.png");
	lockonAlertTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/lockonAlert.png");
	offCourseTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/offCourseAlert.png");
	pullUpTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/pullUpWarning.png");
	stallTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/stallWarning.png");

	pSndMssileAlert = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/msslWarning.wav", false, true);
	pSndLockonWarning = new Sound(Vector3(0.0, 0.0, 0.0), "res/Sounds/lockonWarning.wav", false, true);
	pSndMssileAlert->pause();	// �ڵ� ��� ���
	pSndLockonWarning->pause();
	pSndMssileAlert->setChannelNum(pOwnerPlayer->getPlayerNumber());	// ���� ä�� ����
	pSndLockonWarning->setChannelNum(pOwnerPlayer->getPlayerNumber());
}

WarningSys::WarningSys(WarningSys &obj) {
	pUIMaster = obj.pUIMaster;
	pOwnerPlayer = obj.pOwnerPlayer;
	pOwnerJet = obj.pOwnerJet;
	isMsgDestroy = obj.isMsgDestroy;
	sysLeftDuration = obj.sysLeftDuration;

	sysTexture = obj.sysTexture;
	hitSysTexture = obj.hitSysTexture;
	destroySysTexture = obj.destroySysTexture;
	missileAlertTexture = obj.missileAlertTexture;
	lockonAlertTexture = obj.lockonAlertTexture;
	pullUpTexture = obj.pullUpTexture;
	stallTexture = obj.stallTexture;
}

WarningSys::~WarningSys() {
	pUIMaster = NULL;
	pOwnerPlayer = NULL;
	pOwnerJet = NULL;
}

void WarningSys::setSystemAlarm(bool isDestory, REAL time) {
	if (isDestory == true) { // �ı��� �޽����� �����Ǹ� ���� �޽������� ������ ����
		isMsgDestroy = isDestory;
		sysLeftDuration = time;
	}
	else { // Ÿ�� �޽����� �����Ǹ� ���� �޽����� �ı����� �ƴ� ��쿡�� ������ ����
		if (isMsgDestroy == false) {
			isMsgDestroy = isDestory;
			sysLeftDuration = time;
		}
		else {
			// �ƹ��͵� ���� ���� . . .
		}
	}
}

bool WarningSys::draw() {
	if (isVisible != true || pOwnerJet == NULL || pOwnerPlayer == NULL) return false;

	// �ػ󵵸� ������
	REAL scrWidth = pUIMaster->getScreenWidth();
	REAL scrHeight = pUIMaster->getScreenHeight();

	// ���� ����
	Vector3 sysColor;
	Vector3 warningColor = ui::GetSavedUIColor(ui::red);
	REAL baseAlpha = 0.85;
	REAL dmgBlinkTime = pUIMaster->getDmgBlinkTime();

	if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
		sysColor = ui::GetSavedUIColor(ui::red);
	}
	else {
		if (dmgBlinkTime == 0.0) {
			sysColor = ui::GetSavedUIColor(ui::cyan);
		}
		else {
			if ((int)dmgBlinkTime % 100 == 0) sysColor = ui::GetSavedUIColor(ui::cyan);
			else sysColor = ui::GetSavedUIColor(ui::red);
		}
	}

	// Ǯ�� �˸�
	if (pOwnerJet->getAltitudeWarningState() == true) {
		glPushMatrix();
		{
			static const int sizeX = 128.0; // �ʺ�
			static const int sizeY = 48.0; // ����
			int width = scrWidth * sizeX / 1920;
			int height = scrHeight * sizeY / 1080;

			glColor4f(warningColor.x, warningColor.y, warningColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.5, scrHeight * 0.25, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, pullUpTexture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

	// �ڽ� ��Ż, ���� �˸��� ���� ���� �˸�
	GLuint alertTextureID = 0;
	int sizeX = 128; // �ʺ�
	int sizeY = 48; // ����
	int width = scrWidth * sizeX / 1920;
	int height = scrHeight * sizeY / 1080;

	if (pOwnerJet->getOffCourseTime() > 0.00) { // ��Ȳ�� ���� �ؽ�ó ���� (�켱��: �����ڽ� > ���� > ���� ����)
		alertTextureID = offCourseTexture;
	}
	else if (pOwnerJet->getStallWarningState() == true) {
		alertTextureID = stallTexture;
	}

	if (alertTextureID != 0) { // �׸���
		glPushMatrix();
		{
			glColor4f(warningColor.x, warningColor.y, warningColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.5, scrHeight * 0.125, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, alertTextureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

	// �̻��ϰ� ���� �溸
	JetFighter::jet_warning warnType = pOwnerJet->getLockonWarningState();
	if (warnType != JetFighter::jet_warning::JET_LOCKON_NONE) {
		GLuint warningTexID = 0;

		switch (warnType) { // ���� �� �ؽ�ó ����
		case JetFighter::jet_warning::JET_LOCKON_BY_ENEMY:
			glColor4f(sysColor.x, sysColor.y, sysColor.z, baseAlpha);
			warningTexID = lockonAlertTexture;	break;
		case JetFighter::jet_warning::JET_LOCKON_BY_MISSILE:
			glColor4f(warningColor.x, warningColor.y, warningColor.z, baseAlpha);
			warningTexID = missileAlertTexture;	break;
		default:
			glColor4f(sysColor.x, sysColor.y, sysColor.z, baseAlpha);
			warningTexID = 0;
		}

		glPushMatrix();
		{
			static const int sizeX = 100.0; // �ʺ�
			static const int sizeY = 32.0; // ����
			int width = scrWidth * sizeX / 1920;
			int height = scrHeight * sizeY / 1080;

			glTranslatef(scrWidth * 0.5, scrHeight * 0.8, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, warningTexID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

	// �ı��� Ÿ�� �˸�
	REAL crAlpha = baseAlpha * (sysLeftDuration > 0.33 ? 1.00 : sysLeftDuration);
	REAL widthFx = (sysLeftDuration > 0.25 ? 1.00 : 1.5 - sysLeftDuration);
	REAL heightFx = (sysLeftDuration > 0.25 ? 1.00 : sysLeftDuration);
	if (sysLeftDuration > 0.00) {
		// �������� ǥ���Ǵ� SYS �޽���
		GLuint hitDesTexID = 0;
		int sizeX = 50.0; // �ʺ�
		int sizeY = 15.0; // ����
		int width = (scrWidth * sizeX / 1920) * widthFx;
		int height = (scrHeight * sizeY / 1080) * heightFx;

		glPushMatrix(); // SYS �޽���
		{
			glColor4f(sysColor.x, sysColor.y, sysColor.z, crAlpha + 0.1);
			glTranslatef(scrWidth * 0.5, scrHeight * 0.725, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, sysTexture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		if (isMsgDestroy == true) { // �ı��� ���
			sizeX = 160.0; // �ʺ�
			sizeY = 22.0; // ����
			hitDesTexID = destroySysTexture; // �ؽ�ó ��ȣ
			
		}
		else { // Ÿ���� ���
			sizeX = 64.0; // �ʺ�
			sizeY = 20.0; // ����
			hitDesTexID = hitSysTexture; // �ؽ�ó ��ȣ
		}
		
		width = (scrWidth * sizeX / 1920) * widthFx;
		height = (scrHeight * sizeY / 1080) * heightFx;

		glPushMatrix(); // HIT or DESTROY �޽���
		{
			glColor4f(warningColor.x, warningColor.y, warningColor.z, crAlpha);
			glTranslatef(scrWidth * 0.5, scrHeight * 0.68, 0.0);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, hitDesTexID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

	return true;
}

bool WarningSys::destroy() {
	return true;
}

bool WarningSys::update(REAL timeElapsed) {
	// �÷��̾� ���� ������ ����
	pOwnerJet = pOwnerPlayer->getPlayerJet();

	// �˶� ǥ�� �ð� ����
	if ((sysLeftDuration -= timeElapsed) < 0.0) {
		sysLeftDuration = 0.0;
		isMsgDestroy = false; // false �� �ʱ�ȭ �����ν� �ٸ� �޽����� �Է��� ��� ����
	}

	// ���¿� ���� ���� ��� �� �Ͻ� ����
	switch (pOwnerJet->getLockonWarningState()) {
	case JetFighter::jet_warning::JET_LOCKON_BY_MISSILE:
		pSndMssileAlert->play();
		pSndLockonWarning->pause();
		break;
	case JetFighter::jet_warning::JET_LOCKON_BY_ENEMY:
		pSndMssileAlert->pause();
		pSndLockonWarning->play();
		break;
	default:
		pSndMssileAlert->pause();
		pSndLockonWarning->pause();
	}

	return true;
}


/***************************************************************************************************************/
/**************************************************** RespawnUI ************************************************/
/***************************************************************************************************************/

RespawnUI::RespawnUI(UImaster *pMaster) :GameWidget() {
	is2D = true;
	pUIMaster = pMaster;
	pOwnerPlayer = pMaster->getOwnPlayerPtr();
	pKillerPlayer = NULL;
	respawnElapsedTime = 0.0;
}

RespawnUI::RespawnUI(RespawnUI &obj) : GameWidget(obj) {
	is2D = obj.is2D;
	pUIMaster = obj.pUIMaster;
	pOwnerPlayer = obj.pOwnerPlayer;
	pKillerPlayer = obj.pKillerPlayer;
	respawnElapsedTime = obj.respawnElapsedTime;
	respawnCamPos = obj.respawnCamPos;
}

RespawnUI::~RespawnUI() {
	pUIMaster = NULL;
	pOwnerPlayer = NULL;
	pKillerPlayer = NULL;
}

void RespawnUI::setKillerPlayer(Player *pPlayer) {
	pKillerPlayer = pPlayer;
}

Player* RespawnUI::getKillerPlayerPtr() {
	return pKillerPlayer;
}

bool RespawnUI::draw() {
	// ���� �˻�
	if (isVisible == true && pOwnerPlayer != NULL) {
		string killerName;
		if (pKillerPlayer == NULL) // �ڻ� ��
			killerName = "YOURSELF";
		else { // Ÿ�� ��
			killerName += "BY ";
			killerName += pKillerPlayer->getPlayerName();
		}
		// ��� �� ǥ���� ������ �ϼ���Ŵ
		string str("YOU'VE KILLED ");
		str += killerName + ".";

		// ���� ����
		Vector3 uiColor = ui::GetSavedUIColor(ui::red);
		REAL uiBaseAlpha = 1.00;

		// �ػ󵵸� �޾� ��
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();
		GLTextureFont deadTxt(str, 32.5, "res/Textures/font/koverwatchFont.png");

		// ���ڿ� ���̿� ���� ����
		REAL ptX = (scrWidth / 2.0) - deadTxt.findCenterOfTextPosX();

		// ó���� ���̵� �� ��� ���� ���
		glPushMatrix();
		{
			glColor4f(uiColor.x, uiColor.y, uiColor.z, uiBaseAlpha);
			glTranslatef(ptX, scrHeight * 0.6, 0.0);
			deadTxt.drawText();
		}
		glPopMatrix();

		// ���� ������ �ð� �������
		str = "";
		int leftTime = pOwnerPlayer->getPlayerJet()->getRespawnLeftTime(); // �ش� ��ü�� ���� ������ �ð��� �޾ƿ�
		char leftTimeStr[4] = { "\0", };									// �ִ� 4�ڸ� ���� ��Ȱ �ð��� ���� �� ����
		itoa(leftTime + 1, leftTimeStr, 10);

		str = leftTimeStr; // ���� �迭�� string Ŭ������ ����

		GLTextureFont respawnTxt("YOU WILL RESPAWN IN: " + str, 20.0, "res/Textures/font/koverwatchFont.png");
		ptX = (scrWidth / 2.0) - respawnTxt.findCenterOfTextPosX();
		uiColor = ui::GetSavedUIColor(ui::white);
		glPushMatrix();
		{
			glColor4f(uiColor.x, uiColor.y, uiColor.z, uiBaseAlpha);
			glTranslatef(ptX, scrHeight * 0.4, 0.0);
			respawnTxt.drawText();
		}
		glPopMatrix();

	// ...
	}

	return true;
}

bool RespawnUI::destroy() {
	return true;
}

bool RespawnUI::update(REAL timeElapsed) {
	respawnElapsedTime += timeElapsed; // �ð� ���

	return true;
}

/***************************************************************************************************************/
/*************************************************** ScoreBoard ************************************************/
/***************************************************************************************************************/

ScoreBoard::ScoreBoard(UImaster *pMaster) : GameWidget() {
	is2D = true;
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/uiRectWide.png");
	pUIMaster = pMaster;
	pFrame = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr());
	scoreVector.reserve(16);
	scoreVector.clear();
}

ScoreBoard::ScoreBoard(ScoreBoard& obj) : GameWidget(obj) {
	pUIMaster = obj.pUIMaster;
	pFrame = obj.pFrame;
	scoreVector = obj.scoreVector;
}

ScoreBoard::~ScoreBoard() {
	pUIMaster = NULL;
	pFrame = NULL;
}

bool ScoreBoard::draw() {
	if (pUIMaster != NULL && pFrame != NULL) {
		// �ػ󵵸� �ҷ���
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// ���� ����
		Vector3 uiColor;
		REAL baseAlpha = 0.65;
		if (pUIMaster->getOwnPlayerPtr()->getPlayerJet()->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // �̻��� ���� ��
			uiColor = ui::GetSavedUIColor(ui::red);
		}
		else { // �� ���� ���
			uiColor = ui::GetSavedUIColor(ui::cyan);
		}

		// ���� �ð� �׸���	
		int sizeX = 45.0; // �׸� �ڽ� ũ�� ����
		int sizeY = 20.0;
		int width = (scrWidth * sizeX / 1920);
		int height = (scrHeight * sizeY / 1080);

		glPushMatrix(); // TIME �޽��� �� ���� �ð�
		{
			// TIME �޽���
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.01, scrHeight * 0.97, 0.0);

			GLTextureFont timeTxt("TIME", 6.0);
			timeTxt.drawText();

			// ���� �ð�
			REAL leftTime = pFrame->getSessionLeftTime();
			char leftTimeChar[9] = { '0' };
			int leftMin = ((int)leftTime / 60);	// ��
			int leftSec = ((int)leftTime % 60);	// ��
			int leftMilSec = ((leftTime - (int)leftTime) * 100.0);	// �и���
			sprintf(leftTimeChar, "%02d:%02d:%02d", leftMin, leftSec, leftMilSec);

			// �и���
			string leftTimeStr = leftTimeChar;
			GLTextureFont leftTimeTxt(leftTimeStr, 6.0);
			glTranslatef(scrWidth * 0.05, 0.0, 0.0);
			leftTimeTxt.drawText();
		}
		glPopMatrix();
		glPushMatrix(); // �׸� �ڽ�
		{
			glTranslatef(scrWidth * 0.0275, scrHeight * 0.97, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
				glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
				glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
				glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		// ���ھ�� �׸���
		glPushMatrix(); // ���ھ�� ����
		{
			glTranslatef(scrWidth * 0.01, scrHeight * 0.93, 0.0);
			GLTextureFont infoTxt("RANK  ID   KILLS  SOCRE", 6.0);
			infoTxt.drawText();
		}
		glPopMatrix();

		int rank = 0;
		for (vector<Player*>::iterator iter = scoreVector.begin(); iter != scoreVector.end(); ++iter) {
			glPushMatrix(); // ���ھ�� �÷��̾� �� ����
			{
				Player *pPlayer = (*iter);
				if (pPlayer != NULL) {
					glTranslatef(scrWidth * 0.005, (scrHeight * (0.93 - (++rank * 0.025))), 0.0);

					char rankChar[64];
					sprintf(rankChar, "%2d. %8.8s     %3d     %5d", rank, pPlayer->getPlayerName().c_str(), pPlayer->getPlayerKills(), pPlayer->getPlayerScore());

					if (pPlayer->getPlayerJet()->getState() == FlightObject::object_state::STATE_DEAD) // ��� �� ���� ��Ӱ�
						glColor4f(uiColor.x - 0.25, uiColor.y - 0.25, uiColor.z - 0.25, baseAlpha);
					else 
						glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);

					GLTextureFont rankTxt(string(rankChar), 5.0);
					rankTxt.drawText();
				}
			}
			glPopMatrix();
		}

		return true;
	}

	return false;
}

bool ScoreBoard::destroy() {
	return true;
}

bool ScoreBoard::update(REAL timeElapsed) {
	/* ���ھ� ���� ������Ʈ */
	scoreVector.clear(); // ���ھ� ���� �ʱ�ȭ

	list<Player*> *pPlyaerList = pFrame->getPlayerListPtr(); // ���������κ��� �÷��̾� ����Ʈ�� �޾� �ͼ� ����ȭ��Ŵ
	for (list<Player*>::iterator iter = pPlyaerList->begin(); iter != pPlyaerList->end(); ++iter) {
		scoreVector.push_back(*iter);
	}

	// �켱������ ���߾� ����
	sort(scoreVector.begin(), scoreVector.end(), Player::cmpScore);

	return true;
}

/***************************************************************************************************************/
/*************************************************** MissionMsg ************************************************/
/***************************************************************************************************************/

MissionMsg::MissionMsg(UImaster *pMaster) : GameWidget() {
	is2D = true;

	pUIMaster = pMaster;
	startMsgLeftTime = 2.0;
	compeleteMsgLeftTime = 2.0;
	sessionDestructLeftTime = 1.5;

	sysTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/sys.png");
	missionStartTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/missionStartMsg.png");
	missionCompleteTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/missionCompleteMsg.png");
	serverDisconnectedTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/ui/serverDisconnectedMsg.png");
}

MissionMsg::MissionMsg(MissionMsg &obj) : GameWidget(obj) {
	pUIMaster = obj.pUIMaster;
	startMsgLeftTime = obj.startMsgLeftTime;
	compeleteMsgLeftTime = obj.compeleteMsgLeftTime;
	sessionDestructLeftTime = obj.compeleteMsgLeftTime;

	sysTexID = obj.sysTexID;
	missionStartTexID = obj.missionStartTexID;
	missionCompleteTexID = obj.missionCompleteTexID;
	sessionDestructLeftTime = obj.sessionDestructLeftTime;
}

MissionMsg::~MissionMsg() {
	pUIMaster = NULL;
}

REAL MissionMsg::getSessionDestructLeftTime() {
	return sessionDestructLeftTime;
}

bool MissionMsg::draw() {
	if (isVisible == true && pUIMaster != NULL) {
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();
		REAL widthFx = 1.0;
		REAL heightFx = 1.0;
		REAL baseAlpha = 0.65;
		Vector3 uiColorCyan = ui::GetSavedUIColor(ui::cyan);
		Vector3 uiColorRed = ui::GetSavedUIColor(ui::red);
		int sizeX = 0.0;
		int sizeY = 0.0;
		int width = 0;
		int height = 0;

		// �̼� �ð� ����
		if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() <= 0.00) {
			// �̼� ���� ���
			if (compeleteMsgLeftTime > 0.0) {
				GLuint texID = 0;
				// ������ ��Ŀ���õǾ� �����ϴ� ���
				if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isMultiplayMode() == true &&
					static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isServerDisconnected() == true) {
					texID = serverDisconnectedTexID;
				}
				// ���������� ���� ���� ������ �����Ͽ� ����Ǵ� ���
				else {
					texID = missionCompleteTexID;
				}

				// �׸��� �ɼ�
				sizeX = 50.0;
				sizeY = 15.0;
				widthFx = (compeleteMsgLeftTime > 0.25 ? 1.00 : 1.5 - compeleteMsgLeftTime);
				heightFx = (compeleteMsgLeftTime > 0.25 ? 1.00 : compeleteMsgLeftTime);
				width = (scrWidth * sizeX / 1920) * widthFx;
				height = (scrHeight * sizeY / 1080) * heightFx;
				baseAlpha = (compeleteMsgLeftTime > 0.25 ? 0.65 : compeleteMsgLeftTime);
				
				// SYS �޽���
				glPushMatrix();
				{
					glColor4f(uiColorCyan.x, uiColorCyan.y, uiColorCyan.z, baseAlpha);
					glTranslatef(scrWidth * 0.5, scrHeight * 0.725, 0.0);

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, sysTexID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();

				// MISSION START �޽���
				sizeX = 160.0; // �ʺ�
				sizeY = 32.0; // ����
				width = (scrWidth * sizeX / 1920) * widthFx;
				height = (scrHeight * sizeY / 1080) * heightFx;
				glPushMatrix();
				{
					glColor4f(uiColorRed.x, uiColorRed.y, uiColorRed.z, baseAlpha);
					glTranslatef(scrWidth * 0.5, scrHeight * 0.675, 0.0);

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
						glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();
			}
			// ���� �ı� ���� ���̵� �ƿ�
			else if (sessionDestructLeftTime > 0.0) {
				glPushMatrix();
				{
					glColor4f(0.0, 0.0, 0.0, 1.5 - sessionDestructLeftTime);

					glBegin(GL_QUADS);
					{
						glVertex3f(0.0, scrHeight, 0.0);
						glVertex3f(0.0, 0.0, 0.0);
						glVertex3f(scrWidth, 0.0, 0.0);
						glVertex3f(scrWidth, scrHeight, 0.0);
					}
					glEnd();
				}
				glPopMatrix();
			}
		}
		// �̼� ���� ���
		else if (startMsgLeftTime > 0.0) {
			// SYS �޽���
			sizeX = 50.0;
			sizeY = 15.0;
			widthFx = (startMsgLeftTime > 0.25 ? 1.00 : 1.5 - startMsgLeftTime);
			heightFx = (startMsgLeftTime > 0.25 ? 1.00 : startMsgLeftTime);
			width = (scrWidth * sizeX / 1920) * widthFx;
			height = (scrHeight * sizeY / 1080) * heightFx;
			baseAlpha = (startMsgLeftTime > 0.25 ? 0.65 : startMsgLeftTime);

			glPushMatrix();
			{
				glColor4f(uiColorCyan.x, uiColorCyan.y, uiColorCyan.z, baseAlpha);
				glTranslatef(scrWidth * 0.5, scrHeight * 0.725, 0.0);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, sysTexID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// MISSION START �޽���
			sizeX = 160.0; // �ʺ�
			sizeY = 32.0; // ����
			width = (scrWidth * sizeX / 1920) * widthFx;
			height = (scrHeight * sizeY / 1080) * heightFx;
			glPushMatrix();
			{
				glColor4f(uiColorRed.x, uiColorRed.y, uiColorRed.z, baseAlpha);
				glTranslatef(scrWidth * 0.5, scrHeight * 0.675, 0.0);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, missionStartTexID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0); glVertex3f(-width, height, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(width, -height, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
	}

	return true;
}

bool MissionMsg::destroy() {
	return true;
}

bool MissionMsg::update(REAL timeElapsed) {
	// ���� �� �����
	if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) {
		// �̼� ���� �޽��� ���
		if ((startMsgLeftTime -= timeElapsed) < 0.0) startMsgLeftTime = 0.0;
	}
	// ���� �����
	else {
		// �̼� ���ø�Ʈ �޽��� ���
		if ((compeleteMsgLeftTime -= timeElapsed) < 0.0) compeleteMsgLeftTime = 0.0;
		
		// �޽��� ��� �Ϸ� �� ���� �ı� �ð� ī��Ʈ
		if (compeleteMsgLeftTime <= 0.0) {
			if ((sessionDestructLeftTime -= timeElapsed) < 0.0) sessionDestructLeftTime = 0.0;
		}
	}

	return true;
}

/***************************************************************************************************************/
/************************************************** ScreenEffect ***********************************************/
/***************************************************************************************************************/

ScreenEffect::ScreenEffect(UImaster *pMaster) : GameWidget() {
	pUIMaster = pMaster;
	is2D = true;
	scrEffectVector.reserve(16);
	scrEffectVector.clear();
}

ScreenEffect::ScreenEffect(ScreenEffect& obj) {
	// ...
}

ScreenEffect::~ScreenEffect() {
	// ���� ��ũ�� ����Ʈ�� �ı�
	clearEffects();

	// ������ �����͸� NULL��
	pUIMaster = NULL;
}

void ScreenEffect::createEffect(REAL time, int effectID, ScrEffectAction action, GLuint texID) {
	// �̹� �ش� effectID�� ����Ʈ�� �����Ǿ� ������ ���� �ð� ���� �� ����
	for (int i = 0; i < scrEffectVector.size(); ++i) {
		if (scrEffectVector[i]->iEffectID == effectID) {
			scrEffectVector[i]->rLeftTime = time;
			return;
		}
	}

	// �߰��� ���� ���ٸ� �������� ����
	ScrEffect *pEffect = new ScrEffect;

	// ���� ���� ���� ���� Ȯ��
	if (pEffect != NULL) {
		// ����Ʈ ���� ����
		pEffect->iEffectID = effectID;
		pEffect->enAction = action;
		pEffect->rLeftTime = time;
		pEffect->rValue = 0.00f;

		// ����Ʈ �׼� ����
		if (texID == 0) {
			switch (action) {
			default: case ScreenEffect::ScrEffectAction::SCREFFECT_WARNGLOW:
				pEffect->uiTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/screen/warningGlower.png");
				break;
			case ScreenEffect::ScrEffectAction::SCREFFECT_BOOST:
				pEffect->uiTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/screen/boostGlower.png");
				break;
			case ScreenEffect::ScrEffectAction::SCREENEFFECT_DUST:
				pEffect->uiTexID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/screen/dustGlower.png");
				break;
			}
		}
		else {
			pEffect->uiTexID = texID;
		}

		// ����Ʈ ���Ϳ� Ǫ�ù�
		scrEffectVector.push_back(pEffect);
	}
	else {
		return;
	}
}

void ScreenEffect::clearEffects() {
	// ���͸� ���鼭 ��� ���� �Ҵ�� ����ü�� �ı�
	for (vector<ScrEffect*>::iterator iter = scrEffectVector.begin(); iter != scrEffectVector.end();) {
		if ((*iter) != NULL) {
			delete (*iter);
			iter = scrEffectVector.erase(iter);
		}
		else {
			++iter;
		}
	}
}

bool ScreenEffect::draw() {
	// �׸��� ���ܰų� ������ �ƴ� ���
	if (isVisible == true && pUIMaster != NULL) {
		// ���� ����
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
		REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();

		glTranslatef(scrWidth * 0.5, scrHeight * 0.5, 0.0); // ��ũ���� �߾����� �̵�

		// ����Ʈ ���͸� ��ȸ�ϸ鼭
		for (vector<ScrEffect*>::iterator iter = scrEffectVector.begin(); iter != scrEffectVector.end(); ++iter) {
			ScrEffect *pEffect = (*iter);
			REAL rVal = 0.00;

			glPushMatrix();
			{
				switch (pEffect->enAction) {
				default: case ScreenEffect::ScrEffectAction::SCREFFECT_WARNGLOW:
					// ��� �۷ο� ���� ����
					glColor4f(0.95, 0.05, 0.05, abs(sinf(pEffect->rValue)) * 0.65 + 0.35);  // 0.35 ~ 1.00 ���İ����� ������ ����
					rVal = abs(sinf(pEffect->rValue)) * 0.35 + 0.65;
					glScalef(rVal, rVal, 1.0); // ª�� �þ����� ��ҵ�
					break;
				case ScreenEffect::ScrEffectAction::SCREFFECT_BOOST:
					// �ν�Ʈ �۷ο� ���� �� ȿ�� ����
					glColor4f(1.00, 1.00, 1.00, 0.75 - (pEffect->rValue / 7.00));	// 0.75 -> 0.00 ���İ����� ������ �����
					rVal = abs(sinf(pEffect->rValue / 3.0)) * 0.1 + 0.5;
					glRotatef(sinf(pEffect->rValue * 45.0) * 0.125, 0.0, 0.0, 1.0); // ª�� ����
					glScalef(rVal, rVal, 1.0); // ��� �þ���
					break;
				case ScreenEffect::ScrEffectAction::SCREENEFFECT_DUST:
					// ����Ʈ �۷ο� ���� ����
					glColor4f(0.01, 0.05, 0.03, 1.00 - (pEffect->rValue / 4.0));	// 1.00 -> 0.00 ���İ����� ������ �����
					rVal = abs(sinf(pEffect->rValue / 5.00)) * 0.3 + 0.60;
					glScalef(rVal, rVal, 1.0); // õõ�� �þ���
					break;
				}

				// �׸���
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, pEffect->uiTexID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0, 1.0); glVertex3f(-scrWidth, scrHeight, 0.0);
					glTexCoord2f(0.0, 0.0); glVertex3f(-scrWidth, -scrHeight, 0.0);
					glTexCoord2f(1.0, 0.0); glVertex3f(scrWidth, -scrHeight, 0.0);
					glTexCoord2f(1.0, 1.0); glVertex3f(scrWidth, scrHeight, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

		}
	}
	
	return true;
}

bool ScreenEffect::destroy() {
	return true;
}

bool ScreenEffect::update(REAL timeElapsed) {
	// ���͸� ��ȸ�ϸ� ����Ʈ ���� �ð��� ���Ž�Ű��, 0.00�� ������ ������ ������ ������ ���� ������ ����Ʈ�� �ı���Ŵ
	for (vector<ScrEffect*>::iterator iter = scrEffectVector.begin(); iter != scrEffectVector.end();) {
		if (!((((*iter)->rLeftTime) -= timeElapsed) > 0.00)) {
			delete (*iter);
			iter = scrEffectVector.erase(iter);
		}
		else {
			// ����Ʈ ���� �ð��� �����Ŵ
			(*iter)->rValue += timeElapsed;

			// ���� ����Ʈ ����
			++iter;
		}
	}

	return true;
}
