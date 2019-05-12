#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <windows.h>

class Vector3;
class Matrix4;
class GameCamera;

namespace mymath {

	static const float PI = 3.141592653589;

	bool charIsNum(char c);																	// ���޹��� char�� �ƽ�Ű �ڵ尡 ���� ���ڶ�� true ��ȯ	
	int powd(int x, int y);																	// x�� y���� ���Ͽ� ��ȯ
	int randomInt(int min, int max);														// min ~ max ������ ������ ���� ��ȯ
	float randomFloat();																	// 0.0 ~ 1.0 ������ ������ �Ҽ� ��ȯ (���е� - 0.001)
	float kph2kps(float kmPerHour);															// �ü��� �ʼ����� ȯ��
	float degree360(float degree);															// 0~359 ������ ������ ��ȯ
	float rad2deg(float rad);																// ������ ������		
	float deg2rad(float deg);																// ������ ��������
	float distanceBetween2Points(POINT pt1, POINT pt2);										// �� �� ������ �Ÿ��� ��ȯ
	bool isPointInOval(POINT pt, POINT ovalPt, float ovalWidth, float ovalHeight);			// ���� Ÿ�� �ȿ� �ִ��� ���θ� ��ȯ
	Vector3 Vecor3Transform(const Vector3 &pos, const Matrix4 &mat);						// 
	bool isPointInRect(POINT pt, float xTop, float yTop, float xBot, float yBot);			// �簢�� �ȿ� ���� �ִ� ���θ� ��ȯ
}

#endif
