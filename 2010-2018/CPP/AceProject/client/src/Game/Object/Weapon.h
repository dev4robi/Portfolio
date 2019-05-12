#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Engine/StdHeader.h"

class JetFighter;
class GameWidget;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : Weapon.h(v1)
*	- Last Update : 2016.07.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �����Ⱑ ������ ���� Ŭ����.
*
*	- Dependency : [StdHeader.h], DrawManager.h
*
*   - Version(v1): ���� Ŭ���� ���� ����. (2016.07.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Weapon : public GameObject {
public:
	enum weapon_type {
		WEAPON_DEFAULT, WEAPON_GUN, WEAPON_MISSILE
	};

protected:
	weapon_type wpnType;		// ���� ����
	int wpnID;					// ���� ���̵�
	JetFighter *pWpnOwner;		// ���⸦ ������ ������
	string wpnName;				// ���� �̸�
	GLMmodel *pWpnModel;		// .obj ���� �� ������
	
	int wpnQuantity;			// ���� ���� ����
	int wpnMaxQuantity;			// �ִ� ����
	REAL wpnAoE;				// Ÿ�� ����
	REAL wpnDamage;				// ���ط�
	REAL wpnFirePerSecond;		// �ʴ� ���� �߻� ��
	REAL wpnMissileSpeed;		// �߻�ü �ӵ�
	REAL wpnMissileDuration;	// �߻�ü ���� �ð�

	bool wpnIsReady;			// ���� ��� ���� ����

public:
	Weapon(JetFighter *pOnwer, int id);	// ������
	Weapon(Weapon& obj);				// ���� ������
	~Weapon();							// �Ҹ���

	JetFighter* getWpnOwnerPtr();				// ���� ���� ������ �����͸� ��ȯ
	void setWpnType(weapon_type type);			// ���� ������ ����
	weapon_type getWpnType();					// ���� ������ ��ȯ
	void setQuantity(int quantity);				// ������ ���� ������ ����
	int getQuantity();							// ������ ���� ������ ��ȯ
	void setMaxQuantity(int quantity);			// ������ �ִ� ������ ����
	int getMaxQuantity();						// ������ �ִ� ������ ��ȯ
	void setWpnDamage(REAL damage);				// ������ ���ط��� ����
	REAL getWpnDamage();						// ������ ���ط��� ��ȯ
	void setFirePerSecond(REAL firePerSecond);	// ������ �ʴ� �߻緮�� ����
	REAL getFirePerSecond();					// ������ �ʴ� �߻緮�� ��ȯ
	void setWpnIsReady(bool ready);				// ������ �غ� ���¸� ����
	bool getWpnIsReady();						// ������ �غ� ���¸� ��ȯ
	void setWpnAoE(REAL aoe);					// ������ �ݰ��� ����
	REAL getWpnAoE();							// ������ �ݰ��� ��ȯ
	GLMmodel* getWpnModelPtr();					// ����(�̻���) �� ������ ��ȯ
	REAL getMissileDuration();					// �̻��� ���� �ð��� ��ȯ
	REAL getMissileSpeed();						// �̻��� �ӵ��� ��ȯ


	virtual void drawWeapon();					// ���� �׸���
	virtual void fireWeapon();					// ���⸦ �߻�(���)
	virtual void update(REAL timeElapsed);		// ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : WpnLockonMissile(v2)
*	- Last Update : 2016.08.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �̻��� �� ���� ���� Ŭ����.
*
*	- Dependency : [StdHeader.h],  ResourceManager.h, JetFigher.h, GameWidget.h, UI.h, FlightObject.h, SoundManager.h
*
*   - Version(v1): ���� �̻��� Ŭ���� ���� ����. (2016.07.20)
*	- Version(v2): ����� ���� ������ �� �ֵ��� Ŭ���� ��� ��ȭ. (2016.08.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class LockonUI;
class Sound;

class WpnLockonMissile : public Weapon {
protected:
	GLuint lockonUITexture;					// ���� UI �ؽ�ó ��ȣ
	list<JetFighter*> wpnLockonTgtList;		// ���� Ÿ�� ������ ����Ʈ
	list<LockonUI*> wpnLockonUIList;		// ���� UI ����Ʈ

	bool wpnTgtChangeRequested;			// Ÿ�� ���� ��û ���θ� ����
	int wpnNextFireMissileNum;			// ������ �߻�� �̻��� ��ȣ
	int wpnLockonCount;					// ���ÿ� ������ �� �ִ� ������ ���� (�׻� �̻��� �������� ���ų� ����)
	int wpnMissileCount;				// �̻��� ����					
	vector<REAL> wpnFireLeftTime;		// �̻��Ϻ� ���� �߻�ð�
	REAL wpnLockonSpeed;				// �̻��� ���� �ӵ�
	REAL wpnLockonRadOnScr;				// ȭ�鿡���� ���� ���� ������
	REAL wpnLockonRange;				// ���� ����

	string wpnFireSoundName;			// ���� �߻� �������� �̸�
	Sound *pWpnFireSound;				// ���� �߻� ���� (�����ϴ� ������ ��� ���� ���� �� ���� �ҽ��� ����)

public:
	WpnLockonMissile(JetFighter *pOwner, int id);	// ������
	WpnLockonMissile(WpnLockonMissile& obj);		// ���� ������
	~WpnLockonMissile();							// �Ҹ���

	void wpnTgtChangeRequest();						// Ÿ�� ������ ��û
	int getWpnMissileCount();						// �̻��� ������ ��ȯ

	list<LockonUI*>* getWpnLockonUIListPtr();		// ���� UI ����Ʈ ������ ��ȯ
	vector<REAL>* getWpnFireLeftTimePtr();			// �̻��Ϻ� ���� �߻�ð��� ����� ���� ������ ��ȯ

	virtual void drawWeapon();						// ���� �׸���
	virtual void fireWeapon();						// ���� �߻�
	virtual void stopFireWeapon();					// ���� �߻� ����
	virtual void update(REAL timeElapsed);			// ������Ʈ

private:
	void updateRadarLockonIsLocked(LockonUI *pLockonUI, bool isLocked);			// ���̴� ���� UI�� lockedByWeapon �� �����Ͽ� ���� ������ �����ϱ� ����
	bool isJetInLockonRad(JetFighter *jet);										// �Ű������� ���޵� �����Ⱑ ȭ��� ���� ���� �������� ������ true��ȯ
	bool isTargtInLockonList(JetFighter *target);								// Ÿ�� �����Ⱑ wpnLockonTgtList�� ���ԵǾ� �ִ��� ���θ� ��ȯ
	void initLockonUI(LockonUI *pObj);											// ���� UI��ü�� ������ �ʱ�ȭ��Ŵ
	void initLockonUIAll();														// ��� UI��ü�� ������ �ʱ�ȭ��Ŵ
	int getLockonUITgtCount();													// ���� UI�� �����ϰ� �ִ� ����� ���� ��ȯ
};

#endif