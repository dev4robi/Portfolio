#ifndef __GAME_WIDGET_H__
#define __GAME_WIDGET_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameWidget (v3)
*	- Last Update : 2016.7.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임 화면상에 표현되는 모든 개체를 의미하는 클래스.
*                    drawWidget 가상 함수에 그리기 내용을 정의한다.
*                    게임상에 그려지는 모든 개체들은 이 클래스를 상속하여 작업된다.
*
*	- Dependency : [StdHeader.h], GameCamera.h, DrawManager.h
*
*   - Version(v1): 게임 위젯 기초 설계 (2016.03.23)
*	- Version(v2): 게임 위젯에 hiding, visible, 2d옵션 및 pos의 setter 멤버 함수 추가. (2016.05.16)
*	- Version(v3): 게임 위젯의 좌표 체계를 x, y, z 3개의 REAL에서 Vector3 클래스로 변경. (2016.7.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

static ID nextID = 1;	// 객체의 다음 ID로 생성자/복사 생성자에서 사용하고 자동으로 값을 증가시킴

class GameWidget : public GameObject {
public:
	static const int WIDGET_DEFAULT = 0;	// 디폴트 위젯
	static const int WIDGET_JET = 1;		// 전투기 위젯
	static const int WIDGET_MISSILE = 2;	// 미사일 위젯
	static const int WIDGET_TERRAIN = 3;	// 지형 위젯
	static const int WIDGET_SKYBOX = 4;		// 하늘 상자 위젯
	static const int WIDGET_UI_RADARLOCKON = 5;	// 유저 인터페이스 - 레이더 락온

protected:
	ID id;					// 위젯 ID
	DWORD createdTime;		// 위젯이 생성된 시간
	int widgetType;			// 위젯 종류
	bool isHiding;			// 위젯의 게임에서의 숨김 여부 (숨겨진 위젯은 로직 처리 또한 예외됨)
	bool isVisible;			// 위젯의 그리기 여부
	bool is2D;				// 2차원 좌표를 사용하는 위젯 (UI, HUD 등)
	Vector3 position;		// 위젯의 GL상 좌표
	GLuint textureID;		// 사용할 텍스처 번호(ID)

public:
	GameWidget();
	GameWidget(Vector3 pos);
	GameWidget(GameWidget& tempWidget);		// 복사 생성자로 생성된 객체는 새로운 ID와 생성된 시간값을 가짐
	virtual ~GameWidget();

	ID getID();								// 위젯의 ID를 반환
	DWORD getCreatedTime();					// 위젯이 생성된 시간을 반환
	void setHiding(bool hiding);			// 위젯의 숨김 여부 설정
	bool getHiding();						// 위젯의 숨김 여부를 반환
	void setVisible(bool visible);			// 위젯의 그리기 여부를 설정
	bool getVisible();						// 위젯의 그리기 여부를 반환
	void set2D(bool _2D_or_3D);				// 위젯의 2D/3D 여부를 설정
	bool getDimension();					// 위젯의 2D/3D 여부를 반환
	void setPos(Vector3 newPos);			// 위젯의 위치를 벡터로 설정
	void setPos(REAL x, REAL y, REAL z);	// 위젯의 위치를 설정
	void setPosX(REAL x);					// 위젯의 X축 위치를 설정
	void setPosY(REAL y);					// 위젯의 X축 위치를 설정
	void setPosZ(REAL z);					// 위젯의 X축 위치를 설정
	void setTextureID(GLuint id);			// 위젯의 텍스처를 번호로 설정
	Vector3 getScreenPosition();			// 3차원 좌표를 사용하는 위젯의 2차원 화면상 좌표를 반환
	Vector3 getPos();						// 위젯의 위치를 벡터로 반환
	REAL getPosX();							// 위젯의 X축 위치를 반환
	REAL getPosY();							// 위젯의 Y축 위치를 반환
	REAL getPosZ();							// 위젯의 Z축 위치를 반환
	GLuint getTextureID();					// 위젯의 텍스처 번호를 반환
	void setWidgetType(int typeEnum);		// 위젯의 종류를 설정
	int getWidgetType();					// 위젯의 종류를 반환

	virtual bool draw();					// 위젯에 정의된 그리기를 수행
	virtual bool destroy();					// 위젯을 파괴함
	virtual bool update(REAL timeElapsed);	// 위젯을 업데이트
};

#endif