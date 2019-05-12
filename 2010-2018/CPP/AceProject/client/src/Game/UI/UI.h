#ifndef __UI_H__
#define __UI_H__

#include "Engine/GameWidget.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	< Content of UI.h >
*
*	 0. UImaster(v4) - 2016.08.23
*	 1. RaderLockon(v1) - 2016.07.12
*	 2. SpeedAltMeter(v1) - 2016.07.18
*	 3. RadarMap(v1) - 2016.07.18
*	 4. LockonUI(v1) - 2016.07.23
*	 5. StateSys(v2) - 2016.08.11
*	 6. WarningSys(v1) - 2016.08.11
*	 7. RespawnUI(v1) - 2016.08.18
*	 8. ScoreBoard(v1) - 2016.08.20
*	 9. MissionMsg(v1) - 2016.08.23
*	10. ScreenEffect(v1) - 2016.10.10
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ui {
	enum UI_COLOR_ENUM {
		cyan = 0, red, blue, white
	};

	inline Vector3 GetSavedUIColor(UI_COLOR_ENUM status) {
		Vector3 rtVec;

		switch (status) {
		case UI_COLOR_ENUM::cyan:	rtVec.x = 0.33; rtVec.y = 1.00; rtVec.z = 1.00; break; // û��
		case UI_COLOR_ENUM::red:	rtVec.x = 1.00; rtVec.y = 0.15; rtVec.z = 0.15; break; // ����
		case UI_COLOR_ENUM::blue:	rtVec.x = 0.00; rtVec.y = 0.00; rtVec.z = 1.00; break; // �Ķ�
		case UI_COLOR_ENUM::white:	rtVec.x = 1.00; rtVec.y = 1.00; rtVec.z = 1.00; break; // �Ͼ�
		default: rtVec.x = 1.0, rtVec.y = 1.0, rtVec.z = 1.0;
		}

		return rtVec;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RaderLockon(v2)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���̴��� ������ ����ü ���� �׷����� ���� �������̽�.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h
*
*   - Version(v1): ���̴� ���� Ŭ���� ���� ����. (2016.07.12)
*	- Version(v2): ȭ�� ������ ��� ����� �׸� �� ����� ȭ��ǥ �ؽ�ó �ɹ� �߰� �� ���� ����. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UImaster;
class Player;
class JetFighter;

class RadarLockon : public GameWidget {
public:
	enum RadarLockonType {
		ally = 0, enemy
	};

private:
	REAL uiScale;				// UI ũ��
	JetFighter *targetFighter;	// ���̴� ������ �� Ÿ��
	JetFighter *ownerFighter;	// ���̴� ������ �ϴ� Ÿ��
	bool lockedByWeapon;		// ����� ���� ���µ� ���� ����
	RadarLockonType type;		// ���̴� ���� Ÿ��(����)
	GLuint arrowTextureID;		// ���̴� ���� ȭ��ǥ �ؽ�ó ��ȣ

public:
	RadarLockon();											// ������
	RadarLockon(JetFighter *caster, JetFighter *target);	// ������
	RadarLockon(RadarLockon& obj);							// ���� ������
	~RadarLockon();											// �Ҹ���

	void setLockedByWeapon(bool isLocked);	// ������ ���µ� ���θ� ����
	JetFighter* getTargetFighterPtr();		// ���̴� ������ �� Ÿ���� ������ ��ȯ

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : SpeedAltMeter(v1)
*	- Last Update : 2016.07.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �������� �ӵ��� ���� ǥ�����ִ� �������̽�.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h
*
*   - Version(v1): �ӵ� �� ���Ͱ� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SpeedAltMeter : public GameWidget {
private:
	UImaster *pUIMaster;		// UI ������ ������

	GLuint textureID;
	GLuint breakTextureID;
	GLuint boostingTextureID;
	JetFighter *pTargetJet;
	REAL speedMeter;
	REAL altMeter;

public:
	SpeedAltMeter(UImaster *pMaster);
	SpeedAltMeter(SpeedAltMeter& obj);
	~SpeedAltMeter();

	void setSpeed(REAL speed);
	void setAlt(REAL alt);
	void setTargetJet(JetFighter *jet);		// UIŸ���� �Ǵ� ������ ����
	JetFighter* getTargetJetPtr();			// UIŸ���� �Ǵ� ������ ������ ��ȯ

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RadarMap(v1)
*	- Last Update : 2016.07.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ���̴� �������� ���. �ڽŰ� ���� ��ġ�� �ľ��ϱ� ���� ��.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h, Terrain.h, Player.h
*				   FlightObject.h, CombatFrame.h
*
*   - Version(v1): ���̴� �� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Terrain;
class Player;
class FlightObject;

class RadarMap : public GameWidget {
private:
	UImaster *pUIMaster;					// UI ������ ������

	GLuint mapGridTextureID;				// �̴ϸ� ��� ���� �ؽ�ó
	GLuint jetTextureID;					// �̴ϸ� ������ �ؽ�ó
	GLuint missileTextureID;				// �̴ϸ� �̻��� �ؽ�ó
	GLuint radarRadTextureID;				// �̴ϸ� ���̴� �ݰ� �ؽ�ó

	Terrain *pTerrain;						// ���� ������
	Player *pPlayer;						// �÷��̾� ������
	list<FlightObject*> *pFlightObjList;	// ����ü�� ����Ʈ

	REAL currentScanDist;					// ���� ��ĵ ����
	REAL scanSpeed;							// ���̴� ��ĵ �ӵ�
public:
	RadarMap(UImaster *pMaster);			// ������
	RadarMap(RadarMap& obj);				// ���� ������
	~RadarMap();							// �Ҹ���

	void setTerrain(Terrain *terrain);		// ������ ����
	void setPlayer(Player *player);			// �÷��̾ ����
	void setScanSpeed(REAL speed);			// ���� �ӵ��� ����

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ

private:
	void drawObjIcon(list<FlightObject*>* pObjList, REAL mapWidth, REAL mapHeight);	// �̴ϸʿ� ����ü, �̻��� �������� �׸��� �Լ�
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LockonUI(v1)
*	- Last Update : 2016.07.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� UI�� ȭ�鿡 ǥ��.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, Player.h, FlightObject.h
*
*   - Version(v1): ���� Ŭ���� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class LockonUI : public GameWidget {
private:
	UImaster *pUIMaster;		// UI ������ ������

	REAL uiScale;				// UI ũ��
	JetFighter *pTargetFighter;	// ���⿡ ���� ������ �� Ÿ��
	JetFighter *pOwnerFighter;	// ����� ������ �õ� �ϴ� Ÿ��
	bool isLockonSuccess;		// ����� ���� ������ �Ϸ�� ����

public:
	LockonUI(JetFighter *pOwnerJet, GLuint texID);	// ������
	LockonUI(LockonUI &obj);						// ���� ������
	~LockonUI();									// �Ҹ���

	void setLockonSuccess(bool success);		// ���� ���� ���θ� ����
	bool getLockonSuccess();					// ���� ���� ���θ� ��ȯ
	void setTargetFighter(JetFighter *fighter);	// �̻��� ������ �� Ÿ�� ����
	JetFighter* getTargetFighterPtr();			// �̻��� ������ �� Ÿ���� ������ ��ȯ
	
	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : StateSys(v2)
*	- Last Update : 2016.08.11
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �ڽ��� ���¸� UI�� ���.
*
*	- Dependency : [StdHeader.h], [GameWidget.h], JetFighter.h, GLTextureFont.h, Player.h
*
*   - Version(v1): ���� �ý��� Ŭ���� ���� ����. (2016.08.??)
*   - Version(v2): ���� ��� �� ������Ʈ �߰�. (2016.08.11)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class StateSys : public GameWidget {
private:
	UImaster *pUIMaster;			// UI ������ ������

	Player *pOwnerPlayer;			// UI ������
	JetFighter *pOwnerJet;			// �������� ������
	GLuint jetTexture;				// ������ �̹��� �ؽ�ó
	GLuint misTexture;				// �̻��� �̹��� �ؽ�ó
	GLuint engTexture;				// ���� ��� �ؽ�ó

public:
	StateSys(UImaster *pMaster);		// ������
	StateSys::StateSys(StateSys &obj);	// ���� ������
	~StateSys();						// �Ҹ���

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : WarningSys(v3)
*	- Last Update : 2016.08.27
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ��� UI�� ���
*
*	- Dependency : [StdHeader.h], [GameWidget.h], SoundManager.h, JetFighter.h, GLTextureFont.h, Player.h
*
*   - Version(v1): ��� �˸� �ý��� Ŭ���� ���� ����. (2016.08.11)
*   - Version(v2): ���� �溸 �߰�. (2016.08.17)
*   - Version(v3): �ڽ� ��Ż ��� �߰�. (2016.08.27)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Sound;

class WarningSys : public GameWidget {
private:
	UImaster *pUIMaster;		// UI ������ ������

	Player *pOwnerPlayer;		// UI ������
	JetFighter *pOwnerJet;		// �������� ������

	// SYS �ؽ�ó
	bool isMsgDestroy;				// sys UI�� �ı����� ��Ʈ������ �����ϴ� ������
	REAL sysLeftDuration;			// sys UI ���� ���ӽð�
	GLuint sysTexture;				// �ý��� ǥ�� �ؽ�ó ��ȣ
	GLuint hitSysTexture;			// �̻��� ���� �� ǥ���� �ؽ�ó ��ȣ
	GLuint destroySysTexture;		// �� �ı� �� ǥ���� �ؽ�ó ��ȣ

	// �溸 �ؽ�ó
	GLuint missileAlertTexture;		// �̻��� �溸 �� ǥ���� �ؽ�ó ��ȣ
	GLuint lockonAlertTexture;		// �̻��� ���� �� ǥ���� �ؽ�ó ��ȣ
	GLuint pullUpTexture;			// �� ��� �溸 �ؽ�ó ��ȣ
	GLuint stallTexture;			// ���� �溸 �ؽ�ó ��ȣ
	GLuint offCourseTexture;		// �ڽ� ��Ż �ؽ�ó ��ȣ

	// �溸 ����
	Sound *pSndMssileAlert;		// �̻��� �溸 ����
	Sound *pSndLockonWarning;	// ���� ��� ����

public:
	WarningSys(UImaster *pMaster);		// ������
	WarningSys(WarningSys &obj);		// ���� ������
	~WarningSys();						// �Ҹ���

	void setSystemAlarm(bool isDestory, REAL time); // �ý��� �˶� �߰�

	virtual bool draw();					// ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// ������ �ı���
	virtual bool update(REAL timeElapsed);	// ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : RespawnUI(v1)
*	- Last Update : 2016.08.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ��Ȱ �� ����� ���� UI
*
*	- Dependency : [StdHeader.h], [GameWidget.h], GLTextureFont.h, Player.h
*
*   - Version(v1): ������ UI ���� ����. (2016.08.17)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class RespawnUI : public GameWidget {
private:
	UImaster *pUIMaster;		// UI ������ ������

	REAL respawnElapsedTime;	// ������ ��� �ð�
	Vector3 respawnCamPos;		// �������� �ӽ÷� ���� ī�޶� ��ġ
	Player *pOwnerPlayer;		// UI ������
	Player *pKillerPlayer;		// óġ�� �������� ������

public:
	RespawnUI(UImaster *pMaster);	// ������
	RespawnUI(RespawnUI &obj);		// ���� ������
	~RespawnUI();					// �Ҹ���

	void setKillerPlayer(Player *pPlayer);	// óġ�� �÷��̾� ����
	Player* getKillerPlayerPtr();			// óġ�� �÷��̾� ������ ��ȯ

	virtual bool draw();					// UI ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// UI ������ �ı���
	virtual bool update(REAL timeElapsed);	// UI ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ScoreBoard(v1)
*	- Last Update : 2016.08.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �÷��̾���� ������ ��Ÿ���� ��Ȳ��
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): ScoreBoard Ŭ���� ���� ����. (2016.08.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class CombatFrame;

class ScoreBoard : public GameWidget {
private:
	UImaster *pUIMaster;			// UI ������ ������
	CombatFrame *pFrame;			// �ĺ� ������ ������

	vector<Player*> scoreVector;	// ���ھ� ���� (������ ų�� ������ �÷��̾ front�� �����)

public:
	// ������ �Ҹ���
	ScoreBoard(UImaster *pMaster);
	ScoreBoard(ScoreBoard &obj);
	~ScoreBoard();

	// ���� �Լ�
	virtual bool draw();					// UI ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// UI ������ �ı���
	virtual bool update(REAL timeElapsed);	// UI ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : MissionMsg(v1)
*	- Last Update : 2016.08.23
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �̼� ���� �� ���Ḧ ǥ���ϴ� UI
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): MissionMsg Ŭ���� ���� ����. (2016.08.23)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class MissionMsg : public GameWidget {
private:
	UImaster *pUIMaster;			// UI ������ ������

	REAL startMsgLeftTime;			// �̼� ���� ���� ǥ�� �ð�
	REAL compeleteMsgLeftTime;		// �̼� ���� ���� ǥ�� �ð�
	REAL sessionDestructLeftTime;	// ���� �ı� ���� �ð�

	GLuint sysTexID;				// �ý��� �ؽ�ó ���̵�
	GLuint missionStartTexID;		// �̼� ���� �ؽ�ó ���̵�
	GLuint missionCompleteTexID;	// �̼� ���� �ؽ�ó ���̵�
	GLuint serverDisconnectedTexID;	// ���� ���� ����� �ؽ�ó ���̵�

public:
	// ������ & �Ҹ���
	MissionMsg(UImaster *pMaster);
	MissionMsg(MissionMsg &obj);
	~MissionMsg();

	// ���� & ����
	REAL getSessionDestructLeftTime();		// ���� �ı� ���� �ð� ��ȯ

	// ���� �Լ�
	virtual bool draw();					// UI ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// UI ������ �ı���
	virtual bool update(REAL timeElapsed);	// UI ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ScreenEffect(v1)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ȭ�鿡 Ư�� ���¿� ���� ȿ���� ����ϴ� Ŭ����.
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): ScreenEffect Ŭ���� ���� ����. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ScreenEffect : public GameWidget {
public:
	enum ScrEffectAction {				// ��ũ���� ������ ����Ʈ ȿ�� ������
		SCREFFECT_WARNGLOW, SCREFFECT_BOOST, SCREENEFFECT_DUST
	};

private:
	struct ScrEffect {					// ��ũ�� ȿ�� ����ü
		int iEffectID;					// ����ڰ� ������ ����Ʈ ID (����Ʈ���� ������ ��ȣ�� �ο� ����������, ���� ��ȣ��� ���ӽð��� ���ŵ�: ��Ȱ:1,�ν���:2,����:3)
		ScrEffectAction enAction;		// ������ ����Ʈ ȿ��
		GLuint uiTexID;					// ����Ʈ �ؽ�ó ID
		REAL rLeftTime;					// ����Ʈ ���� �ð�
		REAL rValue;					// ����Ʈ���� ����� ���� ����
	};

	UImaster *pUIMaster;				// UI ������ ������
	vector<ScrEffect*> scrEffectVector;	// ��ũ�� ȿ���� ���� ����

public:
	// ������ & �Ҹ���
	ScreenEffect(UImaster *pMaster);
	ScreenEffect(ScreenEffect& obj);
	~ScreenEffect();

	// ���� & ����
	void createEffect(REAL time, int effectID, ScrEffectAction action, GLuint texID = 0);	// �ش� ��ȣ�� ����Ʈ�� ���� ���� (�ؽ�ó ���̵� 0 ���� ������ �׼ǿ� ���� ����Ʈ �ؽ�ó ����)
	
	// �޼ҵ�
	void clearEffects();					// ����Ʈ ������ �����͵��� ��� ���

	// ���� �Լ�
	virtual bool draw();					// UI ������ ���ǵ� �׸��⸦ ����
	virtual bool destroy();					// UI ������ �ı���
	virtual bool update(REAL timeElapsed);	// UI ������ ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : UImaster(v5)
*	- Last Update : 2016.10.10
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �÷��̾��� ��� UI�� �����ϴ� Ŭ����
*
*	- Dependency : [StdHeader.h], GameObject.h
*
*   - Version(v1): UImaster Ŭ���� ���� ����. (2016.08.09)
*	- Version(v2): WarningSys Ŭ���� �߰�. (2016.08.11)
*	- Version(v3): RespawnUI Ŭ���� �߰� �� update, draw �κ� ó���� Player Ŭ�����κ��� �̰�. (2016.08.17)
*	- Version(v4): MissionMsg Ŭ���� �߰�. (2016.08.23)
*	- Version(v5): ScreenEffect Ŭ���� �߰�. (2016.10.10)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UImaster : public GameObject {
private:
	REAL scrWidth;							// �ػ� ��
	REAL scrHeight;							// �ػ� ����
	REAL dmgBlinkLeftTime;					// ���� ���� �� ���� �����ϴ� ���� �ð�

	Player *pOwnPlayer;						// ������ �÷��̾� ������
	list<GameWidget*> *pUIDrawList;			// �ش� �÷��̾��� UI ��ο� ����Ʈ ������

	StateSys *pStateSys;					// �ش� �÷��̾��� ���� �ý��� UI
	SpeedAltMeter *pSpeedAltMeter;			// �ش� �÷��̾��� �ӵ��� & ����
	list<RadarLockon*> radarLockonUIList;	// �ش� �÷��̾��� ���̴� ���� UI����Ʈ
	RadarMap *pRadarMap;					// �ش� �÷��̾��� ���̴� ��
	WarningSys *pWarningSys;				// �ش� �÷��̾��� ��� �ý��� UI
	RespawnUI *pRespawnUI;					// �ش� �÷��̾��� ������ UI
	ScoreBoard *pScoreBoard;				// �ش� �÷��̾��� ���ھ�� UI
	MissionMsg *pMissionMsg;				// �ش� �÷��̾��� �̼� �޽��� UI
	ScreenEffect *pScreenEffect;			// �ش� �÷��̾��� ��ũ�� ����Ʈ UI
	// ���⿡ UI ��� �߰� . . .

public:
	UImaster(Player *_pOwnPlayer);			// ������
	UImaster(UImaster &obj);				// ���� ������
	~UImaster();							// �Ҹ�

	// Getter & Setter
	REAL getScreenWidth();							// �ػ� �� ��ȯ
	REAL getScreenHeight();							// �ػ� ���� ��ȯ
	void setDmgBlinkTime(REAL time);				// �ǰ� �� �����ϴ� �ð� ����
	REAL getDmgBlinkTime();							// �ǰ� �� �����ϴ� �ð� ��ȯ
	Player* getOwnPlayerPtr();						// UI ������ �÷��̾� ������ ��ȯ
	StateSys* getStateSysPtr();						// ���� �ý��� UI ������ ��ȯ
	SpeedAltMeter* getSpeedAltMeterPtr();			// �ӵ� ���� UI ������ ��ȯ
	list<RadarLockon*>* getRadarLockonUIListPtr();	// ���̴� ���� UI ����Ʈ ������ ��ȯ
	RadarMap* getRadarMapPtr();						// ���̴� �� UI ������ ��ȯ
	WarningSys* getWarningSysPtr();					// ��� �ý��� UI ������ ��ȯ
	RespawnUI* getRespawnUIPtr();					// ������ UI ������ ��ȯ
	MissionMsg* getMissionMsgPtr();					// �̼� �޽��� UI ������ ��ȯ
	ScreenEffect* getScreenEffectPtr();				// ��ũ�� ����Ʈ UI������ ��ȯ

	void updateUI(REAL timeElapsed);				// UI ������Ʈ
};


#endif