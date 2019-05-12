#include "Engine/InputManager.h"

InputManager::InputManager() {
	// Ŭ���� ���� ���
	KEY_HOLDING_TIME = 0.01;
	MOUSE_HOLDING_TIME = 0.01;
	MANAGER_MAX_HOLDING_SAVE_TIME = 10.0;

	// ���콺 ��ġ
	mousePos.x = 0;
	mousePos.y = 0;
	// ���콺 �ʱ�ȭ
	for (int i = 0; i < InputManager::MOUSEMAP_MAX_SIZE; ++i) {
		mousemap[i] = false;
		mousemapTime[i] = 0.001;
		mouseState[i] = mouse_state::MOUSE_UPHOLDING;
	}

	// Ű���� �ʱ�ȭ
	cLastInputChar = '\0';
	for (int j = 0; j < InputManager::KEYMAP_MAX_SIZE; ++j) {
		keymap[j] = false;
		keymapTime[j] = 0.001;
		keyState[j] = key_state::KEY_UPHOLDING;
	}
}

InputManager::InputManager(InputManager& manager) {
	KEY_HOLDING_TIME = manager.KEY_HOLDING_TIME;
	MOUSE_HOLDING_TIME = manager.MOUSE_HOLDING_TIME;
	MANAGER_MAX_HOLDING_SAVE_TIME = manager.MANAGER_MAX_HOLDING_SAVE_TIME;

	mousePos.x = manager.mousePos.x;
	mousePos.y = manager.mousePos.y;
	for (int i = 0; i < InputManager::MOUSEMAP_MAX_SIZE; ++i) {
		mousemap[i] = manager.mousemap[i];
		mousemapTime[i] = manager.mousemapTime[i];
		mouseState[i] = manager.mouseState[i];
	}
	for (int i = 0; i < InputManager::KEYMAP_MAX_SIZE; ++i) {
		keymap[i] = manager.keymap[i];
		keymapTime[i] = manager.keymapTime[i];
		keyState[i] = manager.keyState[i];
	}
}

InputManager::~InputManager() {

}

void InputManager::setLastInputChar(TCHAR ch) {
	cLastInputChar = (char)ch;
}

char InputManager::getLastInputChar() {
	char rtChar = cLastInputChar;

	cLastInputChar = '\0';

	return rtChar;
}

void InputManager::setKeyHoldingStartTime(REAL time) {
	InputManager::KEY_HOLDING_TIME = time;
}

REAL InputManager::getKeyHoldingStartTime() {
	return InputManager::KEY_HOLDING_TIME;
}

void InputManager::setMouseHoldingStartTime(REAL time) {
	InputManager::MOUSE_HOLDING_TIME = time;
}

REAL InputManager::getMouseHoldingStartTime() {
	return InputManager::MOUSE_HOLDING_TIME;
}

void InputManager::setMaxHoldingSaveTime(REAL time) {
	InputManager::MANAGER_MAX_HOLDING_SAVE_TIME = time;
}

REAL InputManager::getMaxHoldingSaveTime() {
	return InputManager::MANAGER_MAX_HOLDING_SAVE_TIME;
}

void InputManager::setMousePos(int _x, int _y) {
	mousePos.x = _x;
	mousePos.y = _y;
}

POINT InputManager::getMousePos() {
	return mousePos;
}

void InputManager::setMousemap(int button, bool isDown) {
	mousemap[button] = isDown;
}

bool InputManager::getMousemapBool(int button) {
	return mousemap[button];
}

mouse_state InputManager::getMouseState(int button) {
	return mouseState[button];
}

void InputManager::setKeymap(int key, bool isDown) {
	keymap[key] = isDown;
}

bool InputManager::getKeymap(int key) {
	return keymap[key];
}

key_state InputManager::getKeyState(int key) {
	return keyState[key];
}

REAL InputManager::getKeyHoldTime(int key) {
	return keymapTime[key];
}

