#include "Game/Effect/GL_Effect.h"
#include "Engine/ResourceManager.h"
#include "Engine/GameFrame.h"
#include "Engine/GameCamera.h"
#include "Engine/DrawManager.h"
#include "Game/Object/FlightObject.h"

/*************************************************************************************************************/
/************************************************* GL_Effect *************************************************/
/*************************************************************************************************************/

GL_Effect::GL_Effect(Vector3 _center) : GameObject() {
	center = _center;
	visible = true;
	bDrawAfterWidget = false;
	pAttachTgt = NULL;
	attachOffset.x = attachOffset.y = attachOffset.z = 0.0;
	permanency = false;
	timeLeft = 1.0;
	scale = Vector3(1.0, 1.0, 1.0);
	for (int i = 0; i < 4; ++i) color[i] = 1.0;

	g_pGameManager->getGameFramePtr()->addEffect(this);
}

GL_Effect::GL_Effect(GL_Effect &obj) : GameObject(obj) {
	center = obj.center;
	visible = obj.visible;
	pAttachTgt = obj.pAttachTgt;
	attachOffset = obj.attachOffset;
	permanency = obj.permanency;
	timeLeft = obj.timeLeft;
	scale = obj.scale;
	for (int i = 0; i < 4; ++i) color[i] = obj.color[i];
}

GL_Effect::~GL_Effect() { 
	pAttachTgt = NULL;
}

void GL_Effect::setCenter(Vector3 _center) {
	center = _center;
}

Vector3 GL_Effect::getCenter() {
	return center;
}

void GL_Effect::setVisible(bool _visible) {
	visible = _visible;
}

bool GL_Effect::getVisible() {
	return visible;
}

void GL_Effect::setDrawAfterWidget(bool drawAfter) {
	bDrawAfterWidget = drawAfter;
}

bool GL_Effect::getDrawAfterWidget() {
	return bDrawAfterWidget;
}

void GL_Effect::setAttachTgt(FlightObject *pWidget) {
	pAttachTgt = pWidget;
}

FlightObject* GL_Effect::getAttachTgtPtr() {
	return pAttachTgt;
}

void GL_Effect::setAttachOffset(Vector3 offset) {
	attachOffset = offset;
}

Vector3 GL_Effect::getAttachOffset() {
	return attachOffset;
}

void GL_Effect::setPermanency(bool _permanency) {
	permanency = _permanency;
}

bool GL_Effect::getPermanency() {
	return permanency;
}

void GL_Effect::setScale(Vector3 newScale) {
	scale = newScale;
}

Vector3 GL_Effect::getScale() {
	return scale;
}

void GL_Effect::setColor(REAL rgba[4]) {
	for (int i = 0; i < 4; ++i) {
		color[i] = rgba[i];
	}
}

void GL_Effect::getColor(REAL colorAry[4]) {
	for (int i = 0; i < 4; ++i)
		colorAry[i] = color[i];
}

void GL_Effect::draw() { /* 가상 함수 . . . */ }

bool GL_Effect::update(REAL timeElapsed) { /* 가상 함수 . . . */ return true; }

/*************************************************************************************************************/
/************************************************ SmokeEffect ************************************************/
/*************************************************************************************************************/

SmokeEffect::SmokeEffect(Vector3 _center, REAL time, REAL speed, REAL base, REAL max, GLuint texID) : GL_Effect(_center) {
	timeLeft = time;
	spreadSpeed = speed;
	baseSize = base;
	maxSize = max;
	currentRotate = 0.0;
	rotateSpeed = 0.0;
	rotateLeft = false;

	if (texID == 0) 
		texture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/basicSmoke.png");
	else
		texture = texID;
}

SmokeEffect::SmokeEffect(SmokeEffect &obj) : GL_Effect(obj) {
	timeLeft = obj.timeLeft;
	baseSize = obj.baseSize;
	maxSize = obj.maxSize;
	spreadSpeed = obj.spreadSpeed;
	currentRotate = obj.currentRotate;
	rotateSpeed = obj.rotateSpeed;
	rotateLeft = obj.rotateLeft;
	texture = obj.texture;
}

