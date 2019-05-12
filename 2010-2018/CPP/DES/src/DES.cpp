#include "DES.h"
#include <iostream>
#include <string>
using namespace std;

DES::DES(const char* planeText, const char* keyText) {
	// Ű ���� Ȯ��
	if (strlen(keyText) != 8) {
		cout << "[����](DES::DES(const char* planeText, const char* keyText)): ��ȣ�� ���� ����." << endl;
		exit(0);
	}

	// Ű ����
	pKeyBitBlock = new Bit(keyText);

	// �� ���(64bit = 8byte)������ ���� ����
	int planeTextLenth = strlen(planeText);											// ���ڿ��� �� ���� �ľ�
	int n64BitBlock = (int)(planeTextLenth / 8);									// 1) 64bit�� ������ ����� ����
	nBitBlock = (planeTextLenth % 8 != 0) ? (n64BitBlock + 1) : (n64BitBlock);		// 2) ��ü bit���� ����

	// ��ü ���� �� ��ŭ�� ���� �����Ͱ��� ����
	pPlaneBitBlockPtr = new Bit*[nBitBlock];

	// ���� 64bit������ ��ȭ�Ͽ� ���� �������� ����
	for (int i = 0; i < n64BitBlock; ++i) {
		char subString[9] = { '\0', }; // ����! : Bit(const char* string) �����ڿ��� ���ڿ��� �������� '\0' ���ڿ��� �ϱ� ������ subString[9]�� ���!
		strncpy(subString, (planeText + (i * 8)), 8);
		pPlaneBitBlockPtr[i] = new Bit(subString);
	}

	// ���� �� (1~63bit ũ��)�� 64bit������ ��ȭ (�� ������ NULL���� '\0' �� �е�)
	if (nBitBlock - n64BitBlock != 0) {
		char subString[8] = { '\0', };
		int nLeftChar = planeTextLenth % 8;
		strncpy(subString, (planeText + ((nBitBlock - 1) * 8)), nLeftChar);
		pPlaneBitBlockPtr[nBitBlock - 1] = new Bit(subString);
	}

	// ��ȣ���� ������ ���� ����
	pCipherBitBlockPtr = new Bit*[nBitBlock];
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i] = new Bit(64);
	}

	// ���� Ű ���� ����
	Bit tempKeyBlock = pKeyBitBlock->permutate(&g_PC1);
	Bit cn = tempKeyBlock.subBit(1, 28);
	Bit dn = tempKeyBlock.subBit(29, 56);
	
	// 16���� ���� Ű ����
	for (int i = 0; i < DES::round; ++i) {
		cn = (cn << g_ShiftLeft.ary[16 + i]);
		dn = (dn << g_ShiftLeft.ary[16 + i]);

		Bit key = cn.append(dn);
		subKey[i] = key.permutate(&g_PC2);
	}
}

DES::~DES() {
	// ��/��ȣ�� ��� �޸� �Ҵ� ����
	for (int i = 0; i < nBitBlock; ++i) {
		delete pPlaneBitBlockPtr[i];
		delete pCipherBitBlockPtr[i];
	}
	delete[] pPlaneBitBlockPtr;
	delete[] pCipherBitBlockPtr;
	
	// Ű �޸� ����
	delete pKeyBitBlock;
}

int DES::getBitBlockNum() {
	return nBitBlock;
}

void DES::en_de_cryption_work(bool isEncrpytion) {
	// ��ȣȭ/��ȣȭ �ɼ� (true: ��ȣȭ / false: ��ȣȭ)
	bool option = isEncrpytion;

	// ���� �ʱ� ������ ��ġ(Permutation)
	for (int i = 0; i < nBitBlock; ++i) {
		// �ʱ� ���� ���
		Bit LRn = pPlaneBitBlockPtr[i]->permutate(&g_IP);

		// Ln, Rn ���� �и�
		Bit L0 = LRn.subBit(1, 32);
		Bit R0 = LRn.subBit(33, 64);

		// Ln, Rn
		Bit  Ln = L0, Rn = R0;

		// n(16)���常ŭ �ݺ�
		for (int n = 0; n < DES::round; ++n) {
			// Rn�� F(���� �Լ�)�� �����Ŵ
			int kn = (option) ? (n) : (DES::round - n - 1);
			Bit F = ((Rn.permutate(&g_E)) ^ subKey[kn]);
			Bit P;

			// S-BOX�� �����ų 8���� ��Ʈ ����� ���� �� S-BOX ���
			P = F.substitute();

			// ���� �Լ��� �����Ŵ
			F = P.permutate(&g_P);
			F = (F ^ Ln);

			// Ln�� xor �� ����� Rn+1�� �����ϰ� Rn�� ���� Ln+1�� ����
			Ln = Rn;
			Rn = F;
		}

		// ���� �ݺ� �� ���� ����
		Bit inverseIP = Rn.append(Ln);

		// ���ʱ���� ��� �� ��ȣȭ�� ��Ʈ ���� ����
		*(pCipherBitBlockPtr[i]) = (inverseIP.permutate(&g_InverseIP));
	}
}

void DES::encryption() {
	en_de_cryption_work(true);
}

void DES::decryption() {
	// ��ȣ���� ���� ���� ������ �� ��ȣȭ ����
	Bit** pTemp = pPlaneBitBlockPtr;
	pPlaneBitBlockPtr = pCipherBitBlockPtr;
	pCipherBitBlockPtr = pTemp;
	en_de_cryption_work(false);
}

void DES::printKey() {
	// ����Ű ���
	cout << "\n< Key >";
	printf("\n[BINARY] - Key: ");
	pKeyBitBlock->print(OPTION_BINARY);
	printf("\n[ HEXA ] - Key: ");
	pKeyBitBlock->print(OPTION_HEXA);
	printf("\n[ CHAR ] - Key: ");
	pKeyBitBlock->print(OPTION_CHAR);
	cout << endl;

	// ����Ű ���
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
	// ��ȣ�� ���
	cout << "< Plane Text >";
	// 2��
	cout << endl << "[BINARY]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_BINARY);
	}
	// 16��
	cout << endl << "[ HEXA ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_HEXA);
	}
	// ����
	cout << endl << "[ CHAR ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pPlaneBitBlockPtr[i]->print(OPTION_CHAR);
	}
	printf("\n");
}

void DES::printCipherText() {
	// ��ȣ�� ���
	cout << "< Cipher Text >";
	// 2��
	cout << endl << "[BINARY]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		 pCipherBitBlockPtr[i]->print(OPTION_BINARY);
	}
	// 16��
	cout << endl << "[ HEXA ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i]->print(OPTION_HEXA);
	}
	// ����
	cout << endl << "[ CHAR ]: ";
	for (int i = 0; i < nBitBlock; ++i) {
		pCipherBitBlockPtr[i]->print(OPTION_CHAR);
	}
	printf("\n");
}
