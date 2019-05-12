#ifndef __STD_HEADER_H__
#define __STD_HEADER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : StdHeader.h
*	- Last Update : 2016.05.15
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ���Ǵ� ������� ����. ���� ��� ������Ͽ��� ȣ���ϹǷ� ���� ���� ���� ����.
*
*	- Dependency : None
*
*   - Version(v1): ���� ���̺귯�� �߰���.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ���� ���
//#define __DEBUG__
#ifndef __DEBUG__
	#define __RELEASE__
#endif

// ��� ����
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// ���� ����
#define GAME_VERSION "1.0.0"

// ���Ĵٵ� ���̺귯��
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

// �� ���̺귯��
#include "GameObject.h"
#include "GameManager.h"
#include "Library/Point2D.h"
#include "Library/Size2D.h"
#include "Library/MyMath.h"

// ���� ��Ƽ ���̺귯��
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Library/glm/glm.h"
#include "Library/dhpoware/mathlib.h"

// ���̺귯�� ��ũ
#pragma comment(lib, "winmm.lib")	// ������ ��Ƽ�̵�� ���̺귯�� ��ũ
#pragma comment(lib, "ws2_32.lib")	// ������ ���� ���̺귯�� ��ũ

// ���ӽ����̽�
using namespace std;

// ���α׷��� ���� �ڷ���
typedef unsigned long ID;
typedef int INT;
typedef float REAL;

// ���� ����
extern wstring g_wsGameTitle;		// ���� Ÿ��Ʋ
extern GameManager* g_pGameManager;	// ���� �Ŵ��� ������

#endif