SmokeEffect::~SmokeEffect() {

}

void SmokeEffect::setTexture(GLuint texID) {
	texture = texID;
}

void SmokeEffect::setRotationSpeed(REAL speed) {
	rotateSpeed = speed;
}

void SmokeEffect::setRotationDirection(bool isLeft) {
	rotateLeft = isLeft;
}

void SmokeEffect::draw() {
	if (visible == false) return;

	// 빌보드화 시키기 위해 카메라 포인터를 가져옴
	GameCamera *pCam = g_pGameManager->getDrawManagerPtr()->getCameraFromFrame();
	
	glDepthMask(GL_FALSE); // 깊이 테스트를 끄지 않고, 깊이를 기록하지 않는다
	if (pCam != NULL) {
		// 월드 좌표 이동
		glTranslatef(center.x, center.y, center.z);

		// 빌보드 회전 (항상 카메라를 바라봄) - (http://www.lighthouse3d.com/opengl/billboarding/index.php?billSphe)
		Vector3 camPos = pCam->getPosition();
		Vector3 lookAt, objToCamProj, objToCam, upAux;
		float angleCosine;

		// objToCamProj is the vector in world coordinates from the 
		// local origin to the camera projected in the XZ plane
		objToCamProj.x = camPos.x - center.z;
		objToCamProj.y = 0;
		objToCamProj.z = camPos.z - center.z;

		// This is the original lookAt vector for the object 
		// in world coordinates
		lookAt.x = 0;
		lookAt.y = 0;
		lookAt.z = 1;

		// normalize both vectors to get the cosine directly afterwards
		objToCamProj.normalize();

		// easy fix to determine wether the angle is negative or positive
		// for positive angles upAux will be a vector pointing in the 
		// positive y direction, otherwise upAux will point downwards
		// effectively reversing the rotation.

		upAux = Vector3::cross(lookAt, objToCamProj);

		// compute the angle
		angleCosine = Vector3::dot(lookAt, objToCamProj);

		// perform the rotation. The if statement is used for stability reasons
		// if the lookAt and objToCamProj vectors are too close together then 
		// |angleCosine| could be bigger than 1 due to lack of precision
		if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
			glRotatef(acos(angleCosine) * 180 / 3.14, upAux.x, upAux.y, upAux.z);

		// so far it is just like the cylindrical billboard. The code for the 
		// second rotation comes now
		// The second part tilts the object so that it faces the camera

		// objToCam is the vector in world coordinates from 
		// the local origin to the camera
		objToCam.x = camPos.x - center.x;
		objToCam.y = camPos.y - center.y;
		objToCam.z = camPos.z - center.z;

		// Normalize to get the cosine afterwards
		objToCam.normalize();

		// Compute the angle between objToCamProj and objToCam, 
		//i.e. compute the required angle for the lookup vector
		angleCosine = Vector3::dot(objToCamProj, objToCam);

		// Tilt the object. The test is done to prevent instability 
		// when objToCam and objToCamProj have a very small
		// angle between them

		if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
			if (objToCam.y < 0)
				glRotatef(acos(angleCosine) * 180 / 3.14, 1, 0, 0);
			else
				glRotatef(acos(angleCosine) * 180 / 3.14, -1, 0, 0);

		// 연기 회전효과
		if (rotateSpeed != 0.0) {
			if (rotateLeft == true)
				glRotatef(-currentRotate, 0.0, 0.0, 1.0);
			else
				glRotatef(currentRotate, 0.0, 0.0, 1.0);
		}

		// 스케일링
		glScalef(scale.x, scale.y, scale.z);

		// 색상 및 투명도
		glColor4f(color[0], color[1], color[2], color[3]);

		// 파티클 그리기
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0, 1.0); glVertex3f(-baseSize, baseSize, 0.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-baseSize, -baseSize, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(baseSize, -baseSize, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(baseSize, baseSize, 0.0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glDepthMask(GL_TRUE);
}

bool SmokeEffect::update(REAL timeElapsed) {
	timeLeft -= timeElapsed;

	// 시간이 남아 있으면 업데이트
	if (timeLeft > 0.0) {
		// 부착 대상이 있느 경우는 부착 대상의 좌표로 이동
		if (pAttachTgt != NULL) {
			Quaternion *q = &(pAttachTgt->transform);
			Matrix4 transMat = q->toMatrix4();

			center = pAttachTgt->getPos();
			center = center + (attachOffset * transMat);
		}

		baseSize += (spreadSpeed * timeElapsed); // 크기 확장
		if (baseSize > maxSize) baseSize = maxSize;
		
		color[3] = 1.0 - (baseSize / maxSize); // 색상 투명화
		
		currentRotate += (rotateSpeed * timeElapsed); // 회전
		if (currentRotate >= 360.0) currentRotate = mymath::degree360(currentRotate);
	}
	// 시간이 초과되면 삭제
	else {
		return false; // false를 반환하여 Frame 에서 삭제 요청
	}
}

/*************************************************************************************************************/
/************************************************ TrailEffect ************************************************/
/*************************************************************************************************************/

TrailEffect::TrailEffect(Vector3 _center, int _lengthMax, int texID) : GL_Effect(_center) {
	permanency = true;
	baseAlpha = 1.00;
	baseSize = 0.25;
	useTrailCreate = true;
	if (texID == 0) texture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/missileTrail.png");
	else texture = texID;
	lengthMax = _lengthMax;
}

TrailEffect::TrailEffect(TrailEffect &obj) : GL_Effect(obj) {
	baseSize = obj.baseSize;
	texture = obj.texture;
	lengthMax = obj.lengthMax;
}

TrailEffect::~TrailEffect() { 
	trailList.clear();
}

REAL TrailEffect::getBaseAlpha() {
	return baseAlpha;
}

void TrailEffect::setBaseAlpha(REAL alpha) {
	baseAlpha = alpha;
}

bool TrailEffect::getTrailCreatable() {
	return useTrailCreate;
}

void TrailEffect::setTrailCreatable(bool useCreate) {
	useTrailCreate = useCreate;
}

void TrailEffect::initTrail() {
	trailList.clear();

}

void TrailEffect::draw() {
	if (visible == false) return;

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_TRUE);

	// 스케일링
	glScalef(scale.x, scale.y, scale.z);

	// 트레일 그리기
	Vector3 firstPos = center, secondPos; // 트라이앵글 스트립의 1, 2 번 좌표(firstPos)와 3, 4번 좌표(secondPos)	// 1 3  ↓↓
	Trail *pTrail = NULL;																							// 2 4  ↗↓
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	for (list<Trail>::iterator iterTrail = trailList.begin(); iterTrail != trailList.end(); ++iterTrail) { // 길이가 1 이상일 때만 그리기 수행됨
		pTrail = &(*iterTrail);
		secondPos = pTrail->pos;

		// 색상 및 투명도
		glColor4f(color[0], color[1], color[2], pTrail->colorAlpha * baseAlpha);

		// 좌표 이동
		glPushMatrix();
		glTranslatef(firstPos.x, firstPos.y, firstPos.z);

		glBegin(GL_TRIANGLE_STRIP);
		{
			// 트레일 세로
			glTexCoord2f(0.0, 1.0);	glVertex3f(0.0, +baseSize, 0.0);
			glTexCoord2f(0.0, 0.0);	glVertex3f(0.0, -baseSize, 0.0);
			glTexCoord2f(1.0, 1.0);	glVertex3f(secondPos.x - firstPos.x, secondPos.y - firstPos.y + baseSize, secondPos.z - firstPos.z);
			glTexCoord2f(1.0, 0.0);	glVertex3f(secondPos.x - firstPos.x, secondPos.y - firstPos.y - baseSize, secondPos.z - firstPos.z);
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		{
			// 트레일 가로
			glTexCoord2f(0.0, 1.0);	glVertex3f(0.0, 0.0, -baseSize);
			glTexCoord2f(0.0, 0.0);	glVertex3f(0.0, 0.0, +baseSize);
			glTexCoord2f(1.0, 1.0);	glVertex3f(secondPos.x - firstPos.x, secondPos.y - firstPos.y, secondPos.z - firstPos.z - baseSize);
			glTexCoord2f(1.0, 0.0);	glVertex3f(secondPos.x - firstPos.x, secondPos.y - firstPos.y, secondPos.z - firstPos.z + baseSize);
		}
		glEnd();

		glPopMatrix();

		firstPos = secondPos; // 다음 트레일의 좌표를 위해 저장
	}
	
	// 원본 식 주석으로 백업
	//glTexCoord2f(0.0, 1.0);	glVertex3f(firstPos.x, firstPos.y + baseSize, firstPos.z);
	//glTexCoord2f(0.0, 0.0);	glVertex3f(firstPos.x, firstPos.y - baseSize, firstPos.z);
	//glTexCoord2f(1.0, 1.0);	glVertex3f(secondPos.x, secondPos.y + baseSize, secondPos.z);
	//glTexCoord2f(1.0, 0.0);	glVertex3f(secondPos.x, secondPos.y - baseSize, secondPos.z);

	pTrail = NULL;
	glDisable(GL_TEXTURE_2D);
	glDepthFunc(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
}

bool TrailEffect::update(REAL timeElapsed) {
	// 이펙트 수명 확인
	if (permanency == false) { // 영구적 속성이 아니고
		if ((timeLeft -= timeElapsed) <= 0.0 && trailList.size() <= 0) { // 수명이 다 했고, 트레일이 없다면 파괴
			return false; // 삭제 요청
		}
	}

	// 부착 대상의 오프셋 위치로 이동
	if (pAttachTgt != NULL) {
		Quaternion *q = &(pAttachTgt->transform);
		Matrix4 transMat = q->toMatrix4();

		center = pAttachTgt->getPos();
		center = center + (attachOffset * transMat);
	}

	// 트레일 추가 확인
	if (lengthMax > trailList.size() && timeLeft > 0.0) { // 추가할 공간이 남아 있고 이펙트가 생존해 있다면
		if (useTrailCreate == true) { // 트레일 추가를 사용한 경우인지를 추가적으로 확인
			Trail newTrail = { center, color[3] };
			trailList.push_front(newTrail); // 새로 추가될 블럭을 앞에 넣음에 유의
		}
	}
	else {
		if (trailList.size() > 0) trailList.pop_back(); // 가장 뒤의 원소 제거
	}

	// 트레일 업데이트
	int cnt = 0;
	Trail *pTrail = NULL;
	list<Trail>::iterator iterTrail = trailList.begin();
	for (; iterTrail != trailList.end(); ++iterTrail) { // 트레일들을 선택하면서
		pTrail = &(*iterTrail);

		// 투명도 업데이트
		if (pTrail != NULL) {
			if (trailList.size() > 0) {
				pTrail->colorAlpha = (1.00 - ((REAL)cnt / trailList.size()));
				++cnt;
			}
		}
	}

	return true;
}

/*************************************************************************************************************/
/********************************************** JetFlameEffect ***********************************************/
/*************************************************************************************************************/

JetFlameEffect::JetFlameEffect(Vector3 _center, REAL _lengthEachCnt, int innerCnt, int outerCnt) : GL_Effect(_center) {
	permanency = true;
	bDrawAfterWidget = true;
	transform.identity();
	glowTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/glow.png");
	blowTexture = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/jetBlow.png");
	currentLoopTime = 0.0;
	loopingTimeBase = 0.5;
	lengthEachCnt = _lengthEachCnt;
	innerBlowLengthCnt = innerCnt;
	outerBlowLengthCnt = outerCnt;
	glowColor = Vector3(0.9, 0.9, 0.9);
}

JetFlameEffect::JetFlameEffect(JetFlameEffect &obj) : GL_Effect(obj) {
	transform = obj.transform;
	glowTexture = obj.glowTexture;
	blowTexture = obj.blowTexture;
	currentLoopTime = obj.currentLoopTime;
	loopingTimeBase = obj.loopingTimeBase;
	lengthEachCnt = obj.lengthEachCnt;
	innerBlowLengthCnt = obj.innerBlowLengthCnt;
	outerBlowLengthCnt = obj.outerBlowLengthCnt;
	glowColor = obj.glowColor;
}

JetFlameEffect::~JetFlameEffect() { }

void JetFlameEffect::setBaseAlpha(REAL alpha) {
	if (alpha < 0.0) alpha = 0.0;
	else if (alpha > 1.0) alpha = 1.0;
	
	color[3] = alpha;
}

REAL JetFlameEffect::getBaseAlpha() {
	return color[3];
}

void JetFlameEffect::setBlowColor(bool isOutter, Vector3 color) {
	if (isOutter == true)
		outterColor = color;
	else
		innerColor = color;
}

Vector3 JetFlameEffect::getBlowColor(bool isOutter) {
	if (isOutter == true)
		return outterColor;
	else
		return innerColor;
}

void JetFlameEffect::setGlowColor(Vector3 color) {
	glowColor = color;
}

void JetFlameEffect::draw() {
	if (visible == false) return;
	if (pAttachTgt != NULL)	{ if (pAttachTgt->getState() != FlightObject::object_state::STATE_ALIVE) return; }

	// 월드 좌표 이동
	glTranslatef(center.x, center.y, center.z);
	
	// 로테이션
	glMultMatrixf(&transform.toMatrix4()[0][0]);

	// 스케일링
	glScalef(scale.x, scale.y, scale.z);
	
	// 글로우 이펙트 그리기
	REAL loopingPercent = (currentLoopTime / loopingTimeBase);
	REAL size = 0.3; // 기본 크기
	REAL glowScale = (1.0 - sinf((loopingPercent * mymath::PI * 4.0)) * 0.05);

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, glowTexture);
		glScalef(1.0, glowScale, glowScale);
		glColor4f(color[0] * glowColor.x, color[1] * glowColor.y, color[2] * glowColor.z, color[3]); // 색상 적용
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0, size, -size);
			glTexCoord2f(0.0, 0.0); glVertex3f(0.0, -size, -size);
			glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -size, size);
			glTexCoord2f(1.0, 1.0); glVertex3f(0.0, size, size);
		}
		glEnd();
	}
	glPopMatrix();

	// 블로우 이펙트 그리기
	glDisable(GL_DEPTH_TEST);
	size *= 0.85;
	static const int n = 12;				// 디폴트 십이각형
	REAL radDt = (mymath::PI * 2.0) / n;	// 한 번에 이동해야 할 각도 
	int innerOuterCnt = 0;
	REAL texOffset = loopingPercent;		// 텍스처 오프셋 좌표
	Vector3 *pColorVec;						// RGB 색상 벡터 포인터

	for (int k = 0; k < 2; ++k) {
		if (k == 1) { // k 가 1일 때는 밖쪽 블로우, 그 외일 때는 안쪽 블로우를 그리도록 함
			innerOuterCnt = outerBlowLengthCnt;
			pColorVec = &(outterColor);
		}
		else {
			innerOuterCnt = innerBlowLengthCnt;
			pColorVec = &(innerColor); 
		}

		for (int j = 0; j < innerOuterCnt; ++j) {
			for (REAL i = radDt; i < (mymath::PI * 2.0); i += radDt) {
				REAL topX = (-lengthEachCnt * j);
				REAL botX = (topX - lengthEachCnt);
				REAL Y1 = (size * sinf(i));
				REAL Y2 = (size * sinf(i + radDt));
				REAL Z1 = size * cosf(i);
				REAL Z2 = (size * cosf(i + radDt));

				// 색상 적용
				glColor4f(color[0] * pColorVec->x, color[1] * pColorVec->y, color[2] * pColorVec->z, color[3] * (1.0 - (REAL)j / innerOuterCnt));
				glBindTexture(GL_TEXTURE_2D, blowTexture);

				// n 각 기둥 그리기
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0 - texOffset, 0.0); glVertex3f(topX, Y1, Z1);	// 1  4  순으로 그림
					glTexCoord2f(1.0 - texOffset, 0.0); glVertex3f(botX, Y1, Z1);	// 2  3
					glTexCoord2f(1.0 - texOffset, 1.0); glVertex3f(botX, Y2, Z2);
					glTexCoord2f(0.0 - texOffset, 1.0); glVertex3f(topX, Y2, Z2);
				}
				glEnd();
			}

			size -= 0.01;
		}

		// 밖쪽 기둥은 조금 더 큰 크기로 그리기
		size = size * 1.33;
	}
}

