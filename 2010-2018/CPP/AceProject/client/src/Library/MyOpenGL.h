#ifndef __MY_OPENGL_H__
#define __MY_OPENGL_H__

#include "Engine/StdHeader.h"

struct GL_Fog {
	GLboolean fogUsing;		// 안개 사용 여부
	GLfloat fogDensity;		// 안개 밀도
	GLfloat fogFxStart;		// 안개 함수 시작 거리
	GLfloat fogFxEnd;		// 암개 함수 종료 거리
	GLfloat fogColor4[4];	// 안개 RGBA 색상
	GLfloat fogColorIndex;	// 안개 색상 인덱스
	GLuint fogMode;			// GL_EXP, GL_EXP2, GL_LINEAR
	GLuint fogHint;			// 안개 힌트(성능)
};

#endif