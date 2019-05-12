#include "Game/UI/Viewer/RectViewer.h"
#include "Engine/DrawManager.h"
#include "Library/GLTextureFont.h"

RectViewer::RectViewer(Vector3 pos, SIZE size, GLuint texID) : GameWidget(pos) {
	is2D = true;
	textureID = texID;
	rElapsedTime = 0.00;
	vecOffset = Vector3(0.0, 0.0, 0.0);
	vecRotate = Vector3(0.0, 0.0, 0.0);
	szSize.cx = size.cx;
	szSize.cy = size.cy;
	vecScale3 = Vector3(1.0, 1.0, 1.0);
	bPermanency = true;
	rLife = 1.00;
	vecColor3 = Vector3(1.0, 1.0, 1.0);
	rColorAlpha = 1.0;
	rTextColorAlpha = 1.0;
	rTextSize = 10.0;
	bTextAlignCenterX = false;
	rTextureX = rTextureY = 0.00f;
	iFont = 0;
	iTag = 0;
}

RectViewer::RectViewer(RectViewer &obj) : GameWidget(obj) {
	rElapsedTime = obj.rElapsedTime;
	vecOffset = obj.vecOffset;
	vecRotate = obj.vecRotate;
	szSize.cx = obj.szSize.cx;
	szSize.cy = obj.szSize.cy;
	vecScale3 = obj.vecScale3;
	bPermanency = obj.bPermanency;
	rLife = obj.rLife;
	vecColor3 = obj.vecColor3;
	rColorAlpha = obj.rColorAlpha;
	rTextSize = obj.rTextSize;
	bTextAlignCenterX = obj.bTextAlignCenterX;
	rTextureX = obj.rTextureX;
	rTextureY = obj.rTextureY;
	iFont = obj.iFont;
	iTag = obj.iTag;
}

RectViewer::~RectViewer() {

}

Vector3 RectViewer::getOffset() {
	return vecOffset;
}

void RectViewer::setOffset(Vector3 offset) {
	vecOffset = offset;
}

Vector3 RectViewer::getRotate() {
	return vecRotate;
}

void RectViewer::setRotate(Vector3 rotate) {
	vecRotate = rotate;
}

SIZE RectViewer::getSize() {
	return szSize;
}

void RectViewer::setSize(int width, int height) {
	szSize.cx = width;
	szSize.cy = height;
}

Vector3 RectViewer::getScale3() {
	return vecScale3;
}

void RectViewer::setScale3(REAL x, REAL y, REAL z) {
	vecScale3.x = x;
	vecScale3.y = y;
	vecScale3.z = z;
}

void RectViewer::setScale3(Vector3 scale) {
	vecScale3 = scale;
}

REAL RectViewer::getElapsedTime() {
	return rElapsedTime;
}

Vector3 RectViewer::getColor3() {
	return vecColor3;
}

void RectViewer::setColor3(REAL r, REAL g, REAL b) {
	vecColor3.x = r;
	vecColor3.y = g;
	vecColor3.z = b;
}

void RectViewer::setColor3(Vector3 rgbVec) {
	vecColor3 = rgbVec;
}

REAL RectViewer::getColorAlpha() {
	return rColorAlpha;
}

void RectViewer::setColorAlpha(REAL alpha) {
	rColorAlpha = alpha;
}

REAL RectViewer::getTextColorAlpha() {
	return rTextColorAlpha;
}

void RectViewer::setTextureXY(REAL x, REAL y) {
	rTextureX = x;
	rTextureY = y;
}

void RectViewer::setTextColorAlpha(REAL txtAlpha) {
	rTextColorAlpha = txtAlpha;
}

void RectViewer::setPermanency(bool permanency) {
	bPermanency = permanency;
}

bool RectViewer::getPermanency() {
	return bPermanency;
}

REAL RectViewer::getViwerLife() {
	return rLife;
}

void RectViewer::setViwerLife(REAL lifeTime) {
	rLife = lifeTime;
}

string RectViewer::getText() {
	return strText;
}

void RectViewer::setText(string txt) {
	strText = txt;
}

REAL RectViewer::getTextSize() {
	return rTextSize;
}

