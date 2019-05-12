#ifndef __RECT_VIEWER_H__
#define __RECT_VIEWER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RectViewer(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 사각형 모양의 뷰어 위젯.
*
*	- Dependency : [StdHeader.h, GameWidget.h], DrawManager.h, GLTextureFont.h
*
*   - Version(v1): RectViewer 클래스 기초 설계. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"

class RectViewer : public GameWidget {
protected:
	Vector3 vecOffset;			// 뷰어 위치 오프셋
	Vector3 vecRotate;			// 뷰어 회전률
	REAL rElapsedTime;			// 뷰어 생성 경과 시간
	Vector3 vecScale3;			// 뷰어 크기 배율
	SIZE szSize;				// 뷰어 폭과 높이
	bool bPermanency;			// 뷰어 영구속성 여부
	REAL rLife;					// 뷰어 생명력
	Vector3 vecColor3;			// 뷰어 색상
	REAL rColorAlpha;			// 알파 색상
	REAL rTextureX, rTextureY;	// 텍스처 보정 좌표
	string strText;				// 텍스트
	REAL rTextSize;				// 텍스트 크기
	REAL rTextColorAlpha;		// 텍스트 알파 색상
	bool bTextAlignCenterX;		// 텍스트를 X축 기준으로 중앙 정렬시키기 사용 여부
	int iFont;					// 폰트 번호
	int iTag;					// 뷰어에 고유로 부여할 수 있는 정수 태그

public:
	// 생성자 소멸자
	RectViewer(Vector3 pos, SIZE size, GLuint texID);
	RectViewer(RectViewer &obj);
	virtual ~RectViewer();

	// 메소드
	Vector3 getOffset();						// 뷰어 위치 오프셋 반환
	void setOffset(Vector3 offset);				// 뷰어 위치 오프셋 설정
	Vector3 getRotate();						// 뷰어 회전률 반환
	void setRotate(Vector3 rotate);				// 뷰어 회전률 설정
	SIZE getSize();								// 크기 반환
	void setSize(int width, int height);		// 크기 설정
	Vector3 getScale3();						// 뷰어 크기 배율 반환
	void setScale3(REAL x, REAL y, REAL z);		// 뷰어 크기 배율 설정
	void setScale3(Vector3 scale);				// 뷰어 크기 배율 설정
	REAL getElapsedTime();						// 생성 후 경과시간 반환
	Vector3 getColor3();						// 뷰어 색상 반환
	void setColor3(REAL r, REAL g, REAL b);		// 뷰어 색상 설정
	void setColor3(Vector3 rgbVec);				// 뷰어 색상 설정
	REAL getColorAlpha();						// 뷰어 알파색상 반환
	void setColorAlpha(REAL alpha);				// 뷰어 알파색상 설정
	REAL getTextColorAlpha();					// 텍스트 알파색상 반환
	void setTextColorAlpha(REAL txtAlpha);		// 텍스트 알파색상 설정
	void setTextureXY(REAL x, REAL y);			// 텍스처 보정 좌표 설정
	void setPermanency(bool permanency);		// 뷰어 영구속성 설정
	bool getPermanency();						// 뷰어 영구속성 여부 반환
	REAL getViwerLife();						// 뷰어 생명력 반환
	void setViwerLife(REAL lifeTime);			// 뷰어 생명력 설정
	string getText();							// 뷰어 텍스트 반환
	void setText(string txt);					// 뷰어 텍스트 설정
	REAL getTextSize();							// 텍스트 크기 반환
	void setTextSize(REAL size);				// 텍스트 크기 설정
	int getFontID();							// 폰트 번호 반환
	void setFontID(int id);						// 폰트 번호 설정
	int getTagInt();							// 정수 태그 번호 반환
	void setTagInt(int tag);					// 정수 태그 번호 설정
	void setTextAlignCenterX(bool alignCenter);	// 텍스트를 X축 기준으로 중앙 정렬 사용시키기 여부 설정
	void setTextAttributes(string txt, REAL size, bool showTextOnly, int fontID); // 텍스트 속성들 일괄 설정

	// 가상 함수
	virtual bool draw();
	virtual bool destroy();
	virtual bool update(REAL timeElapsed);
};

#endif