#ifndef __LOBBY_FRAME_H__
#define __LOBBY_FRAME_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : LobbyFrame(v1)
*	- Last Update : 2016.08.25
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� �κ� ������.
*
*	- Dependency : [StdHeader.h, GameFrame.h], Player.h
*
*   - Version(v1): LobbyFrame ���� ����. (2016.08.25)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameFrame.h"

class RectViewer;
class LogoViewer;

class LobbyFrame : public GameFrame {
private:
	enum LobbySelect {
		SELECT_NONE, SELECT_SINGLEPLAY, SELECT_MULTIPLAY, SELECT_PLAYER, SELECT_CREDIT, SELECT_EXIT,
		SELECT_SETUP_NAME, SELECT_SETUP_IP, SELECT_SETUP_OK
	};
	
	Vector3 vecLobbyDefaultFontColor3;	// �κ��������� ��Ʈ ����Ʈ ����

	char cPlayerName[9];				// �÷��̾� �̸��� �ӽ� �����ϱ� ���� ����
	char cServerIP[16];					// ���� IP�ּҸ� �ӽ� �����ϱ� ���� ����
	REAL rLobbyElapseTime;				// �κ� ��� �ð�
	LobbySelect enCurrentSelect;		// ���� ���� �޴�
	LobbySelect enSelectedNextFrame;	// ���õ� ���� ������
	GLuint texture_backgrondID;			// ��� �ؽ�ó ���̵�
	GLuint texture_worldmapID;			// ��� ����� �ؽ�ó ���̵�
	GLuint texture_gridID;				// ��� �׸��� �ؽ�ó ���̵�

	RectViewer *pWorldmapViewer;		// ��� ����� ��� ���� ������
	RectViewer *pPlayerNameViewer;		// �÷��̾� �̸� ��� ���� ������ 
	RectViewer *pVersionViewer;			// ���� ��� ���� ������
	RectViewer *pSinglePracticeViewer;	// �̱� ����Ƽ�� ��� ���� ������
	RectViewer *pMultiplayCombatViewer;	// ��Ƽ�÷��� ��� ���� ������
	RectViewer *pPlayerSettingViewer;	// �÷��̾� ���� ��� ���� ������
	RectViewer *pCreditViewer;			// ũ���� ��� ���� ������
	RectViewer *pExitGameViewer;		// ���� ���� ��� ���� ������
	RectViewer *pFadeoutViewer;			// ���̵� �ƿ� ��� ���� ������
	RectViewer *pFadeinViewer;			// ���̵� �� ��� ���� ������ (ó�� �κ� �ε� �� ������ ���� ����ϴ� ���)
	
	bool bShowSetupPlayer;				// �¾� �÷��̾� ���̱� ����
	LobbySelect enCurrentSetupSelect;	// �¾� �÷��̾�� ���� ������ ��
	bool bPlayerNameViewerErr;			// �¾� �÷��̾�� �÷��̾� �̸� �� �ùٸ� ���� �ƴ� �� true
	bool bServerIPViewerErr;			// �¾� �÷��̾�� ���� IP �� �ùٸ� ���� �ƴ� �� true
	RectViewer *pSetupPlayerNameViewer;	// �¾� �÷��̾�� �÷��̾� ���̵� ������ �� ����ϴ� ��� ������
	RectViewer *pSetupServerIPViewer;	// �¾� �÷��̾�� ���� �����Ǹ� ������ �� ����ϴ� ��� ������
	RectViewer *pSetupOkViewer;			// �¾� �÷��̾�� ���� Ȯ�� ������ ǥ���ϱ� ���� ��� ������

public:
	// ������ �Ҹ���
	LobbyFrame(Player *pPlayer = NULL, bool bFirstLoad = false);	// ���޹��� �÷��̾�(����Ʈ NULL), ù ��°�� ȣ��� ����(����Ʈ false)
	LobbyFrame(LobbyFrame &obj);
	~LobbyFrame();

	// Getter & Setter
	LobbySelect getLobbySelect();				// ���� �κ� ���ð� ��ȯ
	void setLobbySelect(LobbySelect select);	// ���� �κ� ���ð� ����

	// �޼ҵ�
	void doSinglePractice();						// �̱��÷��� ��ƾ ����
	void doMultiplayCombat();						// ��Ƽ�÷��� ��ƾ ����
	void doPlayerSetting();							// �÷��̾� ���� ��ƾ ����
	void doCredit();								// ũ���� ��ƾ ����
	void doExitGame();								// ���� ���� ����
	void changeSelect(bool moveUp, bool isSetup);	// ���� ����

	// ���� �Լ�
	virtual void updateKey(REAL timeElapsed);
	virtual bool updateFrame(REAL timeElapsed);
	
	// ���� �Լ�
private:
	void showSetup();												// �¾� â ���̱�
	void hideSetup();												// �¾� â �����
	void updateSetupViewerColorAndScale(LobbySelect enCurSelect);	// �¾� ��� ������ ����
	bool checkCollectIPv4Addr(const char *pStr);					// �ùٸ� IPv4 ���ڿ������� Ȯ��				
};

#endif