void RectViewer::setTextSize(REAL size) {
	rTextSize = size;
}

int RectViewer::getFontID() {
	return iFont;
}

void RectViewer::setFontID(int id) {
	iFont = id;
}

int RectViewer::getTagInt() {
	return iTag;
}

void RectViewer::setTagInt(int tag) {
	iTag = tag;
}

void RectViewer::setTextAlignCenterX(bool alignCenter) {
	bTextAlignCenterX = alignCenter;
}

void RectViewer::setTextAttributes(string txt, REAL size, bool showTextOnly, int fontID) {
	setText(txt);
	setTextSize(size);
	iFont = fontID;

	if (showTextOnly == true)
		setColorAlpha(0.0);
}

bool RectViewer::draw() {
	// 그리기 제외 조건 확인
	if (isVisible == false || isHiding == true) return false;

	// 해상도 로드
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	int scrWidth = pResolution->getWidth();
	int scrHeight = pResolution->getHeight();
	float rectWidth = (scrWidth * szSize.cx / 1920.0) / 2.0;
	float rectHeight = (scrHeight * szSize.cy / 1080.0) / 2.0;

	// 네모 그리기 (폭과 너비 중 하나가 0이거나 보다 작은 경우는 그리기 제외)
	if (szSize.cx > 0 && szSize.cy > 0) {
		glPushMatrix();
		{
			glColor4f(vecColor3.x, vecColor3.y, vecColor3.z, rColorAlpha);

			if (is2D == true)
				glTranslatef((position.x + vecOffset.x) * scrWidth, (position.y + vecOffset.y) * scrHeight, (position.z + vecOffset.z));
			else
				glTranslatef((position.x + vecOffset.x), (position.y + vecOffset.y), (position.z + vecOffset.z));

			glRotatef(vecRotate.z, 0.0, 0.0, 1.0);
			glRotatef(vecRotate.y, 0.0, 1.0, 0.0);
			glRotatef(vecRotate.x, 1.0, 0.0, 0.0);
			glScalef(vecScale3.x, vecScale3.y, vecScale3.z);

			if (textureID != 0) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0 + rTextureX, 1.0 + rTextureY); glVertex3f(-rectWidth, rectHeight, 0.0);
					glTexCoord2f(0.0 + rTextureX, 0.0 + rTextureY); glVertex3f(-rectWidth, -rectHeight, 0.0);
					glTexCoord2f(1.0 + rTextureX, 0.0 + rTextureY); glVertex3f(rectWidth, -rectHeight, 0.0);
					glTexCoord2f(1.0 + rTextureX, 1.0 + rTextureY); glVertex3f(rectWidth, rectHeight, 0.0);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else {
				glBegin(GL_QUADS);
				{
					glVertex3f(-rectWidth, rectHeight, 0.0);
					glVertex3f(-rectWidth, -rectHeight, 0.0);
					glVertex3f(rectWidth, -rectHeight, 0.0);
					glVertex3f(rectWidth, rectHeight, 0.0);
				}
				glEnd();
			}
		}
		glPopMatrix();
	}

	// 텍스트 그리기
	if (strText.size() != 0) {
		glPushMatrix();
		{
			glColor4f(vecColor3.x, vecColor3.y, vecColor3.z, rTextColorAlpha);

			if (is2D == true)
				glTranslatef(position.x * scrWidth, position.y * scrHeight, position.z);
			else
				glTranslatef(position.x, position.y, position.z);

			GLTextureFont txt(strText, rTextSize, iFont == 0 ? NULL : "res/Textures/font/koverwatchFont.png");

			glPushMatrix();
			{
				// 문자열 중앙 정렬 사용 시 추가 작업
				if (bTextAlignCenterX == true) {
					REAL p = txt.findCenterOfTextPosX();
					glTranslatef(-p, 0.0, 0.0);
				}

				// 문자 출력
				txt.drawText();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

	return true;
}

bool RectViewer::destroy() {
	return true;
}

bool RectViewer::update(REAL timeElapsed) {
	// 시간 경과
	rElapsedTime += timeElapsed;

	return true;
}
