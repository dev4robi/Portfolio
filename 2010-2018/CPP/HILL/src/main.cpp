#include <iostream>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "GlobalVar.h"
#include "KeyMatrix.h"
#include "Math.h"
using namespace std;

/* ���̺귯�� ��ũ */
#pragma comment(lib, "winmm.lib")

/* �Լ� ������Ÿ�� */
char** getPalneText(char* str, int* col, int* row);										// ���� �� ��Ʈ������ ����� �е����ִ� �Լ�
bool printTextMatrix(char** strMat, int col, int row, bool matMode, bool isNumber);		// ���� ��Ʈ������ ����ϴ� �Լ�

/* ���� ����(Declear: GlobalVar.h) */
// [Text Setting]
const char PADDING_CHAR = 'x';	// �е��� ����
const int TEXT_MAX = 1024;		// ��Ʈ���� �ִ� ����
char** planeTextMat;			// ���� �� �ؽ�Ʈ ��Ʈ����
char** cipherTextMat;			// ���� ��ȣȭ �ؽ�Ʈ ��Ʈ����
char** decrypTextMat;			// ���� ��ȣȭ �ؽ�Ʈ ��Ʈ����
int textMatCol = 0;				// �ؽ�Ʈ ��Ʈ���� ��
int textMatRow = 0;				// �ؽ�Ʈ ��Ʈ���� ��

// [Key Matrix Setting]
int lowBound = -99;				// Ű�� �ּҰ�
int highBound = 99;				// Ű�� �ִ밪
int keyMatSize = KEY_MAX_SIZE;	// Ű ��Ʈ������ ������(KeyMatrix.h)

// [Charset and zStar Setting]
const int charCnt = 26;		// ������ ����
int zStarCnt = 0;			// zStar Count
int* zStar;					// zStar
int* zStarInv;				// zStar^-1 (Inverse)

/* ���� */
int main(int argc, char** argv) {
	// ���α׷� ���� �ð� ����
	bool detOver = true;
	double firstTime = ((double)timeGetTime() / 1000.0);

	// ���� �õ� ����
	srand(time(NULL));

	// zStar ����
	zStar = getZstar(charCnt, &zStarCnt);

	// Ű ��Ʈ���� ��ü ����
	KeyMatrix key(keyMatSize);		// Ű ��Ʈ����
	KeyMatrix invKey(keyMatSize);	// Ű ����� ��Ʈ����
	key.getKeyMatrix(lowBound, highBound);

	cout << "Ű ���: det(Key): " << key.getDeterminant() << endl;
	key.print();
	
	printf("inv(key): \n");
	invKey = key.getInverseMatrix();
	invKey.print();

	printf("Ű ����:\n");
	KeyMatrix idKey = key * invKey;
	idKey.getModMatrix(charCnt);
	idKey.print();

	printf("������� �ɸ� �ð�: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);
	//_getch();

	// �� �Է�
	planeTextMat = getPalneText("thisstringistestforhillcryptographywithfivebyfivekeymatrix", &textMatCol, &textMatRow);
	cout << "�� ��Ʈ����(������):" << endl;
	printTextMatrix(planeTextMat, textMatCol, textMatRow, true, true);
	cout << "�� ��Ʈ����(�ƽ�Ű ��ȯ��):" << endl;
	printTextMatrix(planeTextMat, textMatCol, textMatRow, true, false);
	printf("������� �ɸ� �ð�: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// ��ȣȭ �� ��� ���
	cipherTextMat = key.getEncryptionMat(planeTextMat, textMatCol, textMatRow);
	cout << "��ȣ�� ��Ʈ����(������):" << endl;
	printTextMatrix(cipherTextMat, textMatCol, textMatRow, true, true);
	cout << "��ȣ�� ��Ʈ����(�ƽ�Ű ��ȯ��):" << endl;
	printTextMatrix(cipherTextMat, textMatCol, textMatRow, true, false);
	printf("������� �ɸ� �ð�: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// ��ȣȭ �� ��� ���
	decrypTextMat = invKey.getEncryptionMat(cipherTextMat, textMatCol, textMatRow);
	cout << "��ȣ�� ��Ʈ����(������):" << endl;
	printTextMatrix(decrypTextMat, textMatCol, textMatRow, true, true);
	cout << "��ȣ�� ��Ʈ����(�ƽ�Ű ��ȯ��):" << endl;
	printTextMatrix(decrypTextMat, textMatCol, textMatRow, true, false);
	printf("������� �ɸ� �ð�: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// ��ȯ
	delete planeTextMat;
	delete cipherTextMat;
	delete decrypTextMat;

	// ����
	return 0;
}

char** getPalneText(char* str, int* col, int* row) {
	int i = 0;
	int strLength = strnlen(str, TEXT_MAX);
	int matRow = keyMatSize;
	int matCol = (strLength / matRow);
	// ��Ʈ������ ä���� ���ϴ� ���� ���ڿ��� ������ ���� 1����
	if ((strLength % matRow) != 0) matCol += 1;
	// ���� ��Ʈ���� ����
	char** strMat = (char**)malloc(sizeof(char*) * matRow);
	for (int i = 0; i < matRow; ++i) {
		strMat[i] = (char*)malloc(sizeof(char)* matCol);
	}

	// �� ���� Ȯ��
	if (strMat == NULL)
		return NULL;

	// ���� ������ ��� ũ�� ����
	(*col) = matCol;
	(*row) = matRow;

	// �� ��Ʈ������ ���ڿ� ����
	for (int row = 0; row < matRow; ++row) {
		for (int col = 0; col < matCol; ++col) {
			if (i < strLength)
				strMat[row][col] = (str[i++] - 97);
			else
				strMat[row][col] = (PADDING_CHAR - 97);
		}
	}

	// ������ ���� �� ��Ʈ���� ������ ��ȯ
	return strMat;
}

bool printTextMatrix(char** strMat, int col, int row, bool matMode, bool isNumber) {
	// ��Ʈ���� ��� ���
	if (matMode == true) {
		for (int j = 0; j < row; ++j) {
			for (int i = 0; i < col; ++i) {
				if (isNumber) {
					printf("%3d ", strMat[j][i]);
				}
				else {
					printf("%3c ", strMat[j][i] + 97);
				}
			}
			printf("\n");
		}
	}
	// �Ϲ� ��� ���
	else {
		for (int j = 0; j < row; ++j) {
			for (int i = 0; i < col; ++i) {
				if (isNumber) {
					printf("%d ", strMat[j][i]);
				}
				else {
					printf("%c ", strMat[j][i] + 97);
				}
			}
		}
		printf("\n");
	}

	// ��� ���� ��ȯ
	return true;
}
