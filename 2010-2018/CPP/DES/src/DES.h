#ifndef __HES_H__
#define __DES_H__

#define _CRT_SECURE_NO_WARNINGS

#include "Bit.h"

class DES {
private:
	static const int round = 16;		// ���� ��
	static const int blockSize = 64;	// �� ���� ũ��

	int nBitBlock;						// ��/��ȣ�� ��Ʈ���� ��
	Bit** pPlaneBitBlockPtr;			// �� ��Ʈ �� ������
	Bit** pCipherBitBlockPtr;			// ��ȣ�� ��Ʈ �� ������
	Bit* pKeyBitBlock;					// Ű ��Ʈ ��
	Bit subKey[round];					// ���� Ű ��

public:
	DES(const char* planeText, const char* key);	// ������
	~DES();											// �Ҹ���

	int getBitBlockNum();							// ��Ʈ ����� ���� ��ȯ
	void encryption();								// DES ��ȣȭ
	void decryption();								// DES ��ȣȭ
	void printPlaneText();							// �� ���
	void printCipherText();							// ��ȣ�� ���
	void printKey();								// Ű ���

private:
	void en_de_cryption_work(bool isEncrpytion);	// DES ��/��ȣȭ ���� �Լ� (ȣ�� �Ұ�)
};

#endif