#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__

// [Text Setting]
extern const char PADDING_CHAR;	// 패딩용 문자
extern const int TEXT_MAX;		// 매트릭스 최대 길이
extern char** planeTextMat;		// 동적 텍스트 매트릭스
extern int textMatCol;			// 텍스트 매트릭스 열
extern int textMatRow;			// 텍스트 매트릭스 행

// [Key Matrix Setting]
extern int lowBound;			// 키의 최소값
extern int highBound;			// 키의 최대값
extern int keyMatSize;			// 키 메트릭스의 사이즈(기본 3x3)

// [Charset and zStar Setting]
extern const int charCnt;		// 문자의 개수
extern int zStarCnt;			// zStar Count
extern int* zStar;				// zStar
extern int* zStarInv;			// zStar^-1 (Inverse)

#endif