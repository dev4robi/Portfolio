#include "Engine/InputManager.h"

InputManager::InputManager() {
	// 클래스 설정 상수
	KEY_HOLDING_TIME = 0.01;
	MOUSE_HOLDING_TIME = 0.01;
	MANAGER_MAX_HOLDING_SAVE_TIME = 10.0;

	// 마우스 위치
	mousePos.x = 0;
	mousePos.y = 0;
	// 마우스 초기화
	for (int i = 0; i < InputManager::MOUSEMAP_MAX_SIZE; ++i) {
		mousemap[i] = false;
		mousemapTime[i] = 0.001;
		mouseState[i] = mouse_state::MOUSE_UPHOLDING;
	}

	// 키보드 초기화
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
	// *참고 1
	// (키/마우스가 (REAL)mouse/keymapTime[ ]의 값이 오래 떼여 있을 수록 MANAGER_MAX_HOLDING_SAVE_TIME에, 
	//  오래 눌려 있을수록 -MANAGER_MAX_HOLDING_SAVE_TIME에 가까워짐을 참고)
	
	// *참고 2
	// MOUSE_WHEEL(UP/DOWN/ )BTN = 보통 마우스 휠 버튼은 1개의 버튼으로 구성하지만,
	// 시스템 제한적인 부분으로 위로 스크롤(UP)버튼, 아래로 스크롤(DOWN)버튼, 클릭( )으로 구성함.
	// 휠을 올리거나 내릴 때 MOUSE_DOWN 이벤트만 발생하고 그 외의 상태는 MOUSE_UPHOLDING으로 대기함에 주의.

	// 마우스 업데이트
	for (int i = 0; i < InputManager::MOUSEMAP_MAX_SIZE; ++i) {
		// 마우스 상태 업데이트
		if (mousemap[i] == true && mousemapTime[i] > 0.0) {
			// 마우스 버튼을 누름
			mouseState[i] = mouse_state::MOUSE_DOWN;
			mousemapTime[i] = 0.0;
		}
		else if (mousemap[i] == false && mousemapTime[i] < 0.0) {
			// 마우스 버튼을 뗌
			mouseState[i] = mouse_state::MOUSE_UP;
			mousemapTime[i] = 0.0;
		}
		else if (mousemap[i] == true && mousemapTime[i] < -InputManager::MOUSE_HOLDING_TIME) {
			// 마우스 버튼을 누르고 있음
			mouseState[i] = mouse_state::MOUSE_DOWNHOLDING;

			// 휠 버튼일 경우 버튼 떼어짐 명령을 보내줌
			if (i == mouse_button::MOUSE_WHEELUPBTN || i == mouse_button::MOUSE_WHEELDOWNBTN) {
				mousemap[i] = false;
				mousemapTime[i] = 0.0;
				mouseState[i] = mouse_state::MOUSE_UPHOLDING;
			}
		}
		else if (mousemap[i] == false && mousemapTime[i] > InputManager::MOUSE_HOLDING_TIME) {
			// 마우스 버튼을 떼고 있음
			mouseState[i] = mouse_state::MOUSE_UPHOLDING;
		}
		else {
			// 떼여있음 -> 누름 -> 언노운 -> 눌려있음 -> 뗌 -> 언노운 -> 떼여있음 (누름과 눌려있음, 뗌과 떼여있음의 중간 대기상태)
			mouseState[i] = mouse_state::MOUSE_UNKNOWN;
		}

		// 마우스 시간 업데이트
		if (mousemap[i] == true) {
			// 버튼이 눌려 있는 상태(-)
			(mousemapTime[i] - timeElapsed > -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (mousemapTime[i] -= timeElapsed) : (mousemapTime[i] = -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
		else { 
			// 버튼이 떼여 있는 상태(+)
			(mousemapTime[i] + timeElapsed < InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (mousemapTime[i] += timeElapsed) : (mousemapTime[i] = InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
	}

	// 키보드 업데이트
	for (int j = 0; j < InputManager::KEYMAP_MAX_SIZE; ++j) {
		// 키보드 상태 업데이트
		if (keymap[j] == true && keymapTime[j] > 0.0) {
			// 키보드 버튼을 누름
			keyState[j] = key_state::KEY_DOWN;
			keymapTime[j] = 0.0;
		}
		else if (keymap[j] == false && keymapTime[j] < 0.0) {
			// 키보드 버튼을 뗌
			keyState[j] = key_state::KEY_UP;
			keymapTime[j] = 0.0;
		}
		else if (keymap[j] == true && keymapTime[j] < -InputManager::KEY_HOLDING_TIME) {
			// 키보드 버튼을 누르고 있음
			keyState[j] = key_state::KEY_DOWNHOLDING;
		}
		else if (keymap[j] == false && keymapTime[j] > InputManager::KEY_HOLDING_TIME) {
			// 키보드 버튼을 떼고 있음
			keyState[j] = key_state::KEY_UPHOLDING;
		}
		else {
			// 떼여있음 -> 누름 -> 언노운 -> 눌려있음 -> 뗌 -> 언노운 -> 떼여있음 (누름과 눌려있음, 뗌과 떼여있음의 중간 대기상태)
			keyState[j] = key_state::KEY_UNKNOWN;
		}

		// 키보드 시간 업데이트
		if (keymap[j] == true) {
			// 키가 눌려 있는 상태(-)
			(keymapTime[j] - timeElapsed > -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (keymapTime[j] -= timeElapsed) : (keymapTime[j] = -InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
		else {
			// 키가 떼여 있는 상태(+)
			(keymapTime[j] + timeElapsed < InputManager::MANAGER_MAX_HOLDING_SAVE_TIME) 
			? (keymapTime[j] += timeElapsed) : (keymapTime[j] = InputManager::MANAGER_MAX_HOLDING_SAVE_TIME);
		}
	}

	return true;
}