#ifndef __DATA_PARSER_H__
#define _CRT_SECURE_NO_WARNINGS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : DataParser.h(v1)
*	- Last Update : 2016.10.18
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : *.dat 파일을 읽어서 원하는 내용을 정수, 실수(float), 문자열 로 뽑아내는 클래스.
*
*	// *.dat 파일의 구조는 아래와 같음. //
*	[Header]
*	[Attribute1] Data1 [/Attribute1]
*	[Attribute1] Data2 [/Attribute1]
*	[/Header]
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 데이터 파서 1차 설계 완성. (2016.10.18)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string>

class DataParser {
private:
	char *pHeaderPtr;			// Find 함수로 찾은 헤더 위치를 가르키는 포인터
	char *pHeaderEndPtr;		// Find 함수로 찾은 헤더의 끝나는 위치를 가르키는 포인터
	char *pAttributePtr;		// Find 함수로 찾은 속성 위치를 가르키는 포인터
	char *pAttributeData;		// Find 함수로 찾은 속성의 동적 할당된 데이터를 가르키는 포인터
	char *pDataBuffer;			// *.dat 파일의 바이트 스트림을 임시 복사해 둘 버퍼
	int iDataBufferByteSize;	// 데이터 버퍼의 바이트 크기

public:
	// 상수
	static const int DEFAULT_HEADERNAME_MAX_LENGTH = 256;		// 헤더명의 최대 길이
	static const int DEFAULT_ATTRIBUTENAME_MAX_LENGTH = 256;	// 속성명의 최대 길이

	// 생성자 & 소멸자
	DataParser();
	~DataParser();

	// 게터 & 세터
	// ...

	// 메소드
	bool openFile(char *fileName);		// 파일을 열어서 데이터 버퍼로 옮김
	void printDataBuffer();				// 데이터 버퍼의 내용을 출력
	void clearDataBuffer();				// 임시 보관된 바이트 스트림을 지움
	void initPtrAll();					// 헤더 포인터와 속성 포인터의 위치를 NULL로 초기화
	void initHeaderPtr();				// 헤더 포인터의 위치를 NULL로 초기화
	void initAttributePtr();			// 속성 포인터의 위치를 헤더 포인터의 위치로 초기화

	// 파싱 메소드
	bool findHeader(char *headerName);			// 해당 명칭의 헤더를 찾음
	bool findAttribute(char *attributeName);	// 해당 명칭의 속성을 찾음
	int parseIntFromAttribute();				// pAttribute가 가르키는 데이터를 int로 파싱하여 반환
	float parseFloatFromAttribute();			// pAttribute가 가르키는 데이터를 float으로 파싱하여 반환
	char* parseStringFromAttribute();			// pAttribute가 가르키는 데이터를 문자열로 파싱하여 동적 할당된 포인터 반환 (동적 할당 해제는 사용자의 몫)

	// 테스트 메소드
	int printf_debug(char *str);				// __DEBUG__ 가 선언되어 있을 시 빌드되는 디버그용 출력 함수
};

#endif