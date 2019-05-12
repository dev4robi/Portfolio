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
*	- Introduction : 특수효과 최상위 클래스.
*
*	- Dependency : [StdHeader.h], FlightObject.h
*
*   - Version(v1): GL 이펙트 클래스 기초 설계. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class FlightObject;

class GL_Effect : public GameObject {
protected:
	Vector3 center;				// 이펙트의 중심 좌표
	bool bDrawAfterWidget;		// 위젯들을 그린 후 그리는 옵션
	bool visible;				// 보이기 여부
	FlightObject *pAttachTgt;	// 부착 대상
	Vector3 attachOffset;		// 부착 대상으로부터의 오프셋
	bool permanency;			// 이펙트 객체 영구성 (영구 상태의 이펙트는 생명 시간이 소모되지 않음)
	REAL timeLeft;				// 이펙트 객체의 생명
	Vector3 scale;				// 이펙트의 크기 배율
	REAL color[4];				// 이펙트의 색상 배열 (R G B A)

public:
	GL_Effect(Vector3 _center);	// 생성자
	GL_Effect(GL_Effect &obj);	// 복사 생성자
	virtual ~GL_Effect();		// 소멸자

	void setCenter(Vector3 _center);			// 이펙트 중심 좌표를 설정
	Vector3 getCenter();						// 이펙트 중심 좌표를 반환
	void setVisible(bool _visible);				// 이펙트 보이기 여부를 설정
	bool getVisible();							// 이펙트 보이기 여부를 반환
	void setDrawAfterWidget(bool drawAfter);	// 위젯 그리기 전에 그리기 혹은 후에 그리기를 설정
	bool getDrawAfterWidget();					// 위젯 그리기 전에 그리기 혹은 후에 그리기를 반환
	void setAttachOffset(Vector3 offset);		// 부착 대상으로부터의 오프셋을 설정
	Vector3 getAttachOffset();					// 부착 대상으로부터의 오프셋을 반환
	void setAttachTgt(FlightObject *pWidget);	// 부착 대상을 설정
	FlightObject* getAttachTgtPtr();			// 부착 대상 포인터를 반환
	void setPermanency(bool _permanency);		// 영구성 설정
	bool getPermanency();						// 영구성 반환
	void setScale(Vector3 newScale);			// 크기 배율 설정
	Vector3 getScale();							// 크기 배율 반환
	void setColor(REAL rgba[4]);				// 색상 정보 설정
	void getColor(REAL colorAry[4]);			// 매개변수 배열에 색상 정보를 채워줌

