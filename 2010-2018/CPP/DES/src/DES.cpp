#include "DES.h"
#include <iostream>
#include <string>
using namespace std;

DES::DES(const char* planeText, const char* keyText) {
	// 키 길이 확인
	if (strlen(keyText) != 8) {
		cout << "[오류](DES::DES(const char* planeText, const char* keyText)): 암호문 길이 오류." << endl;
		exit(0);
	}

	// 키 저장
	pKeyBitBlock = new Bit(keyText);

	// 평문 블록(64bit = 8byte)단위의 개수 측정
	int planeTextLenth = strlen(planeText);											// 문자열의 총 길이 파악
	int n64BitBlock = (int)(planeTextLenth / 8);									// 1) 64bit로 가득찬 블록의 개수
	nBitBlock = (planeTextLenth % 8 != 0) ? (n64BitBlock + 1) : (n64BitBlock);		// 2) 전체 bit블럭의 개수

	// 전체 블럭의 수 만큼의 동적 포인터공간 생성
	pPlaneBitBlockPtr = new Bit*[nBitBlock];

	// 평문을 64bit단위로 블럭화하여 동적 공간으로 생성
	for (int i = 0; i < n64BitBlock; ++i) {
		char subString[9] = { '\0', }; // 주의! : Bit(const char* string) 생성자에서 문자열의 마지막이 '\0' 문자여야 하기 때문에 subString[9]를 사용!
		strncpy(subString, (planeText + (i * 8)), 8);
		pPlaneBitBlockPtr[i] = new Bit(subString);
	}

	// 남은 평문 (1~63bit 크기)을 64bit단위로 블럭화 (빈 공간은 NULL문자 '\0' 로 패딩)
	if (nBitBlock - n64BitBlock != 0) {
		char subString[8] = { '\0', };
		int nLeftChar = planeTextLenth % 8;
		strncpy(subString, (planeText + ((nBitBlock - 1) * 8)), nLeftChar);
		pPlaneBitBlockPtr[nBitBlock - 1] = new Bit(subString);
	}

	// 암호문을 저장할 공간 생성
	pCipherBitBlockPtr = new Bit*[nBitBlock];
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i] = new Bit(64);
	}

	// 서브 키 생성 시작
	Bit tempKeyBlock = pKeyBitBlock->permutate(&g_PC1);
	Bit cn = tempKeyBlock.subBit(1, 28);
	Bit dn = tempKeyBlock.subBit(29, 56);
	
	// 16개의 서브 키 생성
	for (int i = 0; i < DES::round; ++i) {
		cn = (cn << g_ShiftLeft.ary[16 + i]);
		dn = (dn << g_ShiftLeft.ary[16 + i]);

		Bit key = cn.append(dn);
		subKey[i] = key.permutate(&g_PC2);
	}
}

DES::~DES() {
	// 평문/암호문 블록 메모리 할당 해제
	for (int i = 0; i < nBitBlock; ++i) {
		delete pPlaneBitBlockPtr[i];
		delete pCipherBitBlockPtr[i];
	}
	delete[] pPlaneBitBlockPtr;
	delete[] pCipherBitBlockPtr;
	
	// 키 메모리 해제
	delete pKeyBitBlock;
}

int DES::getBitBlockNum() {
	return nBitBlock;
}

void DES::en_de_cryption_work(bool isEncrpytion) {
	// 암호화/복호화 옵션 (true: 암호화 / false: 복호화)
	bool option = isEncrpytion;

	// 평문을 초기 순열과 전치(Permutation)
	for (int i = 0; i < nBitBlock; ++i) {
		// 초기 순열 통과
		Bit LRn = pPlaneBitBlockPtr[i]->permutate(&g_IP);

		// Ln, Rn 으로 분리
		Bit L0 = LRn.subBit(1, 32);
		Bit R0 = LRn.subBit(33, 64);

		// Ln, Rn
		Bit  Ln = L0, Rn = R0;

		// n(16)라운드만큼 반복
		for (int n = 0; n < DES::round; ++n) {
			// Rn을 F(순열 함수)를 통과시킴
			int kn = (option) ? (n) : (DES::round - n - 1);
			Bit F = ((Rn.permutate(&g_E)) ^ subKey[kn]);
			Bit P;

			// S-BOX를 통과시킬 8개의 비트 블록을 생성 후 S-BOX 통과
			P = F.substitute();

			// 순열 함수를 통과시킴
			F = P.permutate(&g_P);
			F = (F ^ Ln);

			// Ln과 xor 한 결과를 Rn+1에 저장하고 Rn의 값을 Ln+1에 저장
			Ln = Rn;
			Rn = F;
		}

		// 라운드 반복 후 최종 스왑
		Bit inverseIP = Rn.append(Ln);

		// 역초기순열 통과 후 암호화된 비트 블럭을 저장
		*(pCipherBitBlockPtr[i]) = (inverseIP.permutate(&g_InverseIP));
	}
}

void DES::encryption() {
	en_de_cryption_work(true);
}

void DES::decryption() {
	// 암호문과 평문을 서로 스와핑 후 복호화 시작
	Bit** pTemp = pPlaneBitBlockPtr;
	pPlaneBitBlockPtr = pCipherBitBlockPtr;
	pCipherBitBlockPtr = pTemp;
	en_de_cryption_work(false);
}

void DES::printKey() {
	// 메인키 출력
	cout << "\n< Key >";
	printf("\n[BINARY] - Key: ");
	pKeyBitBlock->print(OPTION_BINARY);
	printf("\n[ HEXA ] - Key: ");
	pKeyBitBlock->print(OPTION_HEXA);
	printf("\n[ CHAR ] - Key: ");
	pKeyBitBlock->print(OPTION_CHAR);
	cout << endl;

	// 서브키 출력
	cout << "\n< Sub Key 1 ~ 16 >";
	for (int i = 0; i < DES::round; ++i) {
		printf("\n[BINARY] - SubKey(%2d): ", i);
		subKey[i].print(OPTION_BINARY);
		printf("\n[ HEXA ] - SubKey(%2d): ", i);
		subKey[i].print(OPTION_HEXA);
		printf("\n[ CHAR ] - SubKey(%2d): ", i);
		subKey[i].print(OPTION_CHAR);
	}
	printf("\n");
}

void DES::printPlaneText() {
	// 암호문 출력
	cout << "< Plane Text >";
	// 2진
	cout << endl << "[BINARY]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_BINARY);
	}
	// 16진
	cout << endl << "[ HEXA ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_HEXA);
	}
	// 문자
	cout << endl << "[ CHAR ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_CHAR);
	}
	printf("\n");
}

void DES::printCipherText() {
	// 암호문 출력
	cout << "< Cipher Text >";
	// 2진
	cout << endl << "[BINARY]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		 pCipherBitBlockPtr[i]->print(OPTION_BINARY);
	}
	// 16진
	cout << endl << "[ HEXA ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i]->print(OPTION_HEXA);
	}
	// 문자
	cout << endl << "[ CHAR ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i]->print(OPTION_CHAR);
	}
	printf("\n");
}
