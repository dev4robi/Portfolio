#ifndef __MISSILE_H__
#define __MISSILE_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : Missile.h(v1)
*	- Last Update : 2016.07.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �̻��Ͽ� ���� ���� Ŭ����.
*
*	- Dependency : [FlightObject.h], Weapon.h, UI.h, GL_Effect.h
*
*   - Version(v1): �̻��� Ŭ���� ���� ���� (2016.07.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Game/Object/FlightObject.h"

class WpnLockonMissile;
class JetFighter;
class LockonUI;
class TrailEffect;
class JetFlameEffect;
class Weapon;

class Missile : public FlightObject {
private:
	bool isBulletStyle;				// �̻��� Ȥ�� �Ѿ� ������
	REAL guideDelayTime;			// ���� ����� ���۵Ǳ� ���� ���� ���� �ð�
	JetFighter *pFireJet;			// �̻����� �߻��� ������
	JetFighter *pLockonTgt;			// ���� ��� ����ü ������
	Weapon *pFireWeapon;			// �߻��� ������ ������
	REAL damage;					// �̻��� ���ط�
	REAL dmgAoE;					// �̻��� ���� �ݰ�
	REAL homingLevel;				// �̻��� ������ ������ ���� �������� ��� �������� ���� (0.0 ~ )
	
	TrailEffect *pMsslTrailEffect;	// �̻��� ���� �ڱ� ����Ʈ
	JetFlameEffect *pFlameEffect;	// �̻��� ��ȭ ����Ʈ
	// ���⿡ �Ӽ� ��� �߰�... (�浹�ڽ�, ����Ʈ ��)
public:
	Missile(JetFighter *pMissileOwner, LockonUI *pLockonUI, Weapon *pWeapon, bool bulletStyle = false);	// ������
	Missile(Missile& obj);																				// ���� ������
	~Missile();																							// �Ҹ���

	JetFighter* getLockonTgt();			// ���� Ÿ�� ������ ��ȯ
	void setDamage(REAL dmg);			// �̻��� ���ط� ����
	REAL getDamage();					// �ǻ��� ���ط� ��ȯ
	void setDmgAoE(REAL AoE);			// �̻��� ���� �ݰ� ����
	REAL getDmgAoE();					// �̻��� ���� �ݰ� ��ȯ
	bool getStyle();					// �Ѿ� ��Ÿ���� �̻����̸� true ��ȯ

	virtual bool checkObjectCollision();	// �̻��� �浹�� Ȯ��
	virtual bool draw();					// �̻��� �׸���
	virtual bool destory();					// �̻��� �ı�
	virtual bool update(REAL timeElapsed);	// �̻��� ������Ʈ
};

#endif