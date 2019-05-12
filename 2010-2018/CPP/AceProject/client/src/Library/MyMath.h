#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <windows.h>

class Vector3;
class Matrix4;
class GameCamera;

namespace mymath {

	static const float PI = 3.141592653589;

	bool charIsNum(char c);																	// 전달받은 char형 아스키 코드가 숫자 문자라면 true 반환	
	int powd(int x, int y);																	// x의 y승을 구하여 반환
	int randomInt(int min, int max);														// min ~ max 사이의 무작위 정수 반환
	float randomFloat();																	// 0.0 ~ 1.0 사이의 무작위 소수 반환 (정밀도 - 0.001)
	float kph2kps(float kmPerHour);															// 시속을 초속으로 환산
	float degree360(float degree);															// 0~359 사이의 각도로 변환
	float rad2deg(float rad);																// 라디안을 각도로		
	float deg2rad(float deg);																// 각도를 라디안으로
	float distanceBetween2Points(POINT pt1, POINT pt2);										// 두 점 사이의 거리를 반환
	bool isPointInOval(POINT pt, POINT ovalPt, float ovalWidth, float ovalHeight);			// 점이 타원 안에 있는지 여부를 반환
	Vector3 Vecor3Transform(const Vector3 &pos, const Matrix4 &mat);						// 
	bool isPointInRect(POINT pt, float xTop, float yTop, float xBot, float yBot);			// 사각형 안에 점이 있는 여부를 반환
}

#endif
