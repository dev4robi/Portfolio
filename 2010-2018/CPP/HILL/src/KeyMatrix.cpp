#include "KeyMatrix.h"
#include "GlobalVar.h"
#include "Math.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

KeyMatrix::KeyMatrix(int size) : _row(size), _column(size) {
	// ������ ����� ��Ҹ� 0���� �ʱ�ȭ
	for (int j = 0; j < _column; ++j) {
		for (int i = 0; i < _row; ++i) {
			_element[j][i] = 0;
		}
	}
}

KeyMatrix::KeyMatrix(KeyMatrix& mat) {
	// ����� ũ�⸦ ����
	_column = mat._column;
	_row = mat._row;

	// ����� ��Ҹ� ����
	for (int j = 0; j < _column; ++j) {
		for (int i = 0; i < _row; ++i) {
			_element[j][i] = mat._element[j][i];
		}
	}
}

KeyMatrix::~KeyMatrix() {

}

void KeyMatrix::setRandomElement(int minBound, int maxBound) {
	// ������ ���� �ӽ� ������ ����
	int random = 0;

	// minBound ~ maxBound������ ������ ������ ����� ä��
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

	// 1x1 ����� ���͹̳�Ʈ
	if (tMat->_column == 1) {
		return modular(tMat->_element[0][0], charCnt);
	}
	// 2x2 ����� ���͹̳�Ʈ
	else if (tMat->_column == 2) {
		// ad - bc (2x2)
		det = ((tMat->_element[0][0] * tMat->_element[1][1]) - (tMat->_element[0][1] * tMat->_element[1][0]));
		return modular(det, charCnt);
	}
	// 3x3 ����� ���͹̳�Ʈ
	else if (tMat->_column == 3) {
		// aei + bfg + cdh - ceg - bdi - afh (3x3)
		det = ((tMat->_element[0][0] * tMat->_element[1][1] * tMat->_element[2][2]) + (tMat->_element[0][1] * tMat->_element[1][2] * tMat->_element[2][0]) +
			   (tMat->_element[0][2] * tMat->_element[1][0] * tMat->_element[2][1]) - (tMat->_element[0][2] * tMat->_element[1][1] * tMat->_element[2][0]) -
			   (tMat->_element[0][1] * tMat->_element[1][0] * tMat->_element[2][2]) - (tMat->_element[0][0] * tMat->_element[1][2] * tMat->_element[2][1]));
		return modular(det, charCnt);
	}
	// nxn(n>3) ����� ���͹̳�Ʈ (a11c11 + a12c12 + a13c13 + a14c14 + ... / c = Cofactor)
	else if (tMat->_column >= 4) {
		for (int i = 0; i < tMat->_column; ++i) {
			det += (tMat->_element[0][i] * tMat->getCofactor(0, i));
			det = modular(det, charCnt);
		}
		return det;
	}

	// �������� ����
	return 0;
}

int KeyMatrix::getCofactor(int j, int i) {
	int cofactor = 0;
	KeyMatrix* minorMat = &getMinorMatrix(j, i);

	// Cij = ((-1)^(i+j)) * det(Mij) / (C = Confactor, M = Minor, det = Determinant)
	cofactor = power(-1, ((j + 1) + (i + 1))) * minorMat->getDeterminant();

	// �ӽ÷� ������ ����� �ı�
	delete minorMat;

	// ���μ� ��ȯ
	return cofactor;
}

bool KeyMatrix::getKeyMatrix(int low, int high) {
	bool detOver = true;

	// Ű ��Ʈ������ ���͹̳�Ʈ�� ���� ������� �� ���� ������ ��� ������ �ݺ� 
	do {
		// -99 ~ 99 ������ ������ ��� ����
		setRandomElement(low, high);

		// ���͹̳�Ʈ�� zStar�� ����ִ��� Ȯ��
		for (int i = 0; i < 12; ++i) {
			if (getDeterminant() == zStar[i]) {
				detOver = false;
			}
		}
	} while (detOver);

	// ��Ŀ� modular�� ������
	getModMatrix(charCnt);

	// ���� ��ȯ
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
	
	// ����� ����
	KeyMatrix* minorMat = new KeyMatrix(_column - 1);
	
	// nxn ��Ŀ��� ����� ���Ҹ� ������� minorList�� ����
	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			if ((col != i) && (row != j)) {
				minorList[k++] = _element[row][col];
			}
		}
	}

	// ����� ����Ʈ�� ����� ���� ����Ŀ� ä�� ����
	k = 0;
	for (int row = 0; row < minorMat->_row; ++row) {
		for (int col = 0; col < minorMat->_column; ++col) {
			minorMat->_element[row][col] = minorList[k++];
		}
	}

	// ������ ����� ��ȯ
	return *minorMat;
}

