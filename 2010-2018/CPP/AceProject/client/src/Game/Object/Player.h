#ifndef __PLAYER_H__
#define __PLHAER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Player(v2)
*	- Last Update : 2016.09.29
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���ӿ��� �����Ǵ� ����(�÷��̾�)�� ������ ������ ���� Ŭ����.
*
*	- Dependency : [StdHeader.h], JetFighter.h, GameCamera.h, DrawManager.h, GameWidget.h, UI.h
*
*   - Version(v1): �÷��̾� Ŭ���� ���� ���� (2016.07.11)
*	- Version(v2): �÷��̾� ��Ʈ��ũ ������Ʈ�� ���θ� ���� �ɹ� �߰�. (2016.09.29)
*	
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GameCamera;
class GameWidget;
class JetFighter;
class Missile;

// For UI
class Test2DWidget;
class RadarLockon;
class SpeedAltMeter;
class RadarMap;
class Missile;
class StateSys;
class UImaster;

enum camera_option {
	CAM_DEFAULT, CAM_FIRST_PERSON, CAM_THIRD_PERSON
};

class Player : public GameObject {
private:
	string name;						// �÷��̾� ��
	int number;							// �÷��̾� ��ȣ
	JetFighter *pJet;					// �÷��̾� ���� ������ ������
	
	bool bNetworkDisconnected;			// ��Ƽ�÷��� ��Ʈ��ũ ���� ����� ����
	bool bNetworkUpdated;				// ��Ƽ�÷��� ��Ʈ��ũ ������Ʈ ����

	int playerKills;					// �÷��̾� ó�� Ƚ��
	int playerScore;					// �÷��̾� ȹ�� ����
	int playerDeathCount;				// �÷��̾� ��� Ƚ��

	GameCamera *pCamera;				// ���� ī�޶� ������
	camera_option camOption;			// ī�޶� �ɼ�
	bool camChacingMissile;				// ī�޶� �̻��� ����
	bool camShowBehind;					// ī�޶� �Ĺ� ��� �ɼ�

	UImaster *pUImanager;					// �÷��̾��� UI���� ������
	list<GameWidget*> playerUIDrawList;		// �ش� �÷��̾�Ը� ���̴� UI���� ����Ʈ (�÷��̾��� ��� UI������ ���������� �̰��� �����)

public:
	// Cons & Des
	Player(int _number, string _name);
	Player(Player& player);
	~Player();

	// Getter & Setter
	bool getNetworkDisconnected();						// ��Ʈ��ũ ���� ���� ���� ��ȯ
	void setNetworkDisconnected(bool disconnected);		// ��Ʈ��ũ ���� ���� ���� ����
	bool getNetworkUpdated();							// ��Ʈ��ũ ������Ʈ�� ���� ��ȯ
	void setNetworkUpdated(bool updated);				// ��Ʈ��ũ ������Ʈ�� ���� ����
	int getPlayerKills();								// �÷��̾� ó�� Ƚ���� ��ȯ
	void setPlayerKills(int val);						// �÷��̾� ó�� Ƚ���� ����
	int getPlayerScore();								// �÷��̾� ������ ��ȯ
	void setPlayeSocre(int val);						// �÷��̾� ������ ����
	int getPlayerDeathCount();							// �÷��̾� ��� Ƚ���� ��ȯ
	void setPlayerDeathCount(int val);					// �÷��̾� ��� Ƚ���� ����
	void setPlayerScore(int score);						// �÷��̾� ������ ����
	void setCamChacingMissile(bool chacing);			// ī�޶� �̻��� ���� �ɼ��� ����
	bool getCamShowBehind();							// �Ĺ� ī�޶� ��� �������� ��ȯ
	void setCamShowBehind(bool useBehind);				// �Ĺ� ī�޶� ��뿩�� ���� ����
	camera_option getCameraOption();					// ī�޶� �ɼ� ���¸� ��ȯ
	void setCameraOption(camera_option option);			// ī�޶� �ɼ��� ����
	GameCamera* getPlayerCamera();						// �÷��̾��� ī�޶� ������ ��ȯ
	void setPlayerJet(JetFighter* jet);					// �÷��̾��� ������ ����
	JetFighter* getPlayerJet();							// �÷��̾��� ������ ������ ��ȯ
	void setPlayerNumber(int playerNumber);				// �÷��̾� ��ȣ ����
	int getPlayerNumber();								// �÷��̾� ��ȣ ��ȯ
	void setPlayerName(char *newName);					// �÷��̾� �̸� ����
	string getPlayerName();								// �÷��̾� �̸� ��ȯ
	string* getPlayerNamePtr();							// �÷��̾� �̸� ������ ��ȯ
	UImaster *getUImanagerPtr();						// �÷��̾� UI �������� ������ ��ȯ
	list<RadarLockon*>* getRadarLockonUIListPtr();		// ���̴� ���� UI����Ʈ ������ ��ȯ
	list<GameWidget*>* getPlayerUIDrawListPtr();		// �÷��̾��� UI ��ο� ����Ʈ ������ ��ȯ
	SpeedAltMeter* getSpeedAltMeterPtr();				// �÷��̾� �������� �ӵ����� ������ ��ȯ
	RadarMap* getRadarMapPtr();							// �÷��̾� ���̴� ���� ������ ��ȯ

	// Method
	static bool cmpScore(const Player *p1, const Player *p2);	// �÷��̾��� ���� ��� ��
	static bool cmpNumber(const Player *p1, const Player *p2);	// �÷��̾��� ��ȣ ��

	void updateCamera(REAL timeElapsed);				// �÷��̾��� ī�޶� ������Ʈ
	void updatePlayer(REAL timeElapsed);				// �÷��̾� ������Ʈ
	void updatePlayerUI(REAL timeElapsed);				// �÷��̾� UI ������Ʈ
};

#endif