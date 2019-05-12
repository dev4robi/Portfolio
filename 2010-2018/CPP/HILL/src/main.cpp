#include <iostream>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "GlobalVar.h"
#include "KeyMatrix.h"
#include "Math.h"
using namespace std;

/* 라이브러리 링크 */
#pragma comment(lib, "winmm.lib")

/* 함수 프로토타입 */
char** getPalneText(char* str, int* col, int* row);										// 평문을 평문 매트릭스로 만들고 패딩해주는 함수
bool printTextMatrix(char** strMat, int col, int row, bool matMode, bool isNumber);		// 문자 매트릭스를 출력하는 함수

/* 전역 변수(Declear: GlobalVar.h) */
// [Text Setting]
const char PADDING_CHAR = 'x';	// 패딩용 문자
const int TEXT_MAX = 1024;		// 매트릭스 최대 길이
char** planeTextMat;			// 동적 평문 텍스트 매트릭스
char** cipherTextMat;			// 동적 암호화 텍스트 매트릭스
char** decrypTextMat;			// 동적 복호화 텍스트 매트릭스
int textMatCol = 0;				// 텍스트 매트릭스 열
int textMatRow = 0;				// 텍스트 매트릭스 행

// [Key Matrix Setting]
int lowBound = -99;				// 키의 최소값
int highBound = 99;				// 키의 최대값
int keyMatSize = KEY_MAX_SIZE;	// 키 메트릭스의 사이즈(KeyMatrix.h)

// [Charset and zStar Setting]
const int charCnt = 26;		// 문자의 개수
int zStarCnt = 0;			// zStar Count
int* zStar;					// zStar
int* zStarInv;				// zStar^-1 (Inverse)

/* 메인 */
int main(int argc, char** argv) {
	// 프로그램 시작 시간 저장
	bool detOver = true;
	double firstTime = ((double)timeGetTime() / 1000.0);

	// 난수 시드 설정
	srand(time(NULL));

	// zStar 생성
	zStar = getZstar(charCnt, &zStarCnt);

	// 키 매트릭스 객체 생성
	KeyMatrix key(keyMatSize);		// 키 메트릭스
	KeyMatrix invKey(keyMatSize);	// 키 역행렬 메트릭스
	key.getKeyMatrix(lowBound, highBound);

	cout << "키 행렬: det(Key): " << key.getDeterminant() << endl;
	key.print();
	
	printf("inv(key): \n");
	invKey = key.getInverseMatrix();
	invKey.print();

	printf("키 증명:\n");
	KeyMatrix idKey = key * invKey;
	idKey.getModMatrix(charCnt);
	idKey.print();

	printf("여기까지 걸린 시간: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);
	//_getch();

	// 평문 입력
	planeTextMat = getPalneText("thisstringistestforhillcryptographywithfivebyfivekeymatrix", &textMatCol, &textMatRow);
	cout << "평문 매트릭스(실제값):" << endl;
	printTextMatrix(planeTextMat, textMatCol, textMatRow, true, true);
	cout << "평문 매트릭스(아스키 변환값):" << endl;
	printTextMatrix(planeTextMat, textMatCol, textMatRow, true, false);
	printf("여기까지 걸린 시간: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// 암호화 및 결과 출력
	cipherTextMat = key.getEncryptionMat(planeTextMat, textMatCol, textMatRow);
	cout << "암호문 매트릭스(실제값):" << endl;
	printTextMatrix(cipherTextMat, textMatCol, textMatRow, true, true);
	cout << "암호문 매트릭스(아스키 변환값):" << endl;
	printTextMatrix(cipherTextMat, textMatCol, textMatRow, true, false);
	printf("여기까지 걸린 시간: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// 복호화 및 결과 출력
	decrypTextMat = invKey.getEncryptionMat(cipherTextMat, textMatCol, textMatRow);
	cout << "복호문 매트릭스(실제값):" << endl;
	printTextMatrix(decrypTextMat, textMatCol, textMatRow, true, true);
	cout << "복호문 매트릭스(아스키 변환값):" << endl;
	printTextMatrix(decrypTextMat, textMatCol, textMatRow, true, false);
	printf("여기까지 걸린 시간: %f\n\n", (double)(timeGetTime() / 1000.0) - firstTime);

	// 반환
	delete planeTextMat;
	delete cipherTextMat;
	delete decrypTextMat;

	// 종료
	return 0;
}

char** getPalneText(char* str, int* col, int* row) {
	int i = 0;
	int strLength = strnlen(str, TEXT_MAX);
	int matRow = keyMatSize;
	int matCol = (strLength / matRow);
	// 매트릭스에 채우지 못하는 남는 문자열이 있으면 행을 1증가
	if ((strLength % matRow) != 0) matCol += 1;
	// 동적 매트릭스 생성
	char** strMat = (char**)malloc(sizeof(char*) * matRow);
	for (int i = 0; i < matRow; ++i) {
		strMat[i] = (char*)malloc(sizeof(char)* matCol);
	}

	// 힙 오류 확인
	if (strMat == NULL)
		return NULL;

	// 전역 변수에 행렬 크기 저장
	(*col) = matCol;
	(*row) = matRow;

	// 평문 매트릭스에 문자열 저장
	for (int row = 0; row < matRow; ++row) {
		for (int col = 0; col < matCol; ++col) {
			if (i < strLength)
				strMat[row][col] = (str[i++] - 97);
			else
				strMat[row][col] = (PADDING_CHAR - 97);
		}
	}

	// 생성된 동적 평문 매트릭스 포인터 반환
	return strMat;
}

bool printTextMatrix(char** strMat, int col, int row, bool matMode, bool isNumber) {
	// 매트릭스 모드 출력
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
	// 일반 모드 출력
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

	// 출력 성공 반환
	return true;
}
