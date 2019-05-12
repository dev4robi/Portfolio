#ifndef __HES_H__
#define __DES_H__

#define _CRT_SECURE_NO_WARNINGS

#include "Bit.h"

class DES {
private:
	static const int round = 16;		// 라운드 수
	static const int blockSize = 64;	// 한 블럭당 크기

	int nBitBlock;						// 평문/암호문 비트블럭의 수
	Bit** pPlaneBitBlockPtr;			// 평문 비트 블럭 포인터
	Bit** pCipherBitBlockPtr;			// 암호문 비트 블럭 포인터
	Bit* pKeyBitBlock;					// 키 비트 블럭
	Bit subKey[round];					// 서브 키 블럭

public:
	DES(const char* planeText, const char* key);	// 생성자
	~DES();											// 소멸자

	int getBitBlockNum();							// 비트 블록의 수를 반환
	void encryption();								// DES 암호화
	void decryption();								// DES 복호화
	void printPlaneText();							// 평문 출력
	void printCipherText();							// 암호문 출력
	void printKey();								// 키 출력

private:
	void en_de_cryption_work(bool isEncrpytion);	// DES 암/복호화 내부 함수 (호출 불가)
};

#endif