KeyMatrix& KeyMatrix::getTransposedMatrix() {
	KeyMatrix* transMat = new KeyMatrix(_column);

	// ���� ���� �ٲ� (��ġ ���)
	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			transMat->_element[row][col] = _element[col][row];
		}
	}

	// ������ ��ġ ��� ��ȯ
	return *transMat;
}

KeyMatrix& KeyMatrix::getAdjointMatrix() {
	// adj(A) = [cof(A)]^T / (adj: ���� ���, cof: ���μ� ���, T: ��ġ ���)
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

	// ��Ŀ� modular�� ������
	cofMat->getModMatrix(charCnt);

	return *cofMat;
}

KeyMatrix& KeyMatrix::getInverseMatrix() {
	int i = 0;
	int det = this->getDeterminant();
	KeyMatrix* adjMat = &(this->getAdjointMatrix());

	// ���� ���� *zStar, *zStarCnt
	for (i = 0; i < zStarCnt; ++i) {
		if (det == zStar[i]) break;
	}

	// ������� �������� ���� (���� ����� ��ȯ)
	if (det == 0 || i == zStarCnt) return *this;

	// A^-1 = (det(A)^-1) * adj(A)
	KeyMatrix* invMat = &(adjMat->operator*(getZstarInverse(det, zStar, &zStarCnt)));

	// mod zStarCnt�� ���������� ����
	for (int row = 0; row < invMat->_row; ++row) {
		for (int col = 0; col < invMat->_column; ++col) {
			invMat->_element[row][col] = invMat->_element[row][col];
		}
	}

	// ��Ŀ� modular�� ������
	invMat->getModMatrix(charCnt);

	// ���� ��� �ı�
	delete adjMat;

	// ����� ������ ��ȯ
	return *invMat;
}

void KeyMatrix::print() {
	// ����� ��Ҹ� ���
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

	// ����� ���� �������� Ȯ��
	if (_column != mat._row) {
		return *reMat;
	}
	
	// �� ��� ����
	reMat = new KeyMatrix(mat._row);

	// �� ��Ŀ� ���� ��� ����
	for (int j = 0; j < _row; ++j) {
		for (int i = 0; i < _column; ++i) {
			for (int k = 0; k < _row; ++k) {   
				reMat->_element[j][i] += (_element[j][k] * mat._element[k][i]);
			}
		}
	}

	// ��ĳ��� ���� �� ��ȯ
	return *reMat;
}

KeyMatrix& KeyMatrix::operator*(int num) {
	KeyMatrix* reMat = new KeyMatrix(_column);

	for (int row = 0; row < _row; ++row) {
		for (int col = 0; col < _column; ++col) {
			reMat->_element[row][col] = (_element[row][col] * num);
		}
	}

	// ������ ���� �� ��� ��ȯ
	return *reMat;
}

char** KeyMatrix::getEncryptionMat(char** strMat, int col, int row) {
	// ��ȣ���� ���� ���� ��Ʈ�������� ����
	char** enMat = (char**)malloc(sizeof(char*)* row);
	for (int i = 0; i < row; ++i) {
		enMat[i] = (char*)malloc(sizeof(char)* col);
	}

	// ���� Ȯ��
	if (enMat == NULL) {
		return NULL;
	}

	char enM = 0; // test

	// ��ȣ�� ��� �ʱ�ȭ �� ��ȣȭ (Ű��İ� ������� ��)
	for (int j = 0; j < row; ++j) {
		for (int i = 0; i < col; ++i) {
			enMat[j][i] = (int)0;
			for (int k = 0; k < row; ++k) {
				enMat[j][i] += modular((_element[j][k] * strMat[k][i]), charCnt);
				enMat[j][i] = modular(enMat[j][i], charCnt);
			}
		}
	}

	// ��ȣȭ ��� ��ȯ
	return enMat;
}
