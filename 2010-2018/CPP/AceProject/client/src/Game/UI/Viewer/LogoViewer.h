#ifndef __LOGO_VIEWER_H__
#define __LOGO_VIEWER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LogoViewer(v1)
*	- Last Update : 2016.08.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 로고를 출력하는 위젯.
*
*	- Dependency : [StdHeader.h, RectViewer.h], DrawManager.h, 
*
*   - Version(v1): RectViewer 클래스 기초 설계. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Game/UI/Viewer/RectViewer.h"

class LogoViewer : public RectViewer {
private:
	bool bUseTurning;					// 로고 회전 사용 여부
	RectViewer *pRingOutter;			// 로고 외부 링
	RectViewer *pRingInner;				// 로고 내부 링
	RectViewer *pFrameViwer;			// 로고 프레임

	GLuint texture_ring_outter;			// 외부 원 텍스처
	GLuint texture_ring_inner;			// 내부 원 텍스처
	GLuint texture_frame;				// 내부 프레임 텍스처

public:
	// 생성자 소멸자
	LogoViewer(Vector3 pos, SIZE size);
	LogoViewer(LogoViewer &obj);
	~LogoViewer();

	// 메소드
	bool getLogoTurning();				// 로고 회전 여부를 반환
	void setLogoTurning(bool useTurn);	// 로고 회전 여부를 설정

	// 가상 함수
	virtual bool draw();
	virtual bool destroy();
	virtual bool update(REAL timeElapsed);
};

#endif