bool JetFlameEffect::update(REAL timeElapsed) {
	// 이펙트 수명 확인
	if (permanency == false) { // 영구적 속성이 아니고
		if ((timeLeft -= timeElapsed) <= 0.0) { // 수명이 다 했다면
			return false; // 삭제 요청
		}
	}

	// 부착 대상의 오프셋 위치로 이동하고 본인의 트랜스폼 행렬을 부착 대상의 트랜스폼 행렬과 같게 함
	if (pAttachTgt != NULL) {
		Quaternion *q = &(pAttachTgt->transform);
		Matrix4 transMat = q->toMatrix4();

		center = pAttachTgt->getPos();
		center = center + (attachOffset * transMat);
		transform = *q;
	}

	// 루프 타임 증가 및 초기화
	if ((currentLoopTime += timeElapsed) > loopingTimeBase) {
		currentLoopTime = 0.0;
	}

	return true;
}

/*************************************************************************************************************/
/********************************************** ExplosionEffect **********************************************/
/*************************************************************************************************************/

ExplosionEffect::ExplosionEffect(Vector3 _center, REAL time, int fragmentCnt) : GL_Effect(_center) {
	permanency = false;
	timeLeft = time;
	smokeEffectList.reserve(16);
	fragmentEffectList.reserve(fragmentCnt);

	GLuint texID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionCircleA.png");
	SmokeEffect *pEffect = NULL;

	// 원형 효과 생성
	REAL ringRange = 50.0;
	REAL rgba[4] = { 1.0, 1.0, 1.0, 1.0 };
	rgba[0] = 1.0;	rgba[1] = 1.0;	rgba[2] = 0.9;	rgba[3] = 0.75;
	pEffect = new SmokeEffect(_center, 1.0, ringRange, ringRange / 5.0, ringRange, texID);
	pEffect->setColor(rgba);
	smokeEffectList.push_back(pEffect);

	// 흩어지는 효과 생성
	texID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionB.png");
	pEffect = new SmokeEffect(_center, 1.0, ringRange * 1.25, ringRange / 5.0, ringRange, texID);
	smokeEffectList.push_back(pEffect);

	// 검은색 연기 효과 생성
	Vector3 newCenter;
	REAL x, y, z, dist, randomScale, speed;
	texID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/basicSmoke.png");
	rgba[0] = 0.1;	rgba[1] = 0.1;	rgba[2] = 0.1;	rgba[3] = 1.0;
	for (int i = 0; i < 4; ++i) {
		x = ((mymath::randomInt(0, 200) - 100) / 100.0);
		y = ((mymath::randomInt(0, 200) - 100) / 100.0);
		z = ((mymath::randomInt(0, 200) - 100) / 100.0);
		dist = (mymath::randomInt(5, 10) * 1.2);
		randomScale = mymath::randomInt(10, 15) * 0.3;

		newCenter = _center + (Vector3(x, y, z) * dist);
		pEffect = new SmokeEffect(newCenter, time, (ringRange - 10.0) / time, 10.0, ringRange, texID);
		pEffect->setColor(rgba);
	}

	// 물리 파편 효과 생성
	for (int k = 0; k < 10; ++k) {
		Fragment *pFrag = new Fragment; // 파편 구조체 생성
		if (pFrag != NULL) {
			pFrag->pos = _center;
			pFrag->lifeTime = 4.5;
			pFrag->speed = (mymath::randomInt(1000, 1350) * 0.1);
			pFrag->direction = Vector3((mymath::randomInt(0, 200) - 100) * 0.01, (mymath::randomInt(0, 200) - 100) * 0.01, (mymath::randomInt(0, 200) - 100) * 0.01);
			pFrag->smokePeriod = 0.05;
			pFrag->currentSmokePeriod = 0.0;
			fragmentEffectList.push_back(pFrag); // 파편 벡터에 추가
		}
	}

	// 폭발 화염 효과 생성
	texID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/effect/explosionA.png");
	
	pEffect = new SmokeEffect(_center, time, ringRange / time, ringRange * 0.4, ringRange * 0.6, texID);
	pEffect->setRotationSpeed(5.0);
	pEffect->setRotationDirection(mymath::randomInt(0, 1) == 0 ? true : false);

	for (int j = 0; j < 5; ++j) {
		x = ((mymath::randomInt(0, 200) - 100) / 100.0);
		y = ((mymath::randomInt(0, 200) - 100) / 100.0);
		z = ((mymath::randomInt(0, 200) - 100) / 100.0);
		dist = (mymath::randomInt(5, 10) * 0.7);
		randomScale = mymath::randomInt(10, 15) * 0.12;
		speed = (mymath::randomInt(10, 20) * 0.5);
		
		newCenter = _center + (Vector3(x, y, z) * dist);
		pEffect = new SmokeEffect(newCenter, time, speed, 1.5, 10.0, texID);
		pEffect->setScale(Vector3(randomScale, randomScale, randomScale));
		pEffect->setRotationSpeed(speed * 10.0);
		pEffect->setRotationDirection(mymath::randomInt(0, 1) == 0 ? true : false);
		smokeEffectList.push_back(pEffect);
	}
}

