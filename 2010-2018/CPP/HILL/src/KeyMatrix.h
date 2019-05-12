#ifndef __KEY_MATRIX_H__
#define __KEY_MATRIX_H__

#define KEY_MAX_SIZE 5

class KeyMatrix {
private:
	static const int maxRow = KEY_MAX_SIZE;				// �ִ� ���� ũ��
	static const int maxColumn = KEY_MAX_SIZE;			// �ִ� ���� ũ��
	int _row;											// ���� ũ��
	int _column;										// ���� ũ��
	int _element[maxRow][maxColumn];					// ����� ���

public:
	KeyMatrix(int size);								// ������
	KeyMatrix(KeyMatrix& mat);							// ���� ������
	~KeyMatrix();										// �Ҹ���

	void setRandomElement(int minBound, int maxBound);	// ����� ��Ҹ� (minBound ~ maxBound) ������ ������ ������ ä�� ����
	int getDeterminant();								// ����� ���͹̳�Ʈ(det)�� ����
	int getCofactor(int j, int i);						// ���μ��� ����
	bool getKeyMatrix(int low, int high);				// Ű ��Ʈ������ ����
	bool getModMatrix(int mod);							// modular�� ���� ��Ʈ������ ����
	KeyMatrix& getMinorMatrix(int j, int i);			// ����Ľ��� ���� (5x5 -> 4x4 -> 3x3)
	KeyMatrix& getTransposedMatrix();					// ��ġ��� ����
	KeyMatrix& getAdjointMatrix();						// ������� ����
	KeyMatrix& getCofactorMatrix();						// ���μ���� ����
	KeyMatrix& getInverseMatrix();						// ����� ����
	void print();										// ����� ��Ҹ� �����

	KeyMatrix& operator*(KeyMatrix& mat);				// ��ĳ��� ����
	KeyMatrix& operator*(int num);						// ��İ� ����� ����

	char** getEncryptionMat(char** strMat, int col, int row);	// ��ȣȭ�� ����� ����
};

#endif