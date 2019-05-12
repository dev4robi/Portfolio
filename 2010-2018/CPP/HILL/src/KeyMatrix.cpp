#include "KeyMatrix.h"
#include "GlobalVar.h"
#include "Math.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

KeyMatrix::KeyMatrix(int size) : _row(size), _column(size) {
	// 생성된 행렬의 요소를 0으로 초기화
	for (int j = 0; j < _column; ++j) {
		for (int i = 0; i < _row; ++i) {
			_element[j][i] = 0;
		}
	}
}

KeyMatrix::KeyMatrix(KeyMatrix& mat) {
	// 행렬의 크기를 복사
	_column = mat._column;
	_row = mat._row;

	// 행렬의 요소를 복사
	for (int j = 0; j < _column; ++j) {
		for (int i = 0; i < _row; ++i) {
			_element[j][i] = mat._element[j][i];
		}
	}
}

KeyMatrix::~KeyMatrix() {

}

void KeyMatrix::setRandomElement(int minBound, int maxBound) {
	// 무작위 값을 임시 저장할 변수
	int random = 0;

	// minBound ~ maxBound사이의 무작위 정수로 행렬을 채움
	for (int j = 0; j < _row; ++j) {
		for (int i = 0; i < _column; ++i) {
			random = getRandomInt(minBound, maxBound);
			_element[j][i] = random;
		}
	}
}

int KeyMatrix::getDeterminant() {
	int det = 0;
	KeyMatrix* tMat = this;

	// 1x1 행렬의 디터미넌트
	if (tMat->_column == 1) {
		return modular(tMat->_element[0][0], charCnt);
	}
	// 2x2 행렬의 디터미넌트
	else if (tMat->_column == 2) {
		// ad - bc (2x2)
		det = ((tMat->_element[0][0] * tMat->_element[1][1]) - (tMat->_element[0][1] * tMat->_element[1][0]));
		return modular(det, charCnt);
	}
	// 3x3 행렬의 디터미넌트
	else if (tMat->_column == 3) {
		// aei + bfg + cdh - ceg - bdi - afh (3x3)
		det = ((tMat->_element[0][0] * tMat->_element[1][1] * tMat->_element[2][2]) + (tMat->_element[0][1] * tMat->_element[1][2] * tMat->_element[2][0]) +
			   (tMat->_element[0][2] * tMat->_element[1][0] * tMat->_element[2][1]) - (tMat->_element[0][2] * tMat->_element[1][1] * tMat->_element[2][0]) -
			   (tMat->_element[0][1] * tMat->_element[1][0] * tMat->_element[2][2]) - (tMat->_element[0][0] * tMat->_element[1][2] * tMat->_element[2][1]));
		return modular(det, charCnt);
	}
	// nxn(n>3) 행렬의 디터미넌트 (a11c11 + a12c12 + a13c13 + a14c14 + ... / c = Cofactor)
	else if (tMat->_column >= 4) {
		for (int i = 0; i < tMat->_column; ++i) {
			det += (tMat->_element[0][i] * tMat->getCofactor(0, i));
			det = modular(det, charCnt);
		}
		return det;
	}

	// 도달하지 않음
	return 0;
}

int KeyMatrix::getCofactor(int j, int i) {
	int cofactor = 0;
	KeyMatrix* minorMat = &getMinorMatrix(j, i);

	// Cij = ((-1)^(i+j)) * det(Mij) / (C = Confactor, M = Minor, det = Determinant)
	cofactor = power(-1, ((j + 1) + (i + 1))) * minorMat->getDeterminant();

	// 임시로 생성된 소행렬 파괴
	delete minorMat;

	// 여인수 반환
	return cofactor;
}

bool KeyMatrix::getKeyMatrix(int low, int high) {
	bool detOver = true;

	// 키 매트릭스의 디터미넌트가 군에 들어있을 때 까지 무작위 행렬 생성을 반복 
	do {
		// -99 ~ 99 사이의 무작위 행렬 생성
		setRandomElement(low, high);

		// 디터미넌트가 zStar에 들어있는지 확인
		for (int i = 0; i < 12; ++i) {
			if (getDeterminant() == zStar[i]) {
				detOver = false;
			}
		}
	} while (detOver);

	// 행렬에 modular를 수행함
	getModMatrix(charCnt);

	// 성공 반환
	return true;
}

bool KeyMatrix::getModMatrix(int mod) {
	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			_element[row][col] = modular(_element[row][col], mod);
		}
	}

	return true;
}

KeyMatrix& KeyMatrix::getMinorMatrix(int j, int i) {
	int k = 0;
	int minorList[(KeyMatrix::maxRow - 1) * (KeyMatrix::maxColumn - 1)]; // [(maxSize-1)^2]
	
	// 소행렬 생성
	KeyMatrix* minorMat = new KeyMatrix(_column - 1);
	
	// nxn 행렬에서 소행렬 원소만 순서대로 minorList에 추출
	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			if ((col != i) && (row != j)) {
				minorList[k++] = _element[row][col];
			}
		}
	}

	// 소행렬 리스트에 저장된 값을 소행렬에 채워 넣음
	k = 0;
	for (int row = 0; row < minorMat->_row; ++row) {
		for (int col = 0; col < minorMat->_column; ++col) {
			minorMat->_element[row][col] = minorList[k++];
		}
	}

	// 생성된 소행렬 반환
	return *minorMat;
}

