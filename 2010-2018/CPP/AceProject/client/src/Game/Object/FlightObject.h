#ifndef __FLIGHT_OBJECT_H__
#define __FLIGHT_OBJECT_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : FlightObject.h
*	- Last Update : 2016.05.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ����ü(������, �̻���)�� ���� ���� Ŭ����.
*
*	- Dependency : [StdHeader.h, GameWidget.h]
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"
#include "Engine/CollisionSytem.h"

class Player;

class FlightObject : public GameWidget {
public:
	// ���� ������
	enum object_state { // ����, ��� ���� ���� ������
		STATE_ALIVE, STATE_DEAD
	};

	// �⺻ ����
	Player *pOwningPlayer;		// ������ �÷��̾� ������

	// �� ���� ����
	GLMmodel *pModel;			// �� ����
	REAL rotateOffset[3];		// ȸ�� ������ x(pitch), y(yaw), z(roll)
	REAL pivotOffset[3];		// �Ǻ� ������ x, y, z
	REAL scale[3];				// ũ�� x, y, z
	REAL color[4];				// ���� r, g, b, a	

	// ����ü ���� ����
	Vector3 directVector;		// ������� ���� ���� ����
	Vector3 upVector;			// ������� ���� ����� ������ ���� ����
	Vector3 rightVector;		// ������� ���� ����/�����Ϳ� ���� ���� ����
	Quaternion transform;		// ����ü ȸ�� ��Ʈ������ ���� �����
	REAL pitch, yaw, roll;		// ��ġ�� �䰢 �Ѱ� (updatePitchRollYaw �Լ��� ����)
	REAL HP, maxHP;				// ����ü �����

	// ����ü ���� ���� ����
	object_state objectState;	// ����ü ���� ����
	bool isAccelerating;		// ����ü ���� ��
	bool isDeaccelerating;		// ����ü ���� ��

	// ����ü �ӵ� ���� ����
	REAL currentSpeed, maxSpeed;		// ���� �ӵ�, �ִ� �ӵ�
	REAL currentEngPower;				// ���� ���� �Ŀ� (0.0 ~ 100.0)
	REAL currentBoost;					// ���� �ν��� ��ġ (0.0 ~ 100.0)
	REAL boostLeftTime;					// �ν��� ���� �ð�
	REAL engPowerInc, engPowerDec;		// �ʴ� �Ŀ� �ʴ� ������
	REAL engOfferSpeed;					// ���� �Ŀ��� �ְ��� �� ������ �����ϴ� �ʴ� �ӵ� (currentSpeed = engOfferSpeed * currentEngPower)

	// ����ü ���� ���
	REAL default_pitchTurn;				// ��ġ ���� �ִ� �ӵ�
	REAL default_yawTurn;				// �� ���� �ִ� �ӵ�
	REAL default_rollTurn;				// �� ���� �ִ� �ӵ�
	REAL default_maxSpeed;				// �⺻ - ���� �ӵ�
	REAL default_engOfferSpeed;			// �⺻ - ���� ���� �ӵ�
	REAL default_boostOfferTime;		// �⺻ - �ν��� ���� �ð�

	// �浹 ���� ��ü
	vector<CollisionObject*> collisionAry; // �浹ü �迭

	// ���⿡ �Ӽ� ��� �߰� . . .
public:
	// Cons & Des
	FlightObject(Vector3 pos);			// ������
	FlightObject(FlightObject& _obj);	// ���� ������
	~FlightObject();					// �Ҹ���

	// Method
	void acceleration(REAL timeElapsed);				// ����ü ���� ����
	void deacceleration(REAL timeElapsed);				// �ӵ� ����
	void boosting(REAL timeElapsed);					// �ν��� ���
	void pitching(REAL angle, bool spdDecomp = false);	// angle��ŭ ��Ī
	void yawing(REAL angle, bool spdDecomp = false);	// angle��ŭ ����
	void rolling(REAL angle, bool spdDecomp = false);	// angle��ŭ �Ѹ�
	object_state getState();							// ����ü ���� ��ȯ
	void setState(object_state state);					// ����ü ���� ����
	REAL dealtDamage(FlightObject *pTgt, REAL amount, bool suicide, bool msgToNetwork = true);	// ����ü���� ���ظ� ������ �������� ����ü�� ���� ������� ��ȯ
	void dealtDamage_Net(short targetPlayerNum, REAL amount, bool suicide);						// �������� ���, ���ط�, �ڻ� ���ε��� ��Ʈ��ũ ��Ŷ�� ä������ (��Ʈ��ũ ����ȭ �޽���)
	void updateCollisionPos(Vector3 _position, Quaternion *_pTransform);						// �浹ü�� ��ġ�� ������Ʈ
	static REAL distanceBetweenObj(FlightObject *obj1, FlightObject *obj2);						// �� ����ü������ �Ÿ��� ��ȯ
	static bool isObjPlaceFront(FlightObject *obj1, FlightObject *obj2);						// ����ü 2�� ����ü1 �տ� ������ true��ȯ
	Vector3 getUpdatedPosition(REAL timeElapsed, int cnt = 1);									// cnt�� ������Ʈ�� ��ġ(����ü�� ���� ������Ʈ ��ġ)�� ��ȯ

protected:
	void updatePosition(REAL timeElapsed);			// ��ġ ���� ����
	void updatePitchRollYaw(REAL timeElapsed);		// ��ġ, ��, �� ������ ����
	void updateSpeed(REAL timeElapsed);				// ����ü�� �ӵ� ������Ʈ

private:
	void killFighter(FlightObject *pKiller);		// ������ �ı� (dealtDamage �Լ����� ���������� ȣ��)

	// Virtual Func
	virtual bool checkObjectCollision();			// �浹�� �˻� (���� �Լ��� Ŭ�������� ������ �浹 �˻� ����)
	virtual bool draw();							// ����ü(����)�� �׸�
	virtual bool destroy();							// ����ü�� �ı���
	virtual bool update(REAL timeElapsed);			// ����ü�� ������Ʈ
};

#endif