	virtual void draw();					// 그리기 (가상 함수)
	virtual bool update(REAL timeElapsed);	// 업데이트 (가상 함수)
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : SmokeEffect.h(v1)
*	- Last Update : 2016.08.03
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 미사일, 폭발 등에서 나오는 동그랗게 퍼지는 연기 효과.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): 스모크 이펙트 클래스 기초 설계. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SmokeEffect : public GL_Effect {
protected:
	REAL baseSize;		// 기본 크기
	REAL spreadSpeed;	// 연기가 퍼지는 초당 속도
	REAL maxSize;		// 연기가 퍼지는 최대 크기
	REAL currentRotate;	// 현재 회전
	REAL rotateSpeed;	// 연기 회전 속도
	bool rotateLeft;	// 왼쪽 회전
	GLuint texture;		// 연기 효과 텍스처 번호

public:
	SmokeEffect(Vector3 _center, REAL time, REAL speed, REAL base, REAL max, GLuint texID);	// 생성자
	SmokeEffect(SmokeEffect &obj);															// 복사 생성자
	~SmokeEffect();																			// 소멸자

	void setTexture(GLuint texID);			// 텍스처 설정
	void setRotationSpeed(REAL speed);		// 회전 속도 설정
	void setRotationDirection(bool isLeft);	// 회전 방향 설정

	virtual void draw();					// 그리기
	virtual bool update(REAL timeElapsed);	// 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : TrailEffect.h(v1)
*	- Last Update : 2016.08.04
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 미사일과 전투기 날개에서 나오는 꼬리 효과.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1):트레일 이펙트 클래스 기초 설계. (2016.08.04)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class TrailEffect : public GL_Effect {
protected:
	struct Trail {
		Vector3 pos;		// 3차원 위치
		REAL colorAlpha;	// 알파 색상 
	};

	bool useTrailCreate;		// 트레일 생성 여부를 설정
	REAL baseAlpha;				// 알파 색상 배율
	REAL baseSize;				// 기본 크기
	GLuint texture;				// 사용되는 텍스처 번호
	int lengthMax;				// 최대로 길어질 수 있는 수
	list<Trail> trailList;		// 트레일 정보를 저장하는 리스트

public:
	TrailEffect(Vector3 _center, int _lengthMax, int texID = 0);	// 생성자
	TrailEffect(TrailEffect &obj);									// 복사 생성자
	~TrailEffect();													// 소멸자
	
	REAL getBaseAlpha();					// 알파 비율 반환
	void setBaseAlpha(REAL alpha);			// 알파 비율 설정
	bool getTrailCreatable();				// 트레일 생성 여부 반환
	void setTrailCreatable(bool useCreate);	// 트레일 생성 여부 설정

	void initTrail();						// 트레일 초기화

	virtual void draw();					// 그리기
	virtual bool update(REAL timeElasped);	// 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : JetFlameEffect(v1)
*	- Last Update : 2016.08.05
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 미사일과 전투기 후면에서 나오는 추진 화염 이펙트.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): 제트 플레임 클래스 기초 설계. (2016.08.05)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class JetFlameEffect : public GL_Effect {
protected:
	Quaternion transform;	// 트랜스폼 쿼터니언
	GLuint glowTexture;		// 가운데 빛나는 점 텍스처 번호
	GLuint blowTexture;		// 원형으로 흘러가는 바람 텍스처 번호

	REAL currentLoopTime;	// 현재 텍스처 반복 시간
	REAL loopingTimeBase;	// 텍스처 반복 시간
	REAL lengthEachCnt;		// 각 blow 단계마다 뒤로 밀려나는 길이
	int innerBlowLengthCnt;	// 원형으로 흘러가는 바람 텍스처의 길이 개수 (내부)
	int outerBlowLengthCnt;	// 원형으로 흘러가는 바람 텍스처의 길이 개수 (외부)
	Vector3 outterColor;	// 외부 바람 색상
	Vector3 innerColor;		// 내부 바람 색상
	Vector3 glowColor;		// 점화 빛 색상

public:
	JetFlameEffect(Vector3 _center, REAL _lengthEachCnt, int innerCnt, int outerCnt);
	JetFlameEffect(JetFlameEffect &obj);
	~JetFlameEffect();

	void setBaseAlpha(REAL alpha);						// 기본 알파색을 설정함
	REAL getBaseAlpha();								// 기본 알파색을 가져옴
	void setBlowColor(bool isOutter, Vector3 color);	// 외부 내부 바람 색상 설정
	Vector3 getBlowColor(bool isOutter);				// 외부 내부 바람 색상 반환
	void setGlowColor(Vector3 color);					// 점화 빛 색상 설정

	virtual void draw();					// 그리기
	virtual bool update(REAL timeElasped);	// 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : ExplosionEffect.h(v1)
*	- Last Update : 2016.08.08
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 폭발 및 파편 효과 이펙트.
*
*	- Dependency : GL_Effect.h
*
*   - Version(v1): GL 이펙트 클래스 기초 설계. (2016.08.03)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ExplosionEffect : public GL_Effect {
protected:
	struct Fragment { // 파편 구조체
		Vector3 pos;				// 파편 위치
		Vector3 direction;			// 파편이 향하는 방향
		REAL lifeTime;				// 생명 시간
		REAL speed;					// 파편의 속도
		REAL smokePeriod;			// 연기 생성 주기
		REAL currentSmokePeriod;	// 연기 생성 현재 주기
	};

	vector<SmokeEffect*> smokeEffectList;	// 연기 효과 리스트
	vector<Fragment*> fragmentEffectList;	// 파편 효과 리스트

public:
	ExplosionEffect(Vector3 _center, REAL time, int fragmentCnt);	// 생성자
	ExplosionEffect(ExplosionEffect &obj);							// 복사 생성자
	~ExplosionEffect();												// 소멸자

	virtual void draw();					// 그리기
	virtual bool update(REAL timeElapsed);	// 업데이트
};

#endif