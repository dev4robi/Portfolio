#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : InputManager(v2)
*	- Last Update : 2016.07.20
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 키 입력에 관한 정보를 보관 및 관리하는 클래스.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 인풋 매니저 기초 설계. (2016.05.11)
*	- Version(v2): 인풋 매니저 마우스/키보드 기능 재설계로 강화. (2016.07.20)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

enum mouse_state {
	MOUSE_UPHOLDING = 0, MOUSE_DOWN, MOUSE_DOWNHOLDING, MOUSE_UP, MOUSE_UNKNOWN
};

enum mouse_button {
	MOUSE_LEFTBTN = 0, MOUSE_RIGHTBTN, MOUSE_WHEELUPBTN, MOUSE_WHEELDOWNBTN, MOUSE_WHEELBTN // 주의!: [MOUSEMAP_MAX_SIZE = 32]
	//								   마우스 휠 업		 마우스 휠 다운		 마우스 휠 버튼 자체
};

enum key_state {
	KEY_UPHOLDING = 0, KEY_DOWN, KEY_DOWNHOLDING, KEY_UP, KEY_UNKNOWN
};

class InputManager : public GameObject {
private:
	static const int KEYMAP_MAX_SIZE = 256;		// 키 맵의 최대 크기
	static const int MOUSEMAP_MAX_SIZE = 32;	// 마우스 맵의 최대 크기
	REAL KEY_HOLDING_TIME;						// 키보드가 눌려/떼여져 있음으로 인지하는 시간
	REAL MOUSE_HOLDING_TIME;					// 마우스가 눌려/떼여져 있음으로 인지하는 시간
	REAL MANAGER_MAX_HOLDING_SAVE_TIME;			// 마우스/키보드가 눌려/떼여진 시간의 최대 기록 시간
												// (ex 10.0 => 최대 10초간 떼여졌거나(-10.0), 눌려졌거나(+10.0)를 감지, 방금 누르거나 떼면 0.0)
	// 마우스
	POINT mousePos;								// 마우스의 x, y좌표
	bool mousemap[MOUSEMAP_MAX_SIZE];			// 마우스 버튼마다 눌러져 있나 떼어져 있나 관리하는 맵핑 번호
	REAL mousemapTime[MOUSEMAP_MAX_SIZE];		// 마우스 버튼마다 눌려진/떼여진 시간을 실수(REAL)로 관리
	mouse_state mouseState[MOUSEMAP_MAX_SIZE];	// 마우스 버튼마다 세부 상태를 관리 (눌렀나, 눌려져 있나, 떼었나, 떼여져 있나, +마우스 휠링)

	// 키보드
	char cLastInputChar;						// 마지막으로 입력된 키
	bool keymap[KEYMAP_MAX_SIZE];				// 키보드가 눌려져 있나 떼어져 있나 관리하는 맵핑 번호 (keyState 데이터보다 세분화되지 못한 데이터 map < state)
	REAL keymapTime[KEYMAP_MAX_SIZE];			// 키보드 버튼마다 눌려진/떼여진 시간을 실수(REAL)로 관리
	key_state keyState[KEYMAP_MAX_SIZE];		// 키보드 키마다 세부 상태를 관리 (누름, 눌려 있음, 뗌, 떼여져 있음) (0~127: 일반 키 / 128~255: 특수 키)

public:
	InputManager();								// 생성자
	InputManager(InputManager& manager);		// 복사 생성자
	~InputManager();							// 소멸자

	void setLastInputChar(TCHAR ch);			// 마지막으로 입력된 키보드 char 문자 설정
	char getLastInputChar();					// 마지막으로 입력한 키보드 char 문자 반환 (영문 채팅 등을 처리하기 위함)
	void setKeyHoldingStartTime(REAL time);		// 키 눌려있음/떼여있음 인지 시간 설정 (default: 0.01초)
	REAL getKeyHoldingStartTime();				// 키 눌려있음/뗴여있음 인지 시간 반환
	void setMouseHoldingStartTime(REAL time);	// 마우스 눌려있음/떼여있음 인지 시간 설정 (default: 0.01초)
	REAL getMouseHoldingStartTime();			// 마우스 눌려있음/떼여있음 인지 시간 반환 
	void setMaxHoldingSaveTime(REAL time);		// 최대 눌려있음/떼여있음 인지 저장 시간 설정 (default: 10.0초)
	REAL getMaxHoldingSaveTime();				// 최대 눌려있음/떼여있음 인지 저장 시간 반환

	void setMousePos(int _x, int _y);			// 마우스 위치를 설정
	POINT getMousePos();						// 마우스 위치를 반환
	void setMousemap(int button, bool isDown);	// 마우스맵 특정 버튼 상태를 설정
	bool getMousemapBool(int button);			// 마우스맵 특정 버튼 상태를 반환
	mouse_state getMouseState(int button);		// 특정 버튼의 상태를 반환(사용 권장)
	void setKeymap(int key, bool isDown);		// 키맵 특정 키의 상태를 설정
	bool getKeymap(int key);					// 키맵 특정 키의 상태를 반환
	key_state getKeyState(int key);				// 특정 키의 상태를 반환(사용 권장)
	REAL getKeyHoldTime(int key);				// 특정 키의 눌려진/떼여진 시간을 반환

	virtual bool update(REAL timeElapsed);		// 키보드 정보 업데이트
};

#endif