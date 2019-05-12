#ifndef __JET_FIGHTER_H__
#define __JET_FIGHTER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : JetFighter.h(v2)
*	- Last Update : 2016.07.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ����ü(������, �̻���)�� ���� ���� Ŭ����.
*
*	- Dependency : [FlightObject.h], Weapon.h, Player.h, UI.h
*
*   - Version(v1): ������ Ŭ���� ���� ���� (2016.05.??)
*   - Version(v2): ������ Ŭ���� ��� �߰�. (2016.07.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Game/Object/FlightObject.h"

class Player;
class Weapon;
class WpnGatlingGun;
class WpnLockonMissile;
class TrailEffect;
class JetFlameEffect;
class Sound;
class GamePacket;

class JetFighter : public FlightObject {
public:
	enum jet_warning {
		JET_LOCKON_NONE, JET_LOCKON_BY_ENEMY, JET_LOCKON_BY_MISSILE
	};

protected:
	// �ִ� ��
	static const int JET_MAX_ALTITUDE = 10000;

	// ��ü��
	string craftName;				// ����� ��Ī

	// ��� �� ����
	REAL jetOffcourseTime;			// ��ü �ڽ� ��� ��� �ð�
	jet_warning jetWarningLockon;	// ��ü ���� ��� 3����
	bool jetWarningAltitude;		// ��ü ���� �� ���
	bool jetWarningStall;			// ��ü �Ǽ� ���

	// ���̴�
	REAL radarRadius;				// ���̴� ���� ����
	list<FlightObject*> radarList;	// ���̴��� ������ ����ü ������ ����Ʈ

	// ����
	WpnLockonMissile *pBaseWeapon;	// �ֹ��� (�����)
	WpnLockonMissile *pFirstWeapon;	// �ֹ��� (�̻���)

	// ������
	bool jetUseRespawn;				// ��Ȱ ��� ����
	REAL jetRespawnLeftTime;		// ��Ȱ ���� �ð�
	REAL jetRespawnReqTime;			// ��Ȱ �䱸 �ð�

	// �ΰ� ����
	bool jetIsRolling;				// ������ �Ѹ� ���� ����
	REAL jetRollingTime;			// ������ �Ѹ� �ð�

	// ����Ʈ
	TrailEffect *pWingTrail[2];		// ���� Ʈ����
	JetFlameEffect *pFlameLeft;		// ���� ���� ��� �Ҳ�
	JetFlameEffect *pFlameRight;	// ���� ���� ��� �Ҳ�

	// ����
	Sound *pSndEngine;	// ���� ��� ���� (����)

public:
	JetFighter(Vector3 pos, Player *_player, char *_craftName);	// ������
	JetFighter(JetFighter& _obj);								// ���� ������
	~JetFighter();												// �Ҹ���

	// Getter & Setter
	string* getCraftNamePtr();						// ����� ��Ī �����͸� ��ȯ
	list<FlightObject*> *getRadarListPtr();			// ���̴� ����Ʈ �����͸� ��ȯ
	void setRespawnLeftTime(REAL time);				// ��Ȱ ��� �ð� ����
	REAL getRespawnLeftTime();						// ��Ȱ ��� �ð� ��ȯ
	REAL getRespawnReqTime();						// ��Ȱ �䱸 �ð� ��ȯ 
	REAL getOffCourseTime();						// �ڽ��� ��Ż�� �ð��� ��ȯ
	void setOffCourseTime(REAL time);				// �ڽ��� ��Ż�� �ð��� ����
	void setLockonWarningState(jet_warning state);	// ���� ��� ���¸� ����
	jet_warning getLockonWarningState();			// ���� ��� ���¸� ��ȯ
	void setAltitudeWarningState(bool warning);		// �� ��� ���¸� ����
	bool getAltitudeWarningState();					// �� ��� ���¸� ��ȯ
	void setStallWarningState(bool warning);		// �Ǽ� ��� ���¸� ����
	bool getStallWarningState();					// �Ǽ� ��� ���¸� ��ȯ
	void setRadarRadius(REAL rad);					// ���̴� �ݰ� ����
	REAL getRadarRadius();							// ���̴� �ݰ� ��ȯ
	WpnLockonMissile* getBaseWeaponPtr();			// �ֹ���(�����) ������ ��ȯ
	WpnLockonMissile* getFirstWeaponPtr();			// �ֹ���(�̻���) ������ ��ȯ
	bool getJetIsRolling();							// ������ �Ѹ� ���� ��ȯ
	void setJetIsRolling(bool rolling);				// ������ �Ѹ� ���� ����

	// Method
	void respawnFigther(Vector3 pos, Vector3 direction);	// �����⸦ ��Ȱ��Ŵ
	char* toGamePacket(GamePacket *pPacket);				// ���� ��Ŷ�� �ʿ��� �������� ä��
	char* fromGamePacket(GamePacket *pPacket);				// Ŭ���� ���� �ɹ��� ��Ŷ �������� ä��

	// Virtual Function
	virtual bool checkObjectCollision();			// ������ �浹�� Ȯ��
	virtual bool draw();							// �����⸦ �׸�
	virtual bool destroy();							// ����ü�� �ı���
	virtual bool update(REAL timeElapsed);			// ����ü�� ������Ʈ

private:
	// Update
	void updateOffCourse(REAL timeElapsed);		// �ڽ� ��Ż ������Ʈ
	void updateRadar();							// ���̴� ����Ʈ ������Ʈ
	void updateWarning();						// ��� ���� ������Ʈ
	void updateBoost(REAL timeElapsed);			// �ν��� ������Ʈ
	void updateTrail(REAL timeElapsed);			// Ʈ���� ����Ʈ ������Ʈ
	void updateFlameColor(REAL timeElapsed);	// ���� ��뷮�� ���� �Ҳ� ����Ʈ ���� ������Ʈ
};

#endif