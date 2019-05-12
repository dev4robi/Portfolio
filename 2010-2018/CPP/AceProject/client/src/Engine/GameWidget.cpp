#include "Engine/GameWidget.h"
#include "Engine/GameCamera.h"
#include "Engine/DrawManager.h"
#include "Engine/GameFrame.h"

GameWidget::GameWidget() : GameObject() {	
	position = Vector3(0.0, 0.0, 0.0);
	id = nextID++;
	createdTime = (DWORD)timeGetTime();
	widgetType = GameWidget::WIDGET_DEFAULT;
	isHiding = false;
	isVisible = true;
	is2D = false;
	textureID = 0;
}

GameWidget::GameWidget(Vector3 pos) : GameObject() {
	position = pos;
	id = nextID++;
	createdTime = (DWORD)timeGetTime();
	widgetType = GameWidget::WIDGET_DEFAULT;
	isHiding = false;
	isVisible = true;
	is2D = false;
	textureID = 0;
}

GameWidget::GameWidget(GameWidget& tempWidget) {
	id = tempWidget.id;
	createdTime = (DWORD)timeGetTime();
	widgetType = tempWidget.widgetType;
	isHiding = tempWidget.isHiding;
	isVisible = tempWidget.isVisible;
	is2D = tempWidget.is2D;
	setPos(tempWidget.getPos());
	textureID = tempWidget.textureID;
}

GameWidget::~GameWidget() {

}

ID GameWidget::getID() {
	return id;
}

DWORD GameWidget::getCreatedTime() {
	return createdTime;
}

void GameWidget::setHiding(bool hiding) {
	isHiding = hiding;
}

bool GameWidget::getHiding() {
	return isHiding;
}

void GameWidget::setVisible(bool visible) {
	isVisible = visible;
}

bool GameWidget::getVisible() {
	return isVisible;
}

void GameWidget::set2D(bool _2D_or_3D) {
	is2D = _2D_or_3D;
}

bool GameWidget::getDimension() {
	return is2D;
}

void GameWidget::setPos(Vector3 newPos) {
	position = newPos;
}

Vector3 GameWidget::getScreenPosition() {
	Vector3 rtScrPos;
	DrawManager *pDrawMgr = g_pGameManager->getDrawManagerPtr();
	Size2D *pResolution = pDrawMgr->getResolutionPtr();
	GameCamera *pCam = pDrawMgr->getCameraFromFrame();
	Vector3 scrPos = this->getPos();

	scrPos = mymath::Vecor3Transform(scrPos, pCam->getViewMatrix());
	scrPos = mymath::Vecor3Transform(scrPos, pCam->getProjectionMatrix());
	scrPos.x = scrPos.x / (scrPos.z == 0 ? 0.0001 : scrPos.z);
	scrPos.y = scrPos.y / (scrPos.z == 0 ? 0.0001 : scrPos.z);
	scrPos.x = (scrPos.x + 1) * pResolution->getWidth() / 2.0;
	scrPos.y = (scrPos.y + 1) * pResolution->getHeight() / 2.0;

	rtScrPos.x = scrPos.x;
	rtScrPos.y = scrPos.y;
	rtScrPos.z = 0.0;

	return rtScrPos;
}

void GameWidget::setPos(REAL x, REAL y, REAL z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void GameWidget::setPosX(REAL x) {
	position.x = x;
}

void GameWidget::setPosY(REAL y) {
	position.y = y;
}

void GameWidget::setPosZ(REAL z) {
	position.z = z;
}

void GameWidget::setTextureID(GLuint id) {
	textureID = id;
}

Vector3 GameWidget::getPos() {
	return position;
}

REAL GameWidget::getPosX() {
	return position.x;
}

REAL GameWidget::getPosY() {
	return position.y;
}

REAL GameWidget::getPosZ() {
	return position.z;
}

GLuint GameWidget::getTextureID() {
	return textureID;
}

void GameWidget::setWidgetType(int typeEnum) {
	widgetType = typeEnum;
}

int GameWidget::getWidgetType() {
	return widgetType;
}

bool GameWidget::draw() {
	// 가상 함수로 여기에서는 아무 작업도 하지 않는다.
	// GameWidget을 상속하는 다른 위젯들은 여기에 그리기 작업을 정의하고 true를 반환한다.
	return true;
}

bool GameWidget::destroy() {
	// 가상 함수로 여기에서는 아무 작업을 하지 않는다.
	// GameWidget을 상속하는 다른 위젯들은 여기에 파괴 작업을 정의하고 true를 반환한다.
	return true;
}

bool GameWidget::update(REAL timeElapsed) {
	// 가상 함수로 여기에서는 아무 작업을 하지 않는다.
	// GameWidget을 상속하는 다른 위젯들은 여기에 파괴 업데이트 작업을 정의하고 true를 반환한다.
	return true;
}