bool InputManager::update(REAL timeElapsed) {
	// *���� 1
	// (Ű/���콺�� (REAL)mouse/keymapTime[ ]�� ���� ���� ���� ���� ���� MANAGER_MAX_HOLDING_SAVE_TIME��, 
	//  ���� ���� �������� -MANAGER_MAX_HOLDING_SAVE_TIME�� ��������� ����)
	
	// *���� 2
	// MOUSE_WHEEL(UP/DOWN/ )BTN = ���� ���콺 �� ��ư�� 1���� ��ư���� ����������,
	// �ý��� �������� �κ����� ���� ��ũ��(UP)��ư, �Ʒ��� ��ũ��(DOWN)��ư, Ŭ��( )���� ������.
	// ���� �ø��ų� ���� �� MOUSE_DOWN �̺�Ʈ�� �߻��ϰ� �� ���� ���´� MOUSE_UPHOLDING���� ����Կ� ����.

	// ���콺 ������Ʈ
	for (int i = 0; i < InputManager::MOUSEMAP_MAX_SIZE; ++i) {
		// ���콺 ���� ������Ʈ
		if (mousemap[i] == true && mousemapTime[i] > 0.0) {
			// ���콺 ��ư�� ����
			mouseState[i] = mouse_state::MOUSE_DOWN;
			mousemapTime[i] = 0.0;
		}
		else if (mousemap[i] == false && mousemapTime[i] < 0.0) {
			// ���콺 ��ư�� ��
			mouseState[i] = mouse_state::MOUSE_UP;
			mousemapTime[i] = 0.0;
		}
		else if (mousemap[i] == true && mousemapTime[i] < -InputManager::MOUSE_HOLDING_TIME) {
			// ���콺 ��ư�� ������ ����
			mouseState[i] = mouse_state::MOUSE_DOWNHOLDING;

			// �� ��ư�� ��� ��ư ������ ����� ������
			if (i == mouse_button::MOUSE_WHEELUPBTN || i == mouse_button::MOUSE_WHEELDOWNBTN) {
				mousemap[i] = false;
				mousemapTime[i] = 0.0;
				mouseState[i] = mouse_state::MOUSE_UPHOLDING;
			}
		}
		else if (mousemap[i] == false && mousemapTime[i] > InputManager::MOUSE_HOLDING_TIME) {
			// ���콺 ��ư�� ���� ����
			mouseState[i] = mouse_state::MOUSE_UPHOLDING;
		}
		else {
			// �������� -> ���� -> ���� -> �������� -> �� -> ���� -> �������� (������ ��������, ���� ���������� �߰� ������)
			mouseState[i] = mouse_state::MOUSE_UNKNOWN;
		}

		// ���콺 �ð� ������Ʈ
		if (mousemap[i] == true) {
			// ��ư�� ���� �ִ� ����(-)
			(mousemapTime[i] - timeElapsed > -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (mousemapTime[i] -= timeElapsed) : (mousemapTime[i] = -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
		else { 
			// ��ư�� ���� �ִ� ����(+)
			(mousemapTime[i] + timeElapsed < InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (mousemapTime[i] += timeElapsed) : (mousemapTime[i] = InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
	}

	// Ű���� ������Ʈ
	for (int j = 0; j < InputManager::KEYMAP_MAX_SIZE; ++j) {
		// Ű���� ���� ������Ʈ
		if (keymap[j] == true && keymapTime[j] > 0.0) {
			// Ű���� ��ư�� ����
			keyState[j] = key_state::KEY_DOWN;
			keymapTime[j] = 0.0;
		}
		else if (keymap[j] == false && keymapTime[j] < 0.0) {
			// Ű���� ��ư�� ��
			keyState[j] = key_state::KEY_UP;
			keymapTime[j] = 0.0;
		}
		else if (keymap[j] == true && keymapTime[j] < -InputManager::KEY_HOLDING_TIME) {
			// Ű���� ��ư�� ������ ����
			keyState[j] = key_state::KEY_DOWNHOLDING;
		}
		else if (keymap[j] == false && keymapTime[j] > InputManager::KEY_HOLDING_TIME) {
			// Ű���� ��ư�� ���� ����
			keyState[j] = key_state::KEY_UPHOLDING;
		}
		else {
			// �������� -> ���� -> ���� -> �������� -> �� -> ���� -> �������� (������ ��������, ���� ���������� �߰� ������)
			keyState[j] = key_state::KEY_UNKNOWN;
		}

		// Ű���� �ð� ������Ʈ
		if (keymap[j] == true) {
			// Ű�� ���� �ִ� ����(-)
			(keymapTime[j] - timeElapsed > -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (keymapTime[j] -= timeElapsed) : (keymapTime[j] = -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
		else {
			// Ű�� ���� �ִ� ����(+)
			(keymapTime[j] + timeElapsed < InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (keymapTime[j] += timeElapsed) : (keymapTime[j] = InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
	}

	return true;
}