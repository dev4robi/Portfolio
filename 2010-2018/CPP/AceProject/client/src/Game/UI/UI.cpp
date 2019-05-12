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
	// 피해 입을 시 점등하는 시간 초기화
	dmgBlinkLeftTime = 0.0;

	// 소유 플레이어와 해당 플레이어의 UI위젯 리스트의 포인터 설정
	pOwnPlayer = _pOwnPlayer;
	pUIDrawList = pOwnPlayer->getPlayerUIDrawListPtr();

	// 플레이어 상태 시스템 UI
	pStateSys = new StateSys(this);

	// 플레이어 속도계와 고도계 UI
	pSpeedAltMeter = new SpeedAltMeter(this);

	// 플레이어 맵 UI
	pRadarMap = new RadarMap(this);

	// 전투기 각종 경보 및 알림 UI
	pWarningSys = new WarningSys(this);

	// 리스폰 UI
	pRespawnUI = new RespawnUI(this);

	// 스코어보드 UI
	pScoreBoard = new ScoreBoard(this);

	// 미션 메시지 UI
	pMissionMsg = new MissionMsg(this);

	// 스크린 이펙트 UI
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
	// 에러 방지
	JetFighter *pJet = pOwnPlayer->getPlayerJet();
	if (pUIDrawList == NULL || pJet == NULL) return;

	// 해상도 갱신
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	scrWidth = pResolution->getWidth();
	scrHeight = pResolution->getHeight();

	// 데미지에 의한 점멸 시간 업데이트
	if ((dmgBlinkLeftTime -= (timeElapsed * 1000.0)) < 0.0) dmgBlinkLeftTime = 0.0;

	/*** UI들 업데이트 ***/
	/** RadarLockon Widget **/
	// 락온 타겟이 radarLockonUI List에 추가된 적이 없으면 추가함
	list<FlightObject*> *pJetRadar = pJet->getRadarListPtr();		// 플레이어 제트기의 레이더 락온된 비행체 리스트를 가져옴
	list<FlightObject*>::iterator iterRadar = pJetRadar->begin();	// 플레이어 제트기의 레이더 락온 리스트의 시작을 가져옴
	for (; iterRadar != pJetRadar->end(); ++iterRadar) {
		if ((*iterRadar)->getWidgetType() == GameWidget::WIDGET_JET) { // 중요!: 레이더 락온 UI는 전투기의 경우에만 표시해줘야 한다! (수정 없이 사용하면 미사일은 오류 발생)
			// 한 번도 추가된 적이 없어서 리스트가 비어 있는 경우에는 바로 추가
			if (radarLockonUIList.size() == 0) {
				radarLockonUIList.push_back(new RadarLockon(pJet, static_cast<JetFighter*>(*iterRadar)));
			}
			// 리스트에 이미 추가된 적이 있다면 이미 추가되어 있는 대상인지 확인
			else {
				bool isExist = false;
				list<RadarLockon*>::iterator iterLockon = radarLockonUIList.begin();
				for (; iterLockon != radarLockonUIList.end(); ++iterLockon) {
					// 레이더 타겟이 리스트에 이미 들어 있는 경우 위치를 갱신해 주고 탈출
					if ((*iterRadar) == (*iterLockon)->getTargetFighterPtr()) {
						isExist = true;
						break;
					}
				}
				// 추가된 적이 없는 경우에는 추가
				if (isExist == false) {
					radarLockonUIList.push_back(new RadarLockon(pJet, static_cast<JetFighter*>(*iterRadar)));
				}
			}
		}
	}
	// 락온 타겟이 radarList에 존재하지 않으면 제거함
	for (list<RadarLockon*>::iterator uiIter = radarLockonUIList.begin(); uiIter != radarLockonUIList.end();) {
		bool isInRadar = false;
		for (iterRadar = pJetRadar->begin(); iterRadar != pJetRadar->end(); ++iterRadar) {
			// 해당 ui가 레이더 리스트 안에 있는지 확인
			if ((*uiIter)->getTargetFighterPtr() == (*iterRadar)) {
				// 있다면 탈출
				isInRadar = true;
				break;
			}
		}
		if (isInRadar == false) { // 레이더 리스트 안에 없다면 삭제
			delete (*uiIter);
			uiIter = radarLockonUIList.erase(uiIter);
		}
		else {
			++uiIter;
		}
	}
	// 레이더 락온 리스트 업데이트
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

	REAL sessionLeftTime = static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime(); // 남은 세션 시간을 받아옴

	// 세션 비 만료 시
	if (sessionLeftTime > 0.00) {
		if (pOwnPlayer->getPlayerJet()->getState() == FlightObject::object_state::STATE_ALIVE) { // 전투기가 생존해 있을 때만 그리는 UI
			/*** UI들 드로우 리스트에 추가 ***/
			/** 레이더 락온 리스트 **/
			for (list<RadarLockon*>::iterator iter = radarLockonUIList.begin(); iter != radarLockonUIList.end(); ++iter) {
				pUIDrawList->push_back(*iter);
			}

			/** 상태 시스템 **/
			pUIDrawList->push_back(pStateSys);

			/** 속도와 고도계 **/
			pUIDrawList->push_back(pSpeedAltMeter);

			/** 레이더 맵 **/
			pUIDrawList->push_back(pRadarMap);

			/** 경고 시스템 **/
			pUIDrawList->push_back(pWarningSys);

			/** 무기 락온 UI **/
			// 주무장(기관총)
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
			// 주무장(미사일)
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
		else { // 전투기가 생존해 있지 않을 때 그리기에 추가하는 UI
			pUIDrawList->push_back(pRespawnUI);
		}

		// 세션 비 만료 시 그리기에 추가하는 UI
		/** 스코어 보드 **/
		pUIDrawList->push_back(pScoreBoard);
	}

	// 항상 그리기에 추가하는 UI (미션 메시지, 스크린 이펙트)
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
		// 카메라 뒤편에 있으면 그리지 않음 (카메라의 보는 방향과 오브젝트와 카메라의 위치를 뺀 방향벡터를 내적하여 > 0 인 경우)
		if (Vector3::dot(pCam->getViewDirection(), (pCam->getPosition() - this->getPos())) > 0) {
			return true;
		}
		// 카메라 앞에 있으면 그리기 작업 시작 ( t < 0 )
		else {
			// 스크린 좌표로 변환
			Size2D *resolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
			Vector3 screenPos = Vector3(getPosX(), getPosY(), getPosZ());
			screenPos = mymath::Vecor3Transform(screenPos, pCam->getViewMatrix());
			screenPos = mymath::Vecor3Transform(screenPos, pCam->getProjectionMatrix());
			screenPos.x = screenPos.x / (screenPos.z == 0 ? 0.0001 : screenPos.z);
			screenPos.y = screenPos.y / (screenPos.z == 0 ? 0.0001 : screenPos.z);
			screenPos.x = (screenPos.x + 1) * resolution->getWidth() / 2;
			screenPos.y = (screenPos.y + 1) * resolution->getHeight() / 2;

			// 스크린 범위에서 심하게 벗어나면 해당 방면에 있다는 것을 화살표로 표시하기 위한 준비
			bool bOutRightside = false, bOutLeftside = false;
			bool bOutTopside = false, bOutDownside = false;
			bool bOutbound = false;
			if (screenPos.x > resolution->getWidth() + 50.0) { // 우
				screenPos.x = resolution->getWidth() - 50.0;
				bOutRightside = true;
			}
			if (screenPos.x < -50.0) { // 좌
				screenPos.x = 50.0;
				bOutLeftside = true;
			}
			if (screenPos.y > resolution->getHeight() + 50.0) { // 상
				screenPos.y = resolution->getHeight() - 50.0;
				bOutTopside = true;
			}
			if (screenPos.y < -50.0) { // 하
				screenPos.y = 50.0;
				bOutDownside = true;
			}
			bOutbound = (bOutRightside || bOutLeftside || bOutTopside || bOutDownside);

			// 적절한 스크린 좌표로 이동
			glTranslatef(screenPos.x, screenPos.y, 0.0);
			
			// 벗어난 방향에 따라 화살표를 로테이션
			if (bOutbound == true) {
				if (bOutRightside == true && bOutTopside == true) // 우측 상향
					glRotatef(45.0, 0.0, 0.0, -1.0);
				else if (bOutRightside == true && bOutDownside == true) // 우측 하향
					glRotatef(135.0, 0.0, 0.0, -1.0);
				else if (bOutRightside == true) // 우향
					glRotatef(90.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true && bOutTopside == true) // 좌측 상향
					glRotatef(315.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true && bOutDownside == true) // 좌측 하향
					glRotatef(225.0, 0.0, 0.0, -1.0);
				else if (bOutLeftside == true)
					glRotatef(270.0, 0.0, 0.0, -1.0);
				else if (bOutDownside == true)
					glRotatef(180.0, 0.0, 0.0, -1.0);
			}

			// 모델 색상 설정
			Vector3 uiColor;
			if (ownerFighter->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // 미사일로 락온된 상태면 적색
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				if (lockedByWeapon == true) { // 무기로 락온된 상태라면
					uiColor = ui::GetSavedUIColor(ui::red);
				}
				else { // 무기로 락온되지 않은 상태라면
					if (type == RadarLockonType::ally) uiColor = ui::GetSavedUIColor(ui::blue);
					else if (type == RadarLockonType::enemy) uiColor = ui::GetSavedUIColor(ui::cyan);
				}
			}
			glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65); // 적군일 경우 청록색으로 그림
			glScalef(uiScale, uiScale, uiScale);

			// 모델 그리기(사각형)
			static const float size = 50.0; // 크기
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, bOutbound == false ? textureID : arrowTextureID); // 주의!: glBindTexture() 는 항상 glBegin() 이전에 호출되어야 한다!
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

			// 화살표로 표시할 때 출력 옵션 설정
			if (bOutbound == true) {
				// 구석에 출력되는 경우는 나머지 정보 출력 생략
				if (bOutRightside == true && bOutTopside == true) return true;
				else if (bOutRightside == true && bOutDownside == true) return true;
				else if (bOutLeftside == true && bOutTopside == true) return true;
				else if (bOutLeftside == true && bOutDownside == true) return true;
				// 아래쪽 출력의 경우는 회전하여 올바른 메시지가 출력되도록 함
				else if (bOutDownside == true) glRotatef(180.0, 0.0, 0.0, 1.0);
			}

			// 거리를 숫자로 표현(소숫점 내림)
			int dist = (ownerFighter->getPos() - targetFighter->getPos()).magnitude(); // 두 비행체 사이의 거리
			if (dist < 0 || dist > 100000000) dist = 0;
			char distChar[10];	// 최대 9999999999(99억) 까지 저장 가능
			itoa(dist, distChar, 10); // 정수를 10진수 아스키로 치환
			GLTextureFont distFont(distChar, 8.5);
			glTranslatef(60.0, -20.0, 0.0);
			distFont.drawText();

			// 전투기를 문자로 표현
			string *pName = targetFighter->getCraftNamePtr();
			if (pName != NULL) {
				GLTextureFont nameFont(*pName, 7.0);
				glTranslatef(0.0, 40.0, 0.0);
				nameFont.drawText();
			}

			// 아이디를 문자로 표현
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
	// 미사일 락온 상태를 아님으로 설정
	lockedByWeapon = false;

	// 타겟 비행체의 위치를 가져와서 자신의 위치로 동기화
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
		// 해상도를 가져옴
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// 색상 지정 (락온 당하지 않은 상태라면 녹색으로, 미사일 추적 중 일 때는 적색으로)
		Vector3 uiColor;
		REAL dmgBlinkTime = pUIMaster->getDmgBlinkTime();
		if (dmgBlinkTime <= 0.00) { // 최근에 받은 피해가 없으면 상태에 따른 기본 색상 적용
			if (pTargetJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) {
				uiColor = ui::GetSavedUIColor(ui::red);
			}
			else {
				uiColor = ui::GetSavedUIColor(ui::cyan);
			}
		}
		else { // 최근에 받은 피해가 있으면 남은 시간에 따라 색상 점멸
			if ((int)dmgBlinkTime % 100 == 0) uiColor = ui::GetSavedUIColor(ui::cyan);
			else uiColor = ui::GetSavedUIColor(ui::red);
		}
		glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65);

		// 속도계
		glPushMatrix();
		{
			// 네모1
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

			// SPEED 문자
			glPushMatrix();
			{
				GLTextureFont speedMeterText("SPEED", 10.0);
				glTranslatef(scrWidth * 0.238, scrHeight * 0.530, 0.0);
				speedMeterText.drawText();
			}
			glPopMatrix();

			// SPEED 숫자
			glPushMatrix();
			{
				GLTextureFont speedText("", 10.0);
				char speedStr[6]; // 출력 가능한 최대 속도는 999999
				itoa(speedMeter, speedStr, 10);
				speedText.setText(speedStr);
				glTranslatef(scrWidth * 0.24, scrHeight * 0.470, 0.0);
				speedText.drawText();
			}
			glPopMatrix();

			// BREAK 알람과 BOOST 알람
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

		// 고도계
		glPushMatrix();
		{
			// 네모2
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

			// ALT 문자
			glPushMatrix();
			{
				GLTextureFont speedMeterText("ALT", 10.0);
				glTranslatef(scrWidth * 0.705, scrHeight * 0.530, 0.0);
				speedMeterText.drawText();
			}
			glPopMatrix();

			// ALT 숫자
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
	// 타겟 전투기 갱신
	pTargetJet = pUIMaster->getOwnPlayerPtr()->getPlayerJet();

	// 속도와 고도 정보를 전투기로부터 갱신
	if (pTargetJet != NULL) {
		setSpeed(pTargetJet->currentSpeed);	// 속도
		setAlt(pTargetJet->getPosY());		// 고도
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
		// 해상도를 가져옴
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// 플레이어의 전투기를 받아 옴
		JetFighter *pOwnerJet = pPlayer->getPlayerJet();
		
		// 레이더 맵 그리기
		glPushMatrix();
		{
			// 색상 설정1
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

			// XZ 좌표 출력
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha - 0.1);
			glPushMatrix();
			{
				char jetCoord[16];
				GLTextureFont jetCoordText(string(""), 7.0);
				glPushMatrix();	// X축
				{
					sprintf(jetCoord, "X: %d", (int)pOwnerJet->getPosX());
					jetCoordText.setText(jetCoord);
					glTranslatef(scrWidth * 0.79, scrHeight * 0.40, 0.0); 
					jetCoordText.drawText();
				}
				glPopMatrix();
				glPushMatrix();	// Y(Z)축
				{
					sprintf(jetCoord, "Y: %d", (int)-pOwnerJet->getPosZ());
					jetCoordText.setText(jetCoord);
					glTranslatef(scrWidth * 0.89, scrHeight * 0.40, 0.0);
					jetCoordText.drawText();
				}
				glPopMatrix();
			}
			glPopMatrix();

			// 배경
			glColor4f(uiColor.x - 0.5, uiColor.y - 0.5, uiColor.z - 0.5, baseAlpha - 0.5);
			static const float mapWidth = 200.0;
			static const float mapHeight = 200.0;
			float width = scrWidth * mapWidth / 1920;
			float height = scrHeight * mapHeight / 1080;
			glTranslatef(scrWidth * 0.89, scrHeight * 0.195, 0.0); // 레이더 맵 UI의 화면상 위치
			glBegin(GL_QUADS);
			{
				 glVertex3f(-width, height, 0.0);
				 glVertex3f(-width, -height, 0.0);
				 glVertex3f(width, -height, 0.0);
				 glVertex3f(width, height, 0.0);
			}
			glEnd();

			// 틀
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

			// 스텐실 버퍼 설정
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

			// 스텐실 버퍼 사용
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glEnable(GL_STENCIL_TEST);
			{
				glStencilFunc(GL_EQUAL, 1, 0xffffffff);
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				
				// 격자
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

				// 전투기 및 미사일
				if (pFlightObjList != NULL) {
					drawObjIcon(pFlightObjList, mapWidth, mapHeight);
				}

				// 레이더 범위
				REAL radarMaxRad = pOwnerJet->getRadarRadius();
				glPushMatrix();
				{
					glColor4f(uiColor.x, uiColor.y, uiColor.z, (0.2 / (currentScanDist / radarMaxRad)));

					glTranslatef(((pOwnerJet->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0,
								 -((pOwnerJet->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // 전투기 위치로 좌표 이동

					// 그리기
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
			// 스텐실 버퍼 사용 종료
			glDisable(GL_STENCIL_TEST);

			// 스텐실 버퍼 초기화
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
		// 소유주 전투기와 레이더 반경 정보
		GameFrame *pFrame = g_pGameManager->getGameFramePtr();
		JetFighter *pJet = pPlayer->getPlayerJet();
		REAL radarRad = pJet->getRadarRadius();
		// 소유주 플레이어 갱신
		pPlayer = pFrame->getOwnerPlayerPtr();
		// 플레이어 소유의 전투기로부터 레이더 리스트를 불러옴
		pFlightObjList = static_cast<CombatFrame*>(pFrame)->getFlightObjListPtr();

		// 스캔 범위 업데이트
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
	list<FlightObject*> ownerFlightObj;								// 소유주의 미사일, 비행체는 가장 마지막에 그려주기 위한 별도의 리스트
	list<FlightObject*>::iterator iterFlight = pObjList->begin();	// 프레임으로부터 받아온 비행체 리스트
	REAL crRed = 0.0, crGreen = 0.0, crBlue = 0.0, crAlpha = 0.0;	// 색상 정보

	// 해상도를 가져옴
	REAL scrWidth = pUIMaster->getScreenWidth();
	REAL scrHeight = pUIMaster->getScreenHeight();

	// 모든 비행체 리스트를 돌면서 자기 소유의 비행체는 별도의 리스트에, 적 소유의 비행체는 아이콘 그리기 작업을 개시
	for (; iterFlight != pObjList->end(); ++iterFlight) { // begin for1
		FlightObject *pObj = (*iterFlight);
		glPushMatrix();
		{
			if (pObj->getState() == FlightObject::object_state::STATE_ALIVE) { // 살아 있는 상태일 때만 그림
				if (pObj->pOwningPlayer == pPlayer) { // 본인 비행체는 별도의 리스트에 담아서 가장 마지막에 그리기 수행
					ownerFlightObj.push_back(pObj);
				}
				else { // begin else
					crRed = 1.0; crGreen = 0.0; crBlue = 0.0; crAlpha = 0.9;
					glColor4f(crRed, crGreen, crBlue, crAlpha);

					// 미니맵 아이콘 그리기
					static float iconWidth = (pObj->getWidgetType() == GameWidget::WIDGET_JET ? 10.0 : 5.0);
					static float iconHeight = iconWidth;
					float width = scrWidth * iconWidth / 1920;
					float height = scrHeight * iconHeight / 1080;
					float scaledMapWidth = 200.0;
					float scaledMapHeight = 200.0;
					glTranslatef(((pObj->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0, -((pObj->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // 비행체의 위치로 좌표 이동
					glRotatef(pObj->yaw - 90.0, 0.0, 0.0, 1.0);	// 바라보는 방향(yaw)에 따른 아이콘 회전
					glEnable(GL_TEXTURE_2D);
					if (pObj->getWidgetType() == GameWidget::WIDGET_JET) { // 전투기
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
					else if (pObj->getWidgetType() == GameWidget::WIDGET_MISSILE) { // 미사일
						if (static_cast<Missile*>(pObj)->getStyle() != true) { // 총알은 그리기 제외
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

	// 자기 소유의 비행체들을 미니맵에 그리기 위해 위와 반복 작업을 해준다. (일관성이 떨어지는 방법으로, 추후 수정 요함)
	for (iterFlight = ownerFlightObj.begin(); iterFlight != ownerFlightObj.end(); ++iterFlight) { // begin for2
		FlightObject *pObj = (*iterFlight);
		glPushMatrix();
		{
			crRed = 1.0; crGreen = 1.0; crBlue = 1.0; crAlpha = 0.9;
			glColor4f(crRed, crGreen, crBlue, crAlpha);

			// 미니맵 아이콘 그리기
			static float iconWidth = (pObj->getWidgetType() == GameWidget::WIDGET_JET ? 10.0 : 5.0);
			static float iconHeight = iconWidth;
			float width = scrWidth * iconWidth / 1920;
			float height = scrHeight * iconHeight / 1080;
			glTranslatef(((pObj->getPosX() / (mapWidth / 2.0)) * scrWidth) / 1920.0, -((pObj->getPosZ() / (mapHeight / 2.0)) * scrHeight) / 1080.0, 0.0); // 비행체의 위치로 좌표 이동
			glRotatef(pObj->yaw - 90.0, 0.0, 0.0, 1.0);	// 바라보는 방향(yaw)에 따른 아이콘 회전
			glEnable(GL_TEXTURE_2D);
			if (pObj->getWidgetType() == GameWidget::WIDGET_JET) { // 전투기
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
			else if (pObj->getWidgetType() == GameWidget::WIDGET_MISSILE) { // 미사일
				if (static_cast<Missile*>(pObj)->getStyle() != true) { // 총알은 그리기 제외
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

		// 스크린 좌표로 변환
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
		POINT screenPos = { getPosX(), getPosY() };

		if (screenPos.x > pResolution->getWidth() + 100.0 || screenPos.y > pResolution->getHeight() + 100.0 ||
			screenPos.x < -100.0 || screenPos.y < -100.0) // 스크린 범위에서 심하게 벗어나면 그리지 않음
		{
			return true;
		}

		glTranslatef(screenPos.x, screenPos.y, 0.0);

		// 모델 색상 설정
		Vector3 uiColor;
		if (isLockonSuccess == true) { // 무기로 락온이 완료된 상태이면
			uiColor = ui::GetSavedUIColor(ui::red);
		}
		else { // 무기 락온이 완료되지 않은 상태일 때
			if (pOwnerFighter->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) uiColor = ui::GetSavedUIColor(ui::red); // 적에게 락온된 경우
			else uiColor = ui::GetSavedUIColor(ui::cyan); // 보통 상태의 경우
		}
		glColor4f(uiColor.x, uiColor.y, uiColor.z, 0.65);
		glScalef(uiScale, uiScale, uiScale);

		// 모델 그리기(사각형)
		static const float size = 50.0; // 크기
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
		// 해상도를 가져옴
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// 상태 시스템 그리기
		glPushMatrix();
		{
			// 색상 설정1
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
			glTranslatef(scrWidth * 0.01, scrHeight * 0.4, 0.0); // UI의 화면상 시작 위치 (좌상단)

			// 아이디, 기체명
			GLTextureFont playerName("PILOT: " + pOwnerJet->pOwningPlayer->getPlayerName(), 5.0);
			GLTextureFont jetName("JET: " + *(pOwnerJet->getCraftNamePtr()), 5.0);
			glTranslatef(0.0, -scrHeight * 0.02, 0.0);
			playerName.drawText();
			glTranslatef(-scrWidth * 0.003, -scrHeight * 0.025, 0.0);
			jetName.drawText();

			// 전투기 이미지
			static const float jetSizeX = 105.0; // 너비
			static const float jetSizeY = 120.0; // 높이
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
				else { // 기본 상태에서는 DMG에 따라 색상 결정, 미사일 추적 상태에서는 무조건 적색으로 설정
					uiColor2.x = ((1.00 - 0.33) * dmgPercent) / 100.0 + 0.33;
					uiColor2.y = ((0.15 - 1.00) * dmgPercent) / 100.0 + 1.00;
					uiColor2.z = ((0.15 - 1.00) * dmgPercent) / 100.0 + 1.00;
					// red:	 rtVec.x = 1.00; rtVec.y = 0.15; rtVec.z = 0.15; break; // 빨강
					// cyan: rtVec.x = 0.33; rtVec.y = 1.00; rtVec.z = 1.00; break; // 청록
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

				// 받은 피해 (사각형)
				glPushMatrix();
				{
					glTranslatef(-scrWidth * 0.005, -scrHeight * 0.155, 0.0);
					glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);

					static const float rect1SizeX = 100.0; // 너비
					static const float rect1SizeY = 25.0; // 높이
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

				// 받은 피해
				glTranslatef(-scrWidth * 0.045, -scrHeight * 0.155, 0.0);
				char dmg[5];
				itoa(dmgPercent, dmg, 10);
				GLTextureFont getDmgTxt("DMG: " + string(dmg) + "%", 7.8);
				getDmgTxt.drawText();

				// 엔진 상태 (막대)
				static const float rect2SizeX = 110.0;	// 너비
				static const float rect2SizeY = 10.0;	// 높이
				width = scrWidth * rect2SizeX / 1920;
				height = scrHeight * rect2SizeY / 1080;

				GLTextureFont engPowerTxt(" ENG:", 5.0);
				GLTextureFont boostTxt("BOOST:", 5.0);
				glTranslatef(scrWidth * 0.1, scrHeight * 0.015, 0.0);
				engPowerTxt.drawText();

				glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);

				// (엔진 파워 막대)
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

				// (부스트 막대)
				static const float rect3SizeX = 52.5; // 너비
				static const float rect3SizeY = 10.0; // 높이
				width = scrWidth * rect3SizeX / 1920;
				height = scrHeight * rect3SizeY / 1080;

				glTranslatef(0.0, -scrHeight * 0.03, 0.0);

				if (pOwnerJet->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // 색상 지정
					uiColor2 = ui::GetSavedUIColor(ui::red);
				}
				else {
					uiColor2 = ui::GetSavedUIColor(ui::cyan);
				}

				glColor4f(uiColor2.x, uiColor2.y, uiColor2.z, baseAlpha);
				boostTxt.drawText();
				glTranslatef(-width, 0.0, 0.0);

				//  좌&우 부스트 게이지 (2회)
				for (int i = 0; i < 2; ++i) {
					glPushMatrix();
					{
						// 테두리
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

						// 채움
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

			// 무장 및 탄약 상태
			// 기본 무장 (기관총)
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.17, -scrHeight * 0.05, 0.0);
			static const float rect2SizeX = 50.0; // 너비
			static const float rect2SizeY = 25.0; // 높이
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

			// 첫번째 무장 (미사일)
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

			// 전투기 각도 정보
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
				if (value < -180.0 || value > 180.0) value = 0; // 가끔 언더플로 발생. 예방 차원의 연산으로 완전한 해결법은 아니다.
				itoa(value, valStr, 10);
				GLTextureFont rollTxt(" ROLL:  " + string(valStr), 5.0);
				rollTxt.drawText();
			}
			glPopMatrix();

			// 미사일 상태
			if (pFirstWpn != NULL) {
				int msslCnt = pFirstWpn->getWpnMissileCount();											// 한번에 장전할 수 있는 미사일의 수
				REAL msslDelayMax = pFirstWpn->getFirePerSecond();										// 미사일 재장전 속도
				vector<REAL> *pFireLeftTime = pOwnerJet->getFirstWeaponPtr()->getWpnFireLeftTimePtr();	// 미사일들의 남은 대기시간이 담긴 벡터

				glTranslatef(-scrWidth * 0.05, -scrHeight * 0.1, 0.0);
				float msslSizeX = 60.0 / msslCnt; // 너비 (4개 기준 => 15.0)
				msslSizeX > 15.0 ? msslSizeX = 15.0 : 0;
				const float msslSizeY = 65.0;	  // 높이
				width = scrWidth * msslSizeX / 1920;
				height = scrHeight * msslSizeY / 1080;

				for (int i = 0; i < msslCnt; ++i) {
					// 미사일 배경
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

					// 미사일 장전 현황 %를 덧칠하는 색으로 표현
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

					glTranslatef(scrWidth * (0.06 / msslCnt), 0.0, 0.0); // (4개 기준 => scrHeight * 0.015)
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
	// 정보를 표현할 전투기를 플레이어의 전투기로 항상 갱신
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
	pSndMssileAlert->pause();	// 자동 재생 취소
	pSndLockonWarning->pause();
	pSndMssileAlert->setChannelNum(pOwnerPlayer->getPlayerNumber());	// 사운드 채널 설정
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
	if (isDestory == true) { // 파괴됨 메시지가 설정되면 기존 메시지들을 강제로 갱신
		isMsgDestroy = isDestory;
		sysLeftDuration = time;
	}
	else { // 타격 메시지가 설정되면 기존 메시지가 파괴됨이 아닐 경우에만 강제로 갱신
		if (isMsgDestroy == false) {
			isMsgDestroy = isDestory;
			sysLeftDuration = time;
		}
		else {
			// 아무것도 하지 않음 . . .
		}
	}
}

bool WarningSys::draw() {
	if (isVisible != true || pOwnerJet == NULL || pOwnerPlayer == NULL) return false;

	// 해상도를 가져옴
	REAL scrWidth = pUIMaster->getScreenWidth();
	REAL scrHeight = pUIMaster->getScreenHeight();

	// 색상 지정
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

	// 풀업 알림
	if (pOwnerJet->getAltitudeWarningState() == true) {
		glPushMatrix();
		{
			static const int sizeX = 128.0; // 너비
			static const int sizeY = 48.0; // 높이
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

	// 코스 이탈, 스톨 알림과 엔진 과열 알림
	GLuint alertTextureID = 0;
	int sizeX = 128; // 너비
	int sizeY = 48; // 높이
	int width = scrWidth * sizeX / 1920;
	int height = scrHeight * sizeY / 1080;

	if (pOwnerJet->getOffCourseTime() > 0.00) { // 상황에 따른 텍스처 선택 (우선도: 오프코스 > 스톨 > 엔진 과열)
		alertTextureID = offCourseTexture;
	}
	else if (pOwnerJet->getStallWarningState() == true) {
		alertTextureID = stallTexture;
	}

	if (alertTextureID != 0) { // 그리기
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

	// 미사일과 락온 경보
	JetFighter::jet_warning warnType = pOwnerJet->getLockonWarningState();
	if (warnType != JetFighter::jet_warning::JET_LOCKON_NONE) {
		GLuint warningTexID = 0;

		switch (warnType) { // 색상 및 텍스처 지정
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
			static const int sizeX = 100.0; // 너비
			static const int sizeY = 32.0; // 높이
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

	// 파괴와 타격 알림
	REAL crAlpha = baseAlpha * (sysLeftDuration > 0.33 ? 1.00 : sysLeftDuration);
	REAL widthFx = (sysLeftDuration > 0.25 ? 1.00 : 1.5 - sysLeftDuration);
	REAL heightFx = (sysLeftDuration > 0.25 ? 1.00 : sysLeftDuration);
	if (sysLeftDuration > 0.00) {
		// 공용으로 표현되는 SYS 메시지
		GLuint hitDesTexID = 0;
		int sizeX = 50.0; // 너비
		int sizeY = 15.0; // 높이
		int width = (scrWidth * sizeX / 1920) * widthFx;
		int height = (scrHeight * sizeY / 1080) * heightFx;

		glPushMatrix(); // SYS 메시지
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

		if (isMsgDestroy == true) { // 파괴의 경우
			sizeX = 160.0; // 너비
			sizeY = 22.0; // 높이
			hitDesTexID = destroySysTexture; // 텍스처 번호
			
		}
		else { // 타격의 경우
			sizeX = 64.0; // 너비
			sizeY = 20.0; // 높이
			hitDesTexID = hitSysTexture; // 텍스처 번호
		}
		
		width = (scrWidth * sizeX / 1920) * widthFx;
		height = (scrHeight * sizeY / 1080) * heightFx;

		glPushMatrix(); // HIT or DESTROY 메시지
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
	// 플레이어 소유 전투기 갱신
	pOwnerJet = pOwnerPlayer->getPlayerJet();

	// 알람 표시 시간 감소
	if ((sysLeftDuration -= timeElapsed) < 0.0) {
		sysLeftDuration = 0.0;
		isMsgDestroy = false; // false 로 초기화 함으로써 다른 메시지의 입력을 허용 받음
	}

	// 상태에 따른 사운드 재생 및 일시 정지
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
	// 오류 검사
	if (isVisible == true && pOwnerPlayer != NULL) {
		string killerName;
		if (pKillerPlayer == NULL) // 자살 시
			killerName = "YOURSELF";
		else { // 타살 시
			killerName += "BY ";
			killerName += pKillerPlayer->getPlayerName();
		}
		// 사망 시 표현될 문구를 완성시킴
		string str("YOU'VE KILLED ");
		str += killerName + ".";

		// 색상 설정
		Vector3 uiColor = ui::GetSavedUIColor(ui::red);
		REAL uiBaseAlpha = 1.00;

		// 해상도를 받아 옴
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();
		GLTextureFont deadTxt(str, 32.5, "res/Textures/font/koverwatchFont.png");

		// 문자열 길이에 따른 보정
		REAL ptX = (scrWidth / 2.0) - deadTxt.findCenterOfTextPosX();

		// 처형자 아이디 및 사망 문구 출력
		glPushMatrix();
		{
			glColor4f(uiColor.x, uiColor.y, uiColor.z, uiBaseAlpha);
			glTranslatef(ptX, scrHeight * 0.6, 0.0);
			deadTxt.drawText();
		}
		glPopMatrix();

		// 남은 리스폰 시간 문구출력
		str = "";
		int leftTime = pOwnerPlayer->getPlayerJet()->getRespawnLeftTime(); // 해당 기체의 남은 리스폰 시간을 받아옴
		char leftTimeStr[4] = { "\0", };									// 최대 4자리 수의 부활 시간을 가질 수 있음
		itoa(leftTime + 1, leftTimeStr, 10);

		str = leftTimeStr; // 문자 배열을 string 클래스에 담음

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
	respawnElapsedTime += timeElapsed; // 시간 경과

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
		// 해상도를 불러옴
		REAL scrWidth = pUIMaster->getScreenWidth();
		REAL scrHeight = pUIMaster->getScreenHeight();

		// 색상 설정
		Vector3 uiColor;
		REAL baseAlpha = 0.65;
		if (pUIMaster->getOwnPlayerPtr()->getPlayerJet()->getLockonWarningState() == JetFighter::jet_warning::JET_LOCKON_BY_MISSILE) { // 미사일 락온 시
			uiColor = ui::GetSavedUIColor(ui::red);
		}
		else { // 그 외의 경우
			uiColor = ui::GetSavedUIColor(ui::cyan);
		}

		// 남은 시간 그리기	
		int sizeX = 45.0; // 네모 박스 크기 설정
		int sizeY = 20.0;
		int width = (scrWidth * sizeX / 1920);
		int height = (scrHeight * sizeY / 1080);

		glPushMatrix(); // TIME 메시지 및 남은 시간
		{
			// TIME 메시지
			glColor4f(uiColor.x, uiColor.y, uiColor.z, baseAlpha);
			glTranslatef(scrWidth * 0.01, scrHeight * 0.97, 0.0);

			GLTextureFont timeTxt("TIME", 6.0);
			timeTxt.drawText();

			// 남은 시간
			REAL leftTime = pFrame->getSessionLeftTime();
			char leftTimeChar[9] = { '0' };
			int leftMin = ((int)leftTime / 60);	// 분
			int leftSec = ((int)leftTime % 60);	// 초
			int leftMilSec = ((leftTime - (int)leftTime) * 100.0);	// 밀리초
			sprintf(leftTimeChar, "%02d:%02d:%02d", leftMin, leftSec, leftMilSec);

			// 밀리초
			string leftTimeStr = leftTimeChar;
			GLTextureFont leftTimeTxt(leftTimeStr, 6.0);
			glTranslatef(scrWidth * 0.05, 0.0, 0.0);
			leftTimeTxt.drawText();
		}
		glPopMatrix();
		glPushMatrix(); // 네모 박스
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

		// 스코어보드 그리기
		glPushMatrix(); // 스코어보드 정보
		{
			glTranslatef(scrWidth * 0.01, scrHeight * 0.93, 0.0);
			GLTextureFont infoTxt("RANK  ID   KILLS  SOCRE", 6.0);
			infoTxt.drawText();
		}
		glPopMatrix();

		int rank = 0;
		for (vector<Player*>::iterator iter = scoreVector.begin(); iter != scoreVector.end(); ++iter) {
			glPushMatrix(); // 스코어보드 플레이어 및 점수
			{
				Player *pPlayer = (*iter);
				if (pPlayer != NULL) {
					glTranslatef(scrWidth * 0.005, (scrHeight * (0.93 - (++rank * 0.025))), 0.0);

					char rankChar[64];
					sprintf(rankChar, "%2d. %8.8s     %3d     %5d", rank, pPlayer->getPlayerName().c_str(), pPlayer->getPlayerKills(), pPlayer->getPlayerScore());

					if (pPlayer->getPlayerJet()->getState() == FlightObject::object_state::STATE_DEAD) // 사망 시 색상 어둡게
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
	/* 스코어 벡터 업데이트 */
	scoreVector.clear(); // 스코어 벡터 초기화

	list<Player*> *pPlyaerList = pFrame->getPlayerListPtr(); // 프레임으로부터 플레이어 리스트를 받아 와서 벡터화시킴
	for (list<Player*>::iterator iter = pPlyaerList->begin(); iter != pPlyaerList->end(); ++iter) {
		scoreVector.push_back(*iter);
	}

	// 우선순위에 맞추어 정렬
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

		// 미션 시간 만료
		if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() <= 0.00) {
			// 미션 종료 출력
			if (compeleteMsgLeftTime > 0.0) {
				GLuint texID = 0;
				// 서버와 디스커넥팅되어 종료하는 경우
				if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isMultiplayMode() == true &&
					static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->isServerDisconnected() == true) {
					texID = serverDisconnectedTexID;
				}
				// 정상적으로 세션 종료 조건을 만족하여 종료되는 경우
				else {
					texID = missionCompleteTexID;
				}

				// 그리기 옵션
				sizeX = 50.0;
				sizeY = 15.0;
				widthFx = (compeleteMsgLeftTime > 0.25 ? 1.00 : 1.5 - compeleteMsgLeftTime);
				heightFx = (compeleteMsgLeftTime > 0.25 ? 1.00 : compeleteMsgLeftTime);
				width = (scrWidth * sizeX / 1920) * widthFx;
				height = (scrHeight * sizeY / 1080) * heightFx;
				baseAlpha = (compeleteMsgLeftTime > 0.25 ? 0.65 : compeleteMsgLeftTime);
				
				// SYS 메시지
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

				// MISSION START 메시지
				sizeX = 160.0; // 너비
				sizeY = 32.0; // 높이
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
			// 세션 파괴 직전 페이드 아웃
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
		// 미션 시작 출력
		else if (startMsgLeftTime > 0.0) {
			// SYS 메시지
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

			// MISSION START 메시지
			sizeX = 160.0; // 너비
			sizeY = 32.0; // 높이
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
	// 세션 비 만료시
	if (static_cast<CombatFrame*>(g_pGameManager->getGameFramePtr())->getSessionLeftTime() > 0.00) {
		// 미션 시작 메시지 출력
		if ((startMsgLeftTime -= timeElapsed) < 0.0) startMsgLeftTime = 0.0;
	}
	// 세션 만료시
	else {
		// 미션 컴플리트 메시지 출력
		if ((compeleteMsgLeftTime -= timeElapsed) < 0.0) compeleteMsgLeftTime = 0.0;
		
		// 메시지 출력 완료 후 세션 파괴 시간 카운트
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
	// 남은 스크린 이펙트들 파괴
	clearEffects();

	// 마스터 포인터를 NULL로
	pUIMaster = NULL;
}

void ScreenEffect::createEffect(REAL time, int effectID, ScrEffectAction action, GLuint texID) {
	// 이미 해당 effectID의 이펙트가 생성되어 있으면 지속 시간 갱신 후 종료
	for (int i = 0; i < scrEffectVector.size(); ++i) {
		if (scrEffectVector[i]->iEffectID == effectID) {
			scrEffectVector[i]->rLeftTime = time;
			return;
		}
	}

	// 추가된 적이 없다면 동적으로 생성
	ScrEffect *pEffect = new ScrEffect;

	// 동적 생성 성공 여부 확인
	if (pEffect != NULL) {
		// 이펙트 정보 설정
		pEffect->iEffectID = effectID;
		pEffect->enAction = action;
		pEffect->rLeftTime = time;
		pEffect->rValue = 0.00f;

		// 이펙트 액션 설정
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

		// 이펙트 벡터에 푸시백
		scrEffectVector.push_back(pEffect);
	}
	else {
		return;
	}
}

void ScreenEffect::clearEffects() {
	// 벡터를 돌면서 모든 동적 할당된 구조체를 파괴
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
	// 그리기 제외거나 오류가 아닌 경우
	if (isVisible == true && pUIMaster != NULL) {
		// 지역 변수
		Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
		REAL scrWidth = pResolution->getWidth(), scrHeight = pResolution->getHeight();

		glTranslatef(scrWidth * 0.5, scrHeight * 0.5, 0.0); // 스크린의 중앙으로 이동

		// 이펙트 벡터를 순회하면서
		for (vector<ScrEffect*>::iterator iter = scrEffectVector.begin(); iter != scrEffectVector.end(); ++iter) {
			ScrEffect *pEffect = (*iter);
			REAL rVal = 0.00;

			glPushMatrix();
			{
				switch (pEffect->enAction) {
				default: case ScreenEffect::ScrEffectAction::SCREFFECT_WARNGLOW:
					// 경고 글로우 색상 지정
					glColor4f(0.95, 0.05, 0.05, abs(sinf(pEffect->rValue)) * 0.65 + 0.35);  // 0.35 ~ 1.00 알파값으로 색상이 점멸
					rVal = abs(sinf(pEffect->rValue)) * 0.35 + 0.65;
					glScalef(rVal, rVal, 1.0); // 짧게 늘어지다 축소됨
					break;
				case ScreenEffect::ScrEffectAction::SCREFFECT_BOOST:
					// 부스트 글로우 색상 및 효과 지정
					glColor4f(1.00, 1.00, 1.00, 0.75 - (pEffect->rValue / 7.00));	// 0.75 -> 0.00 알파값으로 색상이 흐려짐
					rVal = abs(sinf(pEffect->rValue / 3.0)) * 0.1 + 0.5;
					glRotatef(sinf(pEffect->rValue * 45.0) * 0.125, 0.0, 0.0, 1.0); // 짧게 진동
					glScalef(rVal, rVal, 1.0); // 길게 늘어짐
					break;
				case ScreenEffect::ScrEffectAction::SCREENEFFECT_DUST:
					// 더스트 글로우 색상 지정
					glColor4f(0.01, 0.05, 0.03, 1.00 - (pEffect->rValue / 4.0));	// 1.00 -> 0.00 알파값으로 색상이 흐려짐
					rVal = abs(sinf(pEffect->rValue / 5.00)) * 0.3 + 0.60;
					glScalef(rVal, rVal, 1.0); // 천천히 늘어짐
					break;
				}

				// 그리기
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
	// 벡터를 순회하며 이펙트 지속 시간을 갱신시키고, 0.00초 이하의 수명을 가지고 있으면 동적 생성된 이펙트를 파괴시킴
	for (vector<ScrEffect*>::iterator iter = scrEffectVector.begin(); iter != scrEffectVector.end();) {
		if (!((((*iter)->rLeftTime) -= timeElapsed) > 0.00)) {
			delete (*iter);
			iter = scrEffectVector.erase(iter);
		}
		else {
			// 이팩트 내부 시간을 경과시킴
			(*iter)->rValue += timeElapsed;

			// 다음 이펙트 조사
			++iter;
		}
	}

	return true;
}