KeyMatrix& KeyMatrix::getTransposedMatrix() {
	KeyMatrix* transMat = new KeyMatrix(_column);

	// 열과 행을 바꿈 (전치 행렬)
	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			transMat->_element[row][col] = _element[col][row];
		}
	}

	// 생성된 전치 행렬 반환
	return *transMat;
}

KeyMatrix& KeyMatrix::getAdjointMatrix() {
	// adj(A) = [cof(A)]^T / (adj: 수반 행렬, cof: 여인수 행렬, T: 전치 행렬)
	KeyMatrix* cofMat = &(this->getCofactorMatrix());
	KeyMatrix* adjMat = &(cofMat->getTransposedMatrix());

	delete cofMat;

	return *adjMat;
}

KeyMatrix& KeyMatrix::getCofactorMatrix() {
	// cof(A) = (C11, C12, C13, ...)
	KeyMatrix* cofMat = new KeyMatrix(_column);

	for (int row = 0; row < cofMat->_row; ++row) {
		for (int col = 0; col < cofMat->_column; ++col) {
			cofMat->_element[row][col] = getCofactor(row, col);
		}
	}

	// 행렬에 modular를 수행함
	cofMat->getModMatrix(charCnt);

	return *cofMat;
}

KeyMatrix& KeyMatrix::getInverseMatrix() {
	int i = 0;
	int det = this->getDeterminant();
	KeyMatrix* adjMat = &(this->getAdjointMatrix());

	// 전역 변수 *zStar, *zStarCnt
	for (i = 0; i < zStarCnt; ++i) {
		if (det == zStar[i]) break;
	}

	// 역행렬이 존재하지 않음 (원래 행렬을 반환)
	if (det == 0 || i == zStarCnt) return *this;

	// A^-1 = (det(A)^-1) * adj(A)
	KeyMatrix* invMat = &(adjMat->operator*(getZstarInverse(det, zStar, &zStarCnt)));

	// mod zStarCnt를 최종적으로 수행
	for (int row = 0; row < invMat->_row; ++row) {
		for (int col = 0; col < invMat->_column; ++col) {
			invMat->_element[row][col] = invMat->_element[row][col];
		}
	}

	// 행렬에 modular를 수행함
	invMat->getModMatrix(charCnt);

	// 수반 행렬 파괴
	delete adjMat;

	// 역행렬 포인터 반환
	return *invMat;
}

void KeyMatrix::print() {
	// 행렬의 요소를 출력
	for (int j = 0; j < _row; ++j) {
		for (int i = 0; i < _column; ++i) {
			printf("%3d ", _element[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

KeyMatrix& KeyMatrix::operator*(KeyMatrix& mat) {
	KeyMatrix* reMat = NULL;

	// 행렬의 곱이 가능한지 확인
	if (_column != mat._row) {
		return *reMat;
	}
	
	// 새 행렬 생성
	reMat = new KeyMatrix(mat._row);

	// 새 행렬에 곱의 결과 저장
	for (int j = 0; j < _row; ++j) {
		for (int i = 0; i < _column; ++i) {
			for (int k = 0; k < _row; ++k) {   
				reMat->_element[j][i] += (_element[j][k] * mat._element[k][i]);
			}
		}
	}

	// 행렬끼리 곱샘 후 반환
	return *reMat;
}

KeyMatrix& KeyMatrix::operator*(int num) {
	KeyMatrix* reMat = new KeyMatrix(_column);

	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			reMat->_element[row][col] = (_element[row][col] * num);
		}
	}

	// 정수와 곱샘 후 행렬 반환
	return *reMat;
}

char** KeyMatrix::getEncryptionMat(char** strMat, int col, int row) {
	// 암호문을 담을 동적 매트릭스공간 생성
	char** enMat = (char**)malloc(sizeof(char*)* row);
	for (int i = 0; i < row; ++i) {
		enMat[i] = (char*)malloc(sizeof(char)* col);
	}

	// 오류 확인
	if (enMat == NULL) {
		return NULL;
	}

	char enM = 0; // test

	// 암호문 행렬 초기화 및 암호화 (키행렬과 평문행렬의 곱)
	for (int j = 0; j < row; ++j) {
		for (int i = 0; i < col; ++i) {
			enMat[j][i] = (int)0;
			for (int k = 0; k < row; ++k) {
				enMat[j][i] += modular((_element[j][k] * strMat[k][i]), charCnt);
				enMat[j][i] = modular(enMat[j][i], charCnt);
			}
		}
	}

	// 암호화 행렬 반환
	return enMat;
}
