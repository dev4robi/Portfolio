#include "Game/UI/Viewer/LogoViewer.h"
#include "Engine/ResourceManager.h"
#include "Engine/GameFrame.h"

LogoViewer::LogoViewer(Vector3 pos, SIZE size) : RectViewer(pos, size, 0) {
	// 텍스처 로드
	texture_ring_outter = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/logo/logo_ring_outter.png");
	texture_ring_inner = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/logo/logo_ring_inner.png");
	texture_frame = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/logo/logo_frame.png");

	// 로고 이미지 설정 초기화 및 생성
	bUseTurning = true;
	pRingOutter = new RectViewer(pos, size, texture_ring_outter);
	pRingInner = new RectViewer(pos, size, texture_ring_inner);
	pFrameViwer = new RectViewer(pos, size, texture_frame);

	// 로고 이미지를 프레임 위젯에 추가
	g_pGameManager->getGameFramePtr()->addWidget(pRingOutter);
	g_pGameManager->getGameFramePtr()->addWidget(pRingInner);
	g_pGameManager->getGameFramePtr()->addWidget(pFrameViwer);
}

LogoViewer::LogoViewer(LogoViewer &obj) : RectViewer(obj) {
	texture_ring_inner = obj.texture_ring_inner;
	texture_ring_outter = obj.texture_ring_outter;
	texture_frame = obj.texture_frame;
	bUseTurning = obj.bUseTurning;
	pRingOutter = obj.pRingOutter;
	pRingInner = obj.pRingInner;
	pFrameViwer = obj.pFrameViwer;
}

LogoViewer::~LogoViewer() {
	pRingOutter = NULL;
	pRingInner = NULL;
	pFrameViwer = NULL;
}

bool LogoViewer::getLogoTurning() {
	return bUseTurning;
}

void LogoViewer::setLogoTurning(bool useTurn) {
	bUseTurning = useTurn;
}

bool LogoViewer::draw() {
	return true;
}

bool LogoViewer::destroy() {
	return true;
}

bool LogoViewer::update(REAL timeElapsed) {
	// 기본적인 업데이트 수행
	RectViewer::update(timeElapsed);

	// 로고 속성 동기화
	RectViewer *pViwer = NULL;
	for (int i = 0; i < 3; ++i) {
		switch (i) {
		case 0:		pViwer = pRingOutter;	break;
		case 1:		pViwer = pRingInner;	break;
		default:	pViwer = pFrameViwer;
		}

		if (pViwer != NULL) {
			pViwer->setColor3(this->vecColor3);
			pViwer->setColorAlpha(this->rColorAlpha);
			pViwer->setScale3(this->vecScale3);
			pViwer->setSize(this->szSize.cx, this->szSize.cy);
			pViwer->setOffset(this->vecOffset);
			pViwer->setVisible(this->isVisible);
		}
	}

	// 로고 회전 사용
	if (bUseTurning == true) {
		// 바깥쪽 원
		Vector3 rotate = pRingOutter->getRotate();
		rotate.z -= ((abs(sinf(rElapsedTime)) + 0.5) * (135.0 * timeElapsed));
		pRingOutter->setRotate(rotate);

		// 안쪽 원
		rotate = pRingInner->getRotate();
		rotate.z += ((abs(sinf(rElapsedTime)) + 0.5) * (135.0 * timeElapsed));
		pRingInner->setRotate(rotate);
	}
	// 로고 회전 비사용
	else {
		static const Vector3 zeroRotate(0.0, 0.0, 0.0);
		pRingOutter->setRotate(zeroRotate);
		pRingInner->setRotate(zeroRotate);
	}

	return true;
}