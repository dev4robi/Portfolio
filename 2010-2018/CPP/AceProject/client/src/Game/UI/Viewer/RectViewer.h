#ifndef __RECT_VIEWER_H__
#define __RECT_VIEWER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RectViewer(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �簢�� ����� ��� ����.
*
*	- Dependency : [StdHeader.h, GameWidget.h], DrawManager.h, GLTextureFont.h
*
*   - Version(v1): RectViewer Ŭ���� ���� ����. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"

class RectViewer : public GameWidget {
protected:
	Vector3 vecOffset;			// ��� ��ġ ������
	Vector3 vecRotate;			// ��� ȸ����
	REAL rElapsedTime;			// ��� ���� ��� �ð�
	Vector3 vecScale3;			// ��� ũ�� ����
	SIZE szSize;				// ��� ���� ����
	bool bPermanency;			// ��� �����Ӽ� ����
	REAL rLife;					// ��� �����
	Vector3 vecColor3;			// ��� ����
	REAL rColorAlpha;			// ���� ����
	REAL rTextureX, rTextureY;	// �ؽ�ó ���� ��ǥ
	string strText;				// �ؽ�Ʈ
	REAL rTextSize;				// �ؽ�Ʈ ũ��
	REAL rTextColorAlpha;		// �ؽ�Ʈ ���� ����
	bool bTextAlignCenterX;		// �ؽ�Ʈ�� X�� �������� �߾� ���Ľ�Ű�� ��� ����
	int iFont;					// ��Ʈ ��ȣ
	int iTag;					// �� ������ �ο��� �� �ִ� ���� �±�

public:
	// ������ �Ҹ���
	RectViewer(Vector3 pos, SIZE size, GLuint texID);
	RectViewer(RectViewer &obj);
	virtual ~RectViewer();

	// �޼ҵ�
	Vector3 getOffset();						// ��� ��ġ ������ ��ȯ
	void setOffset(Vector3 offset);				// ��� ��ġ ������ ����
	Vector3 getRotate();						// ��� ȸ���� ��ȯ
	void setRotate(Vector3 rotate);				// ��� ȸ���� ����
	SIZE getSize();								// ũ�� ��ȯ
	void setSize(int width, int height);		// ũ�� ����
	Vector3 getScale3();						// ��� ũ�� ���� ��ȯ
	void setScale3(REAL x, REAL y, REAL z);		// ��� ũ�� ���� ����
	void setScale3(Vector3 scale);				// ��� ũ�� ���� ����
	REAL getElapsedTime();						// ���� �� ����ð� ��ȯ
	Vector3 getColor3();						// ��� ���� ��ȯ
	void setColor3(REAL r, REAL g, REAL b);		// ��� ���� ����
	void setColor3(Vector3 rgbVec);				// ��� ���� ����
	REAL getColorAlpha();						// ��� ���Ļ��� ��ȯ
	void setColorAlpha(REAL alpha);				// ��� ���Ļ��� ����
	REAL getTextColorAlpha();					// �ؽ�Ʈ ���Ļ��� ��ȯ
	void setTextColorAlpha(REAL txtAlpha);		// �ؽ�Ʈ ���Ļ��� ����
	void setTextureXY(REAL x, REAL y);			// �ؽ�ó ���� ��ǥ ����
	void setPermanency(bool permanency);		// ��� �����Ӽ� ����
	bool getPermanency();						// ��� �����Ӽ� ���� ��ȯ
	REAL getViwerLife();						// ��� ����� ��ȯ
	void setViwerLife(REAL lifeTime);			// ��� ����� ����
	string getText();							// ��� �ؽ�Ʈ ��ȯ
	void setText(string txt);					// ��� �ؽ�Ʈ ����
	REAL getTextSize();							// �ؽ�Ʈ ũ�� ��ȯ
	void setTextSize(REAL size);				// �ؽ�Ʈ ũ�� ����
	int getFontID();							// ��Ʈ ��ȣ ��ȯ
	void setFontID(int id);						// ��Ʈ ��ȣ ����
	int getTagInt();							// ���� �±� ��ȣ ��ȯ
	void setTagInt(int tag);					// ���� �±� ��ȣ ����
	void setTextAlignCenterX(bool alignCenter);	// �ؽ�Ʈ�� X�� �������� �߾� ���� ����Ű�� ���� ����
	void setTextAttributes(string txt, REAL size, bool showTextOnly, int fontID); // �ؽ�Ʈ �Ӽ��� �ϰ� ����

	// ���� �Լ�
	virtual bool draw();
	virtual bool destroy();
	virtual bool update(REAL timeElapsed);
};

#endif