#include "MyMath.h"
#include "Library/dhpoware/mathlib.h"

bool mymath::charIsNum(char c) {
	if (c > 47 && c < 58) return true;
	else return false;
}

int mymath::powd(int x, int y) {
	int rtInt = x;

	if (y <= 0) return 1;

	for (int i = 0; i < y; ++i) {
		rtInt *= x;
	}

	return rtInt;
}

int mymath::randomInt(int min, int max) {
	return (rand() % (max - min)) + min;
}

float mymath::randomFloat() {
	return (rand() % 1001) * 1000.0; // 0.000 ~ 1.000 (0.001 정밀도)
}

float mymath::kph2kps(float kmPerHour) {
	static const float VAR = 1000.0 / 60.0 / 60.0;

	return kmPerHour * VAR;
}

float mymath::degree360(float degree) {
	while (degree < 0.0) {
		degree += 360.0;
	}
	while (degree >= 360.0) {
		degree -= 360.0;
	}

	return degree;
}

float mymath::rad2deg(float rad) {
	return rad * 180.0 / PI;
}

float mymath::deg2rad(float deg) {
	return deg * PI / 180.0;
}

bool mymath::isPointInOval(POINT pt, POINT ovalPt, float ovalWidth, float ovalHeight) {

	// ( x - a ) * ( x - a )   ( y - b ) * ( y - b )			// x = 점의 x좌표, a = 타원의 x좌표, w = 타원의 넓이 절반
	// --------------------- + --------------------- <= 1.00	// y = 점의 y좌표, b = 타원의 y좌표, h = 타원의 높이 절반
	//      ( w  *  w )             ( h  *  h )

	return ((((pt.x - ovalPt.x) * (pt.x - ovalPt.x)) / (ovalWidth * ovalWidth))
		    + (((pt.y - ovalPt.y) * (pt.y - ovalPt.y)) / (ovalHeight * ovalHeight)) <= 1.00);
}

float mymath::distanceBetween2Points(POINT pt1, POINT pt2) {
	return sqrt(pow(pt1.x - pt2.x, 2.0) + pow(pt1.y - pt2.y, 2.0));
}

Vector3 mymath::Vecor3Transform(const Vector3 &pos, const Matrix4 &mat) {
	Vector3 rtVec;

	rtVec.x = (pos.x * mat[0][0]) + (pos.y * mat[1][0]) + (pos.z * mat[2][0]) + mat[3][0];
	rtVec.y = (pos.x * mat[0][1]) + (pos.y * mat[1][1]) + (pos.z * mat[2][1]) + mat[3][1];
	rtVec.z = (pos.x * mat[0][2]) + (pos.y * mat[1][2]) + (pos.z * mat[2][2]) + mat[3][2];
	
	return rtVec;
}

bool mymath::isPointInRect(POINT pt, float xTop, float yTop, float xBot, float yBot) {
	if (((pt.x >= xTop && pt.x <= xBot) && (pt.y <= yTop && pt.y >= yBot))) {
		return true;
	}

	return false;
}
