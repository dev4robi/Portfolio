#ifndef __KEY_MATRIX_H__
#define __KEY_MATRIX_H__

#define KEY_MAX_SIZE 5

class KeyMatrix {
private:
	static const int maxRow = KEY_MAX_SIZE;				// 최대 행의 크기
	static const int maxColumn = KEY_MAX_SIZE;			// 최대 열의 크기
	int _row;											// 행의 크기
	int _column;										// 열의 크기
	int _element[maxRow][maxColumn];					// 행렬의 요소

public:
	KeyMatrix(int size);								// 생성자
	KeyMatrix(KeyMatrix& mat);							// 복사 생성자
	~KeyMatrix();										// 소멸자

	void setRandomElement(int minBound, int maxBound);	// 행렬의 요소를 (minBound ~ maxBound) 사이의 무작위 값으로 채워 넣음
	int getDeterminant();								// 행렬의 디터미넌트(det)을 구함
	int getCofactor(int j, int i);						// 여인수를 구함
	bool getKeyMatrix(int low, int high);				// 키 매트릭스를 구함
	bool getModMatrix(int mod);							// modular를 취한 매트릭스를 구함
	KeyMatrix& getMinorMatrix(int j, int i);			// 소행렬식을 생성 (5x5 -> 4x4 -> 3x3)
	KeyMatrix& getTransposedMatrix();					// 전치행렬 생성
	KeyMatrix& getAdjointMatrix();						// 수반행렬 생성
	KeyMatrix& getCofactorMatrix();						// 여인수행렬 생성
	KeyMatrix& getInverseMatrix();						// 역행렬 생성
	void print();										// 행렬의 요소를 출력함

	KeyMatrix& operator*(KeyMatrix& mat);				// 행렬끼리 곱함
	KeyMatrix& operator*(int num);						// 행렬과 상수를 곱함

	char** getEncryptionMat(char** strMat, int col, int row);	// 암호화된 행렬을 생성
};

#endif