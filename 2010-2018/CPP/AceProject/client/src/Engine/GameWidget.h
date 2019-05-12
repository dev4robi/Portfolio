#ifndef __GAME_WIDGET_H__
#define __GAME_WIDGET_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameWidget (v3)
*	- Last Update : 2016.7.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ȭ��� ǥ���Ǵ� ��� ��ü�� �ǹ��ϴ� Ŭ����.
*                    drawWidget ���� �Լ��� �׸��� ������ �����Ѵ�.
*                    ���ӻ� �׷����� ��� ��ü���� �� Ŭ������ ����Ͽ� �۾��ȴ�.
*
*	- Dependency : [StdHeader.h], GameCamera.h, DrawManager.h
*
*   - Version(v1): ���� ���� ���� ���� (2016.03.23)
*	- Version(v2): ���� ������ hiding, visible, 2d�ɼ� �� pos�� setter ��� �Լ� �߰�. (2016.05.16)
*	- Version(v3): ���� ������ ��ǥ ü�踦 x, y, z 3���� REAL���� Vector3 Ŭ������ ����. (2016.7.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

static ID nextID = 1;	// ��ü�� ���� ID�� ������/���� �����ڿ��� ����ϰ� �ڵ����� ���� ������Ŵ

class GameWidget : public GameObject {
public:
	static const int WIDGET_DEFAULT = 0;	// ����Ʈ ����
	static const int WIDGET_JET = 1;		// ������ ����
	static const int WIDGET_MISSILE = 2;	// �̻��� ����
	static const int WIDGET_TERRAIN = 3;	// ���� ����
	static const int WIDGET_SKYBOX = 4;		// �ϴ� ���� ����
	static const int WIDGET_UI_RADARLOCKON = 5;	// ���� �������̽� - ���̴� ����

protected:
	ID id;					// ���� ID
	DWORD createdTime;		// ������ ������ �ð�
	int widgetType;			// ���� ����
	bool isHiding;			// ������ ���ӿ����� ���� ���� (������ ������ ���� ó�� ���� ���ܵ�)
	bool isVisible;			// ������ �׸��� ����
	bool is2D;				// 2���� ��ǥ�� ����ϴ� ���� (UI, HUD ��)
	Vector3 position;		// ������ GL�� ��ǥ
	GLuint textureID;		// ����� �ؽ�ó ��ȣ(ID)

public:
	GameWidget();
	GameWidget(Vector3 pos);
	GameWidget(GameWidget& tempWidget);		// ���� �����ڷ� ������ ��ü�� ���ο� ID�� ������ �ð����� ����
	virtual ~GameWidget();

	ID getID();								// ������ ID�� ��ȯ
	DWORD getCreatedTime();					// ������ ������ �ð��� ��ȯ
	void setHiding(bool hiding);			// ������ ���� ���� ����
	bool getHiding();						// ������ ���� ���θ� ��ȯ
	void setVisible(bool visible);			// ������ �׸��� ���θ� ����
	bool getVisible();						// ������ �׸��� ���θ� ��ȯ
	void set2D(bool _2D_or_3D);				// ������ 2D/3D ���θ� ����
	bool getDimension();					// ������ 2D/3D ���θ� ��ȯ
	void setPos(Vector3 newPos);			// ������ ��ġ�� ���ͷ� ����
	void setPos(REAL x, REAL y, REAL z);	// ������ ��ġ�� ����
	void setPosX(REAL x);					// ������ X�� ��ġ�� ����
	void setPosY(REAL y);					// ������ X�� ��ġ�� ����
	void setPosZ(REAL z);					// ������ X�� ��ġ�� ����
	void setTextureID(GLuint id);			// ������ �ؽ�ó�� ��ȣ�� ����
	Vector3 getScreenPosition();			// 3���� ��ǥ�� ����ϴ� ������ 2���� ȭ��� ��ǥ�� ��ȯ
	Vector3 getPos();						// ������ ��ġ�� ���ͷ� ��ȯ
	REAL getPosX();							// ������ X�� ��ġ�� ��ȯ
	REAL getPosY();							// ������ Y�� ��ġ�� ��ȯ
	REAL getPosZ();							// ������ Z�� ��ġ�� ��ȯ
	GLuint getTextureID();					// ������ �ؽ�ó ��ȣ�� ��ȯ
	void setWidgetType(int typeEnum);		// ������ ������ ����
	int getWidgetType();					// ������ ������ ��ȯ

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

#endif