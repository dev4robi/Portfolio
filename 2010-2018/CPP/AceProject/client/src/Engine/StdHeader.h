#ifndef __STD_HEADER_H__
#define __STD_HEADER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : StdHeader.h
*	- Last Update : 2016.05.15
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 자주 사용되는 헤더들의 집합. 거의 모든 헤더파일에서 호출하므로 잦은 수정 금지 요함.
*
*	- Dependency : None
*
*   - Version(v1): 기초 라이브러리 추가됨.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// 실행 모드
//#define __DEBUG__
#ifndef __DEBUG__
	#define __RELEASE__
#endif

// 경고 제거
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// 게임 버전
#define GAME_VERSION "1.0.0"

// 스탠다드 라이브러리
#include <stdlib.h>
#include <string>
#include <atlbase.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
//#include <WinSock2.h>

// 내 라이브러리
#include "GameObject.h"
#include "GameManager.h"
#include "Library/Point2D.h"
#include "Library/Size2D.h"
#include "Library/MyMath.h"

// 서드 파티 라이브러리
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Library/glm/glm.h"
#include "Library/dhpoware/mathlib.h"

// 라이브러리 링크
#pragma comment(lib, "winmm.lib")	// 윈도우 멀티미디어 라이브러리 링크
#pragma comment(lib, "ws2_32.lib")	// 윈도우 소켓 라이브러리 링크

// 네임스페이스
using namespace std;

// 프로그래머 정의 자료형
typedef unsigned long ID;
typedef int INT;
typedef float REAL;

// 전역 변수
extern wstring g_wsGameTitle;		// 게임 타이틀
extern GameManager* g_pGameManager;	// 게임 매니저 포인터

#endif