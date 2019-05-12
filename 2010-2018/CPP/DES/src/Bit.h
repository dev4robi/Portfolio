#ifndef __BIT_H__
#define __BIT_H__

#include "Table.h"

enum Option {
	OPTION_BINARY, OPTION_HEXA, OPTION_CHAR
};

class Bit {
private:
	int size;		// 비트 사이즈
	bool* pBit;		// 첫 번째 비트의 포인터

public:
	Bit();											// 생성자
	Bit(const int _size);							// 생성자 (크기)
	Bit(const char* string);						// 생성자 (문자열)
	Bit(const Bit& _bit);							// 복사 생성자
	~Bit();											// 소멸자

	int getSize() const;							// 사이즈 반환
	void print(const Option option) const;			// 비트 스트림 출력
	void swap();									// 상위 (size/n) 비트와 하위 (size/n) 비트를 스와핑
	Bit permutate(const table* _table);				// 테이블을 입력받아서 전치를 수행한 비트 스트림을 반환
	Bit substitute();								// S-BOX 치환을 통과한 비트 스트림을 반환 (48bit -> 32bit)
	Bit subBit(const int _low, const int _high);	// 서브 비트를 반환하는 함수
	Bit append(const Bit& _bit);					// Bit클래스끼리 합침

	Bit operator^(const Bit& _bit);					// Bit클래스끼리 xor연산 수행
	void operator=(const Bit& _bit);				// Bit클래스끼리 할당 연산 수행
	void operator=(const bool oneOrZero);			// 동적 비트배열에 값 저장
	bool& operator[](const int index);				// 동적 비트배열의 해당 위치 포인터 반환
	Bit operator<<(const int shiftLeft);			// 좌측 라운드 쉬프트
	Bit operator>>(const int shiftRight);			// 우측 라운드 쉬프트
};

#endif