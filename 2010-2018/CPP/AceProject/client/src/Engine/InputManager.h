#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : InputManager(v2)
*	- Last Update : 2016.07.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : Ű �Է¿� ���� ������ ���� �� �����ϴ� Ŭ����.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): ��ǲ �Ŵ��� ���� ����. (2016.05.11)
*	- Version(v2): ��ǲ �Ŵ��� ���콺/Ű���� ��� �缳��� ��ȭ. (2016.07.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

enum mouse_state {
	MOUSE_UPHOLDING = 0, MOUSE_DOWN, MOUSE_DOWNHOLDING, MOUSE_UP, MOUSE_UNKNOWN
};

enum mouse_button {
	MOUSE_LEFTBTN = 0, MOUSE_RIGHTBTN, MOUSE_WHEELUPBTN, MOUSE_WHEELDOWNBTN, MOUSE_WHEELBTN // ����!: [MOUSEMAP_MAX_SIZE = 32]
	//								   ���콺 �� ��		 ���콺 �� �ٿ�		 ���콺 �� ��ư ��ü
};

enum key_state {
	KEY_UPHOLDING = 0, KEY_DOWN, KEY_DOWNHOLDING, KEY_UP, KEY_UNKNOWN
};

class InputManager : public GameObject {
private:
	static const int KEYMAP_MAX_SIZE = 256;		// Ű ���� �ִ� ũ��
	static const int MOUSEMAP_MAX_SIZE = 32;	// ���콺 ���� �ִ� ũ��
	REAL KEY_HOLDING_TIME;						// Ű���尡 ����/������ �������� �����ϴ� �ð�
	REAL MOUSE_HOLDING_TIME;					// ���콺�� ����/������ �������� �����ϴ� �ð�
	REAL MANAGER_MAX_HOLDING_SAVE_TIME;			// ���콺/Ű���尡 ����/������ �ð��� �ִ� ��� �ð�
												// (ex 10.0 => �ִ� 10�ʰ� �������ų�(-10.0), �������ų�(+10.0)�� ����, ��� �����ų� ���� 0.0)
	// ���콺
	POINT mousePos;								// ���콺�� x, y��ǥ
	bool mousemap[MOUSEMAP_MAX_SIZE];			// ���콺 ��ư���� ������ �ֳ� ������ �ֳ� �����ϴ� ���� ��ȣ
	REAL mousemapTime[MOUSEMAP_MAX_SIZE];		// ���콺 ��ư���� ������/������ �ð��� �Ǽ�(REAL)�� ����
	mouse_state mouseState[MOUSEMAP_MAX_SIZE];	// ���콺 ��ư���� ���� ���¸� ���� (������, ������ �ֳ�, ������, ������ �ֳ�, +���콺 �ٸ�)

	// Ű����
	char cLastInputChar;						// ���������� �Էµ� Ű
	bool keymap[KEYMAP_MAX_SIZE];				// Ű���尡 ������ �ֳ� ������ �ֳ� �����ϴ� ���� ��ȣ (keyState �����ͺ��� ����ȭ���� ���� ������ map < state)
	REAL keymapTime[KEYMAP_MAX_SIZE];			// Ű���� ��ư���� ������/������ �ð��� �Ǽ�(REAL)�� ����
	key_state keyState[KEYMAP_MAX_SIZE];		// Ű���� Ű���� ���� ���¸� ���� (����, ���� ����, ��, ������ ����) (0~127: �Ϲ� Ű / 128~255: Ư�� Ű)

public:
	InputManager();								// ������
	InputManager(InputManager& manager);		// ���� ������
	~InputManager();							// �Ҹ���

	void setLastInputChar(TCHAR ch);			// ���������� �Էµ� Ű���� char ���� ����
	char getLastInputChar();					// ���������� �Է��� Ű���� char ���� ��ȯ (���� ä�� ���� ó���ϱ� ����)
	void setKeyHoldingStartTime(REAL time);		// Ű ��������/�������� ���� �ð� ���� (default: 0.01��)
	REAL getKeyHoldingStartTime();				// Ű ��������/������ ���� �ð� ��ȯ
	void setMouseHoldingStartTime(REAL time);	// ���콺 ��������/�������� ���� �ð� ���� (default: 0.01��)
	REAL getMouseHoldingStartTime();			// ���콺 ��������/�������� ���� �ð� ��ȯ 
	void setMaxHoldingSaveTime(REAL time);		// �ִ� ��������/�������� ���� ���� �ð� ���� (default: 10.0��)
	REAL getMaxHoldingSaveTime();				// �ִ� ��������/�������� ���� ���� �ð� ��ȯ

	void setMousePos(int _x, int _y);			// ���콺 ��ġ�� ����
	POINT getMousePos();						// ���콺 ��ġ�� ��ȯ
	void setMousemap(int button, bool isDown);	// ���콺�� Ư�� ��ư ���¸� ����
	bool getMousemapBool(int button);			// ���콺�� Ư�� ��ư ���¸� ��ȯ
	mouse_state getMouseState(int button);		// Ư�� ��ư�� ���¸� ��ȯ(��� ����)
	void setKeymap(int key, bool isDown);		// Ű�� Ư�� Ű�� ���¸� ����
	bool getKeymap(int key);					// Ű�� Ư�� Ű�� ���¸� ��ȯ
	key_state getKeyState(int key);				// Ư�� Ű�� ���¸� ��ȯ(��� ����)
	REAL getKeyHoldTime(int key);				// Ư�� Ű�� ������/������ �ð��� ��ȯ

	virtual bool update(REAL timeElapsed);		// Ű���� ���� ������Ʈ
};

#endif