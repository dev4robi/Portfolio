#ifndef __GL_EFFECT_H__
#define __GL_EFFECT_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	< Content of GL_Effect.h >
*
*	 1. GL_Effect(v1) - 2016.08.03
*	 2. SmokeEffect(v1) - 2016.08.03
*	 3. TrailEffect(v1) - 2016.08.04
*	 4. JetFlameEffect(v1) - 2016.08.05
*	 5. ExplosionEffect(v1) - 2013.08.08
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : GL_Effect.h(v1)
*	- Last Update : 2016.08.03
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : Ư��ȿ�� �ֻ��� Ŭ����.
*
*	- Dependency : [StdHeader.h], FlightObject.h
*
*   - Version(v1): GL ����Ʈ Ŭ���� ���� ����. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class FlightObject;

class GL_Effect : public GameObject {
protected:
	Vector3 center;				// ����Ʈ�� �߽� ��ǥ
	bool bDrawAfterWidget;		// �������� �׸� �� �׸��� �ɼ�
	bool visible;				// ���̱� ����
	FlightObject *pAttachTgt;	// ���� ���
	Vector3 attachOffset;		// ���� ������κ����� ������
	bool permanency;			// ����Ʈ ��ü ������ (���� ������ ����Ʈ�� ���� �ð��� �Ҹ���� ����)
	REAL timeLeft;				// ����Ʈ ��ü�� ����
	Vector3 scale;				// ����Ʈ�� ũ�� ����
	REAL color[4];				// ����Ʈ�� ���� �迭 (R G B A)

public:
	GL_Effect(Vector3 _center);	// ������
	GL_Effect(GL_Effect &obj);	// ���� ������
	virtual ~GL_Effect();		// �Ҹ���

	void setCenter(Vector3 _center);			// ����Ʈ �߽� ��ǥ�� ����
	Vector3 getCenter();						// ����Ʈ �߽� ��ǥ�� ��ȯ
	void setVisible(bool _visible);				// ����Ʈ ���̱� ���θ� ����
	bool getVisible();							// ����Ʈ ���̱� ���θ� ��ȯ
	void setDrawAfterWidget(bool drawAfter);	// ���� �׸��� ���� �׸��� Ȥ�� �Ŀ� �׸��⸦ ����
	bool getDrawAfterWidget();					// ���� �׸��� ���� �׸��� Ȥ�� �Ŀ� �׸��⸦ ��ȯ
	void setAttachOffset(Vector3 offset);		// ���� ������κ����� �������� ����
	Vector3 getAttachOffset();					// ���� ������κ����� �������� ��ȯ
	void setAttachTgt(FlightObject *pWidget);	// ���� ����� ����
	FlightObject* getAttachTgtPtr();			// ���� ��� �����͸� ��ȯ
	void setPermanency(bool _permanency);		// ������ ����
	bool getPermanency();						// ������ ��ȯ
	void setScale(Vector3 newScale);			// ũ�� ���� ����
	Vector3 getScale();							// ũ�� ���� ��ȯ
	void setColor(REAL rgba[4]);				// ���� ���� ����
	void getColor(REAL colorAry[4]);			// �Ű����� �迭�� ���� ������ ä����

	virtual void draw();					// �׸��� (���� �Լ�)
	virtual bool update(REAL timeElapsed);	// ������Ʈ (���� �Լ�)
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : SmokeEffect.h(v1)
*	- Last Update : 2016.08.03
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �̻���, ���� ��� ������ ���׶��� ������ ���� ȿ��.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): ����ũ ����Ʈ Ŭ���� ���� ����. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SmokeEffect : public GL_Effect {
protected:
	REAL baseSize;		// �⺻ ũ��
	REAL spreadSpeed;	// ���Ⱑ ������ �ʴ� �ӵ�
	REAL maxSize;		// ���Ⱑ ������ �ִ� ũ��
	REAL currentRotate;	// ���� ȸ��
	REAL rotateSpeed;	// ���� ȸ�� �ӵ�
	bool rotateLeft;	// ���� ȸ��
	GLuint texture;		// ���� ȿ�� �ؽ�ó ��ȣ

public:
	SmokeEffect(Vector3 _center, REAL time, REAL speed, REAL base, REAL max, GLuint texID);	// ������
	SmokeEffect(SmokeEffect &obj);															// ���� ������
	~SmokeEffect();																			// �Ҹ���

	void setTexture(GLuint texID);			// �ؽ�ó ����
	void setRotationSpeed(REAL speed);		// ȸ�� �ӵ� ����
	void setRotationDirection(bool isLeft);	// ȸ�� ���� ����

	virtual void draw();					// �׸���
	virtual bool update(REAL timeElapsed);	// ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : TrailEffect.h(v1)
*	- Last Update : 2016.08.04
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �̻��ϰ� ������ �������� ������ ���� ȿ��.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1):Ʈ���� ����Ʈ Ŭ���� ���� ����. (2016.08.04)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class TrailEffect : public GL_Effect {
protected:
	struct Trail {
		Vector3 pos;		// 3���� ��ġ
		REAL colorAlpha;	// ���� ���� 
	};

	bool useTrailCreate;		// Ʈ���� ���� ���θ� ����
	REAL baseAlpha;				// ���� ���� ����
	REAL baseSize;				// �⺻ ũ��
	GLuint texture;				// ���Ǵ� �ؽ�ó ��ȣ
	int lengthMax;				// �ִ�� ����� �� �ִ� ��
	list<Trail> trailList;		// Ʈ���� ������ �����ϴ� ����Ʈ

public:
	TrailEffect(Vector3 _center, int _lengthMax, int texID = 0);	// ������
	TrailEffect(TrailEffect &obj);									// ���� ������
	~TrailEffect();													// �Ҹ���
	
	REAL getBaseAlpha();					// ���� ���� ��ȯ
	void setBaseAlpha(REAL alpha);			// ���� ���� ����
	bool getTrailCreatable();				// Ʈ���� ���� ���� ��ȯ
	void setTrailCreatable(bool useCreate);	// Ʈ���� ���� ���� ����

	void initTrail();						// Ʈ���� �ʱ�ȭ

	virtual void draw();					// �׸���
	virtual bool update(REAL timeElasped);	// ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : JetFlameEffect(v1)
*	- Last Update : 2016.08.05
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �̻��ϰ� ������ �ĸ鿡�� ������ ���� ȭ�� ����Ʈ.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): ��Ʈ �÷��� Ŭ���� ���� ����. (2016.08.05)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class JetFlameEffect : public GL_Effect {
protected:
	Quaternion transform;	// Ʈ������ ���ʹϾ�
	GLuint glowTexture;		// ��� ������ �� �ؽ�ó ��ȣ
	GLuint blowTexture;		// �������� �귯���� �ٶ� �ؽ�ó ��ȣ

	REAL currentLoopTime;	// ���� �ؽ�ó �ݺ� �ð�
	REAL loopingTimeBase;	// �ؽ�ó �ݺ� �ð�
	REAL lengthEachCnt;		// �� blow �ܰ踶�� �ڷ� �з����� ����
	int innerBlowLengthCnt;	// �������� �귯���� �ٶ� �ؽ�ó�� ���� ���� (����)
	int outerBlowLengthCnt;	// �������� �귯���� �ٶ� �ؽ�ó�� ���� ���� (�ܺ�)
	Vector3 outterColor;	// �ܺ� �ٶ� ����
	Vector3 innerColor;		// ���� �ٶ� ����
	Vector3 glowColor;		// ��ȭ �� ����

public:
	JetFlameEffect(Vector3 _center, REAL _lengthEachCnt, int innerCnt, int outerCnt);
	JetFlameEffect(JetFlameEffect &obj);
	~JetFlameEffect();

	void setBaseAlpha(REAL alpha);						// �⺻ ���Ļ��� ������
	REAL getBaseAlpha();								// �⺻ ���Ļ��� ������
	void setBlowColor(bool isOutter, Vector3 color);	// �ܺ� ���� �ٶ� ���� ����
	Vector3 getBlowColor(bool isOutter);				// �ܺ� ���� �ٶ� ���� ��ȯ
	void setGlowColor(Vector3 color);					// ��ȭ �� ���� ����

	virtual void draw();					// �׸���
	virtual bool update(REAL timeElasped);	// ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : ExplosionEffect.h(v1)
*	- Last Update : 2016.08.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �� ���� ȿ�� ����Ʈ.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): GL ����Ʈ Ŭ���� ���� ����. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ExplosionEffect : public GL_Effect {
protected:
	struct Fragment { // ���� ����ü
		Vector3 pos;				// ���� ��ġ
		Vector3 direction;			// ������ ���ϴ� ����
		REAL lifeTime;				// ���� �ð�
		REAL speed;					// ������ �ӵ�
		REAL smokePeriod;			// ���� ���� �ֱ�
		REAL currentSmokePeriod;	// ���� ���� ���� �ֱ�
	};

	vector<SmokeEffect*> smokeEffectList;	// ���� ȿ�� ����Ʈ
	vector<Fragment*> fragmentEffectList;	// ���� ȿ�� ����Ʈ

public:
	ExplosionEffect(Vector3 _center, REAL time, int fragmentCnt);	// ������
	ExplosionEffect(ExplosionEffect &obj);							// ���� ������
	~ExplosionEffect();												// �Ҹ���

	virtual void draw();					// �׸���
	virtual bool update(REAL timeElapsed);	// ������Ʈ
};

#endif