ExplosionEffect::ExplosionEffect(ExplosionEffect &obj) : GL_Effect(obj) {
	permanency = obj.permanency;
	timeLeft = obj.timeLeft;
	smokeEffectList = obj.smokeEffectList;
	fragmentEffectList = obj.fragmentEffectList;
}

ExplosionEffect::~ExplosionEffect() { }

void ExplosionEffect::draw() {

}

bool ExplosionEffect::update(REAL timeElapsed) {
	// 이펙트 수명 확인
	if (permanency == false) { // 영구적 속성이 아니고
		if ((timeLeft -= timeElapsed) <= 0.0) { // 수명이 다 됐으면
			// 파편은 별도로 삭제
			for (vector<Fragment*>::iterator iterFrag = fragmentEffectList.begin(); iterFrag != fragmentEffectList.end(); ++iterFrag) {
				delete *iterFrag;
			}

			return false; // 삭제 요청
		}
	}

	// 물리 효과 업데이트
	Fragment *pFrag = NULL;
	REAL rgba[4] = { 0.1, 0.1, 0.1, 1.0 };
	for (int i = 0; i < fragmentEffectList.size(); ++i) {
		pFrag = fragmentEffectList[i];

		if ((pFrag->lifeTime -= timeElapsed) > 0.00) { // 파편 생명력이 남아 있다면
			pFrag->pos += (pFrag->direction * (pFrag->speed * timeElapsed)); // 위치 업데이트
			pFrag->speed *= (1.00 + (timeElapsed / 2.0)); // 속도 강화
			pFrag->direction += (Vector3(0.0, -1.0, 0.0) / (2.0 / timeElapsed)); // 중력 영향
			pFrag->direction.normalize();
			
			if ((pFrag->currentSmokePeriod += timeElapsed) >= pFrag->smokePeriod) {
				pFrag->currentSmokePeriod -= pFrag->smokePeriod; // 연기 생성 주기 초기화
				SmokeEffect *pSmokeEffect = new SmokeEffect(pFrag->pos, 3.0, 3.5, 1.5, 8.0, 0); // 연기 생성
				pSmokeEffect->setScale(Vector3(2.0, 2.0, 2.0));
				pSmokeEffect->setColor(rgba);
			}
		}
		else {
			// 파편 생명력이 다 할시 아무것도 하지 않음 . . .
		}
	}

	return true;
}
